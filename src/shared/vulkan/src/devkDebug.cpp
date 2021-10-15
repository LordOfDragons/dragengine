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

#include <stdlib.h>
#include <string.h>

#include "devkDebug.h"
#include "devkInstance.h"
#include "deSharedVulkan.h"
#include "devkGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class devkDebug
////////////////////

devkDebug::devkDebug( devkInstance &instance ) :
pInstance( instance ),
pEnabled( false ),
pReportCallback( VK_NULL_HANDLE ){
}

devkDebug::~devkDebug(){
	SetEnabled( false );
}



// Management
///////////////

void devkDebug::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	if( pEnabled ){
		pUnregisterReportCallback();
		pEnabled = false;
	}
	
	if( enabled ){
		if( ! pInstance.SupportsExtension( devkInstance::extEXTDebugReport ) ){
			return;
		}
		if( ! pInstance.SupportsLayer( devkInstance::layerKhronosValidation)
		&& ! pInstance.SupportsLayer( devkInstance::layerLunargStandardValidation ) ){
			return;
		}
		
		pRegisterReportCallback();
		pEnabled = true;
	}
}



// Private Functions
//////////////////////

void devkDebug::pRegisterReportCallback(){
	if( ! pInstance.vkCreateDebugReportCallbackEXT
	|| ! pInstance.vkDestroyDebugReportCallbackEXT ){
		return;
	}
	
	VkDebugReportCallbackCreateInfoEXT info;
	memset( &info, 0, sizeof( info ) );
	
	info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	info.pfnCallback = ( PFN_vkDebugReportCallbackEXT )DebugMessageCallback;
	info.pUserData = this;
	
	VK_CHECK( pInstance.GetVulkan(), pInstance.vkCreateDebugReportCallbackEXT(
		pInstance.GetInstance(), &info, VK_NULL_HANDLE, &pReportCallback ) );
	
	pInstance.GetVulkan().GetModule().LogInfo( "Debug: Message Callback Registered" );
}

void devkDebug::pUnregisterReportCallback(){
	if( ! pReportCallback ){
		return;
	}
	
	if( pInstance.vkDestroyDebugReportCallbackEXT ){
		pInstance.vkDestroyDebugReportCallbackEXT( pInstance.GetInstance(), pReportCallback, VK_NULL_HANDLE );
	}
	
	pReportCallback = VK_NULL_HANDLE;
	pInstance.GetVulkan().GetModule().LogInfo( "Debug: Message Callback Unregistered" );
}

void devkDebug::DebugMessage( const char *layerPrefix, const char *message ){
	pInstance.GetVulkan().GetModule().LogInfoFormat( "Debug: Validation: %s - %s", layerPrefix, message );
}

VkBool32 devkDebug::DebugMessageCallback( VkDebugReportFlagsEXT /*flags*/,
VkDebugReportObjectTypeEXT /*objectType*/, uint64_t /*object*/, size_t /*location*/,
int32_t /*messageCode*/, const char* pLayerPrefix, const char* pMessage, void* pUserData ){
	( ( devkDebug* )pUserData )->DebugMessage( pLayerPrefix, pMessage );
	return VK_FALSE;
}
