/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELGPDISABLEMODULEVERSION_H_
#define _DELGPDISABLEMODULEVERSION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Game profile module version disabling.
 */
class DE_DLL_EXPORT delGPDisableModuleVersion : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delGPDisableModuleVersion> Ref;
	
	
	
private:
	decString pName;
	decString pVersion;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game profile disable module version. */
	delGPDisableModuleVersion();
	
	/** \brief Create game profile disable module version. */
	delGPDisableModuleVersion( const char *name, const char *version );
	
	/** \brief Create copy of game profile disable module version. */
	delGPDisableModuleVersion( const delGPDisableModuleVersion &copy );
	
protected:
	/** \brief Clean up game profile disable module version. */
	virtual ~delGPDisableModuleVersion();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set module name. */
	void SetName( const char *name );
	
	/** \brief Module version. */
	inline const decString &GetVersion() const{ return pVersion; }
	
	/** \brief Set module version. */
	void SetVersion( const char *version );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if two disable module versions are the same.
	 * 
	 * Two disable module versions are the same if they have the same name and version.
	 */
	bool operator==( const delGPDisableModuleVersion &other ) const;
	
	/**
	 * \brief Determine if two disable module versions are not the same.
	 * 
	 * Two disable module versions are the same if they have the same name and version.
	 */
	bool operator!=( const delGPDisableModuleVersion &other ) const;
	
	/** \brief Set disable module version to another one. */
	delGPDisableModuleVersion &operator=( const delGPDisableModuleVersion &other );
	/*@}*/
};

#endif
