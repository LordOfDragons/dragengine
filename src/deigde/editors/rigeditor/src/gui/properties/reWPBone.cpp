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

#include "reWPBone.h"
#include "reWPBoneListener.h"
#include "reWindowProperties.h"
#include "../reWindowMain.h"
#include "../../rig/reRig.h"
#include "../../rig/bone/reRigBone.h"
#include "../../rig/bone/reSelectionBones.h"
#include "../../undosys/properties/bone/reUSetBoneName.h"
#include "../../undosys/properties/bone/reUSetBoneParent.h"
#include "../../undosys/properties/bone/reUSetBonePosition.h"
#include "../../undosys/properties/bone/reUSetBoneOrientation.h"
#include "../../undosys/properties/bone/reUSetBoneCentralMassPoint.h"
#include "../../undosys/properties/bone/reUSetBoneMass.h"
#include "../../undosys/properties/bone/reUToggleBoneDynamic.h"

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
	reWPBone &pPanel;
	
public:
	cBaseTextFieldListener( reWPBone &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if( ! rig || ! bone ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, rig, bone ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, reRig *rig, reRigBone *bone ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPBone &pPanel;
	
public:
	cBaseEditVectorListener( reWPBone &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if( ! rig || ! bone ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( editVector->GetVector(), rig, bone ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decVector &vector, reRig *rig, reRigBone *bone ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPBone &pPanel;
	
public:
	cBaseAction( reWPBone &panel, const char *text, const char *description ) :
	igdeAction( text, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if( ! rig || ! bone ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( rig, bone ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( reRig *rig, reRigBone *bone ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPBone &pPanel;
	
public:
	cBaseComboBoxListener( reWPBone &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		reRig * const rig = pPanel.GetRig();
		reRigBone * const bone = pPanel.GetBone();
		if( ! rig || ! bone ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnTextChanged( comboBox, rig, bone ) );
		if( undo ){
			rig->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, reRig *rig, reRigBone *bone ) = 0;
};



class cTextName : public cBaseTextFieldListener{
public:
	cTextName( reWPBone &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, reRig *rig, reRigBone *bone ){
		const decString &name = textField->GetText();
		if( name == bone->GetName() ){
			return NULL;
		}
		if( rig->GetBoneNamed( name ) ){
			igdeCommonDialogs::ErrorFormat( &pPanel, "Invalid Bone Name",
				"There exists already a bone named %s.", name.GetString() );
			return NULL;
		}
		return new reUSetBoneName( bone, name );
	}
};

class cComboParent : public cBaseComboBoxListener{
public:
	cComboParent( reWPBone &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, reRig *rig, reRigBone *bone ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *parent = NULL;
		if( selection ){
			parent = ( reRigBone* )selection->GetData();
		}
		
		if( parent == bone->GetParentBone() ){
			return NULL;
		}
		
		if( parent && ! bone->CanHaveParent( parent ) ){
			igdeCommonDialogs::ErrorFormat( &pPanel, "Invalid Parent Bone",
				"Internal error. Bone '%s' is not valid as parent and should not have been selectable!",
				parent->GetName().GetString() );
			return NULL;
		}

		return new reUSetBoneParent( bone, parent );
	}
};

class cEditPosition : public cBaseEditVectorListener{
public:
	cEditPosition( reWPBone &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, reRig *rig, reRigBone *bone ){
		if( vector.IsEqualTo( bone->GetPosition() ) ){
			return NULL;
		}
		return new reUSetBonePosition( bone, vector );
	}
};

class cEditRotation : public cBaseEditVectorListener{
public:
	cEditRotation( reWPBone &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, reRig *rig, reRigBone *bone ){
		if( vector.IsEqualTo( bone->GetOrientation() ) ){
			return NULL;
		}
		return new reUSetBoneOrientation( bone, vector );
	}
};

class cEditCentralMassPoint : public cBaseEditVectorListener{
public:
	cEditCentralMassPoint( reWPBone &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decVector &vector, reRig *rig, reRigBone *bone ){
		if( vector.IsEqualTo( bone->GetCentralMassPoint() ) ){
			return NULL;
		}
		return new reUSetBoneCentralMassPoint( bone, vector );
	}
};

class cTextMass : public cBaseTextFieldListener{
public:
	cTextMass( reWPBone &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, reRig *rig, reRigBone *bone ){
		const float mass = textField->GetFloat();
		if( fabsf( mass - bone->GetMass() ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new reUSetBoneMass( bone, mass );
	}
};

class cCheckDynamic : public cBaseAction{
public:
	cCheckDynamic( reWPBone &panel ) : cBaseAction( panel, "Dynamic",
		"Determines if the bone is affected by physics." ){ }
	
	virtual igdeUndo *OnAction( reRig *rig, reRigBone *bone ){
		return new reUToggleBoneDynamic( bone );
	}
};

}



// Class reWPBone
///////////////////

// Constructor, destructor
////////////////////////////

reWPBone::reWPBone( reWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pRig( NULL ),
pBone( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new reWPBoneListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// structure
	helper.GroupBox( content, groupBox, "Structure:" );
	
	helper.EditString( groupBox, "Name:", "Unique name of the bone.",
		pEditName, new cTextName( *this ) );
	
	helper.ComboBox( groupBox, "Parent:", true,
		"Name of the parent bone or the empty string if this is a top level bone.",
		pCBParent, new cComboParent( *this ) );
	pCBParent->SetDefaultSorter();
	
	// geometry
	helper.GroupBox( content, groupBox, "Geometry:" );
	
	helper.EditVector( groupBox, "Position:",
		"Position of the bone relative to the world or parent bone.",
		pEditPosition, new cEditPosition( *this ) );
	
	helper.EditVector( groupBox, "Rotation:",
		"Rotation of the bone relative to the world or parent bone.",
		pEditRotation, new cEditRotation( *this ) );
	
	// physics
	helper.GroupBox( content, groupBox, "Physics:" );
	
	helper.EditVector( groupBox, "CMP:",
		"Central mass point relative to the world or parent bone.",
		pEditCentralMassPoint, new cEditCentralMassPoint( *this ) );
	
	helper.EditFloat( groupBox, "Mass:", "Mass in kg of the matter around the bone.",
		pEditMass, new cTextMass( *this ) );
	
	helper.CheckBox( groupBox, pChkDynamic, new cCheckDynamic( *this ), true );
}

reWPBone::~reWPBone(){
	SetRig( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
	
	if( pBone ){
		pBone->FreeReference();
	}
}



// Management
///////////////

void reWPBone::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	SetBone( NULL );
	
	if( pRig ){
		pRig->RemoveNotifier( pListener );
		pRig->FreeReference();
		pRig = NULL;
	}
	
	pRig = rig;
	
	if( rig ){
		rig->AddNotifier( pListener );
		rig->AddReference();
		
		SetBone( rig->GetSelectionBones()->GetActiveBone() );
	}
}

void reWPBone::SetBone( reRigBone *bone ){
	if( bone == pBone ){
		return;
	}
	
	if( pBone ){
		pBone->FreeReference();
	}
	
	pBone = bone;
	
	if( bone ){
		bone->AddReference();
	}
	
	UpdateBone();
}



void reWPBone::UpdateParentBoneList(){
	pCBParent->RemoveAllItems();
	pCBParent->AddItem( "< No Parent >", NULL, NULL );
	
	if( pRig && pBone ){
		const int boneCount = pRig->GetBoneCount();
		int i;
		
		for( i=0; i<boneCount; i++ ){
			reRigBone * const bone = pRig->GetBoneAt( i );
			if( pBone->CanHaveParent( bone ) ){
				pCBParent->AddItem( bone->GetName(), NULL, bone );
			}
		}
	}
	
	pCBParent->SortItems();
}

void reWPBone::UpdateBone(){
	UpdateParentBoneList();
	
	if( pBone ){
		pEditName->SetText( pBone->GetName() );
		pCBParent->SetSelection( pCBParent->IndexOfItemWithData( pBone->GetParentBone() ) );
		pEditPosition->SetVector( pBone->GetPosition() );
		pEditRotation->SetVector( pBone->GetOrientation() );
		pEditCentralMassPoint->SetVector( pBone->GetCentralMassPoint() );
		pEditMass->SetFloat( pBone->GetMass() );
		pChkDynamic->SetChecked( pBone->GetDynamic() );
		
	}else{
		pEditName->ClearText();
		pCBParent->SetSelection( pCBParent->IndexOfItemWithData( NULL ) );
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditCentralMassPoint->SetVector( decVector() );
		pEditMass->ClearText();
		pChkDynamic->SetChecked( false );
	}
	
	const bool enabled = pBone != NULL;
	pEditName->SetEnabled( enabled );
	pCBParent->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditCentralMassPoint->SetEnabled( enabled );
	pEditMass->SetEnabled( enabled );
	pChkDynamic->SetEnabled( enabled );
}
