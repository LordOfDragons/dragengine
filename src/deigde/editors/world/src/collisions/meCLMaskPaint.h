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

// include only once
#ifndef _MECLMASKPAINT_H_
#define _MECLMASKPAINT_H_

// includes
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class meWorld;
class meHeightTerrainSector;
class meHeightTerrainTexture;
class deCollider;



/**
 * @brief Mask Paint Collision Listener.
 *
 * Collision listener for mask painting a height terrain.
 */
class meCLMaskPaint : public deBaseScriptingCollider{
private:
	meWorld *pWorld;
	meHeightTerrainSector *pSector;
	meHeightTerrainTexture *pTexture;
	
	decDVector pRayOrigin;
	decVector pRayDirection;
	
	int pDrawMode;
	float pRadius;
	
	decDVector pHitPoint;
	float pHitDistance;
	bool pHasHit;
	
	decPoint pSessionGrid;
	
	decPoint pPaintSector;
	decVector2 pPaintGrid;
	float pPaintRadius;
	float pPaintInverseScale;
	
	decBoundary pAreaGrid;
	decBoundary pAreaModifyGrid;
	
	unsigned char *pOldValues;
	int pModifyWidth;
	int pModifyHeight;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new listener. */
	meCLMaskPaint(meWorld *world);
	/** Cleans up the listener. */
	virtual ~meCLMaskPaint();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Sets the texture. */
	void SetTexture(meHeightTerrainSector *sector, meHeightTerrainTexture *texture);
	/** Sets the paint ray. */
	void SetRay(const decDVector &origin, const decVector &direction);
	
	/** Determines if painting is possible. */
	bool CanPaint() const;
	
	/** Starts a paint session. */
	void BeginSession();
	/** Prepares for a single paint turn. */
	void PreparePaint();
	/** Paint. */
	void Paint();
	/** End the paint session. */
	void EndSession();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/**
	 * Determines the response for a given collision. If you specified ertCustom
	 * you have to update the info object with the response to the collision. In
	 * all other cases you do must not modify the info object.
	 */
	virtual void CollisionResponse(deCollider *owner, deCollisionInfo *info);
	/**
	 * Determines if a collider can be hit. If this peer is used with a collider
	 * then the script is asked to determine if the two collider can hit each
	 * other. If this peer is used in a ray test owner is NULL and the script
	 * is asked to determine if the ray can hit the collider.
	 * @param owner Collider this peer belongs to or NULL if a ray test is done.
	 * @param collider Collider to test.
	 * @return True if the owner/ray can hit the given collider.
	 */
	virtual bool CanHitCollider(deCollider *owner, deCollider *collider);
	/**
	 * Notifies the scripts that the properties of this collider have changed and
	 * that the attached element has to update. This is usually called after the
	 * collision detection but can also be called multiple times.
	 */
	virtual void ColliderChanged(deCollider *owner);
	/*@}*/
	
private:
	void pPaintDraw();
	void pPaintErase();
	
	void pUpdateOldValues(const decVector2 &grid, const decVector2 &size);
};

// end of include only once
#endif
