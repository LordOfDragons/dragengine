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

#ifndef _DEALGPDISABLEMODULEVERSION_H_
#define _DEALGPDISABLEMODULEVERSION_H_

#include "../../deObject.h"
#include "../../common/string/decString.h"



/**
 * \brief Game profile module version disabling.
 */
class dealGPDisableModuleVersion : public deObject{
private:
	decString pName;
	decString pVersion;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create disabled module version. */
	dealGPDisableModuleVersion();
	
	/** \brief Create disabled module version. */
	dealGPDisableModuleVersion( const char *name, const char *version );
	
	/** \brief Create copy of disabled module version. */
	dealGPDisableModuleVersion( const dealGPDisableModuleVersion &copy );
	
	/** \brief Clean up disabled module version. */
	virtual ~dealGPDisableModuleVersion();
	/*@}*/
	
	
	
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
	 * \brief Disable module versions share the same name and version.
	 */
	bool operator==( const dealGPDisableModuleVersion &other ) const;
	
	/**
	 * \brief Two disable module versions do not share the same name and version.
	 */
	bool operator!=( const dealGPDisableModuleVersion &other ) const;
	
	/** \brief Set disable module version to another one. */
	dealGPDisableModuleVersion &operator=( const dealGPDisableModuleVersion &other );
	/*@}*/
};

#endif
