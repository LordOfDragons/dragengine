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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "reWPPanelShapeSphere.h"
#include "../reWPShape.h"
#include "../reWindowProperties.h"
#include "../../reWindowMain.h"
#include "../../../rig/reRig.h"
#include "../../../rig/shape/reRigShapeSphere.h"
#include "../../../undosys/properties/shape/reUSetShapeSphereRadius.h"
#include "../../../undosys/properties/shape/reUSetShapeSphereAxisScaling.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVector2.h>
#include <deigde/gui/composed/igdeEditVector2Listener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextRadius : public igdeTextFieldListener{
	reWPPanelShapeSphere &pPanel;
public:
	using Ref = deTObjectReference<cTextRadius>;
	cTextRadius(reWPPanelShapeSphere &panel) : pPanel(panel){}
	
	void OnTextChanged(igdeTextField *textField) override{
		reRig * const rig = pPanel.GetRig();
		reRigShapeSphere * const sphere = (reRigShapeSphere*)pPanel.GetShape();
		if(!rig || !sphere){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - sphere->GetRadius()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		reUSetShapeSphereRadius::Ref undo(reUSetShapeSphereRadius::Ref::New(sphere, value));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

class cEditAxisScaling : public igdeEditVector2Listener{
	reWPPanelShapeSphere &pPanel;
public:
	using Ref = deTObjectReference<cEditAxisScaling>;
	cEditAxisScaling(reWPPanelShapeSphere &panel) : pPanel(panel){}
	
	void OnVector2Changed(igdeEditVector2 *editVector2) override{
		auto rig = pPanel.GetRig();
		auto sphere = (reRigShapeSphere*)pPanel.GetShape();
		if(!rig || !sphere || editVector2->GetVector2().IsEqualTo(sphere->GetAxisScaling())){
			return;
		}
		
		auto undo = reUSetShapeSphereAxisScaling::Ref::New(sphere, editVector2->GetVector2());
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

}



// Class reWPPanelShapeSphere
///////////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShapeSphere::reWPPanelShapeSphere(reWPShape &wpShapes) :
reWPPanelShape(wpShapes, reRigShape::estSphere)
{
	igdeEnvironment &env = wpShapes.GetEnvironment();
	igdeContainer::Ref groupBox;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	
	
	helper.GroupBox(*this, groupBox, "@Rig.PanelShapeSphere.GroupBox.SphereParameters");
	helper.EditVector(groupBox, "@Rig.PanelShapeSphere.Position", "@Rig.PanelShapeSphere.Position.ToolTip",
		pEditPosition, cEditPosition::Ref::New(*this));
	helper.EditVector(groupBox, "@Rig.PanelShapeSphere.Rotation", "@Rig.PanelShapeSphere.Rotation.ToolTip",
		pEditRotation, cEditRotation::Ref::New(*this));
	helper.EditString(groupBox, "@Rig.PanelShapeSphere.Radius", "@Rig.PanelShapeSphere.Radius.ToolTip",
		pEditRadius, cTextRadius::Ref::New(*this));
	helper.EditVector2(groupBox, "@Rig.PanelShapeSphere.AxisScaling", "@Rig.PanelShapeSphere.AxisScaling.ToolTip",
		pEditAxisScaling, cEditAxisScaling::Ref::New(*this));
}

reWPPanelShapeSphere::~reWPPanelShapeSphere(){
}



// Management
///////////////

void reWPPanelShapeSphere::UpdateShape(){
	reWPPanelShape::UpdateShape();
	
	reRigShapeSphere * const sphere = (reRigShapeSphere*)GetShape();
	
	if(sphere){
		pEditPosition->SetVector(sphere->GetPosition());
		pEditRotation->SetVector(sphere->GetOrientation());
		pEditRadius->SetFloat(sphere->GetRadius());
		pEditAxisScaling->SetVector2(sphere->GetAxisScaling());
		
	}else{
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditRadius->ClearText();
		pEditAxisScaling->SetVector2(decVector2(1.0f, 1.0f));
	}
	
	const bool enabled = sphere != nullptr;
	pEditPosition->SetEnabled(enabled);
	pEditRotation->SetEnabled(enabled);
	pEditRadius->SetEnabled(enabled);
	pEditAxisScaling->SetEnabled(enabled);
}
