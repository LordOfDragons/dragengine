/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "deoxrGlobalFunctions.h"

#ifdef INTERNAL_XR_LOADER

#define EXPORTED_OPENXR_FUNCTION( name ) PFN_##name name = XR_NULL_HANDLE;
#define GLOBAL_LEVEL_OPENXR_FUNCTION( name ) PFN_##name name = XR_NULL_HANDLE;

XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateApiLayerProperties( uint32_t propertyCapacityInput,
uint32_t *propertyCountOutput, XrApiLayerProperties *properties ){
	*propertyCountOutput = 0;
	return XR_SUCCESS;
}

#endif // INTERNAL_XR_LOADER

#include "deoxrFunctionNames.h"
