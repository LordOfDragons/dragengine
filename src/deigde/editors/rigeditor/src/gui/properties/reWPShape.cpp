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

#include "reWPShape.h"
#include "reWPShapeListener.h"
#include "reWindowProperties.h"
#include "panels/reWPPanelShape.h"
#include "panels/reWPPanelShapeSphere.h"
#include "panels/reWPPanelShapeBox.h"
#include "panels/reWPPanelShapeCylinder.h"
#include "panels/reWPPanelShapeCapsule.h"
#include "panels/reWPPanelShapeHull.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/shape/reRigShape.h"
#include "../../rig/shape/reRigShapeBox.h"
#include "../../rig/shape/reRigShapeSphere.h"
#include "../../rig/shape/reRigShapeCapsule.h"
#include "../../rig/shape/reRigShapeCylinder.h"
#include "../../rig/shape/reSelectionShapes.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeSwitcher.h>

#include <dragengine/common/exceptions.h>



// Class reWPShape
////////////////////

// Constructor, destructor
////////////////////////////

reWPShape::reWPShape(reWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties),
pShape(NULL),
pRig(NULL),
pListener(NULL),

pPanelSphere(NULL),
pPanelBox(NULL),
pPanelCylinder(NULL),
pPanelCapsule(NULL),
pPanelHull(NULL),
pActivePanel(NULL)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new reWPShapeListener(*this);
	
	pSwitcher.TakeOver(new igdeSwitcher(env));
	AddChild(pSwitcher);
	
	helper.Label(pSwitcher, "No Active Shape");
	
	pPanelSphere = new reWPPanelShapeSphere(*this);
	pSwitcher->AddChild(pPanelSphere);
	
	pPanelBox = new reWPPanelShapeBox(*this);
	pSwitcher->AddChild(pPanelBox);
	
	pPanelCylinder = new reWPPanelShapeCylinder(*this);
	pSwitcher->AddChild(pPanelCylinder);
	
	pPanelCapsule = new reWPPanelShapeCapsule(*this);
	pSwitcher->AddChild(pPanelCapsule);
	
	pPanelHull = new reWPPanelShapeHull(*this);
	pSwitcher->AddChild(pPanelHull);
	
	pSwitcher->SetCurrent(0);
}

reWPShape::~reWPShape(){
	SetRig(NULL);
	
	if(pListener){
		pListener->FreeReference();
	}
}



// Management
///////////////

void reWPShape::SetRig(reRig *rig){
	if(rig == pRig){
		return;
	}
	
	SetShape(NULL);
	
	if(pRig){
		pRig->RemoveNotifier(pListener);
		pRig->FreeReference();
		pRig = NULL;
	}
	
	pRig = rig;
	
	if(rig){
		rig->AddNotifier(pListener);
		rig->AddReference();
		
		SetShape(rig->GetSelectionShapes()->GetActiveShape());
	}
}

void reWPShape::SetShape(reRigShape *shape){
	if(shape == pShape){
		return;
	}
	
	if(pActivePanel){
		pActivePanel->SetShape(NULL, NULL);
		pActivePanel = NULL;
	}
	
	if(pShape){
		pShape->FreeReference();
	}
	
	pShape = shape;
	
	if(!shape){
		pSwitcher->SetCurrent(0);
		return;
	}
	
	shape->AddReference();
	
	const reRigShape::eShapeTypes shapeType = shape->GetShapeType();
	
	if(shapeType == pPanelSphere->GetRequiredShapeType()){
		pSwitcher->SetCurrent(1);
		pActivePanel = pPanelSphere;
		
	}else if(shapeType == pPanelBox->GetRequiredShapeType()){
		pSwitcher->SetCurrent(2);
		pActivePanel = pPanelBox;
		
	}else if(shapeType == pPanelCylinder->GetRequiredShapeType()){
		pSwitcher->SetCurrent(3);
		pActivePanel = pPanelCylinder;
		
	}else if(shapeType == pPanelCapsule->GetRequiredShapeType()){
		pSwitcher->SetCurrent(4);
		pActivePanel = pPanelCapsule;
		
	}else if(shapeType == pPanelHull->GetRequiredShapeType()){
		pSwitcher->SetCurrent(5);
		pActivePanel = pPanelHull;
		
	}else{
		pSwitcher->SetCurrent(0);
	}
	
	if(pActivePanel){
		pActivePanel->SetShape(pRig, pShape);
		UpdateShape();
	}
}



void reWPShape::UpdateShape(){
	if(pActivePanel){
		pActivePanel->UpdateShape();
	}
}
