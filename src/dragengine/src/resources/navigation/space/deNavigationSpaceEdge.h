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

#ifndef _DENAVIGATIONSPACEEDGE_H_
#define _DENAVIGATIONSPACEEDGE_H_


/**
 * \brief Navigation Space Edge.
 */
class deNavigationSpaceEdge{
private:
	unsigned short pVertex1;
	unsigned short pVertex2;
	unsigned short pType1;
	unsigned short pType2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space face. */
	deNavigationSpaceEdge();
	
	/** \brief Clean up navigation space face. */
	~deNavigationSpaceEdge();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of the first vertex. */
	inline unsigned short GetVertex1() const{ return pVertex1; }
	
	/** \brief Set index of the first vertex. */
	void SetVertex1( unsigned short vertex );
	
	/** \brief Index of the second vertex. */
	inline unsigned short GetVertex2() const{ return pVertex2; }
	
	/** \brief Set index of the second vertex. */
	void SetVertex2( unsigned short vertex );
	
	/** \brief Type to use for this edge to travel from the first to the second vertex. */
	inline unsigned short GetType1() const{ return pType1; }
	
	/** \brief Set type to use for this edge to travel from the first to the second vertex. */
	void SetType1( unsigned short type );
	
	/** \brief Type to use for this edge to travel from the second to the second first. */
	inline unsigned short GetType2() const{ return pType2; }
	
	/** \brief Set type to use for this edge to travel from the second to the second first. */
	void SetType2( unsigned short type );
	/*@}*/
};

#endif
