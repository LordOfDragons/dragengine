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

#include "reURigGenerateCollisionShapes.h"
#include "../../rig/reRig.h"
#include "../../rig/shape/reRigShapeSphere.h"
#include "../../rig/shape/reRigShapeBox.h"
#include "../../rig/shape/reRigShapeCylinder.h"
#include "../../rig/shape/reRigShapeCapsule.h"
#include "../../rig/shape/reRigShapeHull.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeVisitorIdentify.h>


// Class reURigGenerateCollisionShapes
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

reURigGenerateCollisionShapes::reURigGenerateCollisionShapes(
	reRig *rig, const deRig::Ref &generatedRig) :
pRig(rig)
{
	DEASSERT_NOTNULL(rig);
	DEASSERT_NOTNULL(generatedRig);
	
	SetShortInfo("@Rig.Undo.RigGenerateCollisionShapes");
	
	auto engine = rig->GetEngine();
	decShapeVisitorIdentify visitor;
	
	pOldShapes = rig->GetShapes();
	
	generatedRig->GetShapes().Visit([&](decShape &genShape){
		genShape.Visit(visitor);
		switch(visitor.GetType()){
		case decShapeVisitorIdentify::estSphere:{
			const auto &genSphere = visitor.CastToSphere();
			const auto sphere = reRigShapeSphere::Ref::New(engine);
			sphere->SetPosition(genSphere.GetPosition());
			sphere->SetOrientation(genSphere.GetOrientation().GetEulerAngles() * RAD2DEG);
			sphere->SetRadius(genSphere.GetRadius());
			sphere->SetAxisScaling(genSphere.GetAxisScaling());
			pNewShapes.Add(sphere);
			}break;
			
		case decShapeVisitorIdentify::estBox:{
			const auto &genBox = visitor.CastToBox();
			const auto box = reRigShapeBox::Ref::New(engine);
			box->SetPosition(genBox.GetPosition());
			box->SetOrientation(genBox.GetOrientation().GetEulerAngles() * RAD2DEG);
			box->SetHalfExtends(genBox.GetHalfExtends());
			box->SetTapering(genBox.GetTapering());
			pNewShapes.Add(box);
			}break;
			
		case decShapeVisitorIdentify::estCylinder:{
			const auto &genCylinder = visitor.CastToCylinder();
			const auto cylinder = reRigShapeCylinder::Ref::New(engine);
			cylinder->SetPosition(genCylinder.GetPosition());
			cylinder->SetOrientation(genCylinder.GetOrientation().GetEulerAngles() * RAD2DEG);
			cylinder->SetHalfHeight(genCylinder.GetHalfHeight());
			cylinder->SetBottomRadius(genCylinder.GetBottomRadius());
			cylinder->SetTopRadius(genCylinder.GetTopRadius());
			cylinder->SetBottomAxisScaling(genCylinder.GetBottomAxisScaling());
			cylinder->SetTopAxisScaling(genCylinder.GetTopAxisScaling());
			pNewShapes.Add(cylinder);
			}break;
			
		case decShapeVisitorIdentify::estCapsule:{
			const auto &genCapsule = visitor.CastToCapsule();
			const auto capsule = reRigShapeCapsule::Ref::New(engine);
			capsule->SetPosition(genCapsule.GetPosition());
			capsule->SetOrientation(genCapsule.GetOrientation().GetEulerAngles() * RAD2DEG);
			capsule->SetHalfHeight(genCapsule.GetHalfHeight());
			capsule->SetBottomRadius(genCapsule.GetBottomRadius());
			capsule->SetTopRadius(genCapsule.GetTopRadius());
			capsule->SetBottomAxisScaling(genCapsule.GetBottomAxisScaling());
			capsule->SetTopAxisScaling(genCapsule.GetTopAxisScaling());
			pNewShapes.Add(capsule);
			}break;
			
		case decShapeVisitorIdentify::estHull:{
			const auto &genHull = visitor.CastToHull();
			const auto hull = reRigShapeHull::Ref::New(engine);
			hull->SetPosition(genHull.GetPosition());
			hull->SetOrientation(genHull.GetOrientation().GetEulerAngles() * RAD2DEG);
			genHull.GetPoints().Visit([&](const decVector &point){
				hull->AddPoint(point);
			});
			pNewShapes.Add(hull);
			}break;
			
		default:
			break;
		}
	});
	
	pRig = rig;
}

reURigGenerateCollisionShapes::~reURigGenerateCollisionShapes() = default;


// Undo and Redo operations
/////////////////////////////

void reURigGenerateCollisionShapes::Undo(){
	pRig->RemoveAllShapes();
	pOldShapes.Visit([&](const reRigShape::Ref &shape){
		pRig->AddShape(shape);
	});
}

void reURigGenerateCollisionShapes::Redo(){
	pRig->RemoveAllShapes();
	pNewShapes.Visit([&](const reRigShape::Ref &shape){
		pRig->AddShape(shape);
	});
}
