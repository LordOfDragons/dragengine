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

#include <stdlib.h>
#include <string.h>

#include "deoxrDebug.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"
#include "deoxrGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class deoxrDebug
////////////////////

deoxrDebug::deoxrDebug( deoxrInstance &instance ) :
pInstance( instance ),
pEnabled( false ),
pMessenger( XR_NULL_HANDLE ){
}

deoxrDebug::~deoxrDebug(){
	SetEnabled( false );
}



// Management
///////////////

void deoxrDebug::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	if( pEnabled ){
		pUnregisterReportCallback();
		pEnabled = false;
	}
	
	if( enabled ){
		if( ! pInstance.SupportsExtension( deoxrInstance::extEXTDebugUtils ) ){
			return;
		}
		if( ! pInstance.SupportsLayer( deoxrInstance::layerLunarCoreValidation ) ){
			return;
		}
		
		pRegisterReportCallback();
		pEnabled = true;
	}
}



// Private Functions
//////////////////////

void deoxrDebug::pRegisterReportCallback(){
	if( ! pInstance.xrCreateDebugUtilsMessengerEXT || ! pInstance.xrDestroyDebugUtilsMessengerEXT ){
		return;
	}
	
	XrDebugUtilsMessengerCreateInfoEXT info;
	memset( &info, 0, sizeof( info ) );
	
	info.type = XR_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverities = XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	info.messageTypes = XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
		| XR_DEBUG_UTILS_MESSAGE_TYPE_CONFORMANCE_BIT_EXT;
	info.userCallback = ( PFN_xrDebugUtilsMessengerCallbackEXT )DebugMessageCallback;
	info.userData = this;
	
	OXR_CHECK( pInstance.GetOxr(), pInstance.xrCreateDebugUtilsMessengerEXT(
		pInstance.GetInstance(), &info, &pMessenger ) );
	
	pInstance.GetOxr().LogInfo( "Debug: Message Callback Registered" );
}

void deoxrDebug::pUnregisterReportCallback(){
	if( ! pMessenger ){
		return;
	}
	
	if( pInstance.xrDestroyDebugUtilsMessengerEXT ){
		pInstance.xrDestroyDebugUtilsMessengerEXT( pMessenger );
	}
	
	pMessenger = XR_NULL_HANDLE;
	pInstance.GetOxr().LogInfo( "Debug: Message Callback Unregistered" );
}

void deoxrDebug::DebugMessage( const char *function, const char *messageId, const char *message ){
	pInstance.GetOxr().LogInfoFormat( "Debug: %s: %s - %s", function, messageId, message );
}

XrBool32 deoxrDebug::DebugMessageCallback( XrDebugUtilsMessageSeverityFlagsEXT,
XrDebugUtilsMessageTypeFlagsEXT, const XrDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData ){
	( ( deoxrDebug* )userData )->DebugMessage( callbackData->functionName,
		callbackData->messageId, callbackData->message );
	return XR_FALSE;
}
