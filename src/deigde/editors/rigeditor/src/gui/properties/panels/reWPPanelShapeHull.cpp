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
	using Ref = deTObjectReference<cSpinPoint>;
	cSpinPoint(reWPPanelShapeHull &panel) : pPanel(panel){}
	
	void OnValueChanged(igdeSpinTextField*) override{
		pPanel.UpdatePoint();
	}
};

class cActionAddPoint : public igdeAction{
	reWPPanelShapeHull &pPanel;
public:
	using Ref = deTObjectReference<cActionAddPoint>;
	cActionAddPoint(reWPPanelShapeHull &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiPlus), "@Rig.PanelShapeHull.Action.AddPoint"),
		pPanel(panel){}
	
	void OnAction() override{
		reRig * const rig = pPanel.GetRig();
		reRigShapeHull * const hull = (reRigShapeHull*)pPanel.GetShape();
		if(!rig || !hull){
			return;
		}
		
		reUShapeHullAddPoint::Ref undo(reUShapeHullAddPoint::Ref::New(
			hull, decVector(), hull->GetPoints().GetCount()));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetRig() && pPanel.GetShape());
	}
};

class cActionRemovePoint : public igdeAction{
	reWPPanelShapeHull &pPanel;
public:
	using Ref = deTObjectReference<cActionRemovePoint>;
	cActionRemovePoint(reWPPanelShapeHull &panel) : igdeAction("",
		panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiMinus), "@Rig.PanelShapeHull.Action.RemovePoint"),
		pPanel(panel){}
	
	void OnAction() override{
		reRig * const rig = pPanel.GetRig();
		reRigShapeHull * const hull = (reRigShapeHull*)pPanel.GetShape();
		const int index = pPanel.GetSelectedPoint();
		if(!rig || !hull || index == -1){
			return;
		}
		
		reUShapeHullRemovePoint::Ref undo(reUShapeHullRemovePoint::Ref::New(hull, index));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetRig() && pPanel.GetShape() && pPanel.GetSelectedPoint() != -1);
	}
};

class cEditPoint : public igdeEditVectorListener{
	reWPPanelShapeHull &pPanel;
public:
	using Ref = deTObjectReference<cEditPoint>;
	cEditPoint(reWPPanelShapeHull &panel) : pPanel(panel){}
	
	void OnVectorChanged(igdeEditVector *editVector) override{
		reRig * const rig = pPanel.GetRig();
		reRigShapeHull * const hull = (reRigShapeHull*)pPanel.GetShape();
		const int index = pPanel.GetSelectedPoint();
		if(!rig || !hull || index == -1){
			return;
		}
		
		if(editVector->GetVector().IsEqualTo(hull->GetPoints().GetAt(index))){
			return;
		}
		
		reUShapeHullSetPoint::Ref undo(reUShapeHullSetPoint::Ref::New(
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
	
	
	
	helper.GroupBox(*this, groupHull, "@Rig.PanelShapeHull.GroupBox.HullParameters");
	
	helper.EditVector(groupHull, "@Rig.PanelShapeHull.Position", "@Rig.PanelShapeHull.Position.ToolTip",
		pEditPosition, cEditPosition::Ref::New(*this));
	
	helper.EditVector(groupHull, "@Rig.PanelShapeHull.Rotation", "@Rig.PanelShapeHull.Rotation.ToolTip",
		pEditRotation, cEditRotation::Ref::New(*this));
	
	helper.FormLineStretchFirst(groupHull, "@Rig.PanelShapeHull.Points", "@Rig.PanelShapeHull.Points.ToolTip", formline);
	helper.EditSpinInteger(formline, "@Rig.PanelShapeHull.Points.ToolTip", 0, 0, pSpinPoint, cSpinPoint::Ref::New(*this));
	helper.Button(formline, pBtnPointAdd, cActionAddPoint::Ref::New(*this));
	helper.Button(formline, pBtnPointRemove, cActionRemovePoint::Ref::New(*this));
	
	helper.EditVector(groupHull, "@Rig.PanelShapeHull.Coordinates", "@Rig.PanelShapeHull.Coordinates.ToolTip",
		pEditPoint, cEditPoint::Ref::New(*this));
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
		pSpinPoint->SetRange(0, decMath::max(hull->GetPoints().GetCount() - 1, 0));
		
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
	return hull && hull->GetPoints().GetCount() > 0 ? pSpinPoint->GetValue() : -1;
}

void reWPPanelShapeHull::UpdatePoint(){
	const reRigShapeHull * const hull = (reRigShapeHull*)GetShape();
	const int index = GetSelectedPoint();
	
	if(hull && index != -1){
		pEditPoint->SetVector(hull->GetPoints().GetAt(index));
		
	}else{
		pEditPoint->SetVector(decVector());
	}
	
	const bool enabled = hull && index != -1;
	pEditPoint->SetEnabled(enabled);
}
