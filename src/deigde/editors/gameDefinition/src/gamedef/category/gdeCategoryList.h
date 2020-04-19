/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDECATEGORYLIST_H_
#define _GDECATEGORYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeCategory;



/**
 * \brief Category list.
 */
class gdeCategoryList{
private:
	decObjectOrderedSet pCategories;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create category list. */
	gdeCategoryList();
	
	/** \brief Create copy of category list. */
	gdeCategoryList( const gdeCategoryList &list );
	
	/** \brief Clean up category list. */
	~gdeCategoryList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of categoryes. */
	int GetCount() const;
	
	/** \brief Property at position. */
	gdeCategory *GetAt( int index ) const;
	
	/** \brief Named category or \em NULL if absent. */
	gdeCategory *GetNamed( const char *name ) const;
	
	/** \brief Category with path or \em NULL if absent. */
	gdeCategory *GetWithPath( const char *path ) const;
	
	/** \brief Index of named category or -1 if absent. */
	int IndexOf( gdeCategory *proeprty ) const;
	
	/** \brief Index of named category or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Property is present. */
	bool Has( gdeCategory *proeprty ) const;
	
	/** \brief Named category is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add category. */
	void Add( gdeCategory *proeprty );
	
	/** \brief Remove category. */
	void Remove( gdeCategory *proeprty );
	
	/** \brief Remove all categoryes. */
	void RemoveAll();
	
	/** \brief Set from another category list. */
	gdeCategoryList &operator=( const gdeCategoryList &list );
	/*@}*/
};

#endif
