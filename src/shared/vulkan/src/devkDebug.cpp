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
	
	VkDebugReportCallbackCreateInfoEXT info{VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT};
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
