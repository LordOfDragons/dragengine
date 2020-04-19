/* 
 * Drag[en]gine Console Launcher
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

#ifndef _DECLENGINEMODULE_H_
#define _DECLENGINEMODULE_H_

#include "parameter/declEMParameterList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

class deLoadableModule;



/**
 * @brief Game Engine Module.
 */
class declEngineModule : public deObject{
public:
	/** Module Status. */
	enum eModuleStatus{
		/** Everything is okay. */
		emsReady,
		/** Not tested yet. */
		emsNotTested,
		/** Some kind of problem. */
		emsBroken
	};
	
private:
	int pType;
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
	
	declEMParameterList pParameterList;
	
	deLoadableModule *pLoadableModule;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new engine module. */
	declEngineModule();
	/** Cleans up the engine module. */
	~declEngineModule();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module type. */
	inline int GetType() const{ return pType; }
	/** Sets the module type. */
	void SetType( int type );
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
	inline declEMParameterList &GetParameterList(){ return pParameterList; }
	inline const declEMParameterList &GetParameterList() const{ return pParameterList; }
	
	/** Retrieves the loadable module or NULL if not set. */
	inline deLoadableModule *GetLoadableModule() const{ return pLoadableModule; }
	/** Sets the loadable module or NULL if not set. */
	void SetLoadableModule( deLoadableModule *loadableModule );
	
	/** Retrieves parameter list from the engine module. */
	void GetParamListFromEngineModule();
	/*@}*/
};

#endif
