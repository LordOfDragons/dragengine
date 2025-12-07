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

#include "reWPPanelShapeHull.h"
#include "../reWPShape.h"
#include "../reWindowProperties.h"
#include "../../reWindowMain.h"
#include "../../../rig/reRig.h"
#include "../../../rig/shape/reRigShapeHull.h"
#include "../../../undosys/properties/shape/hull/reUShapeHullAddPoint.h"
#include "../../../undosys/properties/shape/hull/reUShapeHullRemovePoint.h"
#include "../../../undosys/properties/shape/hull/reUShapeHullSetPoint.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cSpinPoint : public igdeSpinTextFieldListener{
	reWPPanelShapeHull &pPanel;
public:
	cSpinPoint(reWPPanelShapeHull &panel) : pPanel(panel){}
	
	virtual void OnValueChanged(igdeSpinTextField*){
		pPanel.UpdatePoint();
	}
};

class cActionAddPoint : public igdeAction{
	reWPPanelShapeHull &pPanel;
public:
	cActionAddPoint(reWPPanelShapeHull &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "Add point"),
		pPanel(panel){}
	
	virtual void OnAction(){
		reRig * const rig = pPanel.GetRig();
		reRigShapeHull * const hull = (reRigShapeHull*)pPanel.GetShape();
		if(!rig || !hull){
			return;
		}
		
		reUShapeHullAddPoint::Ref undo(reUShapeHullAddPoint::Ref::NewWith(
			hull, decVector(), hull->GetPointCount()));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetRig() && pPanel.GetShape());
	}
};

class cActionRemovePoint : public igdeAction{
	reWPPanelShapeHull &pPanel;
public:
	cActionRemovePoint(reWPPanelShapeHull &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "Remove point"),
		pPanel(panel){}
	
	virtual void OnAction(){
		reRig * const rig = pPanel.GetRig();
		reRigShapeHull * const hull = (reRigShapeHull*)pPanel.GetShape();
		const int index = pPanel.GetSelectedPoint();
		if(!rig || !hull || index == -1){
			return;
		}
		
		reUShapeHullRemovePoint::Ref undo(reUShapeHullRemovePoint::Ref::NewWith(hull, index));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual void Update(){
		SetEnabled(pPanel.GetRig() && pPanel.GetShape() && pPanel.GetSelectedPoint() != -1);
	}
};

class cEditPoint : public igdeEditVectorListener{
	reWPPanelShapeHull &pPanel;
public:
	cEditPoint(reWPPanelShapeHull &panel) : pPanel(panel){}
	
	virtual void OnVectorChanged(igdeEditVector *editVector){
		reRig * const rig = pPanel.GetRig();
		reRigShapeHull * const hull = (reRigShapeHull*)pPanel.GetShape();
		const int index = pPanel.GetSelectedPoint();
		if(!rig || !hull || index == -1){
			return;
		}
		
		if(editVector->GetVector().IsEqualTo(hull->GetPointAt(index))){
			return;
		}
		
		reUShapeHullSetPoint::Ref undo(reUShapeHullSetPoint::Ref::NewWith(
			hull, editVector->GetVector(), index));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

}



// Class reWPPanelShapeHull
////////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShapeHull::reWPPanelShapeHull(reWPShape &wpShapes) :
reWPPanelShape(wpShapes, reRigShape::estHull)
{
	igdeEnvironment &env = wpShapes.GetEnvironment();
	igdeContainer::Ref groupHull, formline;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	
	
	helper.GroupBox(*this, groupHull, "Hull Parameters:");
	
	helper.EditVector(groupHull, "Position:", "Position of the box relative to the parent bone.",
		pEditPosition, new cEditPosition(*this));
	
	helper.EditVector(groupHull, "Rotation:", "Rotation of the box.",
		pEditRotation, new cEditRotation(*this));
	
	helper.FormLineStretchFirst(groupHull, "Points:", "Select point to edit", formline);
	helper.EditSpinInteger(formline, "Select point to edit", 0, 0, pSpinPoint, new cSpinPoint(*this));
	helper.Button(formline, pBtnPointAdd, new cActionAddPoint(*this), true);
	helper.Button(formline, pBtnPointRemove, new cActionRemovePoint(*this), true);
	
	helper.EditVector(groupHull, "Coordinates:", "Selected point coordinates.",
		pEditPoint, new cEditPoint(*this));
}

reWPPanelShapeHull::~reWPPanelShapeHull(){
}



// Management
///////////////

void reWPPanelShapeHull::UpdateShape(){
	reRigShapeHull * const hull = (reRigShapeHull*)GetShape();
	
	reWPPanelShape::UpdateShape();
	
	if(hull){
		pEditPosition->SetVector(hull->GetPosition());
		pEditRotation->SetVector(hull->GetOrientation());
		pSpinPoint->SetRange(0, decMath::max(hull->GetPointCount() - 1, 0));
		
	}else{
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pSpinPoint->SetRange(0, 0);
	}
	
	const bool enabled = hull;
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pSpinPoint->SetEnabled(enabled);
	
	pBtnPointAdd->GetAction()->Update();
	pBtnPointRemove->GetAction()->Update();
	
	UpdatePoint();
}

int reWPPanelShapeHull::GetSelectedPoint() const{
	const reRigShapeHull * const hull = (reRigShapeHull*)GetShape();
	return hull && hull->GetPointCount() > 0 ? pSpinPoint->GetValue() : -1;
}

void reWPPanelShapeHull::UpdatePoint(){
	const reRigShapeHull * const hull = (reRigShapeHull*)GetShape();
	const int index = GetSelectedPoint();
	
	if(hull && index != -1){
		pEditPoint->SetVector(hull->GetPointAt(index));
		
	}else{
		pEditPoint->SetVector(decVector());
	}
	
	const bool enabled = hull && index != -1;
	pEditPoint->SetEnabled(enabled);
}
