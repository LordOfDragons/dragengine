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

#ifndef _MEIDGROUP_H_
#define _MEIDGROUP_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/string/decString.h>

class meIDGroupID;


/**
 * \brief Identifier group.
 */
class meIDGroup : public deObject{
private:
	decString pName;
	decObjectDictionary pIDs;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create id group. */
	meIDGroup( const char *name );
	
	/** \brief Clean up id group. */
	virtual ~meIDGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Identifier. */
	inline const decString &GetName() const{ return pName; }
	
	
	
	/** \brief Number of identifiers. */
	int GetCount() const;
	
	/** \brief List of identifiers. */
	decStringList GetIDList() const;
	
	/** \brief Get identifier usage count or 0 if absent. */
	int GetUsageCountFor( const char *id ) const;
	
	/** \brief Identifier is present. */
	bool Has( const char *id ) const;
	
	/** \brief Increase usage count of identifier adding it if absent. */
	void Add( const char *id );
	
	/** \brief Decrease usage count of identifier removing it if dropping to 0. */
	void Remove( const char *id );
	
	/** \brief Remove all identifiers. */
	void RemoveAll();
	/*@}*/
};

#endif
