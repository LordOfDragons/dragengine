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

#include "seUPNUngroupNodes.h"
#include "../../../../skin/property/seProperty.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPNUngroupNodes
////////////////////////////

// Constructor, destructor
////////////////////////////

seUPNUngroupNodes::seUPNUngroupNodes(sePropertyNodeGroup *nodeGroup) :

pIndex(0)
{
	if(!nodeGroup){
		DETHROW(deeInvalidParam);
	}
	
	sePropertyNodeGroup * const parentGroup = nodeGroup->GetParent();
	if(!parentGroup){
		DETHROW(deeInvalidParam);
	}
	
	SetShortInfo("Ungroup nodes");
	
	pIndex = parentGroup->GetNodes().IndexOf(nodeGroup);
	
	nodeGroup->GetNodes().Visit([&](sePropertyNode *node){
		const cNode::Ref unode(cNode::Ref::New());
		unode->position = node->GetPosition();
		unode->size = node->GetSize();
		unode->rotation = node->GetRotation();
		unode->shearing = node->GetShearing();
		unode->node = node;
		pNodes.Add(unode);
	});
	
	pParentGroup = parentGroup;
	
	pNodeGroup = nodeGroup;
}

seUPNUngroupNodes::~seUPNUngroupNodes(){
}



// Management
///////////////

void seUPNUngroupNodes::Undo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	pNodes.Visit([&](const cNode &unode){
		pParentGroup->RemoveNode(unode.node);
		unode.node->SetPosition(unode.position);
		unode.node->SetSize(unode.size);
		unode.node->SetRotation(unode.rotation);
		unode.node->SetShearing(unode.shearing);
		pNodeGroup->AddNode(unode.node);
	});
	
	pParentGroup->InsertNode(pIndex, pNodeGroup);
	
	selection.Add(pNodeGroup);
}

void seUPNUngroupNodes::Redo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	const decTexMatrix2 matrix(pNodeGroup->CreateParentTransformMatrix());
	
	pParentGroup->RemoveNode(pNodeGroup);
	pNodeGroup->RemoveAllNodes();
	
	pNodes.Visit([&](const cNode &unode){
		unode.node->SetFromMatrix(unode.node->CreateParentTransformMatrix() * matrix,
			unode.size, unode.rotation);
		pParentGroup->AddNode(unode.node);
		selection.Add(unode.node);
	});
}
