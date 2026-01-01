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
#include "deMsgdkServiceMsgdk.h"
#include "deMsgdkGameConfig.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>


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

deBaseModule *MSGDKCreateModule(deLoadableModule *loadableModule)
{
	try
	{
		return new deMicrosoftGdk(*loadableModule);
	}
	catch(...)
	{
		return nullptr;
	}
}


// Error code strings
///////////////////////

#define E_GAME_MISSING_GAME_CONFIG ((HRESULT)0x87e5001f)
#define E_GAMEPACKAGE_CONFIG_NO_MSAAPPID_NOTITLEID ((HRESULT)0x8924520b)

const int vErrorCodeCount = 56;
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
	{E_GAMESTREAMING_NOT_STREAMING_CONTROLLER, "E_GAMESTREAMING_NOT_STREAMING_CONTROLLER"},
	{E_GAMERUNTIME_OPTIONS_MISMATCH, "E_GAMERUNTIME_OPTIONS_MISMATCH"},
	{E_GAMERUNTIME_OPTIONS_NOT_SUPPORTED, "E_GAMERUNTIME_OPTIONS_NOT_SUPPORTED"},
	{E_GAMERUNTIME_GAMECONFIG_BAD_FORMAT, "E_GAMERUNTIME_GAMECONFIG_BAD_FORMAT"},
	{E_GAMERUNTIME_INVALID_HANDLE, "E_GAMERUNTIME_INVALID_HANDLE"},
	{E_INVALIDARG, "E_INVALIDARG"},
	{E_ABORT, "E_ABORT"},
	{ERROR_SUCCESS, "SUCCEEDED"}
};


// Class deMicrosoftGDK
/////////////////////////

// Constructor, destructor
////////////////////////////

deMicrosoftGdk::deMicrosoftGdk(deLoadableModule& loadableModule) :
deBaseServiceModule(loadableModule),
pGameConfig(nullptr),
pGdkInited(false),
pXblInited(false)
{
}

deMicrosoftGdk::~deMicrosoftGdk()
{
	pShutdownXbl();
	pShutdownGdk();
}


// Management
///////////////

decString deMicrosoftGdk::GetErrorCodeString(HRESULT code) const
{
	int i;
	for(i=0; i<vErrorCodeCount; i++)
	{
		if(vErrorCodes[i].code == code)
		{
			return vErrorCodes[i].string;
		}
	}

	decString message;
	message.Format("Unknown Error: %x", code);
	return message;
}

decStringSet deMicrosoftGdk::GetSupportedServices()
{
	decStringSet names;
	names.Add(deMsgdkServiceMsgdk::serviceName);
	return names;
}

deBaseServiceService* deMicrosoftGdk::CreateService(deService *service,
	const char *name, const deServiceObject::Ref &data)
{
	DEASSERT_NOTNULL(service)

	if(strcmp(name, deMsgdkServiceMsgdk::serviceName) == 0)
	{
		return new deMsgdkServiceMsgdk(*this, service, data);
	}

	return nullptr;
}

void deMicrosoftGdk::FrameUpdate(float elapsed)
{
	if(deMsgdkServiceMsgdk::GlobalService())
	{
		deMsgdkServiceMsgdk::GlobalService()->OnFrameUpdate(elapsed);
	}
}

void deMicrosoftGdk::InitSdk(const deServiceObject::Ref &data)
{
	pInitGdk(data);
	pInitXbl(data);
}

const deMsgdkGameConfig& deMicrosoftGdk::GetGameConfig() const
{
	DEASSERT_NOTNULL(pGameConfig)
	return *pGameConfig;
}


// Private Functions
//////////////////////

void deMicrosoftGdk::pInitGdk(const deServiceObject::Ref& data)
{
	if(pGdkInited)
	{
		return;
	}

	LogInfo("Initialize GDK");

	const decPath pathGameConfig(decPath::CreatePathUnix(
		data->GetChildAt("pathGameConfig")->GetString()));
	const decBaseFileReader::Ref reader(
		GetGameEngine()->GetVirtualFileSystem()->OpenFileForReading(pathGameConfig));
	const int lenGameConfig = reader->GetLength();
	char *gameConfig = nullptr;

	DEASSERT_TRUE(lenGameConfig > 0)

	try
	{
		gameConfig = new char[lenGameConfig + 1];
		reader->Read(gameConfig, lenGameConfig);
		gameConfig[lenGameConfig] = 0;

		char *useGameConfig = gameConfig;

		// tools like the game definition editor from microsoft love to add BOM
		// to the beginning of the configuration. interestingly the initialize
		// call below fails if this BOM is present. why add a BOM if the function
		// consuming the data later fails if BOM is present? detect the BOM and
		// remove it to avoid problems
		if(strncmp(useGameConfig, "\xef\xbb\xbf", 3) == 0){
			useGameConfig += 3;
		}

		pGameConfig = new deMsgdkGameConfig(*this, useGameConfig);

		XGameRuntimeOptions options{};
		options.gameConfigSource = XGameRuntimeGameConfigSource::Inline;
		options.gameConfig = useGameConfig;

		HRESULT result = XGameRuntimeInitializeWithOptions(&options);
		if(FAILED(result))
		{
			LogErrorFormat("Failed initialize GDK: %s", GetErrorCodeString(result));
			DETHROW(deeInvalidAction);
		}
		pGdkInited = true;
	}
	catch(const deException &e)
	{
		if(pGameConfig)
		{
			delete pGameConfig;
			pGameConfig = nullptr;
		}

		if(gameConfig)
		{
			delete [] gameConfig;
		}

		LogException(e);
		throw;
	}
}

void deMicrosoftGdk::pShutdownGdk()
{
	if(!pGdkInited)
	{
		return;
	}

	LogInfo("Shutdown GDK");
	XGameRuntimeUninitialize();

	if(pGameConfig)
	{
		delete pGameConfig;
		pGameConfig = nullptr;
	}
	
	pGdkInited = false;
}

void deMicrosoftGdk::pInitXbl(const deServiceObject::Ref& data)
{
	if(pXblInited)
	{
		return;
	}

	LogInfo("Initialize XBL");

	XblInitArgs args{};
	args.scid = GetGameConfig().scid;

	HRESULT result = XblInitialize(&args);
	if(FAILED(result))
	{
		LogErrorFormat("Failed initialize XBL: %s", GetErrorCodeString(result));
		DETHROW(deeInvalidAction);
	}
	pXblInited = true;
}

void deMicrosoftGdk::pShutdownXbl()
{
	if(!pXblInited)
	{
		return;
	}
	
	LogInfo("Shutdown XBL");
	bool finished = false;
	XAsyncBlock ab{};
	ab.queue = nullptr;
	ab.context = &finished;
	ab.callback = [](XAsyncBlock* ab){
		*((bool*)(ab->context)) = true;
	};
	XblCleanupAsync(&ab);
	while(!finished)
	{
		SleepEx(250, FALSE);
	}
	pXblInited = false;
}
