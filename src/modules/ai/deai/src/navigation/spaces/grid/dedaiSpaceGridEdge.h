/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAISPACEGRIDEDGE_H_
#define _DEDAISPACEGRIDEDGE_H_



/**
 * \brief Space grid edge.
 * 
 * Edge in a navigation grid. Every edge connects 2 vertices. The first type is used while
 * traveling from the first vertex to the second and the second type for the other direction.
 */
class dedaiSpaceGridEdge{
private:
	unsigned short pVertex1;
	unsigned short pVertex2;
	unsigned short pTypeNumber1;
	unsigned short pTypeNumber2;
	float pLength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create grid edge. */
	dedaiSpaceGridEdge();
	
	/** \brief Clean up grid edge. */
	~dedaiSpaceGridEdge();
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
	
	/** \brief Type number to use traveling the edge from the first vertex to the second. */
	inline unsigned short GetTypeNumber1() const{ return pTypeNumber1; }
	
	/** \brief Set type number to use traveling the edge from the first vertex to the second. */
	void SetTypeNumber1( unsigned short typeNumber );
	
	/** \brief Type number to use traveling the edge from the second vertex to the first. */
	inline unsigned short GetTypeNumber2() const{ return pTypeNumber2; }
	
	/** \brief Set type number to use traveling the edge from the second vertex to the first. */
	void SetTypeNumber2( unsigned short typeNumber );
	
	/** \brief Length of the edge. */
	inline float GetLength() const{ return pLength; }
	
	/** \brief Set length of the edge. */
	void SetLength( float length );
	/*@}*/
};

#endif
