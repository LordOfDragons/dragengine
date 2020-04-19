/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLLIGHTLIST_H_
#define _DEOGLLIGHTLIST_H_

#include <dragengine/common/collection/decObjectSet.h>

class deoglRLight;



/**
 * \brief Light list.
 * 
 * List of pointers to lights. The ordering of the lights in the list is
 * not defined and can change when lights are removed. This allows to
 * remove elements with constant time requirements.
 */
class deoglLightList{
private:
	decObjectSet pLights;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create light list. */
	deoglLightList();
	
	/** \brief Create copy of light list. */
	deoglLightList( const deoglLightList &copy );
	
	/** \brief Clean up light list. */
	~deoglLightList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of lights. */
	int GetCount() const;
	
	/** \brief Light is present. */
	bool Has( deoglRLight *light ) const;
	
	/** \brief Light at index. */
	deoglRLight *GetAt( int index ) const;
	
	/** \brief Add light. */
	void Add( deoglRLight *light );
	
	/**
	 * \brief Add light if absent.
	 * 
	 * \returns \em true if added otherwise \em false.
	 */
	bool AddIfMissing( deoglRLight *light );
	
	/** \brief Remove light. */
	void Remove( deoglRLight *light );
	
	/**
	 * \brief Remove light if present.
	 * 
	 * \returns \em true if removed otherwise \em false.
	 */
	bool RemoveIfExisting( deoglRLight *light );
	
	/** \brief Remove all lights. */
	void RemoveAll();
	/*@}*/
};

#endif
