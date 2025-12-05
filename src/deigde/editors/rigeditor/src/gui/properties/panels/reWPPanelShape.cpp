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

#include "reWPPanelShape.h"
#include "../reWPShape.h"
#include "../reWindowProperties.h"
#include "../../reWindowMain.h"
#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"
#include "../../../rig/shape/reRigShape.h"
#include "../../../undosys/properties/shape/reUSetShapePosition.h"
#include "../../../undosys/properties/shape/reUSetShapeOrientation.h"
#include "../../../undosys/properties/shape/reUShapeSetProperty.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cTextProperty : public igdeTextFieldListener{
	reWPPanelShape &pPanel;
public:
	cTextProperty( reWPPanelShape &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		reRigShape * const shape = pPanel.GetShape();
		if( ! rig || ! shape ){
			return;
		}
		
		if( textField->GetText() == shape->GetProperty() ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new reUShapeSetProperty( shape, textField->GetText() ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
};

}



// class reWPPanelShape::cEditPosition
////////////////////////////////////////

reWPPanelShape::cEditPosition::cEditPosition( reWPPanelShape &panel ) :
pPanel( panel ){
}

void reWPPanelShape::cEditPosition::OnVectorChanged( igdeEditVector *editVector ){
	reRig * const rig = pPanel.GetRig();
	reRigShape * const shape = pPanel.GetShape();
	if( ! rig || ! shape ){
		return;
	}
	
	if( editVector->GetVector().IsEqualTo( shape->GetPosition() ) ){
		return;
	}
	
	igdeUndo::Ref undo;
	undo.TakeOver( new reUSetShapePosition( shape, editVector->GetVector() ) );
	if( undo ){
		rig->GetUndoSystem()->Add( undo );
	}
};



// class reWPPanelShape::cEditRotation
////////////////////////////////////////

reWPPanelShape::cEditRotation::cEditRotation( reWPPanelShape &panel ) :
pPanel( panel ){
}

void reWPPanelShape::cEditRotation::OnVectorChanged( igdeEditVector *editVector ){
	reRig * const rig = pPanel.GetRig();
	reRigShape * const shape = pPanel.GetShape();
	if( ! rig || ! shape ){
		return;
	}
	
	if( editVector->GetVector().IsEqualTo( shape->GetOrientation() ) ){
		return;
	}
	
	igdeUndo::Ref undo;
	undo.TakeOver( new reUSetShapeOrientation( shape, editVector->GetVector() ) );
	if( undo ){
		rig->GetUndoSystem()->Add( undo );
	}
};



// Class reWPPanelShape
//////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShape::reWPPanelShape( reWPShape &wpShape, reRigShape::eShapeTypes requiredShapeType ) :
igdeContainerFlow( wpShape.GetEnvironment(), igdeContainerFlow::eaY ),
pWPShape( wpShape ),
pRequiredShapeType( requiredShapeType ),
pRig( NULL ),
pShape( NULL )
{
	igdeEnvironment &env = wpShape.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	igdeContainer::Ref content;
	content.TakeOver( new igdeContainerForm( env ) );
	AddChild( content );
	
	helper.EditString( content, "Parent:", "Parent bone name or empty string for rig shapes.",
		pEditBone, NULL );
	pEditBone->SetEditable( false );
	
	helper.EditString( content, "Property:", "Custom shape property string.",
		pEditProperty, new cTextProperty( *this ) );
}

reWPPanelShape::~reWPPanelShape(){
}



// Management
///////////////

void reWPPanelShape::SetShape( reRig *rig, reRigShape *shape ){
	if( rig && shape ){
		if( shape->GetShapeType() == pRequiredShapeType ){
			pRig = rig;
			pShape = shape;
			
		}else{
			pRig = NULL;
			pShape = NULL;
		}
		
	}else{
		pRig = NULL;
		pShape = NULL;
	}
}



void reWPPanelShape::UpdateShape(){
	if( pShape ){
		reRigBone * const rigBone = pShape->GetRigBone();
		
		if( rigBone ){
			pEditBone->SetText( rigBone->GetName() );
			
		}else{
			pEditBone->ClearText();
		}
		
		pEditProperty->SetText( pShape->GetProperty() );
		
	}else{
		pEditBone->ClearText();
		pEditProperty->ClearText();
	}
	
	const bool enabled = pShape != NULL;
	pEditBone->SetEnabled( enabled );
	pEditProperty->SetEnabled( enabled );
}
