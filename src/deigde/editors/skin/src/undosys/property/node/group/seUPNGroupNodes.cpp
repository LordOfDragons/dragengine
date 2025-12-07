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

#include "seUPNGroupNodes.h"
#include "../../../../skin/property/seProperty.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNGroupNodes
//////////////////////////

// Constructor, destructor
////////////////////////////

seUPNGroupNodes::seUPNGroupNodes(const sePropertyNodeList &nodes) :
pParentGroup(NULL),
pNodeGroup(NULL),
pNodeCount(0),
pNodes(NULL)
{
	const int count = nodes.GetCount();
	int i;
	
	sePropertyNodeGroup * const parentGroup = nodes.GetAt(0)->GetParent();
	if(! parentGroup || ! parentGroup->GetProperty()){
		DETHROW(deeInvalidParam);
	}
	
	for(i=0; i<count; i++){
		if(nodes.GetAt(i)->GetParent() != parentGroup){
			DETHROW(deeInvalidParam);
		}
	}
	
	SetShortInfo("Group nodes");
	
	decVector2 minBounds;
	decVector2 maxBounds;
	for(i=0; i<count; i++){
		const sePropertyNode &childNode = *nodes.GetAt(i);
		const decTexMatrix2 childTransform(childNode.CreateParentTransformMatrix());
		const decPoint3 &childSize = childNode.GetSize();
		const decVector2 absSize((float)abs(childSize.x), (float)abs(childSize.y));
		
		const decVector2 p1(childTransform.GetPosition());
		const decVector2 p2(childTransform * decVector2(absSize.x, 0.0f));
		const decVector2 p3(childTransform * decVector2(0.0f, absSize.y));
		const decVector2 p4(childTransform * absSize);
		
		const decVector2 smallest(p1.Smallest(p2).Smallest(p3).Smallest(p4));
		const decVector2 largest(p1.Largest(p2).Largest(p3).Largest(p4));
		
		if(i == 0){
			minBounds = smallest;
			maxBounds = largest;
			
		}else{
			minBounds.SetSmallest(smallest);
			maxBounds.SetLargest(largest);
		}
	}
	
	pOffset.Set(minBounds.x, minBounds.y, 0.0f);
	
	try{
		// store nodes sorted by index in the parent group
		pNodes = new sNode[count];
		for(pNodeCount=0; pNodeCount<count; pNodeCount++){
			sePropertyNode * const node = nodes.GetAt(pNodeCount);
			pNodes[pNodeCount].position = node->GetPosition();
			pNodes[pNodeCount].index = parentGroup->IndexOfNode(node);
			pNodes[pNodeCount].node = node;
			node->AddReference();
		}
		
		for(i=1; i<pNodeCount; i++){
			if(pNodes[i].index >= pNodes[i - 1].index){
				continue;
			}
			
			const sNode temp(pNodes[i - 1]);
			pNodes[i - 1] = pNodes[i];
			pNodes[i] = temp;
			
			if(i > 1){
				i -= 2;
			}
		}
		
		// create node group
		pNodeGroup = new sePropertyNodeGroup(parentGroup->GetEngine());
		decPoint rounded(minBounds.Round());
		pNodeGroup->SetPosition(decPoint3(rounded.x, rounded.y, parentGroup->GetPosition().z));
		
		rounded = (maxBounds - minBounds).Round();
		pNodeGroup->SetSize(decPoint3(rounded.x, rounded.y, 1));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	pParentGroup = parentGroup;
	parentGroup->AddReference();
}

seUPNGroupNodes::~seUPNGroupNodes(){
	pCleanUp();
}



// Management
///////////////

void seUPNGroupNodes::Undo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	pNodeGroup->RemoveAllNodes();
	pParentGroup->RemoveNode(pNodeGroup);
	
	int i;
	for(i=0; i<pNodeCount; i++){
		pNodes[i].node->SetPosition(pNodes[i].position);
		pParentGroup->InsertNode(pNodes[i].index, pNodes[i].node);
		selection.Add(pNodes[i].node);
	}
}

void seUPNGroupNodes::Redo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	int i;
	for(i=0; i<pNodeCount; i++){
		pParentGroup->RemoveNode(pNodes[i].node);
		pNodes[i].node->SetPosition((decVector(pNodes[i].position) - pOffset).Round());
		pNodeGroup->AddNode(pNodes[i].node);
	}
	
	pParentGroup->AddNode(pNodeGroup);
	
	selection.Add(pNodeGroup);
}



// Private Functions
//////////////////////

void seUPNGroupNodes::pCleanUp(){
	if(pNodeGroup){
		pNodeGroup->FreeReference();
	}
	if(pParentGroup){
		pParentGroup->FreeReference();
	}
	if(pNodes){
		int i;
		for(i=0; i<pNodeCount; i++){
			pNodes[i].node->FreeReference();
		}
		delete [] pNodes;
	}
}
