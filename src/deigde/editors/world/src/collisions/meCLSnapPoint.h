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

#ifndef _MECLSNAPPOINT_H_
#define _MECLSNAPPOINT_H_

#include "../world/object/meObjectList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class meWorld;
class meObject;
class meObjectSnapPoint;
class igdeGDCSnapPoint;
class decShapeList;



/**
 * \brief Closest Snap Point Visitor.
 *
 * Collision listener storing the closest hit snap point for an object. Snap points matching
 * the object to test are ignored to avoid self-snapping. The object center and each snap
 * point of the object are tested against the found snap points. The pair with the closest
 * distance is chosen.
 */
class meCLSnapPoint : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	
	meObject *pObject;
	
	float pDistance;
	igdeGDCSnapPoint *pSourceSnapPoint;
	meObject *pTargetObject;
	const igdeGDCSnapPoint *pTargetSnapPoint;
	
	meObjectList pIgnoreObjects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	meCLSnapPoint( meWorld &world, meObject *object );
	
	/** \brief Clean up visitor. */
	virtual ~meCLSnapPoint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief World. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	/** \brief Object. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Snapping is enabled due to gui parameters. */
	bool IsSnappingEnabled() const;
	
	/**
	 * \brief Build testing shape list for collider.
	 * 
	 * Creates a box shape enclosing the object center and all snap points in the object
	 * in world space. Use this as shape for collider using this listener. The shape is
	 * relative to the object center. Thus set the collider position and orientation to
	 * match the object position and orientation.
	 */
	void CalcBoundingBoxShape( decShapeList &list );
	
	
	
	/** \brief Add object to ignore. */
	void AddIgnoreObject( meObject *object );
	
	/** \brief Reset visitor. */
	void Reset();
	
	/** \brief Distance between source point and target snap point. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Source snap point or \em NULL to use object position. */
	inline igdeGDCSnapPoint *GetSourceSnapPoint() const{ return pSourceSnapPoint; }
	
	/** \brief Target object or \em NULL if not set. */
	inline meObject *GetTargetObject() const{ return pTargetObject; }
	
	/** \brief Target snap point or \em NULL if not set. */
	inline const igdeGDCSnapPoint *GetTargetSnapPoint() const{ return pTargetSnapPoint; }
	
	
	
	/** \brief Matrix required to apply snapping in world space. */
	decDMatrix CalcCorrectionMatrix() const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Collision response. */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	
	/** \brief Can hit collider. */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	
	/** \brief Collider changed which is not used. */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
};

#endif
