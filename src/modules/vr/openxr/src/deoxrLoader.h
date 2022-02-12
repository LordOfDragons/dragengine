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
// class deoxrApiLayer;


/**
 * OpenXR library loader. Supports loading the OpenXR library as well as retrieving functions.
 */
class deoxrLoader{
private:
	deVROpenXR &pOxr;
	
#if 0
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
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create loader. */
	deoxrLoader( deVROpenXR &oxr );
	
	/** Clean up loader. */
	~deoxrLoader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
#if 0
	/** Runtime configuration file or empty string if not found. */
	inline const decString &GetRuntimeConfigFile() const{ return pRuntimeConfigFile; }
	
	/** Runtime library path or empty string if not found. */
	inline const decString &GetRuntimeLibraryPath() const{ return pRuntimeLibraryPath; }
	
	/** Count of api layers. */
	int GetApiLayerCount() const;
	
	/** API layer at index. */
	deoxrApiLayer *GetApiLayerAt( int index ) const;
#endif
	/*@}*/
	
	
	
private:
	void pCleanUp();
#if 0
	void pLoadOpenXR();
	void pLoadFunctions();
	void pFindRuntimeConfigFile();
	void pReadConfig();
	void pNegotiate();
	void pFindApiLayers();
	
	#ifdef OS_UNIX
	decString pGetHomeDirectory() const;
	#endif
#endif
};

#endif

