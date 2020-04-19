/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALENGINEMODULE_H_
#define _DEALENGINEMODULE_H_

#include "parameter/dealEMParameterList.h"
#include "../../deObject.h"
#include "../../common/string/decString.h"
#include "../../common/string/unicode/decUnicodeString.h"



/**
 * @brief Game Engine Module.
 */
class dealEngineModule : public deObject{
public:
	/** \brief Error Codes. */
	enum eErrorCodes{
		/** \brief Module library loaded successfullym and the module is created and ready for usage. */
		eecSuccess,
		/** \brief Creating module failed. */
		eecCreateModuleFailed,
		
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
	
	/** Module Status. */
	enum eModuleStatus{
		/** Everything is okay. */
		emsReady,
		/** Not tested yet. */
		emsNotTested,
		/** Some kind of problem. */
		emsBroken
	};
	
	/** Module Types. */
	enum eModuleTypes{
		/** Module type is not known or invalid. */
		emtUnknown,
		/** Graphic system module. */
		emtGraphic,
		/** Audio system module. */
		emtAudio,
		/** Input system module. */
		emtInput,
		/** Network system module. */
		emtNetwork,
		/** Physics system module. */
		emtPhysics,
		/** Image system module. */
		emtImage,
		/** Video system module. */
		emtVideo,
		/** Scripting system module. */
		emtScript,
		/** Model system module. */
		emtModel,
		/** Rig system module. */
		emtRig,
		/** Skin system module. */
		emtSkin,
		/** Animation system module. */
		emtAnimation,
		/** Font system module. */
		emtFont,
		/** Crash Recovery system module. */
		emtCrashRecovery,
		/** Language pack system module. */
		emtLanguagePack,
		/** Animator system module. */
		emtAnimator,
		/** Sound system module. */
		emtSound,
		/** AI system module. */
		emtAI,
		/** Occlusion mesh module. */
		emtOcclusionMesh,
		/** \brief Synthesizer. */
		emtSynthesizer
	};
	
private:
	eModuleTypes pType;
	decString pName;
	decUnicodeString pDescription;
	decUnicodeString pAuthor;
	decString pVersion;
	decString pPattern;
	bool pIsFallback;
	int pStatus;
	int pErrorCode;
	
	decString pLibFileName;
	int pLibFileSizeShould;
	int pLibFileSizeIs;
	decString pLibFileHashShould;
	decString pLibFileHashIs;
	decString pLibFileEntryPoint;
	
	dealEMParameterList pParameterList;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine module. */
	dealEngineModule();
	/** Cleans up the engine module. */
	~dealEngineModule();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module type. */
	inline eModuleTypes GetType() const{ return pType; }
	/** Sets the module type. */
	void SetType( eModuleTypes type );
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the description. */
	inline const decUnicodeString &GetDescription () const{ return pDescription; }
	/** Sets the description. */
	void SetDescription( const decUnicodeString &description );
	/** Retrieves the author. */
	inline const decUnicodeString &GetAuthor() const{ return pAuthor; }
	/** Sets the author. */
	void SetAuthor( const decUnicodeString &author );
	/** \brief Retrieves the version. */
	inline const decString &GetVersion() const{ return pVersion; }
	/** \brief Sets the version. */
	void SetVersion( const char *version );
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	/** Sets the pattern. */
	void SetPattern( const char *pattern );
	/** Determines if this is a fallback module. */
	inline bool GetIsFallback() const{ return pIsFallback; }
	/** Sets if this is a fallback module. */
	void SetIsFallback( bool isFallback );
	/** Retrieves the module status. */
	inline int GetStatus() const{ return pStatus; }
	/** Sets the module status. */
	void SetStatus( int status );
	/** Retrieves the module error code. */
	inline int GetErrorCode() const{ return pErrorCode; }
	/** Sets the module error code. */
	void SetErrorCode( int errorCode );
	
	/** Retrieves the library file name. */
	inline const decString &GetLibFileName() const{ return pLibFileName; }
	/** Sets the library file name. */
	void SetLibFileName( const char *name );
	/** Retrieves the size in bytes the library file should have. */
	inline int GetLibFileSizeShould() const{ return pLibFileSizeShould; }
	/** Sets the size in bytes the library file should have. */
	void SetLibFileSizeShould( int size );
	/** Retrieves the size in bytes the library file actually has. */
	inline int GetLibFileSizeIs() const{ return pLibFileSizeIs; }
	/** Sets the size in bytes the library file actually has. */
	void SetLibFileSizeIs( int size );
	/** Retrieves the hash value the library file should have. */
	inline const decString &GetLibFileHashShould() const{ return pLibFileHashShould; }
	/** Sets the hash value the library file should have. */
	void SetLibFileHashShould( const char *hash );
	/** Retrieves the hash value the library file actually has. */
	inline const decString &GetLibFileHashIs() const{ return pLibFileHashIs; }
	/** Sets the hash value the library file actually has. */
	void SetLibFileHashIs( const char *hash );
	/** Retrieves the entry point name. */
	inline const decString &GetLibFileEntryPoint() const{ return pLibFileEntryPoint; }
	/** Sets the entry point name. */
	void SetLibFileEntryPoint( const char *name );
	
	/** Retrieves the parameter list. */
	inline dealEMParameterList &GetParameterList(){ return pParameterList; }
	inline const dealEMParameterList &GetParameterList() const{ return pParameterList; }
	/*@}*/
	
	/** \name Helpers */
	/*@{*/
	/**
	 * \brief Compare two module versions.
	 * \details The version strings have to consist of dot separated entries. The result
	 *          is 0 if both strings are the same, -1 if the first differing version
	 *          string part of the first version is less than the second or 1 otherwise.
	 */
	static int CompareVersion( const char *version1, const char *version2 );
	
	/** Retrieves if the given type is a single type. */
	static bool IsSingleType( eModuleTypes type );
	
	/**
	 * Retrieves the type from eModuleTypes matching the given type string.
	 * This is used to translate type strings from the XML module definitions
	 * into an engine usable type.
	 */
	static eModuleTypes GetTypeFromString( const char *typeString );
	
	/** \brief Get name for type. */
	static const char *GetStringForType( eModuleTypes type );
	/*@}*/
};

#endif
