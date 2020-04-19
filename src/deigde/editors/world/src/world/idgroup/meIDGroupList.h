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

#ifndef _MEIDGROUPLIST_H_
#define _MEIDGROUPLIST_H_

#include <dragengine/common/collection/decObjectDictionary.h>

class meIDGroup;
class decStringList;


/**
 * \brief Shared identifier list.
 */
class meIDGroupList{
private:
	decObjectDictionary pGroups;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create id group list. */
	meIDGroupList();
	
	/** \brief Clean up id group list. */
	~meIDGroupList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of groups. */
	int GetCount() const;
	
	/** \brief Named id group is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Named id group or \em NULL if absent. */
	meIDGroup *GetNamed( const char *name ) const;
	
	/** \brief Named id group creating it if absent. */
	meIDGroup *GetOrAddNamed( const char *name );
	
	/** \brief Remove all id groups. */
	void RemoveAll();
	
	/** \brief Names. */
	decStringList GetNames() const;
	/*@}*/
};

#endif
