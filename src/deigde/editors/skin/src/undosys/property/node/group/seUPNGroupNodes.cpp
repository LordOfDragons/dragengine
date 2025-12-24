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

seUPNGroupNodes::seUPNGroupNodes(const sePropertyNode::List &nodes){
	sePropertyNodeGroup * const parentGroup = nodes.First()->GetParent();
	if(!parentGroup || !parentGroup->GetProperty()){
		DETHROW(deeInvalidParam);
	}
	
	nodes.Visit([&](const sePropertyNode &node){
		DEASSERT_TRUE(node.GetParent() == parentGroup)
	});
	
	SetShortInfo("Group nodes");
	
	decVector2 minBounds;
	decVector2 maxBounds;
	nodes.VisitIndexed([&](int i, const sePropertyNode &childNode){
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
	});
	
	pOffset.Set(minBounds.x, minBounds.y, 0.0f);
	
	// store nodes sorted by index in the parent group
	nodes.Visit([&](sePropertyNode *node){
		const cNode::Ref unode(cNode::Ref::New());
		unode->position = node->GetPosition();
		unode->index = parentGroup->GetNodes().IndexOf(node);
		unode->node = node;
		pNodes.Add(unode);
	});
	
	pNodes.Sort([](const cNode &a, const cNode &b){
		return a.index < b.index ? -1 : (a.index > b.index ? 1 : 0);
	});
	
	// create node group
	pNodeGroup = sePropertyNodeGroup::Ref::New(parentGroup->GetEngine());
	decPoint rounded(minBounds.Round());
	pNodeGroup->SetPosition(decPoint3(rounded.x, rounded.y, parentGroup->GetPosition().z));
	
	rounded = (maxBounds - minBounds).Round();
	pNodeGroup->SetSize(decPoint3(rounded.x, rounded.y, 1));
	
	pParentGroup = parentGroup;
}

seUPNGroupNodes::~seUPNGroupNodes(){
}



// Management
///////////////

void seUPNGroupNodes::Undo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	pNodeGroup->RemoveAllNodes();
	pParentGroup->RemoveNode(pNodeGroup);
	
	pNodes.Visit([&](const cNode &unode){
		unode.node->SetPosition(unode.position);
		pParentGroup->InsertNode(unode.index, unode.node);
		selection.Add(unode.node);
	});
}

void seUPNGroupNodes::Redo(){
	sePropertyNodeSelection &selection = pParentGroup->GetProperty()->GetNodeSelection();
	selection.RemoveAll();
	
	pNodes.Visit([&](const cNode &unode){
		pParentGroup->RemoveNode(unode.node);
		unode.node->SetPosition((decVector(unode.position) - pOffset).Round());
		pNodeGroup->AddNode(unode.node);
	});
	
	pParentGroup->AddNode(pNodeGroup);
	
	selection.Add(pNodeGroup);
}
