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
class DE_DLL_EXPORT deLibraryModule : public deLoadableModule{
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
	deLibraryModule(deModuleSystem *system, const char *xmlDefFilename);
	
	/** \brief Clean up loadable module. */
	virtual ~deLibraryModule();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Filename of the library file. */
	inline const decString &GetLibFileName() const{return pLibFileName;}
	
	/** \brief Required size in bytes of the library file. */
	inline int GetLibFileSize() const{return pLibFileSize;}
	
	/** \brief Required hash code of the library file. */
	inline const decString &GetLibFileHash() const{return pLibFileHash;}
	
	/** \brief Name of the entry point function in the library file. */
	inline const decString &GetLibFileEntryPoint() const{return pLibFileEntryPoint;}
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
	virtual void Visit(deLoadableModuleVisitor &visitor);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	bool pLoadLibrary(const char *filename);
	bool pVerifyLibrary(const char *filename);
	void pLoadXML(const char *filename);
	void pParseXML(const char *filename, decBaseFileReader &reader);
	void pVerifyModule();
	void pPreloadLibraries();
	void pUnloadPreloadedLibraries();
};

#endif
