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

#include "seUPropertyNodeData.h"
#include "seUPropertyNodesResize.h"
#include "../../../../skin/property/node/sePropertyNode.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodesResize
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodesResize::seUPropertyNodesResize(const sePropertyNode::List &nodes,
const decVector2 &pivot, const decVector2 &origin, float rotation, float shearing) :
pPivot(pivot),
pOrigin(origin),
pRotation(rotation),
pShearing(shearing)
{
	SetShortInfo("Resize nodes");
	
	nodes.Visit([&](sePropertyNode *n){
		pAddNodes(n);
	});
}

seUPropertyNodesResize::~seUPropertyNodesResize(){
}



// Management
///////////////

void seUPropertyNodesResize::SetDistance(const decVector2 &distance){
	if(distance.IsEqualTo(pDistance)){
		return;
	}
	
	pDistance = distance;
	
	if(distance.Length() < 0.5f){
		pTransform.SetIdentity();
		return;
	}
	
	const decTexMatrix2 matrix(
		decTexMatrix2::CreateRotation(-pRotation)
		* decTexMatrix2::CreateShear( -pShearing, 0.0f ) );
	
	decVector2 piv2org(matrix * (pOrigin - pPivot));
	if(fabsf(piv2org.x) < 0.5f){
		piv2org.x = 1.0f;
	}
	if(fabsf(piv2org.y) < 0.5f){
		piv2org.y = 1.0f;
	}
	
	const decVector2 distance2(matrix * distance);
	const decVector2 fscale(1.0f + distance2.x / piv2org.x, 1.0f + distance2.y / piv2org.y);
	
	/*if( GetNodeCount() == 1 ){
		rotation = GetNodeAt(0).rotation;
		shearing = GetNodeAt(0).shearing;
		
		SetTransform(
			decTexMatrix2::CreateTranslation()
			decTexMatrix2::CreateTranslation(-pPivot)
			* decTexMatrix2::CreateScale( fscale )
			* decTexMatrix2::CreateTranslation( pPivot ) );
		
	}else{*/
		pTransform =
			decTexMatrix2::CreateTranslation(-pPivot)
			* matrix
			* decTexMatrix2::CreateScale( fscale )
			* decTexMatrix2::CreateShear( pShearing, 0.0f )
			* decTexMatrix2::CreateRotation( pRotation )
			* decTexMatrix2::CreateTranslation( pPivot );
	//}
}

void seUPropertyNodesResize::Undo(){
	pNodes.Visit([](const seUPropertyNodeData &data){
		sePropertyNode &node = *data.GetNode();
		
		node.SetPosition(data.GetPosition());
		node.SetSize(data.GetSize());
		node.SetRotation(data.GetRotation());
		node.SetShearing(data.GetShearing());
	});
}

void seUPropertyNodesResize::Redo(){
	if(pTransform.IsEqualTo(decTexMatrix2())){
		return;
	}
	
	pNodes.Visit([&](const seUPropertyNodeData &data){
		sePropertyNode &node = *data.GetNode();
		
		node.SetFromMatrix(data.GetMatrix() * pTransform * data.CreateInverseParentMatrix(),
			data.GetSize(), data.GetRotation());
	});
}



// Private Functions
//////////////////////

void seUPropertyNodesResize::pAddNodes(sePropertyNode *node){
	pNodes.Add(seUPropertyNodeData::Ref::New(node));
	
	if(node->GetMask()){
		pAddNodes(node->GetMask());
	}
	
	if(node->GetNodeType() == sePropertyNode::entGroup){
		const sePropertyNodeGroup &nodeGroup = (sePropertyNodeGroup&)*node;
		nodeGroup.GetNodes().Visit([&](sePropertyNode *n){
			pAddNodes(n);
		});
	}
}
