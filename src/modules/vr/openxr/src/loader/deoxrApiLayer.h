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

#ifndef _DEOXRAPILAYER_H_
#define _DEOXRAPILAYER_H_

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

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>


class deVROpenXR;
class decStringList;
class decPath;


/**
 * OpenXR api layer. Supports loading and unloading the layer library.
 */
class deoxrApiLayer : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrApiLayer> Ref;
	
	
private:
	deVROpenXR &pOxr;
	decString pConfigFile;
	decString pName;
	decString pLibraryPath;
	
	#ifdef OS_BEOS
	image_id pLibHandle;
	#endif
	
	#ifdef HAS_LIB_DL
	void *pLibHandle;
	#endif
	
	#ifdef OS_W32
	HMODULE pLibHandle;
	#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create loader. */
	deoxrApiLayer( deVROpenXR &oxr, const char *configFile );
	
	/** Clean up loader. */
	virtual ~deoxrApiLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Configuration file. */
	inline const decString &GetConfigFile() const{ return pConfigFile; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Library path. */
	inline const decString &GetLibraryPath() const{ return pLibraryPath; }
	
	/** Library is loaded. */
	bool IsLoaded() const;
	
	/** Load library if not loaded. */
	void LoadLibrary();
	
	/** Unload library if loaded. */
	void UnloadLibrary();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pLoadLibrary();
	void pReadConfig();
	void pNegotiate();
};

#endif

