/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
