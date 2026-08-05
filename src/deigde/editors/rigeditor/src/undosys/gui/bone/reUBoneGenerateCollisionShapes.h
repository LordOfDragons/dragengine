/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _REUBONEGENERATECOLLISIONSHAPES_H_
#define _REUBONEGENERATECOLLISIONSHAPES_H_

#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/collection/decTUniqueList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/rig/deRig.h>

class reRigBone;


/**
 * Undo action generate collision shapes for selected bones.
 */
class reUBoneGenerateCollisionShapes : public igdeUndo{
public:
	using Ref = deTObjectReference<reUBoneGenerateCollisionShapes>;
	
	
private:
	struct cBone{
		reRigBone::Ref bone;
		reRigShape::List oldShapes, newShapes;
	};
	
	
private:
	reRig *pRig;
	decTUniqueList<cBone> pBones;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	reUBoneGenerateCollisionShapes(reRig *rig, const reRigBone::List &bones,
		const deRig::Ref &generatedRig);
	
protected:
	/** Clean up the undo. */
	~reUBoneGenerateCollisionShapes() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	void Undo() override;
	void Redo() override;
	/*@}*/
};

#endif
