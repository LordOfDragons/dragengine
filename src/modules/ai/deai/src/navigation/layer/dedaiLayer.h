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

#ifndef _DEDAILAYER_H_
#define _DEDAILAYER_H_

#include "../costs/dedaiCostTable.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>

class dedaiSpaceMeshFace;
class dedaiSpaceGridVertex;
class dedaiSpaceGridEdge;
class dedaiWorld;



/**
 * \brief Navigation layer.
 */
class dedaiLayer : public deObject{
private:
	dedaiWorld &pWorld;
	int pLayer;
	
	dedaiCostTable pCostTable;
	
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create layer. */
	dedaiLayer( dedaiWorld &world, int layer );
	
	/** \brief Clean up layer. */
	virtual ~dedaiLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent world. */
	inline dedaiWorld &GetWorld() const{ return pWorld; }
	
	/** \brief Layer. */
	inline int GetLayer() const{ return pLayer; }
	
	
	
	/** \brief Cost table. */
	inline dedaiCostTable &GetCostTable(){ return pCostTable; }
	inline const dedaiCostTable &GetCostTable() const{ return pCostTable; }
	
	
	
	/** \brief Update layer. */
	void Update( float elapsed );
	
	/** \brief Prepare layer if dirty. */
	void Prepare();
	
	
	
	/**
	 * \brief Mark dirty.
	 * \details Used by for developer mode to force an update the next time.
	 */
	void MarkDirty();
	
	
	
	/** \brief Navigation grid vertex closest to a given position. */
	dedaiSpaceGridVertex *GetGridVertexClosestTo( const decDVector &position, float &distance );
	
	/** \brief Navigation mesh face closest to a given position. */
	dedaiSpaceMeshFace *GetMeshFaceClosestTo( const decDVector &position, float &distance );
	
	/**
	 * \brief Nearest position inside radius.
	 * \details Sets position and returns face if found otherwise returns \em NULL.
	 */
	dedaiSpaceGridEdge *GetGridNearestPoint( const decDVector &point, float radius,
		decDVector &nearestPoint, float &nearestLambda );
	
	/**
	 * \brief Nearest position inside radius.
	 * \details Sets position and returns face if found otherwise returns \em NULL.
	 */
	dedaiSpaceMeshFace *GetNavMeshNearestPoint( const decDVector &point, float radius, decDVector &nearest );
	
	/**
	 * \brief Check for line colliding with mesh boundaries.
	 */
	bool NavMeshLineCollide( const decDVector &origin, const decVector &direction, float &distance );
	
	
	
	/** \brief Invalidate linking for all navigation spaces. */
	void InvalidateLinks();
	
	/** \brief Invalidate linking for all navigation spaces. */
	void InvalidateLinks( deNavigationSpace::eSpaceTypes type );
	
	/** \brief Invalidate linking for overlapping navigation spaces. */
	void InvalidateLinks( deNavigationSpace::eSpaceTypes type, const decDVector &boxMin, const decDVector &boxMax );
	
	
	
	/**
	 * \brief Invalidate blocking for all navigation spaces.
	 * \details Implicitely invalidates links too.
	 */
	void InvalidateBlocking();
	
	/**
	 * \brief Invalidate blocking for all navigation spaces.
	 * \details Implicitely invalidates links too.
	 */
	void InvalidateBlocking( deNavigationSpace::eSpaceTypes type );
	
	/**
	 * \brief Invalidate blocking for overlapping navigation spaces.
	 * \details Implicitely invalidates links too.
	 */
	void InvalidateBlocking( deNavigationSpace::eSpaceTypes type, const decDVector &boxMin, const decDVector &boxMax );
	/*@}*/
	
	
	
private:
	void pUpdateCostTable();
	void pNavSpacesPrepare();
	void pNavSpacesPrepareLinks();
	void pNavigatorsPrepare();
};

#endif
