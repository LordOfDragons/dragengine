/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SEBODYLIST_H_
#define _SEBODYLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class seBody;



/**
 * \brief Sky body list.
 */
class seBodyList{
private:
	decObjectOrderedSet pBodies;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky body list. */
	seBodyList();
	
	/** \brief Clean up sky body list. */
	~seBodyList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of bodies. */
	int GetCount() const;
	
	/** \brief Body at index. */
	seBody *GetAt( int index ) const;
	
	/** \brief Index of given body or -1 if absent. */
	int IndexOf( seBody *body ) const;
	
	/** \brief Body is present. */
	bool Has( seBody *body ) const;
	
	/** \brief Add body. */
	void Add( seBody *body );
	
	/** \brief Insert body. */
	void InsertAt( seBody *body, int index );
	
	/** \brief Move body. */
	void MoveTo( seBody *body, int index );
	
	/** \brief Remove body. */
	void Remove( seBody *body );
	
	/** \brief Remove all bodies. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	seBodyList &operator=( const seBodyList &list );
	/*@}*/
};

#endif
