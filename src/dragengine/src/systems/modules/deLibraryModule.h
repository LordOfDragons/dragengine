/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELIBRARYMODULE_H_
#define _DELIBRARYMODULE_H_

#include "../../dragengine_configuration.h"

#include "deLoadableModule.h"

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
#include "../../app/include_windows.h"
#endif

#include "../../common/collection/decPointerList.h"

class decBaseFileReader;


/**
 * \brief Loadable Module.
 * 
 * Wraps an engine module loadable from a library file. Retrieves the
 * information from an XML module definition file. Supports loading
 * of modules from libraries on unix and windows systems.
 */
class deLibraryModule : public deLoadableModule{
public:
	/** \brief Error Codes. */
	enum eErrorCodes{
		/** \brief Module library file could not be found. */
		eecLibFileNotFound = 10,
		
		/** \brief Module library file is not a regular library file. */
		eecLibFileNotRegularFile,
		
		/** \brief Size of the module library file does not match the size in the module definitin. */
		eecLibFileSizeMismatch,
		
		/** \brief Checksum of the module library file does not match the checksum in the module definition. */
		eecLibFileCheckSumMismatch,
		
		/** \brief Module library file could not be opened. */
		eecLibFileOpenFailed,
		
		/** \brief Entry point function specified in the module definition could not be found in the library file. */
		eecLibFileEntryPointNotFound,
		
		/** \brief Creating the module using the entry point function failed. */
		eecLibFileCreateModuleFailed,
	};
	
	
	
private:
	decString pLibFileName;
	int pLibFileSize;
	decString pLibFileHash;
	decString pLibFileEntryPoint;
	
	#ifdef OS_BEOS
	image_id pLibHandle;
	#endif
	
	#ifdef HAS_LIB_DL
	void *pLibHandle;
	#endif
	
	#ifdef OS_W32
	HMODULE pLibHandle;
	#endif
	
	decStringList pPreloadLibraryPath;
	decPointerList pPreloadedLibraries;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new library module. */
	deLibraryModule( deModuleSystem *system, const char *xmlDefFilename );
	
	/** \brief Clean up loadable module. */
	virtual ~deLibraryModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Filename of the library file. */
	inline const decString &GetLibFileName() const{ return pLibFileName; }
	
	/** \brief Required size in bytes of the library file. */
	inline int GetLibFileSize() const{ return pLibFileSize; }
	
	/** \brief Required hash code of the library file. */
	inline const decString &GetLibFileHash() const{ return pLibFileHash; }
	
	/** \brief Name of the entry point function in the library file. */
	inline const decString &GetLibFileEntryPoint() const{ return pLibFileEntryPoint; }
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Loads the module from the appropriate source.
	 * 
	 * Sets the error code in both the case of success and failure and stored the module
	 * for later retrieval.
	 */
	virtual void LoadModule();
	
	/** \brief Unloads the module. Sets the module to NULL and clears the error code. */
	virtual void UnloadModule();
	/*@}*/
	
	
	
	/** \name Type Testing and Safe Casting */
	/*@{*/
	/** \brief Determines if this is a library loaded module. */
	virtual bool IsLibraryModule() const;
	
	/** \brief Cast to a library loaded rule. */
	virtual deLibraryModule *CastToLibraryModule();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit module. */
	virtual void Visit( deLoadableModuleVisitor &visitor );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	bool pLoadLibrary( const char *filename );
	bool pVerifyLibrary( const char *filename );
	void pLoadXML( const char *filename );
	void pParseXML( const char *filename, decBaseFileReader &reader );
	void pVerifyModule();
	void pPreloadLibraries();
	void pUnloadPreloadedLibraries();
};

#endif
