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

#ifndef _MECLCLOSESTELEMENT_H_
#define _MECLCLOSESTELEMENT_H_

#include "meCLHitListEntry.h"
#include "../world/object/meObject.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class meWorld;
class meObject;
class meDecal;
class meHeightTerrainSector;
class meFilterObjects;
class meObjectSnapPoint;
class deCollider;



/**
 *  Closest element visitor.
 *
 * Collision listener storing the closes hit element. In addition an object and decal can
 * be defined to ignore during testing. This avoids self-collision results.
 */
class meCLClosestElement : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	
	bool pTestHeightTerrain;
	bool pTestObjects;
	bool pTestDecals;
	bool pTestSnapPoints;
	
	float pHitDistance;
	decVector pHitNormal;
	meHeightTerrainSector *pHitHTSector;
	meObject *pHitObject;
	meDecal *pHitDecal;
	meObjectSnapPoint *pHitSnapPoint;
	bool pHasHit;
	
	const meFilterObjects *pFilterObjects;
	
	meObject::List pIgnoreObjects;
	meDecal *pIgnoreDecal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**  Create visitor. */
	meCLClosestElement(meWorld &world);
	
	/**  Clean up visitor. */
	virtual ~meCLClosestElement();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set if height terrain are tested. */
	void SetTestHeightTerrain(bool test);
	
	/** Set if objects are tested. */
	void SetTestObjects(bool test);
	
	/** Set if decals are tested. */
	void SetTestDecals(bool test);
	
	/** Set if object snap points are tested. */
	void SetTestSnapPoints(bool test);
	
	
	
	/**  Set decal to ignore or nullptr. */
	void SetIgnoreDecal(meDecal *decal);
	
	/** \breif Object filter or nullptr to not use filtering. */
	inline const meFilterObjects *GetFilterObjects() const{ return pFilterObjects; }
	
	/**  Set object filter or nullptr to not use filtering. */
	void SetFilterObjects(const meFilterObjects *filter);
	
	/**  List of objects to ignore. */
	inline meObject::List &GetIgnoreObjects(){ return pIgnoreObjects; }
	inline const meObject::List &GetIgnoreObjects() const{ return pIgnoreObjects; }
	
	
	
	/**  Reset visitor. */
	void Reset();
	
	
	
	/** \breief Element has been hit. */
	inline bool GetHasHit() const{ return pHasHit; }
	
	/**  Hit distance. */
	inline float GetHitDistance() const{ return pHitDistance; }
	
	/**  Hit normal. */
	inline const decVector &GetHitNormal() const{ return pHitNormal; }
	
	/**  Hit height terrain sector or nullptr. */
	inline meHeightTerrainSector *GetHitHTSector() const{ return pHitHTSector; }
	
	/**  Hit object or nullptr. */
	inline meObject *GetHitObject() const{ return pHitObject; }
	
	/**  Hit decal or nullptr. */
	inline meDecal *GetHitDecal() const{ return pHitDecal; }
	
	/**  Hit object snap point or nullptr. */
	inline meObjectSnapPoint *GetHitSnapPoint() const{ return pHitSnapPoint; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**  Collision response. */
	virtual void CollisionResponse(deCollider *owner, deCollisionInfo *info);
	
	/**  Collider can hit another collider. */
	virtual bool CanHitCollider(deCollider *owner, deCollider *collider);
	
	/**  Collider changed. */
	virtual void ColliderChanged(deCollider *owner);
	/*@}*/
};

#endif
