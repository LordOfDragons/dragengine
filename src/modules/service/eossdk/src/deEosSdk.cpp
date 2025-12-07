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

#include "deEosSdk.h"
#include "deEosSdkServiceEos.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>

#ifdef OS_UNIX
	#include <unistd.h>
	#include <linux/limits.h>
#elif defined OS_W32
	#include <dragengine/app/deOSWindows.h>
#endif

#include <eos_logging.h>



// export definition
#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *EosSdkCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// entry function
// used to create a dragonscript scripting module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////

deBaseModule *EosSdkCreateModule(deLoadableModule *loadableModule){
	try{
		return new deEosSdk(*loadableModule);
	}catch(...){
		return nullptr;
	}
}


// Callbacks
//////////////

static void fEosLogging(const EOS_LogMessage* message){
	deEosSdk * const mod = deEosSdk::globalModule;
	if(!mod){
		return;
	}
	
	switch(message->Level){
	case EOS_ELogLevel::EOS_LOG_Fatal:
		mod->LogErrorFormat("EOS Fatal: %s: %s", message->Category, message->Message);
		break;
		
	case EOS_ELogLevel::EOS_LOG_Error:
		mod->LogErrorFormat("EOS Error: %s: %s", message->Category, message->Message);
		break;
		
	case EOS_ELogLevel::EOS_LOG_Warning:
		mod->LogWarnFormat("EOS Warning: %s: %s", message->Category, message->Message);
		break;
		
	case EOS_ELogLevel::EOS_LOG_Info:
		mod->LogInfoFormat("EOS Info: %s: %s", message->Category, message->Message);
		break;
		
	case EOS_ELogLevel::EOS_LOG_Verbose:
		mod->LogInfoFormat("EOS Verbose: %s: %s", message->Category, message->Message);
		break;
		
	case EOS_ELogLevel::EOS_LOG_VeryVerbose:
		mod->LogInfoFormat("EOS VeryVerbose: %s: %s", message->Category, message->Message);
		break;
		
	default:
		mod->LogInfoFormat("EOS Log: %s: %s", message->Category, message->Message);
		break;
	}
}



// Class deEosSdk
/////////////////////

deEosSdk *deEosSdk::globalModule = nullptr;

deEosSdk::cFrameUpdater::cFrameUpdater(){}
deEosSdk::cFrameUpdater::~cFrameUpdater(){}

// Constructor, destructor
////////////////////////////

deEosSdk::deEosSdk(deLoadableModule& loadableModule) :
deBaseServiceModule(loadableModule),
pSdkInited(false){
	globalModule = this;
}

deEosSdk::~deEosSdk(){
	if(pSdkInited){
		LogInfo("Shut down EOS SDK");
		EOS_Shutdown();
	}
	
	globalModule = nullptr;
}


// Management
///////////////

decStringSet deEosSdk::GetSupportedServices(){
	decStringSet names;
	names.Add(deEosSdkServiceEos::serviceName);
	return names;
}

void deEosSdk::InitSdk(const deServiceObject::Ref &data){
	if(pSdkInited){
		return;
	}
	
	LogInfo("Initialize EOS SDK");
	
	const decString &productName = data->GetChildAt("productName")->GetString();
	const decString &productVersion = data->GetChildAt("productVersion")->GetString();
	
	EOS_InitializeOptions options = {};
	options.ApiVersion = EOS_INITIALIZE_API_LATEST;
	options.ProductName = productName.GetString();
	options.ProductVersion = productVersion.GetString();
	
	const EOS_EResult res = EOS_Initialize(&options);
	if(res != EOS_EResult::EOS_Success){
		DETHROW_INFO(deeInvalidAction, EOS_EResult_ToString(res));
	}
	
	LogInfo("EOS SDK initialized successfully");
	pSdkInited = true;
	
	EOS_Logging_SetCallback(fEosLogging);
	EOS_Logging_SetLogLevel(EOS_ELogCategory::EOS_LC_ALL_CATEGORIES, EOS_ELogLevel::EOS_LOG_VeryVerbose);
}

deBaseServiceService* deEosSdk::CreateService(deService *service,
const char *name, const deServiceObject::Ref &data){
	DEASSERT_NOTNULL(service)
	
	if(strcmp(name, deEosSdkServiceEos::serviceName) == 0){
		return new deEosSdkServiceEos(*this, service, data);
	}
	
	return nullptr;
}

void deEosSdk::FrameUpdate(float elapsed){
	const int count = pFrameUpdaters.GetCount();
	int i;
	for(i=0; i<count; i++){
		((cFrameUpdater*)pFrameUpdaters.GetAt(i))->FrameUpdate(elapsed);
	}
}

void deEosSdk::AddFrameUpdater(cFrameUpdater *updater){
	DEASSERT_NOTNULL(updater)
	DEASSERT_FALSE(pFrameUpdaters.Has(updater));
	
	pFrameUpdaters.Add(updater);
}

void deEosSdk::RemoveFrameUpdater(cFrameUpdater *updater){
	DEASSERT_NOTNULL(updater);
	
	const int index = pFrameUpdaters.IndexOf(updater);
	if(index != -1){
		pFrameUpdaters.RemoveFrom(index);
	}
}
