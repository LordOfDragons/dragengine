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

#include "gdeWPSelection.h"
#include "gdeWPSelectionListener.h"
#include "gdeWindowProperties.h"
#include "model/gdeWPSTreeModel.h"
#include "model/gdeWPSTreeItemModel.h"
#include "selection/gdeWPSCategory.h"
#include "selection/gdeWPSObjectClass.h"
#include "selection/gdeWPSOCBillboard.h"
#include "selection/gdeWPSOCCamera.h"
#include "selection/gdeWPSOCComponent.h"
#include "selection/gdeWPSOCEnvMapProbe.h"
#include "selection/gdeWPSOCLight.h"
#include "selection/gdeWPSOCNavigationBlocker.h"
#include "selection/gdeWPSOCNavigationSpace.h"
#include "selection/gdeWPSOCParticleEmitter.h"
#include "selection/gdeWPSOCForceField.h"
#include "selection/gdeWPSOCSnapPoint.h"
#include "selection/gdeWPSOCSpeaker.h"
#include "selection/gdeWPSOCWorld.h"
#include "selection/gdeWPSParticleEmitter.h"
#include "selection/gdeWPSSkin.h"
#include "selection/gdeWPSSky.h"
#include "../../gamedef/gdeGameDefinition.h"

#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeTreeListListener.h>

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum ePanels{
	epNone,
	epCategory,
	epObjectClass,
	epOCBillboard,
	epOCCamera,
	epOCComponent,
	epOCEnvMapProbe,
	epOCLight,
	epOCNavigationBlocker,
	epOCNavigationSpace,
	epOCParticleEmitter,
	epOCForceField,
	epOCSnapPoint,
	epOCSpeaker,
	epOCWorld,
	epParticleEmitter,
	epSkin,
	epSky
};



// Actions
////////////

namespace {

class cTreeObjects : public igdeTreeListListener{
	gdeWPSelection &pPanel;
	
public:
	typedef deTObjectReference<cTreeObjects> Ref;
	cTreeObjects(gdeWPSelection &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeTreeList *treeList){
		if(!pPanel.GetGameDefinition() || !pPanel.GetModelTreeObjects() 
		|| pPanel.GetModelTreeObjects()->GetIgnoreSelectionChange()){
			return;
		}
		
		gdeWPSTreeItemModel * const item = treeList->GetSelection().DynamicCast<gdeWPSTreeItemModel>();
		if(item){
			item->OnSelected();
			
		}else{
			pPanel.GetGameDefinition()->SetSelectedObjectType(gdeGameDefinition::eotNoSelection);
		}
	}
	
	virtual void OnItemCollapsed(igdeTreeList*, igdeTreeItem*){
	}
	
	virtual void OnItemExpanded(igdeTreeList*, igdeTreeItem*){
	}
	
	virtual void AddContextMenuEntries(igdeTreeList *treeList, igdeMenuCascade &menu){
		gdeWPSTreeItemModel * const item = treeList->GetSelection().DynamicCast<gdeWPSTreeItemModel>();
		if(item){
			item->OnContextMenu(menu);
			
		}else{
			((gdeWPSTreeModel*)treeList)->OnContextMenu(menu);
		}
	}
};

}



// Class gdeWPSelection
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeWPSelection::gdeWPSelection(gdeWindowProperties &windowProperties) :
igdeContainerSplitted(windowProperties.GetEnvironment(), igdeContainerSplitted::espBottom,
	igdeApplication::app().DisplayScaled(600)),
pWindowProperties(windowProperties),
pSwitcher(nullptr),
pTreeObjects(nullptr),
pModelTreeObjects(nullptr)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	pListener = gdeWPSelectionListener::Ref::New(*this);
	
	pSwitcher = igdeSwitcher::Ref::New(env);
	AddChild(pSwitcher, igdeContainerSplitted::eaSide);
	
	helper.TreeList(10, "Game definition objects", pTreeObjects, cTreeObjects::Ref::New(*this));
	AddChild(pTreeObjects, igdeContainerSplitted::eaCenter);
	
	helper.Label(pSwitcher, "No Selection");
	
	pPanelCategory = gdeWPSCategory::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelCategory);
	
	pPanelObjectClass = gdeWPSObjectClass::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelObjectClass);
	
	pPanelOCBillboard = gdeWPSOCBillboard::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCBillboard);
	
	pPanelOCCamera = gdeWPSOCCamera::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCCamera);
	
	pPanelOCComponent = gdeWPSOCComponent::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCComponent);
	
	pPanelOCEnvMapProbe = gdeWPSOCEnvMapProbe::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCEnvMapProbe);
	
	pPanelOCLight = gdeWPSOCLight::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCLight);
	
	pPanelOCNavigationBlocker = gdeWPSOCNavigationBlocker::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCNavigationBlocker);
	
	pPanelOCNavigationSpace = gdeWPSOCNavigationSpace::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCNavigationSpace);
	
	pPanelOCParticleEmitter = gdeWPSOCParticleEmitter::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCParticleEmitter);
	
	pPanelOCForceField = gdeWPSOCForceField::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCForceField);
	
	pPanelOCSnapPoint = gdeWPSOCSnapPoint::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCSnapPoint);
	
	pPanelOCSpeaker = gdeWPSOCSpeaker::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCSpeaker);
	
	pPanelOCWorld = gdeWPSOCWorld::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelOCWorld);
	
	pPanelParticleEmitter = gdeWPSParticleEmitter::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelParticleEmitter);
	
	pPanelSkin = gdeWPSSkin::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelSkin);
	
	pPanelSky = gdeWPSSky::Ref::New(windowProperties);
	pSwitcher->AddChild(pPanelSky);
	
	pSwitcher->SetCurrent(epNone);
}

gdeWPSelection::~gdeWPSelection(){
	SetGameDefinition(nullptr);
}



// Management
///////////////

void gdeWPSelection::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	((gdeWPSCategory&)(igdeWidget&)pPanelCategory).SetGameDefinition(nullptr);
	((gdeWPSObjectClass&)(igdeWidget&)pPanelObjectClass).SetGameDefinition(nullptr);
	((gdeWPSOCBillboard&)(igdeWidget&)pPanelOCBillboard).SetGameDefinition(nullptr);
	((gdeWPSOCCamera&)(igdeWidget&)pPanelOCCamera).SetGameDefinition(nullptr);
	((gdeWPSOCComponent&)(igdeWidget&)pPanelOCComponent).SetGameDefinition(nullptr);
	((gdeWPSOCEnvMapProbe&)(igdeWidget&)pPanelOCEnvMapProbe).SetGameDefinition(nullptr);
	((gdeWPSOCLight&)(igdeWidget&)pPanelOCLight).SetGameDefinition(nullptr);
	((gdeWPSOCNavigationBlocker&)(igdeWidget&)pPanelOCNavigationBlocker).SetGameDefinition(nullptr);
	((gdeWPSOCNavigationSpace&)(igdeWidget&)pPanelOCNavigationSpace).SetGameDefinition(nullptr);
	((gdeWPSOCParticleEmitter&)(igdeWidget&)pPanelOCParticleEmitter).SetGameDefinition(nullptr);
	((gdeWPSOCForceField&)(igdeWidget&)pPanelOCForceField).SetGameDefinition(nullptr);
	((gdeWPSOCSnapPoint&)(igdeWidget&)pPanelOCSnapPoint).SetGameDefinition(nullptr);
	((gdeWPSOCSpeaker&)(igdeWidget&)pPanelOCSpeaker).SetGameDefinition(nullptr);
	((gdeWPSOCWorld&)(igdeWidget&)pPanelOCWorld).SetGameDefinition(nullptr);
	((gdeWPSParticleEmitter&)(igdeWidget&)pPanelParticleEmitter).SetGameDefinition(nullptr);
	((gdeWPSSkin&)(igdeWidget&)pPanelSkin).SetGameDefinition(nullptr);
	((gdeWPSSky&)(igdeWidget&)pPanelSky).SetGameDefinition(nullptr);
	
	if(pModelTreeObjects){
		delete pModelTreeObjects;
		pModelTreeObjects = nullptr;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		pModelTreeObjects = new gdeWPSTreeModel(pTreeObjects, pWindowProperties.GetWindowMain(), gameDefinition);
	}
	
	((gdeWPSCategory&)(igdeWidget&)pPanelCategory).SetGameDefinition(gameDefinition);
	((gdeWPSObjectClass&)(igdeWidget&)pPanelObjectClass).SetGameDefinition(gameDefinition);
	((gdeWPSOCBillboard&)(igdeWidget&)pPanelOCBillboard).SetGameDefinition(gameDefinition);
	((gdeWPSOCCamera&)(igdeWidget&)pPanelOCCamera).SetGameDefinition(gameDefinition);
	((gdeWPSOCComponent&)(igdeWidget&)pPanelOCComponent).SetGameDefinition(gameDefinition);
	((gdeWPSOCEnvMapProbe&)(igdeWidget&)pPanelOCEnvMapProbe).SetGameDefinition(gameDefinition);
	((gdeWPSOCLight&)(igdeWidget&)pPanelOCLight).SetGameDefinition(gameDefinition);
	((gdeWPSOCNavigationBlocker&)(igdeWidget&)pPanelOCNavigationBlocker).SetGameDefinition(gameDefinition);
	((gdeWPSOCNavigationSpace&)(igdeWidget&)pPanelOCNavigationSpace).SetGameDefinition(gameDefinition);
	((gdeWPSOCParticleEmitter&)(igdeWidget&)pPanelOCParticleEmitter).SetGameDefinition(gameDefinition);
	((gdeWPSOCForceField&)(igdeWidget&)pPanelOCForceField).SetGameDefinition(gameDefinition);
	((gdeWPSOCSnapPoint&)(igdeWidget&)pPanelOCSnapPoint).SetGameDefinition(gameDefinition);
	((gdeWPSOCSpeaker&)(igdeWidget&)pPanelOCSpeaker).SetGameDefinition(gameDefinition);
	((gdeWPSOCWorld&)(igdeWidget&)pPanelOCWorld).SetGameDefinition(gameDefinition);
	((gdeWPSParticleEmitter&)(igdeWidget&)pPanelParticleEmitter).SetGameDefinition(gameDefinition);
	((gdeWPSSkin&)(igdeWidget&)pPanelSkin).SetGameDefinition(gameDefinition);
	((gdeWPSSky&)(igdeWidget&)pPanelSky).SetGameDefinition(gameDefinition);
	
	if(gameDefinition){
		pModelTreeObjects->SetCurrentItemFromGameDef();
	}
	
	SelectedObjectChanged();
}



void gdeWPSelection::SelectedObjectChanged(){
	if(!pGameDefinition){
		pSwitcher->SetCurrent(epNone);
		return;
	}
	
	switch(pGameDefinition->GetSelectedObjectType()){
	case gdeGameDefinition::eotCategoryObjectClass:
	case gdeGameDefinition::eotCategorySkin:
	case gdeGameDefinition::eotCategorySky:
	case gdeGameDefinition::eotCategoryParticleEmitter:
		pSwitcher->SetCurrent(epCategory);
		break;
		
	case gdeGameDefinition::eotObjectClass:
		pSwitcher->SetCurrent(epObjectClass);
		break;
		
	case gdeGameDefinition::eotOCBillboard:
		pSwitcher->SetCurrent(epOCBillboard);
		break;
		
	case gdeGameDefinition::eotOCCamera:
		pSwitcher->SetCurrent(epOCCamera);
		break;
		
	case gdeGameDefinition::eotOCComponent:
		pSwitcher->SetCurrent(epOCComponent);
		break;
		
	case gdeGameDefinition::eotOCEnvMapProbe:
		pSwitcher->SetCurrent(epOCEnvMapProbe);
		break;
		
	case gdeGameDefinition::eotOCLight:
		pSwitcher->SetCurrent(epOCLight);
		break;
		
	case gdeGameDefinition::eotOCNavigationBlocker:
		pSwitcher->SetCurrent(epOCNavigationBlocker);
		break;
		
	case gdeGameDefinition::eotOCNavigationSpace:
		pSwitcher->SetCurrent(epOCNavigationSpace);
		break;
		
	case gdeGameDefinition::eotOCParticleEmitter:
		pSwitcher->SetCurrent(epOCParticleEmitter);
		break;
		
	case gdeGameDefinition::eotOCForceField:
		pSwitcher->SetCurrent(epOCForceField);
		break;
		
	case gdeGameDefinition::eotOCSnapPoint:
		pSwitcher->SetCurrent(epOCSnapPoint);
		break;
		
	case gdeGameDefinition::eotOCSpeaker:
		pSwitcher->SetCurrent(epOCSpeaker);
		break;
		
	case gdeGameDefinition::eotOCWorld:
		pSwitcher->SetCurrent(epOCWorld);
		break;
		
	case gdeGameDefinition::eotParticleEmitter:
		pSwitcher->SetCurrent(epParticleEmitter);
		break;
		
	case gdeGameDefinition::eotSkin:
		pSwitcher->SetCurrent(epSkin);
		break;
		
	case gdeGameDefinition::eotSky:
		pSwitcher->SetCurrent(epSky);
		break;
		
	default:
		pSwitcher->SetCurrent(epNone);
	}
}

void gdeWPSelection::Find(const char *text){
	if(!pGameDefinition){
		return;
	}
	
	gdeWPSTreeItemModel * const item = pTreeObjects->GetSelection().DynamicCast<gdeWPSTreeItemModel>();
	if(item){
		item->SelectBestMatching(text);
		
	}else{
		pModelTreeObjects->SelectBestMatching(text);
	}
}
