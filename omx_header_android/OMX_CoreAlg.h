/*
 * Copyright (c) 2017 Allegro DVT2. All rights reserved.
 * Copyright (c) 2016 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

/** OMX_CoreAlg.h - OpenMax IL version 1.1.2
 * The OMX_CoreAlg header file contains extensions to the definitions used
 * by both the application and the component to access common items.
 */

#ifndef OMX_CoreAlg_h
#define OMX_CoreAlg_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Each OMX header shall include all required header files to allow the
 * header to compile without errors.  The includes below are required
 * for this header file to compile successfully
 */


/** Define the OMX IL version that corresponds to this set of header files.
 *  We also define a combined version that can be used to write or compare
 *  values of the 32bit nVersion field, assuming a little endian architecture */
#define OMX_VERSION_MAJOR 1
#define OMX_VERSION_MINOR 1
#define OMX_VERSION_REVISION 2
#define OMX_VERSION_STEP 0

#define OMX_VERSION ((OMX_VERSION_STEP << 24) | (OMX_VERSION_REVISION << 16) | (OMX_VERSION_MINOR << 8) | OMX_VERSION_MAJOR)

// This buffer already exist in OpenMax IL version 1.2 (3.7.3.7.1)
// Keep there names and values
#define OMX_BUFFERFLAG_TIMESTAMPINVALID 0x00000100
#define OMX_BUFFERFLAG_READONLY 0x00000200
#define OMX_BUFFERFLAG_ENDOFSUBFRAME 0x00000400
#define OMX_BUFFERFLAG_SKIPFRAME 0x00000800

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OMX_CoreAlg_h */
/* File EOF */

