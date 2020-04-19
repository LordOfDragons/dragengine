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

#ifndef _DEDAISPACEMESHEDGE_H_
#define _DEDAISPACEMESHEDGE_H_



/**
 * \brief Edge in space mesh.
 * 
 * Every edge uses 2 vertices and belongs to 1 or 2 faces. Faces can be -1 if not assigned.
 */
class dedaiSpaceMeshEdge{
private:
	unsigned short pVertex1;
	unsigned short pVertex2;
	short pFace1;
	short pFace2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh edge. */
	dedaiSpaceMeshEdge();
	
	/** \brief Clean up space mesh edge. */
	~dedaiSpaceMeshEdge();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of first vertex. */
	inline unsigned short GetVertex1() const{ return pVertex1; }
	
	/** \brief Set index of first vertex. */
	void SetVertex1( unsigned short vertex );
	
	/** \brief Index of second vertex. */
	inline unsigned short GetVertex2() const{ return pVertex2; }
	
	/** \brief Set index of second vertex. */
	void SetVertex2( unsigned short vertex );
	
	/** \brief Index of first face or -1 if not linked to any face. */
	inline short GetFace1() const{ return pFace1; }
	
	/** \brief Set index of first face or -1 if not linked to any face. */
	void SetFace1( short face );
	
	/** \brief Index of second face or -1 if not linked to two faces. */
	inline short GetFace2() const{ return pFace2; }
	
	/** \brief Set index of second face or -1 if not linked to two faces. */
	void SetFace2( short face );
	/*@}*/
};

#endif
