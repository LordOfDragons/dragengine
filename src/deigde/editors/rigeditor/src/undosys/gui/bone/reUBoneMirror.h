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

#ifndef _REUBONEMIRROR_H_
#define _REUBONEMIRROR_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>
#include "reRig.h"
#include "reRigBone.h"

class reRigShapeList;
class reRigConstraintList;



/**
 * Undo Mirror Bone.
 *
 * Undo action to mirror bones. This includes the physics parameters,
 * shapes and constraints but not the geometry information like the
 * position or rotation since these usually have to stay the same.
 * Bones have to be end with .l or .r for right respective left. For
 * each bone to mirror the opposite bone is looked for and the
 * parameters copied but mirrored along the x axis.
 */
class reUBoneMirror : public igdeUndo{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reUBoneMirror> Ref;
	

private:
	struct sBone{
		reRigBone::Ref boneSource;
		
		reRigBone::Ref boneTarget;
		decVector oldCMP;
		float oldMass;
		bool oldDynamic;
		reRigShapeList *oldShapes;
		reRigConstraintList *oldConstraints;
		decVector oldIKLimitsLower;
		decVector oldIKLimitsUpper;
		decVector oldIKResistance;
		bool oldIKLocked[3];
	};
	
	
	
private:
	reRig::Ref pRig;
	
	sBone *pBones;
	int pBoneCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new undo. */
	reUBoneMirror(reRig *rig);
	
protected:
	/** Clean up the undo. */
	virtual ~reUBoneMirror();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	reRigBone *pGetBoneWithMirroredName(reRig *rig, reRigBone *bone) const;
};

#endif
