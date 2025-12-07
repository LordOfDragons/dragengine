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
	cTreeObjects(gdeWPSelection &panel) : pPanel(panel){}
	
	virtual void OnSelectionChanged(igdeTreeList *treeList){
		if(!pPanel.GetGameDefinition() || !pPanel.GetModelTreeObjects() 
		|| pPanel.GetModelTreeObjects()->GetIgnoreSelectionChange()){
			return;
		}
		
		gdeWPSTreeItemModel * const item = (gdeWPSTreeItemModel*)treeList->GetSelection();
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
		gdeWPSTreeItemModel * const item = (gdeWPSTreeItemModel*)treeList->GetSelection();
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
pListener(NULL),
pGameDefinition(NULL),
pSwitcher(NULL),
pTreeObjects(NULL),
pModelTreeObjects(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	pListener = new gdeWPSelectionListener(*this);
	
	pSwitcher.TakeOver(new igdeSwitcher(env));
	AddChild(pSwitcher, igdeContainerSplitted::eaSide);
	
	helper.TreeList(10, "Game definition objects", pTreeObjects, new cTreeObjects(*this));
	AddChild(pTreeObjects, igdeContainerSplitted::eaCenter);
	
	helper.Label(pSwitcher, "No Selection");
	
	pPanelCategory.TakeOver(new gdeWPSCategory(windowProperties));
	pSwitcher->AddChild(pPanelCategory);
	
	pPanelObjectClass.TakeOver(new gdeWPSObjectClass(windowProperties));
	pSwitcher->AddChild(pPanelObjectClass);
	
	pPanelOCBillboard.TakeOver(new gdeWPSOCBillboard(windowProperties));
	pSwitcher->AddChild(pPanelOCBillboard);
	
	pPanelOCCamera.TakeOver(new gdeWPSOCCamera(windowProperties));
	pSwitcher->AddChild(pPanelOCCamera);
	
	pPanelOCComponent.TakeOver(new gdeWPSOCComponent(windowProperties));
	pSwitcher->AddChild(pPanelOCComponent);
	
	pPanelOCEnvMapProbe.TakeOver(new gdeWPSOCEnvMapProbe(windowProperties));
	pSwitcher->AddChild(pPanelOCEnvMapProbe);
	
	pPanelOCLight.TakeOver(new gdeWPSOCLight(windowProperties));
	pSwitcher->AddChild(pPanelOCLight);
	
	pPanelOCNavigationBlocker.TakeOver(new gdeWPSOCNavigationBlocker(windowProperties));
	pSwitcher->AddChild(pPanelOCNavigationBlocker);
	
	pPanelOCNavigationSpace.TakeOver(new gdeWPSOCNavigationSpace(windowProperties));
	pSwitcher->AddChild(pPanelOCNavigationSpace);
	
	pPanelOCParticleEmitter.TakeOver(new gdeWPSOCParticleEmitter(windowProperties));
	pSwitcher->AddChild(pPanelOCParticleEmitter);
	
	pPanelOCForceField.TakeOver(new gdeWPSOCForceField(windowProperties));
	pSwitcher->AddChild(pPanelOCForceField);
	
	pPanelOCSnapPoint.TakeOver(new gdeWPSOCSnapPoint(windowProperties));
	pSwitcher->AddChild(pPanelOCSnapPoint);
	
	pPanelOCSpeaker.TakeOver(new gdeWPSOCSpeaker(windowProperties));
	pSwitcher->AddChild(pPanelOCSpeaker);
	
	pPanelOCWorld.TakeOver(new gdeWPSOCWorld(windowProperties));
	pSwitcher->AddChild(pPanelOCWorld);
	
	pPanelParticleEmitter.TakeOver(new gdeWPSParticleEmitter(windowProperties));
	pSwitcher->AddChild(pPanelParticleEmitter);
	
	pPanelSkin.TakeOver(new gdeWPSSkin(windowProperties));
	pSwitcher->AddChild(pPanelSkin);
	
	pPanelSky.TakeOver(new gdeWPSSky(windowProperties));
	pSwitcher->AddChild(pPanelSky);
	
	pSwitcher->SetCurrent(epNone);
}

gdeWPSelection::~gdeWPSelection(){
	SetGameDefinition(NULL);
	
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void gdeWPSelection::SetGameDefinition(gdeGameDefinition *gameDefinition){
	if(gameDefinition == pGameDefinition){
		return;
	}
	
	((gdeWPSCategory&)(igdeWidget&)pPanelCategory).SetGameDefinition(NULL);
	((gdeWPSObjectClass&)(igdeWidget&)pPanelObjectClass).SetGameDefinition(NULL);
	((gdeWPSOCBillboard&)(igdeWidget&)pPanelOCBillboard).SetGameDefinition(NULL);
	((gdeWPSOCCamera&)(igdeWidget&)pPanelOCCamera).SetGameDefinition(NULL);
	((gdeWPSOCComponent&)(igdeWidget&)pPanelOCComponent).SetGameDefinition(NULL);
	((gdeWPSOCEnvMapProbe&)(igdeWidget&)pPanelOCEnvMapProbe).SetGameDefinition(NULL);
	((gdeWPSOCLight&)(igdeWidget&)pPanelOCLight).SetGameDefinition(NULL);
	((gdeWPSOCNavigationBlocker&)(igdeWidget&)pPanelOCNavigationBlocker).SetGameDefinition(NULL);
	((gdeWPSOCNavigationSpace&)(igdeWidget&)pPanelOCNavigationSpace).SetGameDefinition(NULL);
	((gdeWPSOCParticleEmitter&)(igdeWidget&)pPanelOCParticleEmitter).SetGameDefinition(NULL);
	((gdeWPSOCForceField&)(igdeWidget&)pPanelOCForceField).SetGameDefinition(NULL);
	((gdeWPSOCSnapPoint&)(igdeWidget&)pPanelOCSnapPoint).SetGameDefinition(NULL);
	((gdeWPSOCSpeaker&)(igdeWidget&)pPanelOCSpeaker).SetGameDefinition(NULL);
	((gdeWPSOCWorld&)(igdeWidget&)pPanelOCWorld).SetGameDefinition(nullptr);
	((gdeWPSParticleEmitter&)(igdeWidget&)pPanelParticleEmitter).SetGameDefinition(NULL);
	((gdeWPSSkin&)(igdeWidget&)pPanelSkin).SetGameDefinition(NULL);
	((gdeWPSSky&)(igdeWidget&)pPanelSky).SetGameDefinition(NULL);
	
	if(pModelTreeObjects){
		delete pModelTreeObjects;
		pModelTreeObjects = NULL;
	}
	
	if(pGameDefinition){
		pGameDefinition->RemoveListener(pListener);
		pGameDefinition->FreeReference();
	}
	
	pGameDefinition = gameDefinition;
	
	if(gameDefinition){
		gameDefinition->AddListener(pListener);
		gameDefinition->AddReference();
		
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
	
	gdeWPSTreeItemModel * const item = (gdeWPSTreeItemModel*)pTreeObjects->GetSelection();
	if(item){
		item->SelectBestMatching(text);
		
	}else{
		pModelTreeObjects->SelectBestMatching(text);
	}
}
