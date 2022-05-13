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

#include "reWPRig.h"
#include "reWPRigListener.h"
#include "reWindowProperties.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/bone/reRigBone.h"
#include "../../undosys/properties/rig/reURigSetRootBone.h"
#include "../../undosys/properties/rig/reURigToggleDynamic.h"
#include "../../undosys/properties/rig/reURigToggleModelCollision.h"
#include "../../undosys/properties/rig/reURigSetCentralMassPoint.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
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
	reWPRig &pPanel;
	
public:
	cBaseTextFieldListener( reWPRig &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		if( ! rig ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, rig ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, reRig *rig ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPRig &pPanel;
	
public:
	cBaseEditVectorListener( reWPRig &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		reRig * const rig = pPanel.GetRig();
		if( ! rig ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( editVector->GetVector(), rig ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, reRig *rig ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPRig &pPanel;
	
public:
	cBaseAction( reWPRig &panel, const char *text, const char *description ) :
	igdeAction( text, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		reRig * const rig = pPanel.GetRig();
		if( ! rig ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( rig ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( reRig *rig ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPRig &pPanel;
	
public:
	cBaseComboBoxListener( reWPRig &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		reRig * const rig = pPanel.GetRig();
		if( ! rig ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnTextChanged( comboBox, rig ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, reRig *rig ) = 0;
};



class cComboRootBone : public cBaseComboBoxListener{
public:
	cComboRootBone( reWPRig &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, reRig *rig ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *bone = NULL;
		if( selection ){
			bone = ( reRigBone* )selection->GetData();
		}
		if( bone == rig->GetRootBone() ){
			return NULL;
		}
		return new reURigSetRootBone( rig, bone );
	}
};

class cCheckDynamic : public cBaseAction{
public:
	cCheckDynamic( reWPRig &panel ) :
	cBaseAction( panel, "Dynamic", "Determines if the rig is afflicted by dynamic physics." ){ }
	
	virtual igdeUndo *OnAction( reRig *rig ){
		return new reURigToggleDynamic( rig );
	}
};

class cCheckModelCollision : public cBaseAction{
public:
	cCheckModelCollision( reWPRig &panel ) :
	cBaseAction( panel, "Model Collision", "Use model collision instead of shapes." ){ }
	
	virtual igdeUndo *OnAction( reRig *rig ){
		return new reURigToggleModelCollision( rig );
	}
};

class cEditCentralMassPoint : public cBaseEditVectorListener{
public:
	cEditCentralMassPoint( reWPRig &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, reRig *rig ){
		if( vector.IsEqualTo( rig->GetCentralMassPoint() ) ){
			return NULL;
		}
		return new reURigSetCentralMassPoint( rig, vector );
	}
};

class cEditMass : public cBaseTextFieldListener{
public:
	cEditMass( reWPRig &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, reRig *rig ){
		rig->SetMass( textField->GetFloat() );
		return nullptr;
	}
};

}



// Class reWPRig
//////////////////

// Constructor, destructor
////////////////////////////

reWPRig::reWPRig( reWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pRig( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new reWPRigListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	helper.GroupBox( content, groupBox, "Rig:" );
	
	helper.ComboBox( groupBox, "Root Bone:", true, "Name of the root bone.",
		pCBRootBone, new cComboRootBone( *this ) );
	pCBRootBone->SetDefaultSorter();
	
	helper.EditVector( groupBox, "CMP:",
		"Central mass point relative to the collider origin.",
		pEditCentralMassPoint, new cEditCentralMassPoint( *this ) );
	
	helper.CheckBox( groupBox, pChkModelCollision, new cCheckModelCollision( *this ), true );
	
	
	helper.GroupBox( content, groupBox, "Simulation:" );
	helper.CheckBox( groupBox, pChkDynamic, new cCheckDynamic( *this ), true );
	helper.EditFloat( groupBox, "Mass:", "Mass in kg.", pEditMass, new cEditMass( *this ) );
}

reWPRig::~reWPRig(){
	SetRig( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void reWPRig::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	if( pRig ){
		pRig->RemoveNotifier( pListener );
		pRig->FreeReference();
		pRig = NULL;
	}
	
	pRig = rig;
	
	if( rig ){
		rig->AddNotifier( pListener );
		rig->AddReference();
	}
	
	UpdateRig();
}



void reWPRig::UpdateRootBoneList(){
	pCBRootBone->RemoveAllItems();
	
	if( pRig ){
		const int count = pRig->GetBoneCount();
		int i;
		
		for( i=0; i<count; i++ ){
			reRigBone * const bone = pRig->GetBoneAt( i );
			if( bone->GetParentBone() ){
				return;
			}
			
			pCBRootBone->AddItem( bone->GetName(), NULL, bone );
		}
	}
	
	pCBRootBone->SortItems();
}

void reWPRig::UpdateRig(){
	UpdateRootBoneList();
	
	if( pRig ){
		pCBRootBone->SetSelection( pCBRootBone->IndexOfItemWithData( pRig->GetRootBone() ) );
		pEditCentralMassPoint->SetVector( pRig->GetCentralMassPoint() );
		pChkModelCollision->SetChecked( pRig->GetModelCollision() );
		pChkDynamic->SetChecked( pRig->GetDynamic() );
		pEditMass->SetFloat( pRig->GetMass() );
		
	}else{
		pCBRootBone->SetSelection( -1 );
		pEditCentralMassPoint->SetVector( decVector() );
		pChkModelCollision->SetChecked( false );
		pChkDynamic->SetChecked( false );
		pEditMass->ClearText();
	}
	
	const bool enabled = pRig != NULL;
	pCBRootBone->SetEnabled( enabled );
	pEditCentralMassPoint->SetEnabled( enabled );
	pChkModelCollision->SetEnabled( enabled );
	pEditMass->SetEnabled( enabled );
}
