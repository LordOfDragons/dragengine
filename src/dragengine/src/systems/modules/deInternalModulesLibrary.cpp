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
///////////////////////////

// Constructor, destructor
////////////////////////////

deInternalModulesLibrary::deInternalModulesLibrary(deModuleSystem &modsys, const decPath &pathModules) :
pFunctions(nullptr)
{
	#ifdef OS_BEOS
	pLibHandle = 0;
	#else
	pLibHandle = nullptr;
	#endif
	
	decPath pathLibrary(pathModules);
	#ifdef OS_W32
	pathLibrary.AddComponent("demodules.dll");
	#elif defined OS_MACOS
	pathLibrary.AddComponent("libdemodules.dylib");
	#else
	pathLibrary.AddComponent("libdemodules.so");
	#endif
	
	try{
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
	FUNC_FUNCTIONS funcFunctions;
	const char * const funcName = "DEInternalModuleFunctions";
	
#ifdef OS_BEOS
	if(get_image_symbol(pLibHandle, funcName, B_SYMBOL_TYPE_TEXT, (void**)&funcFunctions) != B_OK){
		funcFunctions = nullptr;
	}
	
#elif defined HAS_LIB_DL
	funcFunctions = (FUNC_FUNCTIONS)dlsym(pLibHandle, funcName);
	
#elif defined OS_W32
	funcFunctions = (FUNC_FUNCTIONS)GetProcAddress(pLibHandle, funcName);
#endif
	
	if(!funcFunctions){
		DETHROW_INFO(deeInvalidAction, "Entry point function not found");
	}
	
	// store functions
	pFunctions = funcFunctions();
	if(!pFunctions){
		DETHROW_INFO(deeInvalidAction, "Entry point returned null functions");
	}
}
