/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEEDITORMODULEDEFINITION_H_
#define _IGDEEDITORMODULEDEFINITION_H_

#include <dragengine/dragengine_configuration.h>

#if defined( HAS_LIB_DL )
#include <dlfcn.h>

#elif defined( OS_W32 )
#include <dragengine/app/include_windows.h>
#undef RemoveProp
#undef CreateFile

#else
#error No usable module headers found

#endif

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>

class igdeEditorModuleManager;
class igdeEditorModule;
class igdeEnvironment;
class decBaseFileReader;



/**
 * \brief Editor Module Definition.
 *
 * Stores the definition of an editor module including the editor
 * module itself if running. This object is initialized using the
 * XML definition file of an editor module.
 */
class igdeEditorModuleDefinition : public deObject{
public:
	/** \brief Error Codes. */
	enum eErrorCodes{
		/** \brief Module library loaded successfullym and the module is created and ready for usage. */
		eecSuccess,
		
		/** \brief Creating module failed. */
		eecCreateModuleFailed,
		
		/** \brief Module library file could not be found. */
		eecLibFileNotFound,
		
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
	igdeEditorModuleManager &pModuleManager;
	decString pLoggingName;
	
	decString pFilePath;
	
	decString pLibPath;
	int pLibSize;
	decString pLibHash;
	decString pEntryPoint;
#if defined( HAS_LIB_DL )
	void *pLibHandle;
#elif defined( OS_W32 )
	HMODULE pLibHandle;
#endif
	
	decString pID;
	decString pName;
	decString pDescription;
	decString pAuthor;
	decString pVersion;
	decString pDirectoryName;
	decString pIconSmall;
	decString pIconLarge;
	
	bool pCanLoad;
	int pErrorCode;
	igdeEditorModule *pModule;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create editor module definition. */
	igdeEditorModuleDefinition( igdeEditorModuleManager &moduleManager, const char *filePath );
	
protected:
	/** \brief Clean up editor module definition. */
	virtual ~igdeEditorModuleDefinition();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Ffile path. */
	inline const decString &GetFilePath() const{ return pFilePath; }
	
	/** \brief Path of library file. */
	inline const decString &GetLibraryPath() const{ return pLibPath; }
	
	/** \brief Library file size. */
	inline int GetLibrarySize() const{ return pLibSize; }
	
	/** \brief Library file hash. */
	inline const decString &GetLibraryHash() const{ return pLibHash; }
	
	/** \brief Name of the entry point function. */
	inline const decString &GetEntryPoint() const{ return pEntryPoint; }
	
	/** \brief ID. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Author. */
	inline const decString &GetAuthor() const{ return pAuthor; }
	
	/** \brief Version. */
	inline const decString &GetVersion() const{ return pVersion; }
	
	/** \brief Directory name for this module. */
	inline const decString &GetDirectoryName() const{ return pDirectoryName; }
	
	/** \brief Set directory name for this module. */
	void SetDirectoryName( const char *directoryName );
	
	/** \brief Icon to represent editor with. */
	inline const decString &GetIconSmall() const{ return pIconSmall; }
	
	/** \brief Icon to represent editor with. */
	inline const decString &GetIconLarge() const{ return pIconLarge; }
	
	/** \brief Error code. */
	inline int GetErrorCode() const{ return pErrorCode; }
	
	/** \brief Module can be loaded. */
	inline bool CanLoad() const{ return pCanLoad; }
	
	/** \brief Module is running. */
	bool IsModuleRunning() const;
	
	/** \brief Editor module or NULL if not running. */
	inline igdeEditorModule *GetModule() const{ return pModule; }
	
	/**
	 * \brief Load module.
	 * 
	 * After loading the Start method is called on the module. If the module succeeded loading
	 * it is available using the GetModule method. If the loading failed or starting the module
	 * failed the module is set to null and an exception is thrown.
	 */
	bool LoadModule( igdeEnvironment *environment );
	
	/** \brief Unload module. */
	void UnloadModule();
	/*@}*/
	
	
	
private:
	void pLoadFile();
	void pParseFile( decBaseFileReader &reader );
	bool pLoadLibrary();
	void pUnloadLibrary();
	bool pCreateModule( igdeEnvironment *environment );
	void pVerify();
};

#endif
