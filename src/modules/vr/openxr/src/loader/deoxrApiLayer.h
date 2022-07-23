/* 
 * Drag[en]gine OpenXR
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

