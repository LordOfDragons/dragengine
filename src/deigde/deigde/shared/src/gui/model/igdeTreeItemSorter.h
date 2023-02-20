/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDETREEITEMSORTER_H_
#define _IGDETREEITEMSORTER_H_

#include "../resources/igdeIconReference.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>


class igdeTreeItem;


/**
 * \brief IGDE UI Tree Item Sorter.
 * 
 * Provides support to sort tree items.
 * 
 * Default implementation sorts igdeTreeItem by text lexicographically.
 */
class DE_DLL_EXPORT igdeTreeItemSorter : public deObject{
private:
	bool pAscending;
	
	
	
public:
	/** \text Constructors and Destructors */
	/*@{*/
	/** \brief Create tree item sorter. */
	igdeTreeItemSorter( bool ascending = true );
	
	
	
protected:
	/**
	 * \brief Clean up tree item sorter.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTreeItemSorter();
	/*@}*/
	
	
	
public:
	/** \text Management */
	/*@{*/
	/** \brief Sort ascending. */
	inline bool GetAscending() const{ return pAscending; }
	
	/** \brief Sort ascending. */
	void SetAscending( bool ascending );
	
	
	
	/**
	 * \brief Evaluates if item1 precedes is equal to or precedes item2.
	 * 
	 * Default implementation compares item texts lexicographically using
	 * "less than or equal" if sorting ascending otherwise "greater than or equal".
	 */
	virtual bool Precedes( const igdeTreeItem &item1, const igdeTreeItem &item2 );
	/*@}*/
};

#endif
