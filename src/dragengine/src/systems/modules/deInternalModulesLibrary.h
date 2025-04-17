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

#ifndef _DEINTERNALMODULESLIBRARY_H_
#define _DEINTERNALMODULESLIBRARY_H_

#include "../deModuleSystem.h"
#include "../../dragengine_configuration.h"
#include "../../common/file/decPath.h"
#include "../../common/collection/decPointerList.h"

#ifdef OS_BEOS
#include <kernel/image.h>
#elif defined HAS_LIB_DL
#include <dlfcn.h>
#elif defined OS_W32
#include "../../app/include_windows.h"
#endif

/**
 * \brief Internal modules library.
 */
class deInternalModulesLibrary{
private:
	#ifdef OS_BEOS
	image_id pLibHandle;
	#elif defined HAS_LIB_DL
	void *pLibHandle;
	#elif defined OS_W32
	HMODULE pLibHandle;
	#endif
	
	decStringList pPreloadLibraryPath;
	decPointerList pPreloadedLibraries;
	
	const deModuleSystem::FPRegisterInternalModule *pFunctions;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create internal modules. */
	deInternalModulesLibrary(deModuleSystem &modsys, const decPath &pathModules);
	
	/** \brief Clean up internal modules. */
	~deInternalModulesLibrary();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of function pointers. */
	inline const deModuleSystem::FPRegisterInternalModule* GetFunctions() const{ return pFunctions; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLoadLibrary(deModuleSystem &modsys, const decPath &path);
	void pPreloadLibraries(deModuleSystem &modsys, const decPath &pathModules);
	void pUnloadPreloadedLibraries();
};

#endif
