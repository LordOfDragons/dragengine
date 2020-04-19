/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEIDGROUPID_H_
#define _MEIDGROUPID_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/deObject.h>


/**
 * \brief Identifier group identifier.
 */
class meIDGroupID : public deObject{
private:
	decString pID;
	int pUsageCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create identifier group identifier with usage count 1. */
	meIDGroupID( const char *id );
	
	/** \brief Clean up identifier group identifier. */
	virtual ~meIDGroupID();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Usage count. */
	inline int GetUsageCount() const{ return pUsageCount; }
	
	/** \brief Increment usage count by one. */
	void Increment();
	
	/** \brief Decrement usage count by one if larger than 0. */
	void Decrement();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Objects are equal. */
	bool operator==( const meIDGroupID &id ) const;
	/*@}*/
};

#endif
