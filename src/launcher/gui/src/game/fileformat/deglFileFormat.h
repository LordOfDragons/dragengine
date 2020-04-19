/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLFILEFORMAT_H_
#define _DEGLFILEFORMAT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include <dragengine/systems/deModuleSystem.h>



/**
 * @brief File Format.
 */
class deglFileFormat : public deObject{
private:
	deModuleSystem::eModuleTypes pType;
	decString pPattern;
	bool pSupported;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new file format. */
	deglFileFormat();
	/** Cleans up the file format. */
	virtual ~deglFileFormat();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the file type. */
	inline deModuleSystem::eModuleTypes GetType() const{ return pType; }
	/** Sets the type. */
	void SetType( deModuleSystem::eModuleTypes type );
	/** Retrieves the pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	/** Sets the pattern. */
	void SetPattern( const char *pattern );
	/** Determines if the file format is supported. */
	inline bool GetSupported() const{ return pSupported; }
	/** Sets if the file format is supported. */
	void SetSupported( bool supported );
	/*@}*/
};

#endif // _DEGLGAME_H_
