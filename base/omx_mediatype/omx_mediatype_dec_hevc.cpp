/******************************************************************************
*
* Copyright (C) 2017 Allegro DVT2.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX OR ALLEGRO DVT2 BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of  Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
*
* Except as contained in this notice, the name of Allegro DVT2 shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Allegro DVT2.
*
******************************************************************************/

#include "omx_mediatype_dec_hevc.h"
#include "base/omx_settings/omx_convert_module_soft.h"
#include "base/omx_settings/omx_convert_module_soft_hevc.h"
#include "base/omx_utils/roundup.h"
#include <cstring> // memset

using namespace std;

DecMediatypeHEVC::DecMediatypeHEVC()
{
  Reset();
}

void DecMediatypeHEVC::Reset()
{
  memset(&settings, 0, sizeof(settings));
  settings.iStackSize = 5;
  settings.uFrameRate = 60000;
  settings.uClkRatio = 1000;
  settings.uDDRWidth = 32;
  settings.eDecUnit = AL_AU_UNIT;
  settings.eDpbMode = AL_DPB_NORMAL;
  settings.eFBStorageMode = AL_FB_RASTER;
  settings.bIsAvc = false;

#ifdef ANDROID
  auto & stream = settings.tStream;
  stream.tDim = { 3840, 2176 };
  stream.eChroma = CHROMA_4_2_0;
  stream.iBitDepth = 8;
  stream.iLevel = 51;
  stream.iProfileIdc = AL_PROFILE_HEVC_MAIN10;
#else
  auto & stream = settings.tStream;
  stream.tDim = { 176, 144 };
  stream.eChroma = CHROMA_4_2_0;
  stream.iBitDepth = 8;
  stream.iLevel = 10;
  stream.iProfileIdc = AL_PROFILE_HEVC_MAIN;
#endif

  stride = RoundUp(AL_Decoder_RoundPitch(stream.tDim.iWidth, stream.iBitDepth, settings.eFBStorageMode), strideAlignment);
  sliceHeight = RoundUp(AL_Decoder_RoundHeight(stream.tDim.iHeight), sliceHeightAlignment);
  tier = 0;
}

CompressionType DecMediatypeHEVC::Compression() const
{
  return COMPRESSION_HEVC;
}

string DecMediatypeHEVC::Mime() const
{
  return "video/x-h265";
}

static bool isHighTierProfile(HEVCProfileType const& profile)
{
  switch(profile)
  {
  case HEVC_PROFILE_MAIN_HIGH_TIER:
  case HEVC_PROFILE_MAIN_10_HIGH_TIER:
  case HEVC_PROFILE_MAIN_422_HIGH_TIER:
  case HEVC_PROFILE_MAIN_422_10_HIGH_TIER:
  case HEVC_PROFILE_MAIN_STILL_HIGH_TIER:
    return true;
  default: return false;
  }

  return false;
}

static bool isCompliant(HEVCProfileType const& profile, int const& level)
{
  if(isHighTierProfile(profile) && level < 40)
    return false;
  return true;
}

vector<ProfileLevelType> DecMediatypeHEVC::ProfileLevelSupported() const
{
  vector<ProfileLevelType> vector;

  for(auto const& profile : profiles)
    for(auto const & level : levels)
    {
      if(isCompliant(profile, level))
      {
        ProfileLevelType tmp;
        tmp.profile.hevc = profile;
        tmp.level = level;
        vector.push_back(tmp);
      }
    }

  return vector;
}

static bool IsHighTier(uint8_t const& tier)
{
  return tier != 0;
}

static bool IsHighTier(HEVCProfileType const& profile)
{
  switch(profile)
  {
  case HEVC_PROFILE_MAIN_HIGH_TIER:
  case HEVC_PROFILE_MAIN_10_HIGH_TIER:
  case HEVC_PROFILE_MAIN_422_HIGH_TIER:
  case HEVC_PROFILE_MAIN_STILL_HIGH_TIER:
    return true;
  default: return false;
  }

  return false;
}

ProfileLevelType DecMediatypeHEVC::ProfileLevel() const
{
  ProfileLevelType p;
  p.profile.hevc = IsHighTier(tier) ? ConvertSoftToModuleHEVCHighTierProfile(static_cast<AL_EProfile>(settings.tStream.iProfileIdc)) : ConvertSoftToModuleHEVCMainTierProfile(static_cast<AL_EProfile>(settings.tStream.iProfileIdc));
  p.level = settings.tStream.iLevel;
  return p;
}

bool DecMediatypeHEVC::IsInProfilesSupported(HEVCProfileType const& profile)
{
  for(auto const& p : profiles)
  {
    if(p == profile)
      return true;
  }

  return false;
}

bool DecMediatypeHEVC::IsInLevelsSupported(int const& level)
{
  for(auto const& l : levels)
  {
    if(l == level)
      return true;
  }

  return false;
}

bool DecMediatypeHEVC::SetProfileLevel(ProfileLevelType const& profileLevel)
{
  if(!IsInProfilesSupported(profileLevel.profile.hevc))
    return false;

  if(!IsInLevelsSupported(profileLevel.level))
    return false;

  auto const profile = ConvertModuleToSoftHEVCProfile(profileLevel.profile.hevc);

  if(profile == AL_PROFILE_HEVC)
    return false;

  settings.tStream.iProfileIdc = profile;
  settings.tStream.iLevel = profileLevel.level;
  tier = IsHighTier(profileLevel.profile.hevc) ? 1 : 0;

  return true;
}

int DecMediatypeHEVC::GetRequiredOutputBuffers() const
{
  return AL_HEVC_GetMinOutputBuffersNeeded(settings.tStream, settings.iStackSize, settings.eDpbMode);
}

Format DecMediatypeHEVC::GetFormat() const
{
  Format format;
  format.color = ConvertSoftToModuleColor(settings.tStream.eChroma);
  format.bitdepth = settings.tStream.iBitDepth;
  return format;
}

vector<Format> DecMediatypeHEVC::FormatsSupported() const
{
  vector<Format> formatsSupported;
  formatsSupported.push_back(GetFormat());
  return formatsSupported;
}

