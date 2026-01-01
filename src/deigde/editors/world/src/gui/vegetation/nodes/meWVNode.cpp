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

#include "meWVNode.h"
#include "meWVNodeSlot.h"
#include "../meWindowVegetation.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/meUHTVRuleMove.h"
#include "../../../world/meWorld.h"
#include "../../../world/heightterrain/meHTVegetationLayer.h"
#include "../../../world/heightterrain/rules/meHTVRule.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/meUHTVRuleAdd.h"
#include "../../../undosys/gui/heightterrain/vegetation/rule/meUHTVRuleRemove.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/nodeview/igdeNVNodeListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cActionDeleteNode : public igdeAction {
	meWVNode &pNode;
	
public:
	typedef deTObjectReference<cActionDeleteNode> Ref;
	cActionDeleteNode(meWVNode &node) : igdeAction("Delete Node",
		node.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Delete Node"),
	pNode(node){}
	
	void OnAction() override{
		if(!pNode.CanDelete()){
			return;
		}
		
		meWindowVegetation &view = *((meWindowVegetation*)pNode.GetOwnerBoard());
		view.GetWorld()->GetUndoSystem()->Add(
			meUHTVRuleRemove::Ref::New(view.GetVLayer(), pNode.GetRule()));
	}
	
	void Update() override{
		SetEnabled(pNode.CanDelete());
	}
};

class cActionDuplicateNode : public igdeAction {
	meWVNode &pNode;
	
public:
	typedef deTObjectReference<cActionDuplicateNode> Ref;
	cActionDuplicateNode(meWVNode &node) : igdeAction("Duplicate Node",
		node.GetEnvironment().GetStockIcon(igdeEnvironment::esiDuplicate), "Duplicate Node"),
	pNode(node){}
	
	void OnAction() override{
		if(!pNode.CanDuplicate()){
			return;
		}
		
		meWindowVegetation &view = pNode.GetWindowVegetation();
		
		const meHTVRule::Ref rule(pNode.GetRule()->Copy());
		rule->SetPosition(rule->GetPosition() + decVector2(0.5f, 0.5f));
		
		view.GetWorld()->GetUndoSystem()->Add(meUHTVRuleAdd::Ref::New(view.GetVLayer(), rule));
	}
	
	void Update() override{
		SetEnabled(pNode.CanDuplicate());
	}
};


class cActivationListener : public igdeNVNodeListener{
	meWVNode &pNode;
	
public:
	typedef deTObjectReference<cActivationListener> Ref;
	
	cActivationListener(meWVNode &node) : pNode(node){}
	
	virtual void OnActivated(igdeNVNode*){
		meHTVegetationLayer * const vlayer = pNode.GetWindowVegetation().GetVLayer();
		if(vlayer){
			vlayer->SetActiveRule(pNode.GetRule());
		}
	}
	
	virtual void AddContextMenuEntries(igdeNVNode*, igdeMenuCascade &menu){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand(menu, cActionDeleteNode::Ref::New(pNode));
		helper.MenuCommand(menu, cActionDuplicateNode::Ref::New(pNode));
	}
};

class cDragNodeListener : public igdeNVNodeListener{
	meWVNode &pNode;
	igdeUndo::Ref &pUndo;
	
public:
	typedef deTObjectReference<cDragNodeListener> Ref;
	
	cDragNodeListener(meWVNode &node, igdeUndo::Ref &undo) :
		pNode(node), pUndo(undo){}
	
	virtual void OnDragBegin(igdeNVNode*){
		meHTVegetationLayer * const vlayer = pNode.GetWindowVegetation().GetVLayer();
		if(vlayer){
			pUndo = meUHTVRuleMove::Ref::New(vlayer, pNode.GetRule());
		}
	}
	
	virtual void OnDraging(igdeNVNode *node){
		if(!pUndo){
			return;
		}
		
		((meUHTVRuleMove&)(igdeUndo&)pUndo).SetNewPosition(
			decVector2(node->GetPosition()) * pNode.GetWindowVegetation().GetPixelToUnits());
		pUndo->Redo();
	}
	
	virtual void OnDragEnd(igdeNVNode*){
		if(!pUndo){
			return;
		}
		
		pNode.GetWindowVegetation().GetWorld()->GetUndoSystem()->Add(pUndo, false);
		pUndo = nullptr;
	}
};

}



// Class meWVNode
///////////////////

// Constructor, destructor
////////////////////////////

meWVNode::meWVNode(meWindowVegetation &windowVegetation, meHTVRule *rule) :
igdeNVNode(windowVegetation.GetEnvironment(), "Rule"),
pWindowVegetation(windowVegetation)
{
	if(!rule){
		DETHROW(deeInvalidParam);
	}
	
	AddListener(cActivationListener::Ref::New(*this));
	AddListener(cDragNodeListener::Ref::New(*this, pUndoMove));
	
	pRule = rule;
}

meWVNode::~meWVNode(){
}



// Management
///////////////

void meWVNode::Update(){
}

void meWVNode::UpdateClassLists(){
}

void meWVNode::UpdatePositionFromRule(){
	SetPosition(decPoint(pRule->GetPosition() * pWindowVegetation.GetUnitsToPixel()));
}

bool meWVNode::CanDelete() const{
	return true;
}

bool meWVNode::CanDuplicate() const{
	return true;
}

void meWVNode::OnGameDefinitionChanged(){
}
