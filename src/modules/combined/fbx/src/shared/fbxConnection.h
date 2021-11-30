/* 
 * FBX Modules
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

#ifndef _FBXCONNECTION_H_
#define _FBXCONNECTION_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>


class deBaseModule;


/**
 * \brief FBX connection.
 */
class fbxConnection : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<fbxConnection> Ref;
	
	
	
private:
	int64_t pSource;
	int64_t pTarget;
	decString pProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create connection. */
	fbxConnection( int64_t source, int64_t target, const char *property = "" );
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxConnection();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Source ID. */
	inline int64_t GetSource() const{ return pSource; }
	
	/** \brief Target ID. */
	inline int64_t GetTarget() const{ return pTarget; }
	
	/** \brief Other ID. */
	int64_t OtherID( int64_t id ) const;
	
	/** \brief Property or empty string. */
	inline const decString &GetProperty() const{ return pProperty; }
	
	/** \brief Debug print property structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix ) const;
	/*@}*/
};

#endif
