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
		
		reUSetShapeSphereRadius::Ref undo(reUSetShapeSphereRadius::Ref::New(
			new reUSetShapeSphereRadius( sphere, value ) ));
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
	igdeContainer::Ref groupBox;
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
