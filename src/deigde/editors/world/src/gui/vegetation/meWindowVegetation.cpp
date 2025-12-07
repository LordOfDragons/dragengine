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

#include "meWindowVegetation.h"
#include "meWindowVegetationListener.h"
#include "nodes/meWVNode.h"
#include "nodes/meWVNodeSlot.h"
#include "nodes/meWVNodeClosestProp.h"
#include "nodes/meWVNodeClosestVegetation.h"
#include "nodes/meWVNodeCombine.h"
#include "nodes/meWVNodeComponents.h"
#include "nodes/meWVNodeCurve.h"
#include "nodes/meWVNodeGeometry.h"
#include "nodes/meWVNodeMapping.h"
#include "nodes/meWVNodeMath.h"
#include "nodes/meWVNodeMultiMath.h"
#include "nodes/meWVNodeVectorMath.h"
#include "nodes/meWVNodeRandom.h"
#include "nodes/meWVNodeResult.h"
#include "nodes/meWVNodeConstant.h"
#include "nodes/meWVNodePropCount.h"
#include "../meWindowMain.h"
#include "../../undosys/gui/heightterrain/vegetation/rule/meUHTVRuleAdd.h"
#include "../../undosys/gui/heightterrain/vegetation/link/meUHTVLinkAdd.h"
#include "../../undosys/gui/heightterrain/vegetation/link/meUHTVLinkCut.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/heightterrain/meHTVegetationLayer.h"
#include "../../world/heightterrain/rules/meHTVRLink.h"
#include "../../world/heightterrain/rules/meHTVRule.h"
#include "../../world/heightterrain/rules/meHTVRuleClosestProp.h"
#include "../../world/heightterrain/rules/meHTVRuleClosestVegetation.h"
#include "../../world/heightterrain/rules/meHTVRuleCombine.h"
#include "../../world/heightterrain/rules/meHTVRuleComponents.h"
#include "../../world/heightterrain/rules/meHTVRuleCurve.h"
#include "../../world/heightterrain/rules/meHTVRuleGeometry.h"
#include "../../world/heightterrain/rules/meHTVRuleMapping.h"
#include "../../world/heightterrain/rules/meHTVRuleMath.h"
#include "../../world/heightterrain/rules/meHTVRuleMultiMath.h"
#include "../../world/heightterrain/rules/meHTVRuleVectorMath.h"
#include "../../world/heightterrain/rules/meHTVRuleRandom.h"
#include "../../world/heightterrain/rules/meHTVRuleResult.h"
#include "../../world/heightterrain/rules/meHTVRuleConstant.h"
#include "../../world/heightterrain/rules/meHTVRulePropCount.h"
#include "../../world/heightterrain/rules/meHTVRSlot.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/nodeview/igdeNVBoardListener.h>
#include <deigde/gui/nodeview/igdeNVLink.h>
#include <deigde/gui/nodeview/igdeNVNode.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cActionAddNode : public igdeAction {
protected:
	meWindowVegetation &pView;
	meHTVRule::eRuleTypes pType;
	decVector2 pPosition;
	
public:
	cActionAddNode(meWindowVegetation &view, meHTVRule::eRuleTypes type,
		const decVector2 &position, const char *text, igdeIcon *icon) :
	igdeAction(text, icon, "Add Node"), pView(view), pType(type), pPosition(position){}
	
	virtual void OnAction(){
		if(!pView.GetVLayer() || !pView.GetWorld()){
			return;
		}
		
		const meHTVRule::Ref rule(meHTVRule::Ref::New(CreateRule()));
		rule->SetPosition(pPosition);
		
		pView.GetWorld()->GetUndoSystem()->Add(meUHTVRuleAdd::Ref::NewWith(pView.GetVLayer(), rule));
	}
	
	virtual void Update(){
		SetEnabled(pView.GetVLayer() && pView.GetWorld());
	}
	
	virtual meHTVRule *CreateRule() = 0;
};

class cActionAddClosestProp : public cActionAddNode {
public:
	cActionAddClosestProp(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Closest Prop", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleClosestProp;
	}
};

class cActionAddClosestVegetation : public cActionAddNode {
public:
	cActionAddClosestVegetation(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Closest Vegetation", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleClosestVegetation;
	}
};

class cActionAddCombine : public cActionAddNode {
public:
	cActionAddCombine(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Combine", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleCombine;
	}
};

class cActionAddComponents : public cActionAddNode {
public:
	cActionAddComponents(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Components", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleComponents;
	}
};

class cActionAddCurve : public cActionAddNode {
public:
	cActionAddCurve(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Curve", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleCurve;
	}
};

class cActionAddGeometry : public cActionAddNode {
public:
	cActionAddGeometry(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Geometry", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleGeometry;
	}
};

class cActionAddMapping : public cActionAddNode {
public:
	cActionAddMapping(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Mapping", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleMapping;
	}
};

class cActionAddMath : public cActionAddNode {
public:
	cActionAddMath(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Math", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleMath;
	}
};

class cActionAddMultiMath : public cActionAddNode {
public:
	cActionAddMultiMath(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Multi-Math", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleMultiMath;
	}
};

class cActionAddVectorMath : public cActionAddNode {
public:
	cActionAddVectorMath(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Vector Math", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleVectorMath;
	}
};

class cActionAddRandom : public cActionAddNode {
public:
	cActionAddRandom(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Random", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleRandom;
	}
};

class cActionAddConstant : public cActionAddNode {
public:
	cActionAddConstant(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Constant", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRuleConstant;
	}
};

class cActionAddPropCount : public cActionAddNode {
public:
	cActionAddPropCount(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Prop Count", NULL){}
	
	meHTVRule * CreateRule() override{
		return new meHTVRulePropCount;
	}
};


class cActionViewAutoUpdate : public igdeAction {
	meWindowVegetation &pView;
	
public:
	cActionViewAutoUpdate(meWindowVegetation &view) : igdeAction(
		"Auto Update Vegetation", NULL, "Auto Update Vegetation"), pView(view){}
	
	virtual void OnAction(){
		if(pView.GetWorld()){
			pView.GetWorld()->GetGuiParameters().SetAutoUpdateVegetation(
				!pView.GetWorld()->GetGuiParameters().GetAutoUpdateVegetation());
		}
	}
	
	virtual void Update(){
		SetEnabled(pView.GetWorld());
		SetSelected(pView.GetWorld() && pView.GetWorld()->GetGuiParameters().GetAutoUpdateVegetation());
	}
};


class cBoardListener : public igdeNVBoardListener {
	meWindowVegetation &pView;
	
public:
	cBoardListener(meWindowVegetation &view) : pView(view){}
	
	meHTVRLink *RuleLinkForNodeLink(igdeNVSlot *sourceSlot, igdeNVSlot *targetSlot) const{
		if(!pView.GetVLayer()){
			return NULL;
		}
		
		const meWVNode &wvnodeSource = (meWVNode&)*sourceSlot->GetOwnerNode();
		const int indexSlotSource = wvnodeSource.IndexOfSlot(sourceSlot);
		const meWVNode &wvnodeTarget = (meWVNode&)*targetSlot->GetOwnerNode();
		const int indexSlotTarget = wvnodeTarget.IndexOfSlot(targetSlot);
		if(indexSlotSource == -1 || indexSlotTarget == -1){
			return NULL;
		}
		
		const meHTVegetationLayer &vlayer = *pView.GetVLayer();
		const int count = vlayer.GetLinkCount();
		int i;
		
		for(i=0; i<count; i++){
			meHTVRLink * const ruleLink = vlayer.GetLinkAt(i);
			if(ruleLink->GetSourceRule() == wvnodeSource.GetRule()
			&& ruleLink->GetSourceSlot() == indexSlotSource
			&& ruleLink->GetDestinationRule() == wvnodeTarget.GetRule()
			&& ruleLink->GetDestinationSlot() == indexSlotTarget){
				return ruleLink;
			}
		}
		return NULL;
	}
	
	virtual void AddContextMenuEntries(igdeNVBoard*, igdeMenuCascade &menu, const decPoint &position){
		if(!pView.GetVLayer()){
			return;
		}
		
		igdeEnvironment &env = menu.GetEnvironment();
		igdeMenuCascade::Ref subMenu, subMenu2;
		igdeUIHelper &helper = env.GetUIHelper();
		
		
		// nodes
		const decVector2 addNodePosition(decVector2(position - pView.GetSize() / 2)
			* pView.GetPixelToUnits() - pView.GetVLayer()->GetViewCenter() );
		
		subMenu.TakeOver(new igdeMenuCascade(env, "Add Node",
			env.GetStockIcon(igdeEnvironment::esiPlus), "Add Node"));
		menu.AddChild(subMenu);
		
		subMenu2.TakeOver(new igdeMenuCascade(env, "Input", NULL, "Input"));
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, new cActionAddConstant(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddGeometry(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddRandom(pView, addNodePosition), true);
		
		subMenu2.TakeOver(new igdeMenuCascade(env, "Proximity", NULL, "Proximity"));
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, new cActionAddClosestProp(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddClosestVegetation(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddPropCount(pView, addNodePosition), true);
		
		subMenu2.TakeOver(new igdeMenuCascade(env, "Conversion", NULL, "Conversion"));
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, new cActionAddCurve(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddMapping(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddMath(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddMultiMath(pView, addNodePosition), true);
		
		subMenu2.TakeOver(new igdeMenuCascade(env, "Vector", NULL, "Vector"));
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, new cActionAddCombine(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddComponents(pView, addNodePosition), true);
		helper.MenuCommand(subMenu2, new cActionAddVectorMath(pView, addNodePosition), true);
		
		
		// hover link
		
		
		// view
		helper.MenuSeparator(menu);
		helper.MenuCheck(menu, new cActionViewAutoUpdate(pView), true);
	}
	
	virtual bool CanLink(igdeNVBoard*, igdeNVSlot *source, igdeNVSlot *target){
		if(!pView.GetVLayer()){
			return false;
		}
		
		const meWVNodeSlot &wvslotSource = (meWVNodeSlot&)*source;
		const meWVNodeSlot &wvslotTarget = (meWVNodeSlot&)*target;
		if(wvslotTarget.GetType() == meWVNodeSlot::estValue && wvslotSource.GetType() != meWVNodeSlot::estValue){
			return false;
		}
		
		const meWVNode &wvnodeSource = (meWVNode&)*source->GetOwnerNode();
		const int indexSlotSource = wvnodeSource.IndexOfSlot(source);
		const meWVNode &wvnodeTarget = (meWVNode&)*target->GetOwnerNode();
		const int indexSlotTarget = wvnodeTarget.IndexOfSlot(target);
		if(indexSlotSource == -1 || indexSlotTarget == -1){
			DETHROW(deeInvalidParam);
		}
		
		return !pView.GetVLayer()->LinkProducesLoop(wvnodeSource.GetRule(),
			indexSlotSource, wvnodeTarget.GetRule(), indexSlotTarget);
	}
	
	virtual void OnLinkAdded(igdeNVBoard*, igdeNVLink *link){
		if(!pView.GetVLayer()){
			return;
		}
		
		const meWVNode &wvnodeSource = (meWVNode&)*link->GetSource()->GetOwnerNode();
		const int indexSlotSource = wvnodeSource.IndexOfSlot(link->GetSource());
		const meWVNode &wvnodeTarget = (meWVNode&)*link->GetTarget()->GetOwnerNode();
		const int indexSlotTarget = wvnodeTarget.IndexOfSlot(link->GetTarget());
		if(indexSlotSource == -1 || indexSlotTarget == -1){
			DETHROW(deeInvalidParam);
		}
		
		const meHTVRLink::Ref ruleLink(meHTVRLink::Ref::NewWith(wvnodeSource.GetRule(), indexSlotSource,
			wvnodeTarget.GetRule(), indexSlotTarget));
		
			pView.GetWorld()->GetUndoSystem()->Add(
				meUHTVLinkAdd::Ref::NewWith(pView.GetVLayer(), ruleLink));
	}
	
	virtual void OnLinkRemoved(igdeNVBoard*, igdeNVSlot *source, igdeNVSlot *target){
		if(!pView.GetVLayer()){
			return;
		}
		
		meHTVRLink * const ruleLink = RuleLinkForNodeLink(source, target);
		if(!ruleLink){
			return;
		}
		
		meUHTVLinkCut::Ref undo(meUHTVLinkCut::Ref::NewWith(pView.GetVLayer()));
		undo->AddLinkToCut(ruleLink);
		pView.GetWorld()->GetUndoSystem()->Add(undo);
	}
	
	virtual void OnOffsetChanged(igdeNVBoard *board){
		if(!pView.GetVLayer()){
			return;
		}
		pView.GetVLayer()->SetViewCenter(decVector2(board->GetOffset()) * pView.GetPixelToUnits());
	}
};

}



// Class meWindowVegetation
/////////////////////////////

// Constructor, destructor
////////////////////////////

meWindowVegetation::meWindowVegetation(meWindowMain &windowMain) :
igdeNVBoard(windowMain.GetEnvironment()),
pWindowMain(windowMain),
pListener(NULL),
pWorld(NULL),
pVLayer(NULL),
pUnitsToPixel(igdeApplication::app().DisplayScaled(100.0f)),
pPixelToUnits(1.0f / pUnitsToPixel)
{
	pListener = new meWindowVegetationListener(*this);
	AddListener(new cBoardListener(*this));
}

meWindowVegetation::~meWindowVegetation(){
	SetWorld(NULL);
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meWindowVegetation::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	SetVLayer(NULL);
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		world->AddReference();
		
		const decPoint3 sector; // = pWorld->GetActiveSector();
		SetSector(decPoint(sector.x, sector.z));
	}
}

void meWindowVegetation::SetSector(const decPoint &sector){
	pSector = sector;
	
	meHTVegetationLayer *vlayer = NULL;
	if(pWorld){
		vlayer = pWorld->GetHeightTerrain()->GetActiveVLayer();
	}
	
	SetVLayer(vlayer);
}

void meWindowVegetation::SetVLayer(meHTVegetationLayer *vlayer){
	if(!pWorld && vlayer){
		DETHROW(deeInvalidParam);
	}
	
	if(vlayer == pVLayer){
		return;
	}
	
	Clear();
	
	if(pVLayer){
		pVLayer->FreeReference();
	}
	
	pVLayer = vlayer;
	
	if(vlayer){
		vlayer->AddReference();
	}
	
	UpdateNodesFromVLayer();
}

meHTVRule *meWindowVegetation::GetVRule() const{
	return pVLayer ? pVLayer->GetActiveRule() : NULL;
}



void meWindowVegetation::UpdateNodesFromVLayer(){
	RemoveAllNodes();
	
	if(!pVLayer){
		return;
	}
	
	const int count = pVLayer->GetRuleCount();
	igdeNVNode::Ref node;
	int i;
	
	for(i=0; i<count; i++){
		meHTVRule * const rule = pVLayer->GetRuleAt(i);
		
		switch(rule->GetType()){
		case meHTVRule::ertClosestProp:
			node.TakeOver(new meWVNodeClosestProp(*this, (meHTVRuleClosestProp*)rule));
			break;
			
		case meHTVRule::ertClosestVegetation:
			node.TakeOver(new meWVNodeClosestVegetation(*this, (meHTVRuleClosestVegetation*)rule));
			break;
			
		case meHTVRule::ertCombine:
			node.TakeOver(new meWVNodeCombine(*this, (meHTVRuleCombine*)rule));
			break;
			
		case meHTVRule::ertComponents:
			node.TakeOver(new meWVNodeComponents(*this, (meHTVRuleComponents*)rule));
			break;
			
		case meHTVRule::ertCurve:
			node.TakeOver(new meWVNodeCurve(*this, (meHTVRuleCurve*)rule));
			break;
			
		case meHTVRule::ertGeometry:
			node.TakeOver(new meWVNodeGeometry(*this, (meHTVRuleGeometry*)rule));
			break;
			
		case meHTVRule::ertMapping:
			node.TakeOver(new meWVNodeMapping(*this, (meHTVRuleMapping*)rule));
			break;
			
		case meHTVRule::ertMath:
			node.TakeOver(new meWVNodeMath(*this, (meHTVRuleMath*)rule));
			break;
			
		case meHTVRule::ertMultiMath:
			node.TakeOver(new meWVNodeMultiMath(*this, (meHTVRuleMultiMath*)rule));
			break;
			
		case meHTVRule::ertVectorMath:
			node.TakeOver(new meWVNodeVectorMath(*this, (meHTVRuleVectorMath*)rule));
			break;
			
		case meHTVRule::ertRandom:
			node.TakeOver(new meWVNodeRandom(*this, (meHTVRuleRandom*)rule));
			break;
			
		case meHTVRule::ertResult:
			node.TakeOver(new meWVNodeResult(*this, (meHTVRuleResult*)rule));
			break;
			
		case meHTVRule::ertConstant:
			node.TakeOver(new meWVNodeConstant(*this, (meHTVRuleConstant*)rule));
			break;
			
		case meHTVRule::ertPropCount:
			node.TakeOver(new meWVNodePropCount(*this, (meHTVRulePropCount*)rule));
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
		meWVNode &wvnode = (meWVNode&)(igdeNVNode&)node;
		wvnode.UpdatePositionFromRule();
		wvnode.Update();
		
		AddNode(node);
	}
	
	UpdateLinksFromVLayer();
}

void meWindowVegetation::UpdateLinksFromVLayer(){
	RemoveAllLinks();
	
	if(!pVLayer){
		return;
	}
	
	const int count = pVLayer->GetLinkCount();
	int i;
	
	for(i=0; i<count; i++){
		const meHTVRLink &ruleLink = *pVLayer->GetLinkAt(i);
		
		igdeNVNode * const sourceNode = GetNodeWithRule(ruleLink.GetSourceRule());
		igdeNVNode * const targetNode = GetNodeWithRule(ruleLink.GetDestinationRule());
		if(!sourceNode || !targetNode){
			DETHROW(deeInvalidParam);
		}
		
		AddLink(sourceNode->GetSlotAt(ruleLink.GetSourceSlot()),
			targetNode->GetSlotAt(ruleLink.GetDestinationSlot()));
	}
}

void meWindowVegetation::SelectNodeOfActiveRule(){
	meHTVRule * const rule = pVLayer ? pVLayer->GetActiveRule() : NULL;
	if(!rule){
		return;
	}
	
	meWVNode * const node = GetNodeWithRule(rule);
	if(node){
		// TODO select node
	}
}

meWVNode *meWindowVegetation::GetNodeWithRule(meHTVRule *rule){
	const int count = GetNodeCount();
	int i;
	
	for(i=0; i<count; i++){
		meWVNode * const node = (meWVNode*)GetNodeAt(i);
		if(node->GetRule() == rule){
			return node;
		}
	}
	
	return NULL;
}

void meWindowVegetation::UpdateNodeWithRule(meHTVRule *rule){
	meWVNode * const node = GetNodeWithRule(rule);
	if(node){
		node->Update();
	}
}

void meWindowVegetation::NodeWithRuleMoved(meHTVRule *rule){
	meWVNode * const node = GetNodeWithRule(rule);
	if(node){
		node->UpdatePositionFromRule();
	}
}
