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

#include "sePropertyNodeGroup.h"
#include "../seProperty.h"
#include "../../seSkin.h"
#include "../../texture/seTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>



// Class sePropertyNodeGroup
//////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeGroup::sePropertyNodeGroup(const deEngine &engine) :
sePropertyNode(entGroup, engine, MappedCount),
pProperty(nullptr),
pActiveGroup(false){
}

sePropertyNodeGroup::sePropertyNodeGroup(const sePropertyNodeGroup &node) :
sePropertyNode(node),
pProperty(nullptr),
pActiveGroup(false)
{
	node.pNodes.Visit([&](const sePropertyNode &child){
		const sePropertyNode::Ref childNode(child.Copy());
		pNodes.Add(childNode);
		childNode->SetParent(this);
	});
}

sePropertyNodeGroup::~sePropertyNodeGroup(){
}



// Management
///////////////

seProperty *sePropertyNodeGroup::GetProperty() const{
	if(pProperty){
		return pProperty;
		
	}else{
		return sePropertyNode::GetProperty();
	}
}

void sePropertyNodeGroup::SetProperty(seProperty *property){
	if(property == pProperty){
		return;
	}
	
	pProperty = property;
	UpdateResources();
}

void sePropertyNodeGroup::SetActiveGroup(bool active){
	pActiveGroup = active;
}



void sePropertyNodeGroup::AddNode(sePropertyNode *node){
	if(!node || node->GetProperty()){
		DETHROW(deeInvalidParam);
	}
	
	pNodes.Add(node);
	node->SetParent(this);
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::InsertNode(int index, sePropertyNode *node){
	if(!node || node->GetProperty()){
		DETHROW(deeInvalidParam);
	}
	
	pNodes.Insert(node, index);
	node->SetParent(this);
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::RemoveNode(sePropertyNode *node){
	seProperty * const property = GetProperty();
	if(property){
		if(property->GetNodeSelection().GetActive() == node){
			property->GetNodeSelection().SetActive(nullptr);
		}
	}
	
	if(!pNodes.Has(node)){
		DETHROW(deeInvalidParam);
	}
	
	node->SetParent(nullptr);
	pNodes.Remove(node);
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::RemoveAllNodes(){
	seProperty * const property = GetProperty();
	pNodes.Visit([&](sePropertyNode *node){
		if(property && property->GetNodeSelection().GetActive() == node){
			property->GetNodeSelection().SetActive(nullptr);
		}
		
		node->SetParent(nullptr);
	});
	
	pNodes.RemoveAll();
	
	NotifyStructreChanged();
}

void sePropertyNodeGroup::MoveNode(sePropertyNode *node, int index){
	pNodes.Move(node, index);
	NotifyStructreChanged();
}



sePropertyNode::Ref sePropertyNodeGroup::Copy() const{
	return sePropertyNodeGroup::Ref::New(*this);
}

void sePropertyNodeGroup::UpdateResources(){
	sePropertyNode::UpdateResources();
	pNodes.Visit([&](sePropertyNode &node){
		node.UpdateResources();
	});
}
