/* 
 * Drag[en]gine Live Launcher
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

#ifndef _DELLGPDISABLEMODULEVERSION_H_
#define _DELLGPDISABLEMODULEVERSION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Game profile module version disabling.
 */
class dellGPDisableModuleVersion : public deObject{
private:
	decString pName;
	decString pVersion;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new game profile disable module version. */
	dellGPDisableModuleVersion();
	dellGPDisableModuleVersion( const char *name, const char *version );
	/** \brief Create a copy of a game profile disable module version. */
	dellGPDisableModuleVersion( const dellGPDisableModuleVersion &copy );
	/** \brief Clean up the game profile disable module version. */
	virtual ~dellGPDisableModuleVersion();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the module name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Set the module name. */
	void SetName( const char *name );
	/** \brief Retrieve the module version. */
	inline const decString &GetVersion() const{ return pVersion; }
	/** \brief Set the module version. */
	void SetVersion( const char *version );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/**
	 * \brief Determine if two disable module versions are the same.
	 * \details Two disable module versions are the same if they have the same name and version.
	 */
	bool operator==( const dellGPDisableModuleVersion &other ) const;
	/**
	 * \brief Determine if two disable module versions are not the same.
	 * \details Two disable module versions are the same if they have the same name and version.
	 */
	bool operator!=( const dellGPDisableModuleVersion &other ) const;
	/** \brief Set disable module version to another one. */
	dellGPDisableModuleVersion &operator=( const dellGPDisableModuleVersion &other );
	/*@}*/
};

#endif
