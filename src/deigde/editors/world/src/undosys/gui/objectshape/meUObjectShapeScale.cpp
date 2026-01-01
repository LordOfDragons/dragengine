/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#include <stdio.h>
#include <stdlib.h>

#include "meUObjectShapeScale.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeList.h>
#include <dragengine/common/shape/decShapeVisitor.h>
#include <deigde/codec/igdeCodecPropertyString.h>



// Shape scaling visitor
//////////////////////////

namespace{

class cShapeScaler : public decShapeVisitor{
private:
	const decVector &pFactors;
	const bool pModifySize;
	
public:
	cShapeScaler(const decVector &factors, bool modifySize) :
	pFactors(factors),
	pModifySize(modifySize){
	}
	
	virtual void VisitShapeSphere(decShapeSphere &sphere){
		if(pModifySize){
			sphere.SetRadius(sphere.GetRadius() * (pFactors.x + pFactors.y + pFactors.z) / 3.0f);
		}
	}
	
	virtual void VisitShapeBox(decShapeBox &box){
		if(pModifySize){
			box.SetHalfExtends(box.GetHalfExtends().Multiply(pFactors));
		}
	}
	
	virtual void VisitShapeCylinder(decShapeCylinder &cylinder){
		if(pModifySize){
			const float factorRadius = (pFactors.x + pFactors.z) / 2.0f;
			cylinder.SetTopRadius(cylinder.GetTopRadius() * factorRadius);
			cylinder.SetBottomRadius(cylinder.GetBottomRadius() * factorRadius);
			cylinder.SetHalfHeight(cylinder.GetHalfHeight() * pFactors.y);
		}
	}
	
	virtual void VisitShapeCapsule(decShapeCapsule &capsule){
		if(pModifySize){
			const float factorRadius = (pFactors.x + pFactors.z) / 2.0f;
			capsule.SetTopRadius(capsule.GetTopRadius() * factorRadius);
			capsule.SetBottomRadius(capsule.GetBottomRadius() * factorRadius);
			capsule.SetHalfHeight(capsule.GetHalfHeight() * pFactors.y);
		}
	}
};

}



// Class meUObjectShapeScale
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectShapeScale::meUObjectShapeScale(meObject *object, const char *property,
const meObjectShape::List &list){
	DEASSERT_TRUE(list.IsNotEmpty())
	DEASSERT_NOTNULL(object)
	DEASSERT_NOTNULL(property)
	DEASSERT_NOTNULL(object->GetWorld())
	
	SetShortInfo("Scale object shapes");
	SetLongInfo("Scale object shapes");
	
	pPropertyExists = object->GetProperties().Has(property);
	if(pPropertyExists){
		pOldValue = object->GetProperties().GetAt(property);
	}
	
	const meObjectShape::List &shapeList = object->GetWorld()->GetObjectShapes();
	list.Visit([&](meObjectShape *shape){
		pList.Add(shapeList.IndexOf(shape));
	});
	
	pProperty = property;
	pObject = object;
}

meUObjectShapeScale::~meUObjectShapeScale(){
}



// Management
///////////////

void meUObjectShapeScale::Undo(){
	meWorld &world = *pObject->GetWorld();
	
	if(pPropertyExists){
		pObject->SetProperty(pProperty, pOldValue);
		
	}else{
		pObject->RemoveProperty(pProperty);
	}
	
	world.NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeScale::Redo(){
	const decDMatrix matrixParent(pObject->GetObjectMatrix());
	const decDMatrix matrixParentInverse(matrixParent.Invert());
	const bool modifyPosition = GetModifyPosition();
	
	decShapeList shapeList;
	igdeCodecPropertyString codec;
	codec.DecodeShapeList(pOldValue.GetString(), shapeList);
	
	cShapeScaler scaler(GetFactors(), GetModifySize());
	
	pList.Visit([&](int shapeIndex){
		decShape &shape = *shapeList.GetAt(shapeIndex);
		
		decDMatrix matrixShape(decDMatrix::CreateWorld(
			decDVector(shape.GetPosition()), shape.GetOrientation()) * matrixParent);
		decDVector shapePosition(matrixShape.GetPosition());
		
		decDVector position(shapePosition);
		decVector size(1.0f, 1.0f, 1.0f);
		TransformElement(position, size);
		
		if(modifyPosition){
			shapePosition = position;
		}
		
		shape.Visit(scaler);
		
		matrixShape = decDMatrix::CreateWorld(shapePosition, matrixShape.ToQuaternion()) * matrixParentInverse;
		shape.SetPosition(matrixShape.GetPosition().ToVector());
		shape.SetOrientation(matrixShape.ToQuaternion());
	});
	
	decString newValue;
	codec.EncodeShapeList(shapeList, newValue);
	
	pObject->SetProperty(pProperty, newValue);
	
	pObject->GetWorld()->NotifyObjectShapeSelectionChanged();
}

void meUObjectShapeScale::ProgressiveRedo(){
	Redo();
}
