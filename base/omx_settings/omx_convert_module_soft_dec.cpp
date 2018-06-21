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

#include "omx_convert_module_soft_dec.h"
#include <assert.h>

AL_EDpbMode ConvertModuleToSoftDecodedPictureBuffer(DecodedPictureBufferType const& mode)
{
  switch(mode)
  {
  case DECODED_PICTURE_BUFFER_NORMAL: return AL_DPB_NORMAL;
  case DECODED_PICTURE_BUFFER_LOW_REFERENCE: return AL_DPB_LOW_REF;
  case DECODED_PICTURE_BUFFER_MAX: // fallthrough
  default: return AL_DPB_MAX_ENUM;
  }

  return AL_DPB_MAX_ENUM;
}

AL_EDecUnit ConvertModuleToSoftDecodeUnit(DecodeUnitType const& unit)
{
  switch(unit)
  {
  case DECODE_UNIT_FRAME: return AL_AU_UNIT;
  case DECODE_UNIT_SLICE: return AL_VCL_NAL_UNIT;
  case DECODE_UNIT_MAX: // fallthrough
  default: assert(0);
  }

  assert(0);
  return AL_VCL_NAL_UNIT;
}

DecodedPictureBufferType ConvertSoftToModuleDecodedPictureBuffer(AL_EDpbMode const& mode)
{
  switch(mode)
  {
  case AL_DPB_NORMAL: return DECODED_PICTURE_BUFFER_NORMAL;
  case AL_DPB_LOW_REF: return DECODED_PICTURE_BUFFER_LOW_REFERENCE;
  case AL_DPB_MAX_ENUM: // fallthrough
  default: return DECODED_PICTURE_BUFFER_MAX;
  }

  return DECODED_PICTURE_BUFFER_MAX;
}

DecodeUnitType ConvertSoftToModuleDecodeUnit(AL_EDecUnit const& unit)
{
  switch(unit)
  {
  case AL_AU_UNIT: return DECODE_UNIT_FRAME;
  case AL_VCL_NAL_UNIT: return DECODE_UNIT_SLICE;
  default: return DECODE_UNIT_MAX;
  }

  return DECODE_UNIT_MAX;
}

