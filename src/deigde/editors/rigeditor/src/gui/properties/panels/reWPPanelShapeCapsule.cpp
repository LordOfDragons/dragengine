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

#include "reWPPanelShapeCapsule.h"
#include "../reWPShape.h"
#include "../reWindowProperties.h"
#include "../../reWindowMain.h"
#include "../../../rig/reRig.h"
#include "../../../rig/shape/reRigShapeCapsule.h"
#include "../../../undosys/properties/shape/reUSetShapeCapsuleHalfHeight.h"
#include "../../../undosys/properties/shape/reUSetShapeCapsuleTopRadius.h"
#include "../../../undosys/properties/shape/reUSetShapeCapsuleBottomRadius.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextHalfHeight : public igdeTextFieldListener{
	reWPPanelShapeCapsule &pPanel;
public:
	cTextHalfHeight(reWPPanelShapeCapsule &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		reRig * const rig = pPanel.GetRig();
		reRigShapeCapsule * const capsule = (reRigShapeCapsule*)pPanel.GetShape();
		if(! rig || ! capsule){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - capsule->GetHalfHeight()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		reUSetShapeCapsuleHalfHeight::Ref undo(reUSetShapeCapsuleHalfHeight::Ref::NewWith(
			capsule, value));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextTopRadius : public igdeTextFieldListener{
	reWPPanelShapeCapsule &pPanel;
public:
	cTextTopRadius(reWPPanelShapeCapsule &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		reRig * const rig = pPanel.GetRig();
		reRigShapeCapsule * const capsule = (reRigShapeCapsule*)pPanel.GetShape();
		if(! rig || ! capsule){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - capsule->GetTopRadius()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		reUSetShapeCapsuleTopRadius::Ref undo(reUSetShapeCapsuleTopRadius::Ref::NewWith(capsule, value));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

class cTextBottomRadius : public igdeTextFieldListener{
	reWPPanelShapeCapsule &pPanel;
public:
	cTextBottomRadius(reWPPanelShapeCapsule &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		reRig * const rig = pPanel.GetRig();
		reRigShapeCapsule * const capsule = (reRigShapeCapsule*)pPanel.GetShape();
		if(! rig || ! capsule){
			return;
		}
		
		const float value = textField->GetFloat();
		if(fabsf(value - capsule->GetBottomRadius()) < FLOAT_SAFE_EPSILON){
			return;
		}
		
		reUSetShapeCapsuleBottomRadius::Ref undo(reUSetShapeCapsuleBottomRadius::Ref::NewWith(
			capsule, value));
		if(undo){
			rig->GetUndoSystem()->Add(undo);
		}
	}
};

}



// Class reWPPanelShapeCapsule
////////////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShapeCapsule::reWPPanelShapeCapsule(reWPShape &wpShapes) :
reWPPanelShape(wpShapes, reRigShape::estCapsule)
{
	igdeEnvironment &env = wpShapes.GetEnvironment();
	igdeContainer::Ref groupBox;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	
	
	helper.GroupBox(*this, groupBox, "Capsule Parameters:");
	
	helper.EditVector(groupBox, "Position:", "Position of the sphere relative to the parent bone.",
		pEditPosition, new cEditPosition(*this));
	
	helper.EditVector(groupBox, "Rotation:", "Rotation of the capsule.",
		pEditRotation, new cEditRotation(*this));
	
	helper.EditString(groupBox, "Half Height:", "Half height of the capsule.",
		pEditHalfHeight, new cTextHalfHeight(*this));
	
	helper.EditString(groupBox, "Top Radius:", "Top radius in meters.",
		pEditTopRadius, new cTextTopRadius(*this));
	
	helper.EditString(groupBox, "Bottom Radius:", "Bottom radius in meters.",
		pEditBottomRadius, new cTextBottomRadius(*this));
}

reWPPanelShapeCapsule::~reWPPanelShapeCapsule(){
}



// Management
///////////////

void reWPPanelShapeCapsule::UpdateShape(){
	const reRigShapeCapsule * const capsule = (reRigShapeCapsule*)GetShape();
	
	reWPPanelShape::UpdateShape();
	
	if(capsule){
		pEditPosition->SetVector(capsule->GetPosition());
		pEditRotation->SetVector(capsule->GetOrientation());
		pEditHalfHeight->SetFloat(capsule->GetHalfHeight());
		pEditTopRadius->SetFloat(capsule->GetTopRadius());
		pEditBottomRadius->SetFloat(capsule->GetBottomRadius());
		
	}else{
		pEditPosition->SetVector(decVector());
		pEditRotation->SetVector(decVector());
		pEditHalfHeight->ClearText();
		pEditTopRadius->ClearText();
		pEditBottomRadius->ClearText();
	}
}
