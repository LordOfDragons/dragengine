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
#include <stdint.h>

#include "reWPPush.h"
#include "reWindowProperties.h"
#include "reWPPushListener.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/push/reRigPush.h"
#include "../../rig/push/reSelectionPushes.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	reWPPush &pPanel;
	
public:
	cBaseTextFieldListener( reWPPush &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		reRigPush * const push = pPanel.GetPush();
		if( rig && push ){
			OnChanged( textField, rig, push );
		}
	}
	
	virtual void OnChanged( igdeTextField *textField, reRig *rig, reRigPush *push ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPPush &pPanel;
	
public:
	cBaseEditVectorListener( reWPPush &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		reRig * const rig = pPanel.GetRig();
		reRigPush * const push = pPanel.GetPush();
		if( rig && push ){
			OnChanged( editVector->GetVector(), rig, push );
		}
	}
	
	virtual void OnChanged( const decVector &vector, reRig *rig, reRigPush *push ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPPush &pPanel;
	
public:
	cBaseComboBoxListener( reWPPush &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		reRig * const rig = pPanel.GetRig();
		reRigPush * const push = pPanel.GetPush();
		if( rig && push ){
			OnTextChanged( comboBox, rig, push );
		}
	}
	
	virtual void OnTextChanged( igdeComboBox *comboBox, reRig *rig, reRigPush *push ) = 0;
};



class cComboType : public cBaseComboBoxListener{
public:
	cComboType( reWPPush &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox, reRig *rig, reRigPush *push ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if( selection ){
			push->SetType( ( reRigPush::ePushTypes )( intptr_t )selection->GetData() );
		}
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	cEditPosition( reWPPush &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnChanged( const decVector &vector, reRig *rig, reRigPush *push ){
		push->SetPosition( vector );
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	cEditRotation( reWPPush &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnChanged( const decVector &vector, reRig *rig, reRigPush *push ){
		push->SetOrientation( vector );
	}
};

class cTextImpuls : public cBaseTextFieldListener{
public:
	cTextImpuls( reWPPush &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig *rig, reRigPush *push ){
		push->SetImpuls( textField->GetFloat() );
	}
};

class cTextRayCount : public cBaseTextFieldListener{
public:
	cTextRayCount( reWPPush &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig *rig, reRigPush *push ){
		push->SetRayCount( textField->GetInteger() );
	}
};

class cTextConeAngle : public cBaseTextFieldListener{
public:
	cTextConeAngle( reWPPush &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig *rig, reRigPush *push ){
		push->SetConeAngle( textField->GetFloat() );
	}
};

}



// Class reWPPush
////////////////////

// Constructor, destructor
////////////////////////////

reWPPush::reWPPush( reWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pRig( NULL ),
pPush( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new reWPPushListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// structure
	helper.GroupBox( content, groupBox, "Geometry:" );
	
	helper.ComboBox( groupBox, "Type:", "Type of the push.", pCBType, new cComboType( *this ) );
	pCBType->AddItem( "Simple Push", NULL, ( void* )( intptr_t )reRigPush::eptSimple );
	pCBType->AddItem( "Explosion", NULL, ( void* )( intptr_t )reRigPush::eptExplosion );
	
	helper.EditVector( groupBox, "Position:", "Position the push originates from.",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation of the push determining the push direction.",
		pEditRotation, new cEditRotation( *this ) );
	
	helper.EditFloat( groupBox, "Impuls:", "Power of the push in Ns.",
		pEditImpuls, new cTextImpuls( *this ) );
	helper.EditInteger( groupBox, "Ray Count:", "Number of rays to shoot.",
		pEditRayCount, new cTextRayCount( *this ) );
	helper.EditFloat( groupBox, "Cone:", "Cone angle in degrees if required by the push type.",
		pEditConeAngle, new cTextConeAngle( *this ) );
}

reWPPush::~reWPPush(){
	SetRig( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void reWPPush::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	SetPush( NULL );
	
	if( pRig ){
		pRig->RemoveNotifier( pListener );
		pRig->FreeReference();
		pRig = NULL;
	}
	
	pRig = rig;
	
	if( rig ){
		rig->AddNotifier( pListener );
		rig->AddReference();
		
		SetPush( rig->GetSelectionPushes()->GetActivePush() );
	}
}

void reWPPush::SetPush( reRigPush *push ){
	if( push == pPush ){
		return;
	}
	
	if( pPush ){
		pPush->FreeReference();
	}
	
	pPush = push;
	
	if( push ){
		push->AddReference();
	}
	
	UpdatePush();
}

void reWPPush::UpdatePush(){
	if( pPush ){
		pCBType->SetSelection( pCBType->IndexOfItemWithData( ( void* )( intptr_t )pPush->GetType() ) );
		pEditPosition->SetVector( pPush->GetPosition() );
		pEditRotation->SetVector( pPush->GetOrientation() );
		pEditImpuls->SetFloat( pPush->GetImpuls() );
		pEditRayCount->SetInteger( pPush->GetRayCount() );
		pEditConeAngle->SetFloat( pPush->GetConeAngle() );
		
	}else{
		pCBType->SetSelection( pCBType->IndexOfItemWithData( ( void* )( intptr_t )reRigPush::eptSimple ) );
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditImpuls->ClearText();
		pEditRayCount->ClearText();
		pEditConeAngle->ClearText();
	}
	
	const bool enabled = pPush != NULL;
	pCBType->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditImpuls->SetEnabled( enabled );
	pEditRayCount->SetEnabled( enabled );
	pEditConeAngle->SetEnabled( enabled );
}
