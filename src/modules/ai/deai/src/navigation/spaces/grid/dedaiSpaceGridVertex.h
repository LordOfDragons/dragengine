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

#ifndef _DEDAISPACEGRIDVERTEX_H_
#define _DEDAISPACEGRIDVERTEX_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpaceGrid;



/**
 * \brief Space grid vertex.
 */
class dedaiSpaceGridVertex{
public:
	/** \brief Path finding type. */
	enum ePathFindingTypes{
		/** \brief Vertex is in no list. */
		epftFree,
		
		/** \brief Vertex is in the open list. */
		epftOpen,
		
		/** \brief Vertex is in the closed list. */
		epftClosed
	};
	
	
	
private:
	dedaiSpaceGrid *pGrid;
	decVector pPosition;
	int pFirstEdge;
	unsigned short pIndex;
	unsigned short pEdgeCount;
	unsigned short pFirstLink;
	unsigned short pLinkCount;
	
	bool pEnabled;
	
	dedaiSpaceGridVertex *pPathParent;
	float pPathCostF;
	float pPathCostG;
	float pPathCostH;
	ePathFindingTypes pPathType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create grid vertex. */
	dedaiSpaceGridVertex();
	
	/** \brief Clean up grid vertex. */
	~dedaiSpaceGridVertex();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent grid. */
	inline dedaiSpaceGrid *GetGrid() const{ return pGrid; }
	
	/** \brief Set parent grid. */
	void SetGrid( dedaiSpaceGrid *grid );
	
	
	
	/** \brief Vertex index. */
	inline unsigned short GetIndex() const{ return pIndex; }
	
	/** \brief Set vertex index. */
	void SetIndex( unsigned short index );
	
	
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Index of the first edge. */
	inline unsigned short GetFirstEdge() const{ return pFirstEdge; }
	
	/** \brief Set index of the first edge. */
	void SetFirstEdge( unsigned short first );
	
	/** \brief Number of edges. */
	inline unsigned short GetEdgeCount() const{ return pEdgeCount; }
	
	/** \brief Set number of edges. */
	void SetEdgeCount( unsigned short count );
	
	/** \brief Index of the first link. */
	inline unsigned short GetFirstLink() const{ return pFirstLink; }
	
	/** \brief Set index of the first link. */
	void SetFirstLink( unsigned short first );
	
	/** \brief Number of links. */
	inline unsigned short GetLinkCount() const{ return pLinkCount; }
	
	/** \brief Set number of links. */
	void SetLinkCount( unsigned short count );
	
	
	
	/** \brief Vertex is enabled for path finding. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if vertex is enabled for path finding. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Path parent vertex or \em NULL. */
	inline dedaiSpaceGridVertex *GetPathParent() const{ return pPathParent; }
	
	/** \brief Set path parent vertex or \em NULL. */
	void SetPathParent( dedaiSpaceGridVertex *face );
	
	/** \brief F path cost. */
	inline float GetPathCostF() const{ return pPathCostF; }
	
	/** \brief Set F path cost. */
	void SetPathCostF( float cost );
	
	/** \brief G path cost. */
	inline float GetPathCostG() const{ return pPathCostG; }
	
	/** \brief Set G path cost. */
	void SetPathCostG( float cost );
	
	/** \brief H path cost. */
	inline float GetPathCostH() const{ return pPathCostH; }
	
	/** \brief Set H path cost. */
	void SetPathCostH( float cost );
	
	/** \brief Path type. */
	inline int GetPathType() const{ return pPathType; }
	
	/** \brief Set path type. */
	void SetPathType( ePathFindingTypes type );
	
	/** \brief Clear path information. */
	void ClearPath();
	/*@}*/
};

#endif
