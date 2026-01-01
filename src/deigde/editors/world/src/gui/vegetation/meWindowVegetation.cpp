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
	typedef deTObjectReference<cActionAddNode> Ref;
	cActionAddNode(meWindowVegetation &view, meHTVRule::eRuleTypes type,
		const decVector2 &position, const char *text, igdeIcon *icon) :
	igdeAction(text, icon, "Add Node"), pView(view), pType(type), pPosition(position){}
	
	void OnAction() override{
		if(!pView.GetVLayer() || !pView.GetWorld()){
			return;
		}
		
		const meHTVRule::Ref rule(CreateRule());
		rule->SetPosition(pPosition);
		
		pView.GetWorld()->GetUndoSystem()->Add(meUHTVRuleAdd::Ref::New(pView.GetVLayer(), rule));
	}
	
	void Update() override{
		SetEnabled(pView.GetVLayer() && pView.GetWorld());
	}
	
	virtual meHTVRule::Ref CreateRule() = 0;
};

class cActionAddClosestProp : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddClosestProp> Ref;
	cActionAddClosestProp(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Closest Prop", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleClosestProp::Ref::New();
	}
};

class cActionAddClosestVegetation : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddClosestVegetation> Ref;
	cActionAddClosestVegetation(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Closest Vegetation", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleClosestVegetation::Ref::New();
	}
};

class cActionAddCombine : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddCombine> Ref;
	cActionAddCombine(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Combine", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleCombine::Ref::New();
	}
};

class cActionAddComponents : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddComponents> Ref;
	cActionAddComponents(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Components", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleComponents::Ref::New();
	}
};

class cActionAddCurve : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddCurve> Ref;
	cActionAddCurve(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Curve", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleCurve::Ref::New();
	}
};

class cActionAddGeometry : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddGeometry> Ref;
	cActionAddGeometry(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Geometry", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleGeometry::Ref::New();
	}
};

class cActionAddMapping : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddMapping> Ref;
	cActionAddMapping(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Mapping", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleMapping::Ref::New();
	}
};

class cActionAddMath : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddMath> Ref;
	cActionAddMath(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Math", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleMath::Ref::New();
	}
};

class cActionAddMultiMath : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddMultiMath> Ref;
	cActionAddMultiMath(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Multi-Math", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleMultiMath::Ref::New();
	}
};

class cActionAddVectorMath : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddVectorMath> Ref;
	cActionAddVectorMath(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Vector Math", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleVectorMath::Ref::New();
	}
};

class cActionAddRandom : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddRandom> Ref;
	cActionAddRandom(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Random", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleRandom::Ref::New();
	}
};

class cActionAddConstant : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddConstant> Ref;
	cActionAddConstant(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Constant", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRuleConstant::Ref::New();
	}
};

class cActionAddPropCount : public cActionAddNode {
public:
	typedef deTObjectReference<cActionAddPropCount> Ref;
	cActionAddPropCount(meWindowVegetation &view, const decVector2 &position) :
	cActionAddNode(view, meHTVRule::ertClosestProp, position, "Prop Count", nullptr){}
	
	meHTVRule::Ref CreateRule() override{
		return meHTVRulePropCount::Ref::New();
	}
};


class cActionViewAutoUpdate : public igdeAction {
	meWindowVegetation &pView;
	
public:
	typedef deTObjectReference<cActionViewAutoUpdate> Ref;
	cActionViewAutoUpdate(meWindowVegetation &view) : igdeAction(
		"Auto Update Vegetation", nullptr, "Auto Update Vegetation"), pView(view){}
	
	void OnAction() override{
		if(pView.GetWorld()){
			pView.GetWorld()->GetGuiParameters().SetAutoUpdateVegetation(
				!pView.GetWorld()->GetGuiParameters().GetAutoUpdateVegetation());
		}
	}
	
	void Update() override{
		SetEnabled(pView.GetWorld());
		SetSelected(pView.GetWorld() && pView.GetWorld()->GetGuiParameters().GetAutoUpdateVegetation());
	}
};


class cBoardListener : public igdeNVBoardListener {
	meWindowVegetation &pView;
	
public:
	typedef deTObjectReference<cBoardListener> Ref;
	cBoardListener(meWindowVegetation &view) : pView(view){}
	
	meHTVRLink *RuleLinkForNodeLink(igdeNVSlot *sourceSlot, igdeNVSlot *targetSlot) const{
		if(!pView.GetVLayer()){
			return {};
		}
		
		const meWVNode &wvnodeSource = (meWVNode&)*sourceSlot->GetOwnerNode();
		const int indexSlotSource = wvnodeSource.GetSlots().IndexOf(sourceSlot);
		const meWVNode &wvnodeTarget = (meWVNode&)*targetSlot->GetOwnerNode();
		const int indexSlotTarget = wvnodeTarget.GetSlots().IndexOf(targetSlot);
		if(indexSlotSource == -1 || indexSlotTarget == -1){
			return {};
		}
		
		return pView.GetVLayer()->GetLinks().FindOrDefault([&](const meHTVRLink &l){
			return l.GetSourceRule() == wvnodeSource.GetRule()
				&& l.GetSourceSlot() == indexSlotSource
				&& l.GetDestinationRule() == wvnodeTarget.GetRule()
				&& l.GetDestinationSlot() == indexSlotTarget;
		});
	}
	
	void AddContextMenuEntries(igdeNVBoard*, igdeMenuCascade &menu, const decPoint &position) override {
		if(!pView.GetVLayer()){
			return;
		}
		
		igdeEnvironment &env = menu.GetEnvironment();
		igdeMenuCascade::Ref subMenu, subMenu2;
		igdeUIHelper &helper = env.GetUIHelper();
		
		
		// nodes
		const decVector2 addNodePosition(decVector2(position - pView.GetSize() / 2)
			* pView.GetPixelToUnits() - pView.GetVLayer()->GetViewCenter() );
		
		subMenu = igdeMenuCascade::Ref::New(env, "Add Node",
			env.GetStockIcon(igdeEnvironment::esiPlus), "Add Node");
		menu.AddChild(subMenu);
		
		subMenu2 = igdeMenuCascade::Ref::New(env, "Input", nullptr, "Input");
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, cActionAddConstant::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddGeometry::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddRandom::Ref::New(pView, addNodePosition));
		
		subMenu2 = igdeMenuCascade::Ref::New(env, "Proximity", nullptr, "Proximity");
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, cActionAddClosestProp::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddClosestVegetation::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddPropCount::Ref::New(pView, addNodePosition));
		
		subMenu2 = igdeMenuCascade::Ref::New(env, "Conversion", nullptr, "Conversion");
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, cActionAddCurve::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddMapping::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddMath::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddMultiMath::Ref::New(pView, addNodePosition));
		
		subMenu2 = igdeMenuCascade::Ref::New(env, "Vector", nullptr, "Vector");
		subMenu->AddChild(subMenu2);
		helper.MenuCommand(subMenu2, cActionAddCombine::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddComponents::Ref::New(pView, addNodePosition));
		helper.MenuCommand(subMenu2, cActionAddVectorMath::Ref::New(pView, addNodePosition));
		
		
		// hover link
		
		
		// view
		helper.MenuSeparator(menu);
		helper.MenuCheck(menu, cActionViewAutoUpdate::Ref::New(pView));
	}
	
	bool CanLink(igdeNVBoard*, igdeNVSlot *source, igdeNVSlot *target) override {
		if(!pView.GetVLayer()){
			return false;
		}
		
		const meWVNodeSlot &wvslotSource = (meWVNodeSlot&)*source;
		const meWVNodeSlot &wvslotTarget = (meWVNodeSlot&)*target;
		if(wvslotTarget.GetType() == meWVNodeSlot::estValue && wvslotSource.GetType() != meWVNodeSlot::estValue){
			return false;
		}
		
		const meWVNode &wvnodeSource = (meWVNode&)*source->GetOwnerNode();
		const int indexSlotSource = wvnodeSource.GetSlots().IndexOf(source);
		const meWVNode &wvnodeTarget = (meWVNode&)*target->GetOwnerNode();
		const int indexSlotTarget = wvnodeTarget.GetSlots().IndexOf(target);
		if(indexSlotSource == -1 || indexSlotTarget == -1){
			DETHROW(deeInvalidParam);
		}
		
		return !pView.GetVLayer()->LinkProducesLoop(wvnodeSource.GetRule(),
			indexSlotSource, wvnodeTarget.GetRule(), indexSlotTarget);
	}
	
	void OnLinkAdded(igdeNVBoard*, igdeNVLink *link) override {
		if(!pView.GetVLayer()){
			return;
		}
		
		const meWVNode &wvnodeSource = (meWVNode&)*link->GetSource()->GetOwnerNode();
		const int indexSlotSource = wvnodeSource.GetSlots().IndexOf(link->GetSource());
		const meWVNode &wvnodeTarget = (meWVNode&)*link->GetTarget()->GetOwnerNode();
		const int indexSlotTarget = wvnodeTarget.GetSlots().IndexOf(link->GetTarget());
		if(indexSlotSource == -1 || indexSlotTarget == -1){
			DETHROW(deeInvalidParam);
		}
		
		const meHTVRLink::Ref ruleLink(meHTVRLink::Ref::New(wvnodeSource.GetRule(), indexSlotSource,
			wvnodeTarget.GetRule(), indexSlotTarget));
		
			pView.GetWorld()->GetUndoSystem()->Add(
				meUHTVLinkAdd::Ref::New(pView.GetVLayer(), ruleLink));
	}
	
	void OnLinkRemoved(igdeNVBoard*, igdeNVSlot *source, igdeNVSlot *target) override {
		if(!pView.GetVLayer()){
			return;
		}
		
		meHTVRLink * const ruleLink = RuleLinkForNodeLink(source, target);
		if(!ruleLink){
			return;
		}
		
		meUHTVLinkCut::Ref undo(meUHTVLinkCut::Ref::New(pView.GetVLayer()));
		undo->GetCutLinks().Add(ruleLink);
		pView.GetWorld()->GetUndoSystem()->Add(undo);
	}
	
	void OnAllLinksRemoved(igdeNVBoard*) override{
		if(!pView.GetVLayer() || pView.GetVLayer()->GetLinks().IsEmpty()){
			return;
		}
		
		meUHTVLinkCut::Ref undo(meUHTVLinkCut::Ref::New(pView.GetVLayer()));
		undo->GetCutLinks() += pView.GetVLayer()->GetLinks();
		pView.GetWorld()->GetUndoSystem()->Add(undo);
	}
	
	void OnOffsetChanged(igdeNVBoard *board) override {
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
pUnitsToPixel(igdeApplication::app().DisplayScaled(100.0f)),
pPixelToUnits(1.0f / pUnitsToPixel)
{
	pListener = meWindowVegetationListener::Ref::New(*this);
	AddListener(new cBoardListener(*this));
}

meWindowVegetation::~meWindowVegetation(){
	SetWorld(nullptr);
}



// Management
///////////////

void meWindowVegetation::SetWorld(meWorld *world){
	if(world == pWorld){
		return;
	}
	
	SetVLayer(nullptr);
	
	if(pWorld){
		pWorld->RemoveNotifier(pListener);
	}
	
	pWorld = world;
	
	if(world){
		world->AddNotifier(pListener);
		const decPoint3 sector; // = pWorld->GetActiveSector();
		SetSector(decPoint(sector.x, sector.z));
	}
}

void meWindowVegetation::SetSector(const decPoint &sector){
	pSector = sector;
	
	meHTVegetationLayer::Ref vlayer;
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
	pVLayer = vlayer;
	UpdateNodesFromVLayer();
}

meHTVRule::Ref meWindowVegetation::GetVRule() const{
	return pVLayer ? pVLayer->GetActiveRule() : meHTVRule::Ref();
}



void meWindowVegetation::UpdateNodesFromVLayer(){
	RemoveAllNodes();
	
	if(!pVLayer){
		return;
	}
	
	pVLayer->GetRules().Visit([&](const meHTVRule::Ref &rule){
		meWVNode::Ref node;
		
		switch(rule->GetType()){
		case meHTVRule::ertClosestProp:
			node = meWVNodeClosestProp::Ref::New(*this, rule.DynamicCast<meHTVRuleClosestProp>());
			break;
			
		case meHTVRule::ertClosestVegetation:
			node = meWVNodeClosestVegetation::Ref::New(*this, rule.DynamicCast<meHTVRuleClosestVegetation>());
			break;
			
		case meHTVRule::ertCombine:
			node = meWVNodeCombine::Ref::New(*this, rule.DynamicCast<meHTVRuleCombine>());
			break;
			
		case meHTVRule::ertComponents:
			node = meWVNodeComponents::Ref::New(*this, rule.DynamicCast<meHTVRuleComponents>());
			break;
			
		case meHTVRule::ertCurve:
			node = meWVNodeCurve::Ref::New(*this, rule.DynamicCast<meHTVRuleCurve>());
			break;
			
		case meHTVRule::ertGeometry:
			node = meWVNodeGeometry::Ref::New(*this, rule.DynamicCast<meHTVRuleGeometry>());
			break;
			
		case meHTVRule::ertMapping:
			node = meWVNodeMapping::Ref::New(*this, rule.DynamicCast<meHTVRuleMapping>());
			break;
			
		case meHTVRule::ertMath:
			node = meWVNodeMath::Ref::New(*this, rule.DynamicCast<meHTVRuleMath>());
			break;
			
		case meHTVRule::ertMultiMath:
			node = meWVNodeMultiMath::Ref::New(*this, rule.DynamicCast<meHTVRuleMultiMath>());
			break;
			
		case meHTVRule::ertVectorMath:
			node = meWVNodeVectorMath::Ref::New(*this, rule.DynamicCast<meHTVRuleVectorMath>());
			break;
			
		case meHTVRule::ertRandom:
			node = meWVNodeRandom::Ref::New(*this, rule.DynamicCast<meHTVRuleRandom>());
			break;
			
		case meHTVRule::ertResult:
			node = meWVNodeResult::Ref::New(*this, rule.DynamicCast<meHTVRuleResult>());
			break;
			
		case meHTVRule::ertConstant:
			node = meWVNodeConstant::Ref::New(*this, rule.DynamicCast<meHTVRuleConstant>());
			break;
			
		case meHTVRule::ertPropCount:
			node = meWVNodePropCount::Ref::New(*this, rule.DynamicCast<meHTVRulePropCount>());
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
		node->UpdatePositionFromRule();
		node->Update();
		
		AddNode(node);
	});
	
	UpdateLinksFromVLayer();
}

void meWindowVegetation::UpdateLinksFromVLayer(){
	RemoveAllLinks();
	
	if(!pVLayer){
		return;
	}
	
	pVLayer->GetLinks().Visit([&](const meHTVRLink &l){
		igdeNVNode * const sourceNode = GetNodeWithRule(l.GetSourceRule());
		igdeNVNode * const targetNode = GetNodeWithRule(l.GetDestinationRule());
		DEASSERT_NOTNULL(sourceNode)
		DEASSERT_NOTNULL(targetNode)
		
		AddLink(sourceNode->GetSlots().GetAt(l.GetSourceSlot()),
			targetNode->GetSlots().GetAt(l.GetDestinationSlot()));
	});
}

void meWindowVegetation::SelectNodeOfActiveRule(){
	const meHTVRule::Ref rule(GetVRule());
	if(!rule){
		return;
	}
	
	meWVNode * const node = GetNodeWithRule(rule);
	if(node){
		// TODO select node
	}
}

meWVNode *meWindowVegetation::GetNodeWithRule(meHTVRule *rule){
	return GetNodes().FindOrDefault([&](const igdeNVNode::Ref &n){
		return n.DynamicCast<meWVNode>()->GetRule() == rule;
	}).DynamicCast<meWVNode>();
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


void meWindowVegetation::OnGameDefinitionChanged(){
	GetNodes().Visit([&](const igdeNVNode::Ref &n){
		n.DynamicCast<meWVNode>()->OnGameDefinitionChanged();
	});
}
