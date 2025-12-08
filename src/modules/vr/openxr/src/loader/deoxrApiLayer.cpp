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

#include "deoxrApiLayer.h"
#include "loader_interfaces.h"
#include "../deVROpenXR.h"
#include "../deoxrBasics.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/threading/deThread.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// class deoxrApiLayer
/////////////////////

deoxrApiLayer::deoxrApiLayer(deVROpenXR &oxr, const char *configFile) :
pOxr(oxr),
pConfigFile(configFile),
#ifdef OS_BEOS
pLibHandle(0)
#else
pLibHandle(NULL)
#endif
{
	try{
		pReadConfig();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrApiLayer::~deoxrApiLayer(){
	pCleanUp();
}



// Management
///////////////

bool deoxrApiLayer::IsLoaded() const{
	return pLibHandle;
}

void deoxrApiLayer::LoadLibrary(){
	if(pLibHandle){
		return;
	}
	
	try{
		pLoadLibrary();
		pOxr.LogInfoFormat("API layer library loaded: %s", pName.GetString());
		
		pNegotiate();
		pOxr.LogInfoFormat("API library negotiated: %s", pName.GetString());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

void deoxrApiLayer::UnloadLibrary(){
	if(!pLibHandle){
		return;
	}
	
	pOxr.LogInfoFormat("Unload API layer library: %s", pLibraryPath.GetString());
	#ifdef OS_BEOS
	unload_add_on(pLibHandle);
	#endif
	
	#ifdef HAS_LIB_DL
	dlclose(pLibHandle);
	#endif
	
	#ifdef OS_W32
	FreeLibrary(pLibHandle);
	#endif
	
	pLibHandle = nullptr;
}



// Private Functions
//////////////////////

void deoxrApiLayer::pCleanUp(){
	UnloadLibrary();
}

void deoxrApiLayer::pLoadLibrary(){
	pOxr.LogInfoFormat("Load API layer library: %s", pLibraryPath.GetString());
	#ifdef OS_BEOS
	pLibHandle = load_add_on(pLibraryPath);
	
	if(pLibHandle < 0){
		DETHROW_INFO(deeInvalidAction, "Load image failed");
	}
	#endif
	
	#ifdef HAS_LIB_DL
	pLibHandle = dlopen(pLibraryPath, RTLD_NOW);
	if(!pLibHandle){
		pOxr.LogErrorFormat("dlerror: %s.", dlerror());
		DETHROW_INFO(deeInvalidAction, "Load library failed");
	}
	#endif
	
	#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(pLibraryPath, widePath, MAX_PATH);
	pLibHandle = ::LoadLibrary(widePath);
	
	if(!pLibHandle){
		int err = GetLastError();
		wchar_t messageBuffer[251];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			messageBuffer, 250, NULL);
		
		pOxr.LogErrorFormat("LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8(messageBuffer).GetString());
		
		DETHROW_INFO(deeInvalidAction, "Load DLL failed");
	}
	#endif
}

void deoxrApiLayer::pReadConfig(){
	const decDiskFileReader::Ref reader(decDiskFileReader::Ref::NewWith(pConfigFile));
	const int contentLength = reader->GetLength();
	decString content;
	content.Set(' ', contentLength);
	reader->Read((void*)content.GetString(), contentLength);
	
	// name
	int index = content.FindString("\"name\"");
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
	
	pName = content.GetMiddle(index + 1, index2);
	
	// library
	index = content.FindString("\"library_path\"");
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
	
	index2 = content.Find('"', index + 1);
	if(index2 == -1){
		DETHROW_INFO(deeInvalidFileFormat, "invalid config file format");
	}
	
	// library file can be absolute or relative
	decPath configDir(decPath::CreatePathNative(pConfigFile));
	configDir.RemoveLastComponent();
	
	pLibraryPath.TakeOver(decPath::CreatePathNative(content.GetMiddle(index + 1, index2)).
		AbsolutePath(configDir).GetPathNative();
}

void deoxrApiLayer::pNegotiate(){
	// get function pointer
	PFN_xrNegotiateLoaderApiLayerInterface fNegotiate = nullptr;
	
	#ifdef OS_BEOS
	if(get_image_symbol(pLibHandle, "xrNegotiateLoaderApiLayerInterface",
	B_SYMBOL_TYPE_TEXT, (void**)&fNegotiate) != B_OK){
		fNegotiate = nullptr;
	}
	#endif
	
	#ifdef HAS_LIB_DL
	fNegotiate = (PFN_xrNegotiateLoaderApiLayerInterface)dlsym(pLibHandle, "xrNegotiateLoaderApiLayerInterface");
	#endif
	
	#ifdef OS_W32
	fNegotiate = (PFN_xrNegotiateLoaderApiLayerInterface)GetProcAddress(pLibHandle, "xrNegotiateLoaderApiLayerInterface");
	#endif
	
	if(!fNegotiate){
		DETHROW_INFO(deeInvalidAction, "Function xrNegotiateLoaderApiLayerInterface not found");
	}
	
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
	loaderInfo.maxApiVersion = XR_MAKE_VERSION(1, 0x3ff, 0xfff);  // Maximum allowed version for this major version.
	
	XrNegotiateApiLayerRequest layerRequest;
	memset(&layerRequest, 0, sizeof(layerRequest));
	layerRequest.structType = XR_LOADER_INTERFACE_STRUCT_API_LAYER_REQUEST;
	layerRequest.structVersion = XR_API_LAYER_INFO_STRUCT_VERSION;
	layerRequest.structSize = sizeof(layerRequest);
	
	OXR_CHECK(fNegotiate(&loaderInfo, pName, &layerRequest));
	if(!layerRequest.getInstanceProcAddr){
		DETHROW_INFO(deeNullPointer, "negotiate.getInstanceProcAddr");
	}
}
