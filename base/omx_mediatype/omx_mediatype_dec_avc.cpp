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

#include "omx_mediatype_dec_avc.h"
#include "base/omx_settings/omx_convert_module_soft.h"
#include "base/omx_settings/omx_convert_module_soft_avc.h"
#include "base/omx_utils/roundup.h"
#include <cstring> // memset

using namespace std;

DecMediatypeAVC::DecMediatypeAVC()
{
  Reset();
}

void DecMediatypeAVC::Reset()
{
  memset(&settings, 0, sizeof(settings));
  settings.iStackSize = 5;
  settings.uFrameRate = 60000;
  settings.uClkRatio = 1000;
  settings.uDDRWidth = 32;
  settings.eDecUnit = AL_AU_UNIT;
  settings.eDpbMode = AL_DPB_NORMAL;
  settings.eFBStorageMode = AL_FB_RASTER;
  settings.bIsAvc = true;

#ifdef ANDROID
  auto& stream = settings.tStream;
  stream.tDim = { 3840, 2176};
  stream.eChroma = CHROMA_4_2_0;
  stream.iBitDepth = 8;
  stream.iLevel = 51;
  stream.iProfileIdc = AL_PROFILE_AVC_HIGH_422;
#else
  auto& stream = settings.tStream;
  stream.tDim = { 176, 144 };
  stream.eChroma = CHROMA_4_2_0;
  stream.iBitDepth = 8;
  stream.iLevel = 10;
  stream.iProfileIdc = AL_PROFILE_AVC_C_BASELINE;
#endif

  stride = RoundUp(AL_Decoder_RoundPitch(stream.tDim.iWidth, stream.iBitDepth, settings.eFBStorageMode), strideAlignment);
  sliceHeight = RoundUp(AL_Decoder_RoundHeight(stream.tDim.iHeight), sliceHeightAlignment);
}

CompressionType DecMediatypeAVC::Compression() const
{
  return COMPRESSION_AVC;
}

string DecMediatypeAVC::Mime() const
{
  return "video/x-h264";
}

vector<ProfileLevelType> DecMediatypeAVC::ProfileLevelSupported() const
{
  vector<ProfileLevelType> vector;

  for(auto const& profile : profiles)
    for(auto const & level : levels)
    {
      ProfileLevelType tmp;
      tmp.profile.avc = profile;
      tmp.level = level;
      vector.push_back(tmp);
    }

  return vector;
}

ProfileLevelType DecMediatypeAVC::ProfileLevel() const
{
  ProfileLevelType p;
  p.profile.avc = ConvertSoftToModuleAVCProfile(static_cast<AL_EProfile>(settings.tStream.iProfileIdc));
  p.level = settings.tStream.iLevel;
  return p;
}

bool DecMediatypeAVC::IsInProfilesSupported(AVCProfileType const& profile)
{
  for(auto const& p : profiles)
  {
    if(p == profile)
      return true;
  }

  return false;
}

bool DecMediatypeAVC::IsInLevelsSupported(int const& level)
{
  for(auto const& l : levels)
  {
    if(l == level)
      return true;
  }

  return false;
}

bool DecMediatypeAVC::SetProfileLevel(ProfileLevelType const& profileLevel)
{
  if(!IsInProfilesSupported(profileLevel.profile.avc))
    return false;

  if(!IsInLevelsSupported(profileLevel.level))
    return false;

  auto const profile = ConvertModuleToSoftAVCProfile(profileLevel.profile.avc);

  if(profile == AL_PROFILE_AVC)
    return false;

  settings.tStream.iProfileIdc = profile;
  settings.tStream.iLevel = profileLevel.level;
  return true;
}

int DecMediatypeAVC::GetRequiredOutputBuffers() const
{
  return AL_AVC_GetMinOutputBuffersNeeded(settings.tStream, settings.iStackSize, settings.eDpbMode);
}

Format DecMediatypeAVC::GetFormat() const
{
  Format format;
  format.color = ConvertSoftToModuleColor(settings.tStream.eChroma);
  format.bitdepth = settings.tStream.iBitDepth;
  return format;
}

vector<Format> DecMediatypeAVC::FormatsSupported() const
{
  vector<Format> formatsSupported;
  formatsSupported.push_back(GetFormat());
  return formatsSupported;
}

