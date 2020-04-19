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

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextRadius : public igdeTextFieldListener{
	reWPPanelShapeSphere &pPanel;
public:
	cTextRadius( reWPPanelShapeSphere &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		reRigShapeSphere * const sphere = ( reRigShapeSphere* )pPanel.GetShape();
		if( ! rig || ! sphere ){
			return;
		}
		
		const float value = textField->GetFloat();
		if( fabsf( value - sphere->GetRadius() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new reUSetShapeSphereRadius( sphere, value ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
};

}



// Class reWPPanelShapeSphere
///////////////////////////////

// Constructor, destructor
////////////////////////////

reWPPanelShapeSphere::reWPPanelShapeSphere( reWPShape &wpShapes ) :
reWPPanelShape( wpShapes, reRigShape::estSphere )
{
	igdeEnvironment &env = wpShapes.GetEnvironment();
	igdeContainerReference groupBox;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	
	
	helper.GroupBox( *this, groupBox, "Sphere Parameters:" );
	
	helper.EditVector( groupBox, "Position:", "Position of the sphere relative to the parent bone.",
		pEditPosition, new cEditPosition( *this ) );
	
	helper.EditString( groupBox, "Radius:", "Radius of the sphere in meters.",
		pEditRadius, new cTextRadius( *this ) );
}

reWPPanelShapeSphere::~reWPPanelShapeSphere(){
}



// Management
///////////////

void reWPPanelShapeSphere::UpdateShape(){
	reWPPanelShape::UpdateShape();
	
	reRigShapeSphere * const sphere = ( reRigShapeSphere* )GetShape();
	
	if( sphere ){
		pEditPosition->SetVector( sphere->GetPosition() );
		pEditRadius->SetFloat( sphere->GetRadius() );
		
	}else{
		pEditPosition->SetVector( decVector() );
		pEditRadius->ClearText();
	}
	
	const bool enabled = sphere != NULL;
	pEditPosition->SetEnabled( enabled );
	pEditRadius->SetEnabled( enabled );
}
