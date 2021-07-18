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

#ifndef _DELGAMEICONLIST_H_
#define _DELGAMEICONLIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGameIcon;


/**
 * \brief Icon icon list.
 */
class delGameIconList{
private:
	decObjectList pIcons;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	delGameIconList();
	
	/** \brief Clean up list. */
	~delGameIconList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of icons. */
	int GetCount() const;
	
	/** \brief Icon at index. */
	delGameIcon *GetAt( int index ) const;
	
	/** \brief Icon with size or nullptr if absent. */
	delGameIcon *GetWithSize( int size ) const;
	
	/** \brief Icon with larges size less than or equal or nullptr if absent. */
	delGameIcon *GetLargest( int size ) const;
	
	/** \brief Icon with smallest size larger than or equal or nullptr if absent. */
	delGameIcon *GetSmallest( int size ) const;
	
	/** \brief Icon with larges size or nullptr if absent. */
	delGameIcon *GetLargest() const;
	
	/** \brief Icon with smallest size larger than or equal or nullptr if absent. */
	delGameIcon *GetSmallest() const;
	
	/** \brief Icon is present. */
	bool Has( delGameIcon *icon ) const;
	
	/** \brief Icon with size. */
	bool HasWithSize( int size ) const;
	
	/** \brief Index of icon or -1 if absent. */
	int IndexOf( delGameIcon *icon ) const;
	
	/** \brief Index of icon with size or -1 if absent. */
	int IndexOfWithSize( int size ) const;
	
	/** \brief Add icon. */
	void Add( delGameIcon *icon );
	
	/** \brief Remove icon. */
	void Remove( delGameIcon *icon );
	
	/** \brief Remove all icons. */
	void RemoveAll();
	/*@}*/
};

#endif
