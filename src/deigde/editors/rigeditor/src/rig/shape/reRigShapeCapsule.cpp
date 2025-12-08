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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigShapeCapsule.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/exceptions.h>



// Class reRigShapeCapsule
////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeCapsule::reRigShapeCapsule(deEngine *engine) : reRigShape(engine, estCapsule){
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pHalfHeight = 0.5f;
}

reRigShapeCapsule::~reRigShapeCapsule(){
}



// Management
///////////////

void reRigShapeCapsule::SetHalfHeight(float halfHeight){
	if(fabs(halfHeight - pHalfHeight) > 1e-5f){
		pHalfHeight = halfHeight;
		NotifyShapeChanged();
	}
}

void reRigShapeCapsule::SetTopRadius(float topRadius){
	if(fabs(topRadius - pTopRadius) > 1e-5f){
		pTopRadius = topRadius;
		NotifyShapeChanged();
	}
}

void reRigShapeCapsule::SetBottomRadius(float bottomRadius){
	if(fabs(bottomRadius - pBottomRadius) > 1e-5f){
		pBottomRadius = bottomRadius;
		NotifyShapeChanged();
	}
}

reRigShape *reRigShapeCapsule::Duplicate() const{
	reRigShapeCapsule *shape = NULL;
	
	try{
		shape = new reRigShapeCapsule(GetEngine());
		if(!shape) DETHROW(deeOutOfMemory);
		
		shape->SetPosition(GetPosition());
		shape->SetOrientation(GetOrientation());
		shape->SetHalfHeight(GetHalfHeight());
		shape->SetTopRadius(GetTopRadius());
		shape->SetBottomRadius(GetBottomRadius());
		
	}catch(const deException &){
		throw;
	}
	
	return shape;
}

void reRigShapeCapsule::Scale(float scale){
	SetPosition(GetPosition() * scale);
	pHalfHeight *= scale;
	pTopRadius *= scale;
	pBottomRadius *= scale;
}

decShape *reRigShapeCapsule::CreateShape(){
	return new decShapeCapsule(pHalfHeight, pTopRadius, pBottomRadius,
		GetPosition(), decQuaternion::CreateFromEuler(GetOrientation() * DEG2RAD));
}
