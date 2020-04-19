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

#ifndef _GDEOCLIGHTLIST_H_
#define _GDEOCLIGHTLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCLight;



/**
 * \brief Object class light list.
 */
class gdeOCLightList{
private:
	decObjectOrderedSet pLights;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class light list. */
	gdeOCLightList();
	
	/** \brief Create copy of object class light list. */
	gdeOCLightList( const gdeOCLightList &list );
	
	/** \brief Clean up object class light list. */
	~gdeOCLightList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class lights. */
	int GetCount() const;
	
	/** \brief Object class light at position. */
	gdeOCLight *GetAt( int index ) const;
	
	/** \brief Index of named object class light or -1 if absent. */
	int IndexOf( gdeOCLight *light ) const;
	
	/** \brief Object class light is present. */
	bool Has( gdeOCLight *light ) const;
	
	/** \brief Add object class light. */
	void Add( gdeOCLight *light );
	
	/** \brief Remove object class light. */
	void Remove( gdeOCLight *light );
	
	/** \brief Remove all object class lights. */
	void RemoveAll();
	
	/** \brief Set from another object class light list. */
	gdeOCLightList &operator=( const gdeOCLightList &list );
	/*@}*/
};

#endif
