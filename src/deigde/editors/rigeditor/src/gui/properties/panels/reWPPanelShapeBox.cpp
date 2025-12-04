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

#include "reWPPanelShapeBox.h"
#include "../reWPShape.h"
#include "../reWindowProperties.h"
#include "../../reWindowMain.h"
#include "../../../rig/reRig.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "../../../undosys/properties/shape/reUSetShapeBoxHalfExtends.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cEditHalfExtends : public igdeEditVectorListener{
	reWPPanelShapeBox &pPanel;
public:
	cEditHalfExtends( reWPPanelShapeBox &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		reRig * const rig = pPanel.GetRig();
		reRigShapeBox * const box = ( reRigShapeBox* )pPanel.GetShape();
		if( ! rig || ! box ){
			return;
		}
		
		if( editVector->GetVector().IsEqualTo( box->GetHalfExtends() ) ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new reUSetShapeBoxHalfExtends( box, editVector->GetVector() ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
};

}



// Class reWPPanelShapeBox
////////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShapeBox::reWPPanelShapeBox( reWPShape &wpShapes ) :
reWPPanelShape( wpShapes, reRigShape::estBox )
{
	igdeEnvironment &env = wpShapes.GetEnvironment();
	igdeContainerReference groupBox;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	
	
	helper.GroupBox( *this, groupBox, "Box Parameters:" );
	
	helper.EditVector( groupBox, "Position:", "Position of the box relative to the parent bone.",
		pEditPosition, new cEditPosition( *this ) );
	
	helper.EditVector( groupBox, "Rotation:", "Rotation of the box.",
		pEditRotation, new cEditRotation( *this ) );
	
	helper.EditVector( groupBox, "Half Extends:", "Half extends of the box.",
		pEditHalfExtends, new cEditHalfExtends( *this ) );
}

reWPPanelShapeBox::~reWPPanelShapeBox(){
}



// Management
///////////////

void reWPPanelShapeBox::UpdateShape(){
	reRigShapeBox * const box = ( reRigShapeBox* )GetShape();
	
	reWPPanelShape::UpdateShape();
	
	if( box ){
		pEditPosition->SetVector( box->GetPosition() );
		pEditRotation->SetVector( box->GetOrientation() );
		pEditHalfExtends->SetVector( box->GetHalfExtends() );
		
	}else{
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditHalfExtends->SetVector( decVector() );
	}
	
	const bool enabled = box != NULL;
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditHalfExtends->SetEnabled( enabled );
}
