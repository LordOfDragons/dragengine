/* 
 * Drag[en]gine Game Engine
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

#ifndef _DENAVIGATIONSPACECORNER_H_
#define _DENAVIGATIONSPACECORNER_H_


/**
 * \brief Navigation Space Corner.
 */
class deNavigationSpaceCorner{
private:
	unsigned short pVertex;
	unsigned short pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space corner. */
	deNavigationSpaceCorner();
	
	/** \brief Clean up navigation space corner. */
	~deNavigationSpaceCorner();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of the vertex. */
	inline unsigned short GetVertex() const{ return pVertex; }
	
	/** \brief Set index of the vertex. */
	void SetVertex( unsigned short vertex );
	
	/** \brief Type to use for crossing the face edge starting with this corner. */
	inline unsigned short GetType() const{ return pType; }
	
	/** \brief Set type to use for crossing the face edge starting with this corner. */
	void SetType( unsigned short type );
	/*@}*/
};

#endif
