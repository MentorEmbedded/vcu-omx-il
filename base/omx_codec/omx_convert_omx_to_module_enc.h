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

#pragma once

#include <OMX_Video.h>
#include <OMX_VideoExt.h>
#include <OMX_Allegro.h>
#include "base/omx_mediatype/omx_mediatype_enc_enums.h"
#include "base/omx_module/omx_module_enc_enums.h"
#include "base/omx_module/omx_module_enc_structs.h"

int ConvertToModuleBFrames(OMX_U32 const& bFrames, OMX_U32 const& pFrames);
int ConvertToModuleGopLength(OMX_U32 const& bFrames, OMX_U32 const& pFrames);
EntropyCodingType ConvertToModuleEntropyCoding(OMX_BOOL const& isCabac);
int ConvertToModuleQPInitial(OMX_U32 const& qpI);
int ConvertToModuleQPDeltaIP(OMX_U32 const& qpI, OMX_U32 const& qpP);
int ConvertToModuleQPDeltaPB(OMX_U32 const& pP, OMX_U32 const& qpB);
int ConvertToModuleQPMin(OMX_S32 const& qpMin);
int ConvertToModuleQPMax(OMX_S32 const& qpMax);
QPControlType ConvertToModuleQPControl(OMX_ALG_EQpCtrlMode const& mode);
RateControlType ConvertToModuleControlRate(OMX_VIDEO_CONTROLRATETYPE const& mode);
AspectRatioType ConvertToModuleAspectRatio(OMX_ALG_EAspectRatio const& aspectRatio);
GopControlType ConvertToModuleGopControl(OMX_ALG_EGopCtrlMode const& mode);
GdrType ConvertToModuleGdr(OMX_ALG_EGdrMode const& gdr);
RateControlOptionType ConvertToModuleDisableSceneChangeResilience(OMX_BOOL const& disable);
ScalingListType ConvertToModuleScalingList(OMX_ALG_EScalingList const& scalingListMode);

