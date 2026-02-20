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

#include "reWPPanelShapeCylinder.h"
#include "../reWPShape.h"
#include "../reWindowProperties.h"
#include "../../reWindowMain.h"
#include "../../../rig/reRig.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "../../../undosys/properties/shape/reUSetShapeCylinderHalfHeight.h"
#include "../../../undosys/properties/shape/reUSetShapeCylinderTopRadius.h"
#include "../../../undosys/properties/shape/reUSetShapeCylinderBottomRadius.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextHalfHeight : public igdeTextFieldListener{
	reWPPanelShapeCylinder &pPanel;
public:
	using Ref = deTObjectReference<cTextHalfHeight>;
	cTextHalfHeight(reWPPanelShapeCylinder &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		reRig * const rig = pPanel.GetRig();
		reRigShapeCylinder * const cylinder = (reRigShapeCylinder*)pPanel.GetShape();
		if(!rig || !cylinder){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - cylinder->GetHalfHeight()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		reUSetShapeCylinderHalfHeight::Ref undo(reUSetShapeCylinderHalfHeight::Ref::New(
			cylinder, value));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextTopRadius : public igdeTextFieldListener{
	reWPPanelShapeCylinder &pPanel;
public:
	using Ref = deTObjectReference<cTextTopRadius>;
	cTextTopRadius(reWPPanelShapeCylinder &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		reRig * const rig = pPanel.GetRig();
		reRigShapeCylinder * const cylinder = (reRigShapeCylinder*)pPanel.GetShape();
		if(!rig || !cylinder){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - cylinder->GetTopRadius()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		reUSetShapeCylinderTopRadius::Ref undo(reUSetShapeCylinderTopRadius::Ref::New(
			cylinder, value));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextBottomRadius : public igdeTextFieldListener{
	reWPPanelShapeCylinder &pPanel;
public:
	using Ref = deTObjectReference<cTextBottomRadius>;
	cTextBottomRadius(reWPPanelShapeCylinder &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		reRig * const rig = pPanel.GetRig();
		reRigShapeCylinder * const cylinder = (reRigShapeCylinder*)pPanel.GetShape();
		if(!rig || !cylinder){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - cylinder->GetBottomRadius()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		reUSetShapeCylinderBottomRadius::Ref undo(reUSetShapeCylinderBottomRadius::Ref::New(
			cylinder, value));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

}



// Class reWPPanelShapeCylinder
/////////////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShapeCylinder::reWPPanelShapeCylinder(reWPShape &wpShapes) :
reWPPanelShape(wpShapes, reRigShape::estCylinder)
{
	igdeEnvironment &env = wpShapes.GetEnvironment();
	igdeContainer::Ref groupBox;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	
	
	helper.GroupBox(*this, groupBox, "@Rig.PanelShapeCylinder.GroupBox.CylinderParameters");
	
	helper.EditVector(groupBox, "@Rig.PanelShapeCylinder.Position", "@Rig.PanelShapeCylinder.Position.ToolTip",
		pEditPosition, cEditPosition::Ref::New(*this));
	
	helper.EditVector(groupBox, "@Rig.PanelShapeCylinder.Rotation", "@Rig.PanelShapeCylinder.Rotation.ToolTip",
		pEditRotation, cEditRotation::Ref::New(*this));
	
	helper.EditString(groupBox, "@Rig.PanelShapeCylinder.HalfHeight", "@Rig.PanelShapeCylinder.HalfHeight.ToolTip",
		pEditHalfHeight, cTextHalfHeight::Ref::New(*this));
	
	helper.EditString(groupBox, "@Rig.PanelShapeCylinder.TopRadius", "@Rig.PanelShapeCylinder.TopRadius.ToolTip",
		pEditTopRadius, cTextTopRadius::Ref::New(*this));
	
	helper.EditString(groupBox, "@Rig.PanelShapeCylinder.BottomRadius", "@Rig.PanelShapeCylinder.BottomRadius.ToolTip",
		pEditBottomRadius, cTextBottomRadius::Ref::New(*this));
}

reWPPanelShapeCylinder::~reWPPanelShapeCylinder(){
}



// Management
///////////////

void reWPPanelShapeCylinder::UpdateShape(){
	const reRigShapeCylinder * const cylinder = (reRigShapeCylinder*)GetShape();
	
	reWPPanelShape::UpdateShape();
	
	if(cylinder){
		pEditPosition->SetVector(cylinder->GetPosition());
		pEditRotation->SetVector(cylinder->GetOrientation());
		pEditHalfHeight->SetFloat(cylinder->GetHalfHeight());
		pEditTopRadius->SetFloat(cylinder->GetTopRadius());
		pEditBottomRadius->SetFloat(cylinder->GetBottomRadius());
		
	}else{
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditHalfHeight->ClearText();
		pEditTopRadius->ClearText();
		pEditBottomRadius->ClearText();
	}
}
