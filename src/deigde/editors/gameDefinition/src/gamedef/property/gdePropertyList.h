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

#ifndef _GDEPROPERTYLIST_H_
#define _GDEPROPERTYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeProperty;



/**
 * \brief Property list.
 */
class gdePropertyList{
private:
	decObjectOrderedSet pProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create property list. */
	gdePropertyList();
	
	/** \brief Create copy of property list. */
	gdePropertyList( const gdePropertyList &list );
	
	/** \brief Clean up property list. */
	~gdePropertyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of propertyes. */
	int GetCount() const;
	
	/** \brief Property at position. */
	gdeProperty *GetAt( int index ) const;
	
	/** \brief Named property or \em NULL if absent. */
	gdeProperty *GetNamed( const char *name ) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOf( gdeProperty *proeprty ) const;
	
	/** \brief Index of named property or -1 if absent. */
	int IndexOfNamed( const char *name ) const;
	
	/** \brief Property is present. */
	bool Has( gdeProperty *proeprty ) const;
	
	/** \brief Named property is present. */
	bool HasNamed( const char *name ) const;
	
	/** \brief Add property. */
	void Add( gdeProperty *proeprty );
	
	/** \brief Remove property. */
	void Remove( gdeProperty *proeprty );
	
	/** \brief Remove all propertyes. */
	void RemoveAll();
	
	/** \brief Set from another property list. */
	gdePropertyList &operator=( const gdePropertyList &list );
	/*@}*/
};

#endif
