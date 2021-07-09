/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "devkBasics.h"
#include "deSharedVulkan.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


#ifdef VK_CHECKCOMMANDS

void devkDebugCheckCommand( VkResult result, deSharedVulkan &vulkan, const char *file, int line ){
	#define CASE_ERROR(e) \
		case e: \
			vulkan.GetModule().LogException( deeInvalidAction( file, line, #e ) ); \
			break;
	
	switch( result ){
	case VK_SUCCESS:
		break;
		
	CASE_ERROR( VK_INCOMPLETE )
	CASE_ERROR( VK_ERROR_DEVICE_LOST )
	CASE_ERROR( VK_ERROR_OUT_OF_HOST_MEMORY )
	CASE_ERROR( VK_ERROR_OUT_OF_DEVICE_MEMORY )
	CASE_ERROR( VK_ERROR_INITIALIZATION_FAILED )
	CASE_ERROR( VK_ERROR_LAYER_NOT_PRESENT )
	CASE_ERROR( VK_ERROR_EXTENSION_NOT_PRESENT )
	CASE_ERROR( VK_ERROR_INCOMPATIBLE_DRIVER )
	
	default:{
		decString message;
		message.Format( "Error %x (%d)", result, result );
		vulkan.GetModule().LogException( deeInvalidAction( __FILE__, __LINE__, message ) );
		}
	}
	
	#undef CASE_ERROR
}

#endif
