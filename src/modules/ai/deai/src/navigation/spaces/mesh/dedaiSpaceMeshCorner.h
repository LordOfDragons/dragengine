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

#ifndef _DEDAISPACEMESHCORNER_H_
#define _DEDAISPACEMESHCORNER_H_



/**
 * \brief No-cost table entry.
 * \details Special entry used for split edges to be excluded from cost look-up.
 */
#define CORNER_NO_COST		( ( unsigned short )~0 )
#define CORNER_NO_LINK		( ( short )-1 )



/**
 * \brief Space mesh corner.
 * 
 * Corner of a face in a space mesh. Stores the index to the vertex of the corner as well
 * as the index of the edge leading from this corner to the next one.
 */
class dedaiSpaceMeshCorner{
private:
	unsigned short pVertex;
	unsigned short pEdge;
	unsigned short pTypeNumber;
	short pLink;
	bool pEnabled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create space mesh corner. */
	dedaiSpaceMeshCorner();
	
	/** \brief Clean up space mesh corner. */
	~dedaiSpaceMeshCorner();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of vertex. */
	inline unsigned short GetVertex() const{ return pVertex; }
	
	/** \brief Set index of vertex. */
	void SetVertex( unsigned short vertex );
	
	/** \brief Index of edge. */
	inline unsigned short GetEdge() const{ return pEdge; }
	
	/** \brief Set index of edge. */
	void SetEdge( unsigned short edge );
	
	/** \brief Type number to cross edge. */
	inline unsigned short GetTypeNumber() const{ return pTypeNumber; }
	
	/** \brief Set type number to cross edge. */
	void SetTypeNumber( unsigned short typeNumber );
	
	/** \brief Link index or -1 if edge is not linked. */
	inline short GetLink() const{ return pLink; }
	
	/** \brief Set link index or -1 if edge is not linked. */
	void SetLink( short link );
	
	/** \brief Corner is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if corner is enabled. */
	void SetEnabled( bool enabled );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brier Copy corner. */
	dedaiSpaceMeshCorner &operator=( const dedaiSpaceMeshCorner &corner );
	/*@}*/
};

#endif
