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

#include "reSelectionShapes.h"
#include "reRigShape.h"
#include "../reRig.h"

#include <dragengine/common/exceptions.h>



// Class reSelectionShapes
////////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionShapes::reSelectionShapes(reRig *rig){
	DEASSERT_NOTNULL(rig)
	pRig = rig;
}

reSelectionShapes::~reSelectionShapes(){
	Reset();
}



// Management
///////////////

int reSelectionShapes::IndexOfShapeWith(deColliderVolume *collider) const{
	return pShapes.IndexOfMatching([&](const reRigShape &s){
		return s.GetCollider() == collider;
	});
}

void reSelectionShapes::AddShape(reRigShape *shape){
	if(pShapes.Has(shape)) DETHROW(deeInvalidParam);
	
	pShapes.Add(shape);
	shape->SetSelected(true);
	
	pRig->NotifyShapeSelectedChanged(shape);
	
	if(!pActiveShape){
		SetActiveShape(shape);
	}
}

void reSelectionShapes::RemoveShape(reRigShape *shape){
	const int index = pShapes.IndexOf(shape);
	if(index == -1) DETHROW(deeInvalidParam);
	
	pShapes.RemoveFrom(index);
	shape->SetSelected(false);
	
	if(shape == pActiveShape){
		if(pShapes.IsNotEmpty()){
			SetActiveShape(pShapes.First());
			
		}else{
			SetActiveShape(nullptr);
		}
	}
	
	pRig->NotifyShapeSelectedChanged(shape);
}

void reSelectionShapes::RemoveAllShapes(){
	SetActiveShape(nullptr);
	
	pRig->NotifyAllShapesDeselected();
	
	pShapes.Visit([](reRigShape *shape){
		shape->SetSelected(false);
	});
	pShapes.RemoveAll();
}



bool reSelectionShapes::HasActiveShape() const{
	return pActiveShape.IsNotNull();
}

void reSelectionShapes::SetActiveShape(reRigShape *shape){
	if(shape != pActiveShape){
		if(shape){
			DEASSERT_TRUE(pShapes.Has(shape))
		}
		
		if(pActiveShape){
			pActiveShape->SetActive(false);
		}
		
		pActiveShape = shape;
		
		if(shape){
			shape->SetActive(true);
		}
		
		pRig->NotifyActiveShapeChanged();
	}
}

void reSelectionShapes::Reset(){
	RemoveAllShapes();
}
void reSelectionShapes::AddVisibleShapesTo(reRigShape::List &list) const{
	pShapes.Visit([&list](reRigShape *shape){
		if(shape->IsVisible()){
			list.Add(shape);
		}
	});
}
