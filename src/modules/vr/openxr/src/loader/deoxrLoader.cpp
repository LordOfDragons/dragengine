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

#include "deoxrLoader.h"
#include "deoxrApiLayer.h"
#include "../deVROpenXR.h"
#include "../deoxrBasics.h"
#include "../deoxrGlobalFunctions.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deContainerFileSearch.h>
#include <dragengine/filesystem/dePathList.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/threading/deThread.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif

// has to come last to avoid openxr.h being loaded without special defines set if needed
#include "loader_interfaces.h"



// class deoxrLoader
/////////////////////

deoxrLoader::deoxrLoader(deVROpenXR &oxr) :
pOxr(oxr),
#ifdef OS_BEOS
pLibHandle(0)
#else
pLibHandle(NULL)
#endif
{
#ifdef INTERNAL_XR_LOADER
	xrGetInstanceProcAddr = nullptr;
	
	try{
		// find runtime configuration file
		pFindRuntimeConfigFile();
		if(pRuntimeConfigFile.IsEmpty()){
			DETHROW_INFO(deeInvalidAction, "runtime config file not found");
		}
		
		// read runtime config file to get library file to load
		pReadConfig();
		
		// load oxr library
		pLoadOpenXR();
		oxr.LogInfo("OpenXR library loaded");
		
		// negotiate
		pNegotiate();
		oxr.LogInfo("OpenXR library negotiated");
		if(! xrGetInstanceProcAddr){
			DETHROW_INFO(deeInvalidAction, "runtime provided no xrGetInstanceProcAddr");
		}
		
		// load functions
		pLoadFunctions();
		oxr.LogInfo("OpenXR functions loaded");
		
		// find api layers
		pFindApiLayers();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
#endif // INTERNAL_XR_LOADER
}

deoxrLoader::~deoxrLoader(){
	pCleanUp();
}



// Management
///////////////

int deoxrLoader::GetApiLayerCount() const{
	return pApiLayers.GetCount();
}

deoxrApiLayer *deoxrLoader::GetApiLayerAt(int index) const{
	return (deoxrApiLayer*)pApiLayers.GetAt(index);
}



// Private Functions
//////////////////////

void deoxrLoader::pCleanUp(){
#ifdef INTERNAL_XR_LOADER
	if(pOxr.GetLastDetectedSystem() == deoxrSystem::esSteamVR){
		// SteamVR hangs in a dead-loop on pthrad condition releasing. there is no
		// known workaround for this bug. not calling dlclose pushes the deadlock
		// further back but does not work. when will they fix this mess?
#ifdef HAS_LIB_DL
		if(pLibHandle){
			pOxr.LogWarnFormat("SteamVR dlcose Bug Workaround");
// 			return;
			class SteamVRdlcloseBugWorkaround : public deThread{
				void *pLibrary;
				
			public:
				SteamVRdlcloseBugWorkaround(void *library) : pLibrary(library){}
				
				void Run(){
					try{
						dlclose(pLibrary);
					}catch(...){}
				}
			} workaround(pLibHandle);
			
			workaround.Start();
			
			decTimer timer;
			while(workaround.IsRunning() && timer.GetElapsedTime() > 1.5f);
			try{
				workaround.Stop();
			}catch(...){}
		}
#endif // HAS_LIB_DL
	}
#endif // INTERNAL_XR_LOADER
	
	pApiLayers.RemoveAll();
	
	if(pLibHandle){
		#ifdef OS_BEOS
		unload_add_on(pLibHandle);
		#endif
		
		#ifdef HAS_LIB_DL
		dlclose(pLibHandle);
		#endif
		
		#ifdef OS_W32
		FreeLibrary(pLibHandle);
		#endif
	}
}

void deoxrLoader::pLoadOpenXR(){
	pOxr.LogInfoFormat("Loading runtime: %s", pRuntimeLibraryPath.GetString());
	
	#ifdef OS_BEOS
	pLibHandle = load_add_on("openxr");
	
	if(pLibHandle < 0){
		DETHROW_INFO(deeInvalidAction, "Load OpenXR image failed");
	}
	#endif
	
	#ifdef HAS_LIB_DL
	pLibHandle = dlopen(pRuntimeLibraryPath, RTLD_NOW);
	if(! pLibHandle){
		pOxr.LogErrorFormat("dlerror: %s.", dlerror());
		DETHROW_INFO(deeInvalidAction, "Load OpenXR library failed");
	}
	#endif
	
	#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(pRuntimeLibraryPath, widePath, MAX_PATH);
	pLibHandle = LoadLibrary(widePath);
	
	if(! pLibHandle){
		int err = GetLastError();
		wchar_t messageBuffer[251];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			messageBuffer, 250, NULL);
		
		pOxr.LogErrorFormat("LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8(messageBuffer).GetString());
		
		DETHROW_INFO(deeInvalidAction, "Load OpenXR DLL failed");
	}
	#endif
}

void deoxrLoader::pLoadFunctions(){
#ifdef INTERNAL_XR_LOADER
	#define GLOBAL_LEVEL_OPENXR_FUNCTION(name) \
		if(xrGetInstanceProcAddr(XR_NULL_HANDLE, #name, (PFN_xrVoidFunction*)&name) != XR_SUCCESS){ \
			DETHROW_INFO(deeInvalidAction, "Function " #name " not found"); \
		}
	#define GLOBAL_LEVEL_OPENXR_FUNCTION_WARN(name) \
		if(xrGetInstanceProcAddr(XR_NULL_HANDLE, #name, (PFN_xrVoidFunction*)&name) != XR_SUCCESS){ \
			pOxr.LogInfoFormat("Function " #name " not found! Broken VR Runtime!"); \
		}
	
	#include "../deoxrFunctionNames.h"
#endif
}

void deoxrLoader::pFindRuntimeConfigFile(){
	#ifdef OS_UNIX
	const decPath home(decPath::CreatePathUnix(pGetHomeDirectory()));
	decStringList directories;
	
	const char *envPath = getenv("XDG_CONFIG_HOME");
	if(envPath){
		directories += decString(envPath).Split(':');
	}
	
	envPath = getenv("XDG_CONFIG_DIRS");
	if(envPath){
		directories += decString(envPath).Split(':');
	}
	
	directories += "~/.config";
	directories += "/etc";
	
	const int count = directories.GetCount();
	const deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::NewWith(decPath::CreatePathUnix("/"), decPath::CreatePathUnix("/"), true));
	const decPath childPath(decPath::CreatePathUnix("openxr/1/active_runtime.json"));
	int i;
	
	for(i=0; i<count; i++){
		decPath path(decPath::CreatePathUnix(directories.GetAt(i)));
		if(path.GetComponentCount() > 0 && path.GetComponentAt(0) == "~"){
			path.RemoveComponentFrom(0);
			path = home + path;
		}
		
		path += childPath;
		if(container->CanReadFile(path)){
			pRuntimeConfigFile = path.GetPathUnix();
			return;
		}
	}
	
	#elif defined OS_W32
	pRuntimeConfigFile = deOSWindows::GetRegistryValueCurrentUser("SOFTWARE\\Khronos\\OpenXR\\1", "ActiveRuntime", "");
	if(pRuntimeConfigFile.IsEmpty()){
		pRuntimeConfigFile = deOSWindows::GetRegistryValue("SOFTWARE\\Khronos\\OpenXR\\1", "ActiveRuntime", "");
	}
	#endif
}

void deoxrLoader::pReadConfig(){
	pOxr.LogInfoFormat("Reading runtime configuration: %s", pRuntimeConfigFile.GetString());
	
	const decDiskFileReader::Ref reader(decDiskFileReader::Ref::NewWith(pRuntimeConfigFile));
	const int contentLength = reader->GetLength();
	decString content;
	content.Set(' ', contentLength);
	reader->Read((void*)content.GetString(), contentLength);
	
	int index = content.FindString("\"library_path\"");
	if(index == -1){
		DETHROW_INFO(deeInvalidFileFormat, "invalid config file format");
	}
	
	index = content.Find(':', index + 1);
	if(index == -1){
		DETHROW_INFO(deeInvalidFileFormat, "invalid config file format");
	}
	
	index = content.Find('"', index + 1);
	if(index == -1){
		DETHROW_INFO(deeInvalidFileFormat, "invalid config file format");
	}
	
	int index2 = content.Find('"', index + 1);
	if(index2 == -1){
		DETHROW_INFO(deeInvalidFileFormat, "invalid config file format");
	}
	
	// library file can be absolute or relative
	decPath configDir(decPath::CreatePathNative(pRuntimeConfigFile));
	configDir.RemoveLastComponent();
	
	pRuntimeLibraryPath = decPath::CreatePathNative(content.GetMiddle(index + 1, index2)).
		AbsolutePath(configDir).GetPathNative();
}

void deoxrLoader::pNegotiate(){
	// get function pointer
	PFN_xrNegotiateLoaderRuntimeInterface fNegotiate = nullptr;
	
	#ifdef OS_BEOS
	if(get_image_symbol(pLibHandle, "xrNegotiateLoaderRuntimeInterface",
	B_SYMBOL_TYPE_TEXT, (void**)&fNegotiate) != B_OK){
		fNegotiate = nullptr;
	}
	#endif
	
	#ifdef HAS_LIB_DL
	fNegotiate = (PFN_xrNegotiateLoaderRuntimeInterface)dlsym(pLibHandle, "xrNegotiateLoaderRuntimeInterface");
	#endif
	
	#ifdef OS_W32
	fNegotiate = (PFN_xrNegotiateLoaderRuntimeInterface)GetProcAddress(pLibHandle, "xrNegotiateLoaderRuntimeInterface");
	#endif
	
	if(! fNegotiate){
		DETHROW_INFO(deeInvalidAction, "Function xrNegotiateLoaderRuntimeInterface not found");
	}
	
#ifdef INTERNAL_XR_LOADER
	// negotiate
	// https://www.khronos.org/registry/OpenXR/specs/1.0/loader.html#loader-runtime-interface-negotiation
	XrNegotiateLoaderInfo loaderInfo;
	memset(&loaderInfo, 0, sizeof(loaderInfo));
	loaderInfo.structType = XR_LOADER_INTERFACE_STRUCT_LOADER_INFO;
	loaderInfo.structVersion = XR_LOADER_INFO_STRUCT_VERSION;
	loaderInfo.structSize = sizeof(loaderInfo);
	loaderInfo.minInterfaceVersion = 1;
	loaderInfo.maxInterfaceVersion = XR_CURRENT_LOADER_API_LAYER_VERSION;
	loaderInfo.minApiVersion = XR_MAKE_VERSION(1, 0, 0);
	loaderInfo.maxApiVersion = XR_MAKE_VERSION(1, 0, 0);
	
	XrNegotiateRuntimeRequest runtimeRequest;
	memset(&runtimeRequest, 0, sizeof(runtimeRequest));
	runtimeRequest.structType = XR_LOADER_INTERFACE_STRUCT_RUNTIME_REQUEST;
	runtimeRequest.structVersion = XR_RUNTIME_INFO_STRUCT_VERSION;
	runtimeRequest.structSize = sizeof(runtimeRequest);
	
	OXR_CHECK(fNegotiate(&loaderInfo, &runtimeRequest));
	xrGetInstanceProcAddr = runtimeRequest.getInstanceProcAddr;
#endif
}

void deoxrLoader::pFindApiLayers(){
	#ifdef OS_UNIX
	class cLoadApiLayer : public deContainerFileSearch{
		deVROpenXR &pOxr;
		const decPath pBasePath;
		decObjectOrderedSet &pApiLayers;
		
	public:
		cLoadApiLayer(deVROpenXR &oxr, const decPath &basePath, decObjectOrderedSet &apiLayers) :
			pOxr(oxr), pBasePath(basePath), pApiLayers(apiLayers){}
		
		void Add(const char *name, deVFSContainer::eFileTypes type){
			if(type != deVFSContainer::eftRegularFile){
				return;
			}
			
			decPath path(pBasePath);
			path.AddUnixPath(name);
			
			const decString filename(path.GetPathNative());
			pOxr.LogInfoFormat("Loading API layer config file: %s", filename.GetString());
			try{
				pApiLayers.Add(deoxrApiLayer::Ref::NewWith(pOxr, filename));
				
			}catch(const deException &e){
				pOxr.LogException(e);
			}
		}
		
		void Remove(const char *){}
	};
	
	const decPath home(decPath::CreatePathUnix(pGetHomeDirectory()));
	decStringList directories;
	
	const char *envPath = getenv("XDG_CONFIG_HOME");
	if(envPath){
		directories += decString(envPath).Split(':');
	}
	
	envPath = getenv("XDG_CONFIG_DIRS");
	if(envPath){
		directories += decString(envPath).Split(':');
	}
	
	directories += "~/.config";
	directories += "/etc";
	
	const int count = directories.GetCount();
	const deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::NewWith(decPath::CreatePathUnix("/"), decPath::CreatePathUnix("/"), true));
	const decPath childPath(decPath::CreatePathUnix("openxr/1/api_layers/explicit.d"));
	int i;
	
	for(i=0; i<count; i++){
		decPath path(decPath::CreatePathUnix(directories.GetAt(i)));
		if(path.GetComponentCount() > 0 && path.GetComponentAt(0) == "~"){
			path.RemoveComponentFrom(0);
			path = home + path;
		}
		
		path += childPath;
		
		if(container->ExistsFile(path)){
			cLoadApiLayer loadApiLayer(pOxr, path, pApiLayers);
			container->SearchFiles(path, loadApiLayer);
		}
	}
	
	#elif defined OS_W32
	// TODO
	// enum DWORD keys under: HKEY_LOCAL_MACHINE\SOFTWARE\Khronos\OpenXR\1\ApiLayers\Explicit
	// enum DWORD keys under: HKEY_CURRENT_USER\SOFTWARE\Khronos\OpenXR\1\ApiLayers\Explicit
	// each key looks like this: "C:\vendor a\layer_a.json"=dword:00000000
	// key name is file name and key value is 0 to enable layer and 1 to disable
	
	// HKEY hKey;
	// RegOpenKeyExA( HKEY_CURRENT_USER, key, 0, KEY_READ, &hKey )
	
	/*
	pRuntimeConfigFile = deOSWindows::GetRegistryValueCurrentUser("SOFTWARE\\Khronos\\OpenXR\\1", "ActiveRuntime", "");
	if(pRuntimeConfigFile.IsEmpty()){
		pRuntimeConfigFile = deOSWindows::GetRegistryValue("SOFTWARE\\Khronos\\OpenXR\\1", "ActiveRuntime", "");
	}
	*/
	#endif
}

#ifdef OS_UNIX
decString deoxrLoader::pGetHomeDirectory() const{
	const char *envPath = getenv("HOME");
	if(envPath){
		return decString(envPath);
	}
	
	envPath = getenv("USER");
	if(envPath){
		return decString("/home/") + envPath;
	}
	
	envPath = getenv("LOGUSER");
	if(envPath){
		return decString("/home/") + envPath;
	}
	
	DETHROW_INFO(deeInvalidParam, "home directory not found");
}
#endif
