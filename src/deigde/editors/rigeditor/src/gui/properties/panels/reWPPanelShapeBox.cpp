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
#include <deigde/undo/igdeUndoReference.h>

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
		
		igdeUndoReference undo;
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
