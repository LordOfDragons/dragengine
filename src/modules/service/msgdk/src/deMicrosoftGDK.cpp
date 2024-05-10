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

#include <string.h>

#include "deMicrosoftGDK.h"
#include "deGDKServiceSystem.h"

#include <dragengine/common/exceptions.h>


// export definition
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) deBaseModule *MSGDKCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *MSGDKCreateModule(deLoadableModule *loadableModule){
	try{
		return new deMicrosoftGDK(*loadableModule);
	}catch(...){
		return nullptr;
	}
}


// Error code strings
///////////////////////

#define E_GAME_MISSING_GAME_CONFIG 0x87e5001f
#define E_GAMEPACKAGE_CONFIG_NO_MSAAPPID_NOTITLEID 0x8924520b

const int vErrorCodeCount = 49;
static struct sErrorCode{
	HRESULT code;
	const char *string;
} vErrorCodes[vErrorCodeCount] = {
	{E_GAME_MISSING_GAME_CONFIG, "E_GAME_MISSING_GAME_CONFIG"},
	{E_GAMERUNTIME_NOT_INITIALIZED, "E_GAMERUNTIME_NOT_INITIALIZED"},
	{E_GAMERUNTIME_DLL_NOT_FOUND, "E_GAMERUNTIME_DLL_NOT_FOUND"},
	{E_GAMERUNTIME_VERSION_MISMATCH, "E_GAMERUNTIME_VERSION_MISMATCH"},
	{E_GAMERUNTIME_WINDOW_NOT_FOREGROUND, "E_GAMERUNTIME_WINDOW_NOT_FOREGROUND"},
	{E_GAMERUNTIME_SUSPENDED, "E_GAMERUNTIME_SUSPENDED"},
	{E_GAMERUNTIME_UNINITIALIZE_ACTIVEOBJECTS, "E_GAMERUNTIME_UNINITIALIZE_ACTIVEOBJECTS"},
	{E_GAMERUNTIME_MULTIPLAYER_NOT_CONFIGURED, "E_GAMERUNTIME_MULTIPLAYER_NOT_CONFIGURED"},
	{E_GAMEUSER_MAX_USERS_ADDED, "E_GAMEUSER_MAX_USERS_ADDED"},
	{E_GAMEUSER_SIGNED_OUT, "E_GAMEUSER_SIGNED_OUT"},
	{E_GAMEUSER_RESOLVE_USER_ISSUE_REQUIRED, "E_GAMEUSER_RESOLVE_USER_ISSUE_REQUIRED"},
	{E_GAMEUSER_DEFERRAL_NOT_AVAILABLE, "E_GAMEUSER_DEFERRAL_NOT_AVAILABLE"},
	{E_GAMEUSER_USER_NOT_FOUND, "E_GAMEUSER_USER_NOT_FOUND"},
	{E_GAMEUSER_NO_TOKEN_REQUIRED, "E_GAMEUSER_NO_TOKEN_REQUIRED"},
	{E_GAMEUSER_NO_DEFAULT_USER, "E_GAMEUSER_NO_DEFAULT_USER"},
	{E_GAMEUSER_FAILED_TO_RESOLVE, "E_GAMEUSER_FAILED_TO_RESOLVE"},
	{E_GAMEUSER_NO_TITLE_ID, "E_GAMEUSER_NO_TITLE_ID"},
	{E_GAMEUSER_UNKNOWN_GAME_IDENTITY, "E_GAMEUSER_UNKNOWN_GAME_IDENTITY"},
	{E_GAMEUSER_NO_PACKAGE_IDENTITY, "E_GAMEUSER_NO_PACKAGE_IDENTITY"},
	{E_GAMEUSER_FAILED_TO_GET_TOKEN, "E_GAMEUSER_FAILED_TO_GET_TOKEN"},
	{E_GAMEUSER_INVALID_APP_CONFIGURATION, "E_GAMEUSER_INVALID_APP_CONFIGURATION"},
	{E_GAMEUSER_MALFORMED_MSAAPPID, "E_GAMEUSER_MALFORMED_MSAAPPID"},
	{E_GAMEUSER_INCONSISTENT_MSAAPPID_AND_TITLEID, "E_GAMEUSER_INCONSISTENT_MSAAPPID_AND_TITLEID"},
	{E_GAMEPACKAGE_APP_NOT_PACKAGED, "E_GAMEPACKAGE_APP_NOT_PACKAGED"},
	{E_GAMEPACKAGE_NO_INSTALLED_LANGUAGES, "E_GAMEPACKAGE_NO_INSTALLED_LANGUAGES"},
	{E_GAMEPACKAGE_NO_STORE_ID, "E_GAMEPACKAGE_NO_STORE_ID"},
	{E_GAMEPACKAGE_INVALID_SELECTOR, "E_GAMEPACKAGE_INVALID_SELECTOR"},
	{E_GAMEPACKAGE_DOWNLOAD_REQUIRED, "E_GAMEPACKAGE_DOWNLOAD_REQUIRED"},
	{E_GAMEPACKAGE_NO_TAG_CHANGE, "E_GAMEPACKAGE_NO_TAG_CHANGE"},
	{E_GAMEPACKAGE_DLC_NOT_SUPPORTED, "E_GAMEPACKAGE_DLC_NOT_SUPPORTED"},
	{E_GAMEPACKAGE_DUPLICATE_ID_VALUES, "E_GAMEPACKAGE_DUPLICATE_ID_VALUES"},
	{E_GAMEPACKAGE_NO_PACKAGE_IDENTIFIER, "E_GAMEPACKAGE_NO_PACKAGE_IDENTIFIER"},
	{E_GAMEPACKAGE_CONFIG_NO_ROOT_NODE, "E_GAMEPACKAGE_CONFIG_NO_ROOT_NODE"},
	{E_GAMEPACKAGE_CONFIG_ZERO_VERSION, "E_GAMEPACKAGE_CONFIG_ZERO_VERSION"},
	{E_GAMEPACKAGE_CONFIG_NO_MSAAPPID_NOTITLEID, "E_GAMEPACKAGE_CONFIG_NO_MSAAPPID_NOTITLEID"},
	{E_GAMEPACKAGE_CONFIG_DEPRECATED_PC_ENTRIES, "E_GAMEPACKAGE_CONFIG_DEPRECATED_PC_ENTRIES"},
	{E_GAMEPACKAGE_CONFIG_SUM_REQUIRES_MSAAPPID, "E_GAMEPACKAGE_CONFIG_SUM_REQUIRES_MSAAPPID"},
	{E_GAMEPACKAGE_CONFIG_NO_CODE_CLOUD_SAVES_REQUIRES_MSAAPPID, "E_GAMEPACKAGE_CONFIG_NO_CODE_CLOUD_SAVES_REQUIRES_MSAAPPID"},
	{E_GAMEPACKAGE_CONFIG_MSAAPPID_OR_TITLEID_IS_DEFAULT, "E_GAMEPACKAGE_CONFIG_MSAAPPID_OR_TITLEID_IS_DEFAULT"},
	{E_GAMESTORE_LICENSE_ACTION_NOT_APPLICABLE_TO_PRODUCT, "E_GAMESTORE_LICENSE_ACTION_NOT_APPLICABLE_TO_PRODUCT"},
	{E_GAMESTORE_NETWORK_ERROR, "E_GAMESTORE_NETWORK_ERROR"},
	{E_GAMESTORE_SERVER_ERROR, "E_GAMESTORE_SERVER_ERROR"},
	{E_GAMESTORE_INSUFFICIENT_QUANTITY, "E_GAMESTORE_INSUFFICIENT_QUANTITY"},
	{E_GAMESTORE_ALREADY_PURCHASED, "E_GAMESTORE_ALREADY_PURCHASED"},
	{E_GAMESTREAMING_NOT_INITIALIZED, "E_GAMESTREAMING_NOT_INITIALIZED"},
	{E_GAMESTREAMING_CLIENT_NOT_CONNECTED, "E_GAMESTREAMING_CLIENT_NOT_CONNECTED"},
	{E_GAMESTREAMING_NO_DATA, "E_GAMESTREAMING_NO_DATA"},
	{E_GAMESTREAMING_NO_DATACENTER, "E_GAMESTREAMING_NO_DATACENTER"},
	{E_GAMESTREAMING_NOT_STREAMING_CONTROLLER, "E_GAMESTREAMING_NOT_STREAMING_CONTROLLER"}
};


// Class deMicrosoftGDK
/////////////////////////

// Constructor, destructor
////////////////////////////

deMicrosoftGDK::deMicrosoftGDK(deLoadableModule& loadableModule) :
deBaseServiceModule(loadableModule),
pSdkInited(false)
{
	InitSdk();
}

deMicrosoftGDK::~deMicrosoftGDK(){
	if(pSdkInited){
		LogInfo("Shutdown GDK");
		XGameRuntimeUninitialize();
	}
}


// Management
///////////////

const char *deMicrosoftGDK::GetErrorCodeString(HRESULT code) const{
	static const char * const unknown = "?";

	int i;
	for(i=0; i<vErrorCodeCount; i++){
		if(vErrorCodes[i].code == code){
			return vErrorCodes[i].string;
		}
	}

	return unknown;
}

decStringSet deMicrosoftGDK::GetSupportedServices(){
	decStringSet names;
	names.Add(deGDKServiceSystem::serviceName);
	return names;
}

deBaseServiceService* deMicrosoftGDK::CreateService(deService *service, const char* name){
	DEASSERT_NOTNULL(service)

	if(strcmp(name, deGDKServiceSystem::serviceName) == 0){
		return new deGDKServiceSystem(*this, service);
	}

	return nullptr;
}

void deMicrosoftGDK::InitSdk(){
	if(pSdkInited){
		return;
	}

	LogInfo("Initialize GDK");
	HRESULT rc = XGameRuntimeInitialize();
	if(FAILED(rc)){
		LogErrorFormat("Failed initialize GDK: %s", GetErrorCodeString(rc));
		DETHROW(deeInvalidAction);
	}
	pSdkInited = true;
}
