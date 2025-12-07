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

#include "deoxrDebug.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"
#include "deoxrGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class deoxrDebug
////////////////////

deoxrDebug::deoxrDebug(deoxrInstance &instance) :
pInstance(instance),
pEnabled(false),
pMessenger(XR_NULL_HANDLE){
}

deoxrDebug::~deoxrDebug(){
	SetEnabled(false);
}



// Management
///////////////

void deoxrDebug::SetEnabled(bool enabled){
	if(enabled == pEnabled){
		return;
	}
	
	if(pEnabled){
		pUnregisterReportCallback();
		pEnabled = false;
	}
	
	if(enabled){
		if(! pInstance.SupportsExtension(deoxrInstance::extEXTDebugUtils)){
			return;
		}
		
		pRegisterReportCallback();
		pEnabled = true;
	}
}



// Private Functions
//////////////////////

void deoxrDebug::pRegisterReportCallback(){
	if(! pInstance.xrCreateDebugUtilsMessengerEXT || ! pInstance.xrDestroyDebugUtilsMessengerEXT){
		return;
	}
	
	XrDebugUtilsMessengerCreateInfoEXT info;
	memset(&info, 0, sizeof(info));
	
	info.type = XR_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	info.messageSeverities = XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
		| XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	info.messageTypes = XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
		| XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
		| XR_DEBUG_UTILS_MESSAGE_TYPE_CONFORMANCE_BIT_EXT;
	info.userCallback = (PFN_xrDebugUtilsMessengerCallbackEXT)DebugMessageCallback;
	info.userData = this;
	
	OXR_CHECK(pInstance.xrCreateDebugUtilsMessengerEXT(pInstance.GetInstance(), &info, &pMessenger));
	
	pInstance.GetOxr().LogInfo("Debug: Message Callback Registered");
}

void deoxrDebug::pUnregisterReportCallback(){
	if(! pMessenger){
		return;
	}
	
	if(pInstance.xrDestroyDebugUtilsMessengerEXT){
		pInstance.xrDestroyDebugUtilsMessengerEXT(pMessenger);
	}
	
	pMessenger = XR_NULL_HANDLE;
	pInstance.GetOxr().LogInfo("Debug: Message Callback Unregistered");
}

void deoxrDebug::DebugMessage(const char *function, const char *messageId, const char *message){
	pInstance.GetOxr().LogInfoFormat("Debug: %s: %s - %s", function, messageId, message);
}

XrBool32 deoxrDebug::DebugMessageCallback(XrDebugUtilsMessageSeverityFlagsEXT,
XrDebugUtilsMessageTypeFlagsEXT, const XrDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData){
	((deoxrDebug*)userData)->DebugMessage(callbackData->functionName,
		callbackData->messageId, callbackData->message);
	return XR_FALSE;
}
