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

#ifndef _MECLCOLLECT_H_
#define _MECLCOLLECT_H_

#include "meCLHitList.h"
#include "../world/object/meObjectList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class meDecal;
class meObject;
class meFilterObjects;
class meWorld;
class deCollider;


/**
 * @brief Collect elements visitor.
 *
 * Collision listener collecting all hit elements.
 */
class meCLCollect : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	
	bool pTestHeightTerrain;
	bool pTestObjects;
	bool pTestDecals;
	bool pTestSnapPoints;
	
	meCLHitList pElements;
	
	const meFilterObjects *pFilterObjects;
	
	meObjectList pIgnoreObjects;
	meDecal *pIgnoreDecal;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a object. */
	meCLCollect(meWorld &world);
	
	/** Clean up the object. */
	~meCLCollect() override;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief World. */
	inline meWorld &GetWorld() const{ return pWorld; }
	
	
	/** Set if height terrain are tested. */
	void SetTestHeightTerrain(bool test);
	
	/** Set if objects are tested. */
	void SetTestObjects(bool test);
	
	/** Set if decals are tested. */
	void SetTestDecals(bool test);
	
	/** Set if object snap points are tested. */
	void SetTestSnapPoints(bool test);
	
	
	/** Set decal to ignore or nullptr. */
	void SetIgnoreDecal(meDecal *decal);
	
	/** Object filter or nullptr to not use filtering. */
	inline const meFilterObjects *GetFilterObjects() const{ return pFilterObjects; }
	
	/** Set object filter or nullptr to not use filtering. */
	void SetFilterObjects(const meFilterObjects *filter);
	
	/** List of objects to ignore. */
	inline meObjectList &GetIgnoreObjects(){ return pIgnoreObjects; }
	inline const meObjectList &GetIgnoreObjects() const{ return pIgnoreObjects; }
	
	
	/** Resets the collector. */
	void Reset();
	
	/** List of collected elements. */
	inline meCLHitList &GetCollectedElements(){ return pElements; }
	inline const meCLHitList &GetCollectedElements() const{ return pElements; }
	
	
	virtual void CollisionResponse(deCollider *owner, deCollisionInfo *info);
	virtual bool CanHitCollider(deCollider *owner, deCollider *collider);
	virtual void ColliderChanged(deCollider *owner);
	/*@}*/
};

#endif
