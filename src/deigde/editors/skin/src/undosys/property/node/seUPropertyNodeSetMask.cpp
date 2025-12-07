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
#include <string.h>
#include <stdlib.h>

#include "seUPropertyNodeSetMask.h"
#include "../../../skin/property/seProperty.h"
#include "../../../skin/property/node/sePropertyNode.h"
#include "../../../skin/property/node/sePropertyNodeGroup.h"
#include "../../../skin/property/node/sePropertyNodeSelection.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodeSetMask
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeSetMask::seUPropertyNodeSetMask(sePropertyNode *node, sePropertyNode *mask) :
pNode(NULL),
pMask(NULL)
{
	if(!node || !node->GetProperty() || node->GetMask() || !mask){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Node set mask");
	
	pOldPosition = mask->GetPosition();
	pOldSize = mask->GetSize();
	pOldRotation = mask->GetRotation();
	pOldShearing = mask->GetShearing();
	
	pMask = mask;
	mask->AddReference();
	
	pNode = node;
	node->AddReference();
}

seUPropertyNodeSetMask::~seUPropertyNodeSetMask(){
	if(pNode){
		pNode->FreeReference();
	}
	if(pMask){
		pMask->FreeReference();
	}
}



// Management
///////////////

void seUPropertyNodeSetMask::Undo(){
	pNode->SetMask(NULL);
	
	pMask->SetPosition(pOldPosition);
	pMask->SetSize(pOldSize);
	pMask->SetRotation(pOldRotation);
	pMask->SetShearing(pOldShearing);
	
	pNode->GetParent()->AddNode(pMask);
}

void seUPropertyNodeSetMask::Redo(){
	const decTexMatrix2 matrix((pMask->CreateParentTransformMatrix().ToTexMatrix()
		* pNode->CreateParentTransformMatrix().Invert() ).ToTexMatrix2() );
	
	pNode->GetProperty()->GetNodeSelection().Remove(pMask);
	pNode->GetParent()->RemoveNode(pMask);
	pMask->SetFromMatrix(matrix, pOldSize, pOldRotation);
	pNode->SetMask(pMask);
}
