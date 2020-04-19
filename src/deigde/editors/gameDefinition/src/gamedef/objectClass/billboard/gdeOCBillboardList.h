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

#ifndef _GDEOCBILLBOARDLIST_H_
#define _GDEOCBILLBOARDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCBillboard;



/**
 * \brief Object class billboard list.
 */
class gdeOCBillboardList{
private:
	decObjectOrderedSet pBillboards;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class billboard list. */
	gdeOCBillboardList();
	
	/** \brief Create copy of object class billboard list. */
	gdeOCBillboardList( const gdeOCBillboardList &list );
	
	/** \brief Clean up object class billboard list. */
	~gdeOCBillboardList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class billboards. */
	int GetCount() const;
	
	/** \brief Object class billboard at position. */
	gdeOCBillboard *GetAt( int index ) const;
	
	/** \brief Index of named object class billboard or -1 if absent. */
	int IndexOf( gdeOCBillboard *billboard ) const;
	
	/** \brief Object class billboard is present. */
	bool Has( gdeOCBillboard *billboard ) const;
	
	/** \brief Add object class billboard. */
	void Add( gdeOCBillboard *billboard );
	
	/** \brief Remove object class billboard. */
	void Remove( gdeOCBillboard *billboard );
	
	/** \brief Remove all object class billboards. */
	void RemoveAll();
	
	/** \brief Set from another object class billboard list. */
	gdeOCBillboardList &operator=( const gdeOCBillboardList &list );
	/*@}*/
};

#endif
