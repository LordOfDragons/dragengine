/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deInternalModulesLibrary.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#ifdef OS_W32
#include "../../app/deOSWindows.h"
#endif


// Definitions
////////////////

#define LOGSOURCE "Dragengine"

typedef const deModuleSystem::FPRegisterInternalModule* (*FUNC_FUNCTIONS)();


// class deLibraryModule
//////////////////////////

// Constructor, destructor
////////////////////////////

deInternalModulesLibrary::deInternalModulesLibrary(deModuleSystem &modsys, const decPath &pathModules) :
pFunctionsPriority(nullptr),
pFunctions(nullptr)
{
	#ifdef OS_BEOS
	pLibHandle = 0;
	#else
	pLibHandle = nullptr;
	#endif
	
	decPath pathLibrary(pathModules);
	#if defined OS_W32
	pathLibrary.AddComponent("demodules.dll");
	#elif defined OS_MACOS
	pathLibrary.AddComponent("libdemodules.dylib");
	#else
	#ifdef WEB_WASM
	pathLibrary.SetEmpty();
	#endif
	pathLibrary.AddComponent("libdemodules.so");
	#endif
	
	try{
		pPreloadLibraries(modsys, pathModules);
		pLoadLibrary(modsys, pathLibrary);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deInternalModulesLibrary::~deInternalModulesLibrary(){
	pCleanUp();
}


// Private functions
//////////////////////

void deInternalModulesLibrary::pCleanUp(){
	if(pLibHandle){
		#ifdef OS_BEOS
		unload_add_on(pLibHandle);
		#elif defined HAS_LIB_DL
		dlclose(pLibHandle);
		#elif defined OS_W32
		FreeLibrary(pLibHandle);
		#endif
	}
	
	pUnloadPreloadedLibraries();
}

void deInternalModulesLibrary::pLoadLibrary(deModuleSystem &modsys, const decPath &path){
	deLogger &logger = *modsys.GetEngine()->GetLogger();
	logger.LogInfoFormat(LOGSOURCE, "Loading internal modules library: %s",
		path.GetPathNative().GetString());
	
	// load library
#ifdef OS_BEOS
	decString newEnvVarLibPath(path.GetParent().GetPathNative());
	
	const char * const checkEnvVarLibPath = getenv("LIBRARY_PATH");
	decString oldEnvVarLibPath;
	if(checkEnvVarLibPath){
		oldEnvVarLibPath = checkEnvVarLibPath;
		newEnvVarLibPath = newEnvVarLibPath + ":" + checkEnvVarLibPath;
	}
	setenv("LIBRARY_PATH", newEnvVarLibPath, 1);
	
	pLibHandle = load_add_on(filename);
	
	if(checkEnvVarLibPath){
		setenv("LIBRARY_PATH", oldEnvVarLibPath, 1);
		
	}else{
		unsetenv("LIBRARY_PATH");
	}
	
	if(pLibHandle < 0){
		logger.LogErrorFormat(LOGSOURCE, "load_add_on failed: %s", strerror(pLibHandle));
		DETHROW_INFO(deeInvalidAction, "Failed loading library");
	}
	
#elif defined HAS_LIB_DL
	pLibHandle = dlopen(path.GetPathNative(), RTLD_NOW);
	if(!pLibHandle){
		logger.LogErrorFormat(LOGSOURCE, "dlerror: %s.", dlerror());
		DETHROW_INFO(deeInvalidAction, "Failed loading library");
	}
	
#elif defined OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(path.GetParent().GetPathNative(), widePath, MAX_PATH);
	SetDllDirectory(widePath);
	deOSWindows::Utf8ToWide(path.GetPathNative(), widePath, MAX_PATH);
	pLibHandle = LoadLibrary(widePath);
	SetDllDirectory(NULL);
	if(!pLibHandle){
		int err = GetLastError();
		wchar_t messageBuffer[251];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			messageBuffer, 250, NULL);
		
		logger.LogErrorFormat(LOGSOURCE, "LoadLibrary(err=%i): %s.",
			err, deOSWindows::WideToUtf8(messageBuffer).GetString());
	}
#endif
	
	if(!pLibHandle){
		DETHROW_INFO(deeInvalidAction, "Failed loading library");
	}
	
	// look for the entry point function
	FUNC_FUNCTIONS funcFunctions = nullptr;
	FUNC_FUNCTIONS funcFunctionsPriority = nullptr;
	const char * const funcName = "DEInternalModuleFunctions";
	const char * const funcNamePriority = "DEInternalModuleFunctionsPriority";
	
#ifdef OS_BEOS
	if(get_image_symbol(pLibHandle, funcName, B_SYMBOL_TYPE_TEXT, (void**)&funcFunctions) != B_OK){
		funcFunctions = nullptr;
	}
	if(get_image_symbol(pLibHandle, funcNamePriority, B_SYMBOL_TYPE_TEXT, (void**)&funcFunctionsPriority) != B_OK){
		funcFunctionsPriority = nullptr;
	}
	
#elif defined HAS_LIB_DL
	funcFunctions = (FUNC_FUNCTIONS)dlsym(pLibHandle, funcName);
	funcFunctionsPriority = (FUNC_FUNCTIONS)dlsym(pLibHandle, funcNamePriority);
	
#elif defined OS_W32
	funcFunctions = (FUNC_FUNCTIONS)GetProcAddress(pLibHandle, funcName);
	funcFunctionsPriority = (FUNC_FUNCTIONS)GetProcAddress(pLibHandle, funcNamePriority);
#endif
	
	if(!funcFunctions || !funcFunctionsPriority){
		DETHROW_INFO(deeInvalidAction, "Entry point function not found");
	}
	
	// store functions
	pFunctions = funcFunctions();
	pFunctionsPriority = funcFunctionsPriority();
	if(!pFunctions || !pFunctionsPriority){
		DETHROW_INFO(deeInvalidAction, "Entry point returned nullptr");
	}
}

void deInternalModulesLibrary::pPreloadLibraries(deModuleSystem &modsys, const decPath &pathModules){
#ifdef OS_BEOS
	deLogger &logger = *modsys.GetEngine()->GetLogger();
	const int count = pPreloadLibraryPath.GetCount();
	image_id handleLibrary = 0;
	decPath path;
	int i;
	
	for(i=0; i<count; i++){
		const decString &filename = pPreloadLibraryPath.GetAt(i);
		path = pathModules;
		path.AddNativePath(filename);
		
		handleLibrary = load_add_on(filename);
		if(!handleLibrary){
			logger.LogErrorFormat(LOGSOURCE, "Preload %s: load_add_on failed: %s",
				filename.GetString(), strerror(pLibHandle));
			DETHROW_INFO(deeInvalidAction, "Failed preloading library");
		}
		
		pPreloadedLibraries.Add((void*)(intptr_t)handleLibrary);
		handleLibrary = 0;
		logger.LogInfoFormat(LOGSOURCE, "  - Preloaded %s", filename.GetString());
	}
	
#elif defined OS_W32
	deLogger &logger = *modsys.GetEngine()->GetLogger();
	const int count = pPreloadLibraryPath.GetCount();
	HMODULE handleLibrary = NULL;
	wchar_t widePath[MAX_PATH];
	decPath path;
	int i;
	
	for(i=0; i<count; i++){
		const decString &filename = pPreloadLibraryPath.GetAt(i);
		path = pathModules;
		path.AddNativePath(filename);
		
		deOSWindows::Utf8ToWide(path.GetParent().GetPathNative(), widePath, MAX_PATH);
		SetDllDirectory(widePath);
		deOSWindows::Utf8ToWide(path.GetPathNative(), widePath, MAX_PATH);
		handleLibrary = LoadLibrary(widePath);
		SetDllDirectory(NULL);
		
		if(!handleLibrary){
			int err = GetLastError();
			wchar_t messageBuffer[251];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				messageBuffer, 250, NULL);
			
			logger.LogErrorFormat(LOGSOURCE, "Preload %s: %s (err=%d)",
				filename.GetString(), deOSWindows::WideToUtf8(messageBuffer).GetString(), err);
			DETHROW_INFO(deeInvalidAction, "Failed preloading library");
		}
		
		pPreloadedLibraries.Add(handleLibrary);
		handleLibrary = NULL;
		logger.LogInfoFormat(LOGSOURCE, "  - Preloaded %s", filename.GetString());
	}
	
#elif defined OS_UNIX
	deLogger &logger = *modsys.GetEngine()->GetLogger();
	const int count = pPreloadLibraryPath.GetCount();
	void *handleLibrary = nullptr;
	decPath path;
	int i;
	
	for(i=0; i<count; i++){
		const decString &filename = pPreloadLibraryPath.GetAt(i);
		path = pathModules;
		path.AddNativePath(filename);
		
		handleLibrary = dlopen(filename, RTLD_NOW);
		
		if(!handleLibrary){
			logger.LogErrorFormat(LOGSOURCE, "Preload %s dlerror: %s.", filename.GetString(), dlerror());
			DETHROW_INFO(deeInvalidAction, "Failed preloading library");
		}
		
		pPreloadedLibraries.Add(handleLibrary);
		handleLibrary = nullptr;
		logger.LogInfoFormat(LOGSOURCE, "  - Preloaded %s", filename.GetString());
	}
#endif
}

void deInternalModulesLibrary::pUnloadPreloadedLibraries(){
#ifdef OS_BEOS
	int index = pPreloadedLibraries.GetCount();
	while(index-- > 0){
		unload_add_on((image_id)(intptr_t)pPreloadedLibraries.GetAt(index));
		pPreloadedLibraries.RemoveFrom(index);
	}
	
#elif defined OS_W32
	int index = pPreloadedLibraries.GetCount();
	while(index-- > 0){
		FreeLibrary((HMODULE)pPreloadedLibraries.GetAt(index));
		pPreloadedLibraries.RemoveFrom(index);
	}
	
#elif defined OS_UNIX
	int index = pPreloadedLibraries.GetCount();
	while(index-- > 0){
		dlclose(pPreloadedLibraries.GetAt(index));
		pPreloadedLibraries.RemoveFrom(index);
	}
#endif
}
