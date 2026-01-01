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

#ifndef _RECLHITLISTENTRY_H_
#define _RECLHITLISTENTRY_H_

#include "../rig/bone/reRigBone.h"
#include "../rig/shape/reRigShape.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../rig/push/reRigPush.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decTOrderedSet.h>


/**
 * Entry of a hit list.
 */
class reCLHitListEntry : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reCLHitListEntry> Ref;
	
	typedef decTObjectOrderedSet<reCLHitListEntry> List;
	
	
private:
	reRigBone::Ref pBone;
	reRigShape::Ref pShape;
	reRigConstraint::Ref pConstraint;
	reRigPush::Ref pPush;
	float pDistance;
	decVector pNormal;
	
public:
	// constructor, destructor
	reCLHitListEntry();
	
protected:
	~reCLHitListEntry() override;
	
public:
	/** @name Management */
	/*@{*/
	/** Clears the entry. */
	void Clear();
	/** Retrieves the rig bone or nullptr. */
	inline const reRigBone::Ref &GetBone() const{ return pBone; }
	/** Sets the rig bone or nullptr. */
	void SetBone(reRigBone *bone);
	/** Retrieves the rig shape or nullptr. */
	inline const reRigShape::Ref &GetShape() const{ return pShape; }
	/** Sets the rig shape. */
	void SetShape(reRigShape *shape);
	/** Retrieves the rig constraint or nullptr. */
	inline const reRigConstraint::Ref &GetConstraint() const{ return pConstraint; }
	/** Sets the rig constraint. */
	void SetConstraint(reRigConstraint *constraint);
	/** Retrieves the rig push or nullptr. */
	inline const reRigPush::Ref &GetPush() const{ return pPush; }
	/** Sets the rig push. */
	void SetPush(reRigPush *push);
	
	/** Retrieves the distance. */
	inline float GetDistance() const{ return pDistance; }
	/** Sets the distance. */
	void SetDistance(float distance);
	/** Retrieves the normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	/** Sets the normal. */
	void SetNormal(const decVector &normal);
	/*@}*/
	
	/**
	 * Compares this entry to another one. Returns 1 if this entry is ordinally larger,
	 * -1 if smaller or 0 if equal.
	 */
	int Compare(const reCLHitListEntry &entry) const;
	/*@}*/
};

// end of include only once
#endif
