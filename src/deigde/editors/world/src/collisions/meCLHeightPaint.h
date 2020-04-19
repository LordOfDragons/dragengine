/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MECLHEIGHTPAINT_H_
#define _MECLHEIGHTPAINT_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class meWorld;
class deCollider;



/**
 * \brief Height paint collision listener.
 */
class meCLHeightPaint : public deBaseScriptingCollider{
private:
	meWorld *pWorld;
	
	decDVector pRayOrigin;
	decVector pRayDirection;
	
	int pDrawMode;
	float pRadius;
	float pStrength;
	float pPaintStrength;
	
	float pGaussRadius;
	float pGaussParam;
	
	float pGaussKernel3x3[ 3 ][ 3 ];
	
	decDVector pHitPoint;
	float pHitDistance;
	bool pHasHit;
	
	decPoint pSessionSector;
	decPoint pSessionGrid;
	
	decPoint pPaintSector;
	decVector2 pPaintGrid;
	float pPaintRadius;
	float pPaintInverseScale;
	
	decBoundary pAreaSector;
	decBoundary pAreaGrid;
	
	decBoundary pAreaModifySector;
	decBoundary pAreaModifyGrid;
	
	float *pOldHeights;
	int pModifyWidth;
	int pModifyHeight;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new listener. */
	meCLHeightPaint( meWorld *world );
	/** \brief Cleans up the listener. */
	virtual ~meCLHeightPaint();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the ray origin. */
	inline const decDVector &GetRayOrigin() const{ return pRayOrigin; }
	/** \brief Retrieves the ray direction. */
	inline const decVector &GetRayDirection() const{ return pRayDirection; }
	/** \brief Sets the paint ray. */
	void SetRay( const decDVector &origin, const decVector &direction );
	
	/** \brief Starts a paint session. */
	void BeginSession();
	/** \brief Prepares for a single paint turn. */
	void PreparePaint();
	/** \brief Paint. */
	void Paint( float elapsed );
	/** \brief End the paint session. */
	void EndSession();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * \brief Determines the response for a given collision.
	 * \details If you specified ertCustom you have to update the info object
	 *          with the response to the collision. In all other cases you do
	 *          must not modify the info object.
	 */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	/**
	 * \brief Determines if a collider can be hit.
	 * \details If this peer is used with a collider then the script is asked to
	 *          determine if the two collider can hit each other. If this peer is
	 *          used in a ray test owner is NULL and the script is asked to
	 *          determine if the ray can hit the collider.
	 * \param owner Collider this peer belongs to or NULL if a ray test is done.
	 * \param collider Collider to test.
	 * \return True if the owner/ray can hit the given collider.
	 */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	/**
	 * \brief Notifies the scripts that the properties of this collider have changed
	 *        and that the attached element has to update.
	 * \details This is usually called after the collision detection but can also be
	 *          called multiple times.
	 */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
	
private:
	void pCalculateGaussParameter();
	float pGauss( float x, float y );
	
	void pPaintRaise();
	void pPaintLower();
	void pPaintLevel();
	void pPaintSmooth();
	
	void pFetch3x3( float *values, int sectorX, int sectorY, int gridX, int gridY );
	float pSmooth3x3( float *values );
	
	void pUpdateOldHeights( const decPoint &sector, const decVector2 &grid, const decVector2 &size );
};

#endif
