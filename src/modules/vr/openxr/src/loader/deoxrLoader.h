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

#ifndef _DEOXRLOADER_H_
#define _DEOXRLOADER_H_

#include <dragengine/dragengine_configuration.h>

#ifdef OS_BEOS
#include <kernel/image.h>
#endif

#ifdef HAS_LIB_DL
#include <dlfcn.h>
#endif

#ifdef OS_W32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif
#include <dragengine/app/include_windows.h>
#endif

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>


class deVROpenXR;
class deoxrApiLayer;


/**
 * OpenXR library loader. Supports loading the OpenXR library as well as retrieving functions.
 */
class deoxrLoader{
private:
	deVROpenXR &pOxr;
	
	decString pRuntimeConfigFile;
	decString pRuntimeLibraryPath;
	
	#ifdef OS_BEOS
	image_id pLibHandle;
	#endif
	
	#ifdef HAS_LIB_DL
	void *pLibHandle;
	#endif
	
	#ifdef OS_W32
	HMODULE pLibHandle;
	#endif
	
	decObjectOrderedSet pApiLayers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create loader. */
	deoxrLoader(deVROpenXR &oxr);
	
	/** Clean up loader. */
	~deoxrLoader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Runtime configuration file or empty string if not found. */
	inline const decString &GetRuntimeConfigFile() const{ return pRuntimeConfigFile; }
	
	/** Runtime library path or empty string if not found. */
	inline const decString &GetRuntimeLibraryPath() const{ return pRuntimeLibraryPath; }
	
	/** Count of api layers. */
	int GetApiLayerCount() const;
	
	/** API layer at index. */
	deoxrApiLayer *GetApiLayerAt(int index) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLoadOpenXR();
	void pLoadFunctions();
	void pFindRuntimeConfigFile();
	void pReadConfig();
	void pNegotiate();
	void pFindApiLayers();
	
	#ifdef OS_UNIX
	decString pGetHomeDirectory() const;
	#endif
};

#endif

