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

#ifndef _DELFILEFORMAT_H_
#define _DELFILEFORMAT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include <dragengine/systems/deModuleSystem.h>


/**
 * \brief File Format.
 */
class DE_DLL_EXPORT delFileFormat : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<delFileFormat> Ref;
	
	
	
private:
	deModuleSystem::eModuleTypes pType;
	decString pPattern;
	bool pSupported;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file format. */
	delFileFormat();
	
	/** \brief Create file format. */
	delFileFormat( deModuleSystem::eModuleTypes type, const char *pattern );
	
protected:
	/** \brief Clean up file format. */
	virtual ~delFileFormat();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief File type. */
	inline deModuleSystem::eModuleTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( deModuleSystem::eModuleTypes type );
	
	/** \brief Pattern. */
	inline const decString &GetPattern() const{ return pPattern; }
	
	/** \brief Set pattern. */
	void SetPattern( const char *pattern );
	
	/** \brief File format is supported. */
	inline bool GetSupported() const{ return pSupported; }
	
	/** \brief Set if file format is supported. */
	void SetSupported( bool supported );
	/*@}*/
};

#endif
