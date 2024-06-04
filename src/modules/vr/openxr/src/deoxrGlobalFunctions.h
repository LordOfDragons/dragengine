/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOXRGLOBALFUNCTIONS_H_
#define _DEOXRGLOBALFUNCTIONS_H_

#include "deoxrBasics.h"

#ifdef INTERNAL_XR_LOADER

#define EXPORTED_OPENXR_FUNCTION( name ) extern PFN_##name name;
#define GLOBAL_LEVEL_OPENXR_FUNCTION( name ) extern PFN_##name name;

#include "deoxrFunctionNames.h"

extern XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateApiLayerProperties(
	uint32_t propertyCapacityInput, uint32_t *propertyCountOutput,
	XrApiLayerProperties *properties );

#undef EXPORTED_OPENXR_FUNCTION
#undef GLOBAL_LEVEL_OPENXR_FUNCTION

#endif // INTERNAL_XR_LOADER

#endif

