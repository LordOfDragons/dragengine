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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reSelectionShapes.h"
#include "reRigShape.h"
#include "reRigShapeList.h"
#include "../reRig.h"
#include "dragengine/common/exceptions.h"



// Class reSelectionShapes
////////////////////////////

// Constructor, destructor
////////////////////////////

reSelectionShapes::reSelectionShapes(reRig *rig){
	if(!rig) DETHROW(deeInvalidParam);
	
	pRig = rig;
	
	pShapes = NULL;
	pShapeCount = 0;
	pShapeSize = 0;
	pActiveShape = NULL;
}

reSelectionShapes::~reSelectionShapes(){
	Reset();
	if(pShapes) delete [] pShapes;
}



// Management
///////////////

reRigShape *reSelectionShapes::GetShapeAt(int index) const{
	if(index < 0 || index >= pShapeCount) DETHROW(deeOutOfBoundary);
	
	return pShapes[index];
}

bool reSelectionShapes::HasShape(reRigShape *shape) const{
	if(!shape) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(shape == pShapes[i]){
			return true;
		}
	}
	
	return false;
}
	
int reSelectionShapes::IndexOfShape(reRigShape *shape) const{
	if(!shape) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(shape == pShapes[i]){
			return i;
		}
	}
	
	return -1;
}

int reSelectionShapes::IndexOfShapeWith(deColliderVolume *collider) const{
	if(!collider) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pShapeCount; i++){
		if(collider == pShapes[i]->GetCollider()){
			return i;
		}
	}
	
	return -1;
}

void reSelectionShapes::AddShape(reRigShape *shape){
	if(HasShape(shape)) DETHROW(deeInvalidParam);
	
	if(pShapeCount == pShapeSize){
		int newSize = pShapeSize * 3 / 2 + 1;
		reRigShape **newArray = new reRigShape*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pShapes){
			memcpy(newArray, pShapes, sizeof(reRigShape*) * pShapeSize);
			delete [] pShapes;
		}
		pShapes = newArray;
		pShapeSize = newSize;
	}
	
	pShapes[pShapeCount] = shape;
	pShapeCount++;
	shape->SetSelected(true);
	
	pRig->NotifyShapeSelectedChanged(shape);
	
	if(pActiveShape == NULL){
		SetActiveShape(shape);
	}
}

void reSelectionShapes::RemoveShape(reRigShape *shape){
	int i, index = IndexOfShape(shape);
	if(index == -1) DETHROW(deeInvalidParam);
	
	for(i=index+1; i<pShapeCount; i++){
		pShapes[i - 1] = pShapes[i];
	}
	pShapes[pShapeCount - 1] = NULL;
	pShapeCount--;
	
	shape->SetSelected(false);
	
	if(shape == pActiveShape){
		if(pShapeCount > 0){
			SetActiveShape(pShapes[0]);
			
		}else{
			SetActiveShape(NULL);
		}
	}
	
	pRig->NotifyShapeSelectedChanged(shape);
}

void reSelectionShapes::RemoveAllShapes(){
	SetActiveShape(NULL);
	
	pRig->NotifyAllShapesDeselected();
	
	while(pShapeCount > 0){
		pShapeCount--;
		
		pShapes[pShapeCount]->SetSelected(false);
		pShapes[pShapeCount]->FreeReference();
	}
}



bool reSelectionShapes::HasActiveShape() const{
	return pActiveShape != NULL;
}

void reSelectionShapes::SetActiveShape(reRigShape *shape){
	if(shape != pActiveShape){
		if(shape && !HasShape(shape)) DETHROW(deeInvalidParam);
		
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

void reSelectionShapes::AddVisibleShapesTo(reRigShapeList &list) const{
	int s;
	
	for(s=0; s<pShapeCount; s++){
		if(pShapes[s]->IsVisible()){
			list.AddShape(pShapes[s]);
		}
	}
}
