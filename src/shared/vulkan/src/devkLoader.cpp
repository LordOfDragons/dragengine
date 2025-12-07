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

#include "devkLoader.h"
#include "deSharedVulkan.h"
#include "devkGlobalFunctions.h"
#include "devkBasics.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// class devkLoader
/////////////////////

devkLoader::devkLoader(deSharedVulkan &vulkan) :
pVulkan(vulkan),
#ifdef OS_BEOS
pLibHandle(0)
#else
pLibHandle(NULL)
#endif
{
	try{
		// load vulkan library
		pLoadVulkan();
		vulkan.GetModule().LogInfo("Vulkan library loaded");
		
		// get function pointer query function
		#ifdef OS_BEOS
		if(get_image_symbol(pLibHandle, "vkGetInstanceProcAddr", B_SYMBOL_TYPE_TEXT, (void**)&pvkGetInstanceProcAddr) != B_OK){
			pvkGetInstanceProcAddr = NULL;
		}
		if(get_image_symbol(pLibHandle, "vkGetDeviceProcAddr", B_SYMBOL_TYPE_TEXT, (void**)&pvkGetDeviceProcAddr) != B_OK){
			pvkGetDeviceProcAddr = NULL;
		}
		#endif
		
		#ifdef HAS_LIB_DL
		pvkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(pLibHandle, "vkGetInstanceProcAddr");
		pvkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)dlsym(pLibHandle, "vkGetDeviceProcAddr");
		#endif
		
		#ifdef OS_W32
		pvkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(pLibHandle, "vkGetInstanceProcAddr");
		pvkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)GetProcAddress(pLibHandle, "vkGetDeviceProcAddr");
		#endif
		
		if(!pvkGetInstanceProcAddr){
			DETHROW_INFO(deeInvalidAction, "Function vkGetInstanceProcAddr not found");
		}
		if(!pvkGetDeviceProcAddr){
			DETHROW_INFO(deeInvalidAction, "Function vkGetDeviceProcAddr not found");
		}
		
		// load functions
		pLoadFunctions();
		vulkan.GetModule().LogInfo("Vulkan functions loaded");
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkLoader::~devkLoader(){
	pCleanUp();
}



// Management
///////////////




// Private Functions
//////////////////////

void devkLoader::pCleanUp(){
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

void devkLoader::pLoadVulkan(){
	#ifdef OS_BEOS
	pLibHandle = load_add_on("vulkan");
	
	if(pLibHandle < 0){
		DETHROW_INFO(deeInvalidAction, "Load Vulkan image failed");
	}
	#endif
	
	#ifdef HAS_LIB_DL
	pLibHandle = dlopen("libvulkan.so.1", RTLD_NOW);
	if(!pLibHandle){
		pVulkan.GetModule().LogErrorFormat("dlerror: %s.", dlerror());
		DETHROW_INFO(deeInvalidAction, "Load Vulkan library failed");
	}
	#endif
	
	#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide("vulkan-1.dll", widePath, MAX_PATH);
	pLibHandle = LoadLibrary(widePath);
	
	if(!pLibHandle){
		int err = GetLastError();
		wchar_t messageBuffer[251];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			messageBuffer, 250, NULL);
		
		pVulkan.GetModule().LogErrorFormat("LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8(messageBuffer).GetString());
		
		DETHROW_INFO(deeInvalidAction, "Load Vulkan DLL failed");
	}
	#endif
}

void devkLoader::pLoadFunctions(){
	#define GLOBAL_LEVEL_VULKAN_FUNCTION(name) \
		p##name = (PFN_##name)pvkGetInstanceProcAddr(NULL, #name); \
		if(!p##name){ \
			DETHROW_INFO(deeInvalidAction, "Function " #name " not found"); \
		}
	
	#include "devkFunctionNames.h"
}
