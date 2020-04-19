/* 
 * Drag[en]gine IGDE Rig Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

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
		
		igdeUndoReference undo;
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
	
	igdeUndoReference undo;
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
	
	igdeUndoReference undo;
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
	
	igdeContainerReference content;
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
