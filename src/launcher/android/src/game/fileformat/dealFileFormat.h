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

#ifndef _DEALFILEFORMAT_H_
#define _DEALFILEFORMAT_H_

#include "../../engine/modules/dealEngineModule.h"

#include "../../deObject.h"
#include "../../common/string/decString.h"



/**
 * \brief File Format.
 */
class dealFileFormat : public deObject{
private:
	dealEngineModule::eModuleTypes pType;
	decString pPattern;
	bool pSupported;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create file format. */
	dealFileFormat();
	
	/** \brief Clean up file format. */
	virtual ~dealFileFormat();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File type. */
	inline dealEngineModule::eModuleTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( dealEngineModule::eModuleTypes type );
	
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
