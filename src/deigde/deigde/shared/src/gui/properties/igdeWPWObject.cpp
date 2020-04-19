/* 
 * Drag[en]gine IGDE
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

#include "igdeWPWObject.h"
#include "../igdeUIHelper.h"
#include "../igdeCommonDialogs.h"
#include "../igdeButton.h"
#include "../igdeCheckBox.h"
#include "../igdeGroupBox.h"
#include "../igdeTextField.h"
#include "../browse/igdeDialogBrowserObjectClass.h"
#include "../composed/igdeEditVector.h"
#include "../composed/igdeEditVectorListener.h"
#include "../event/igdeAction.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerFlow.h"
#include "../wrapper/igdeWObject.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/class/igdeGDClass.h"
#include "../../gamedefinition/class/igdeGDClassManager.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseEditVector : public igdeEditVectorListener{
protected:
	igdeWPWObject &pPanel;
	
public:
	cBaseEditVector( igdeWPWObject &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		if( pPanel.GetObject() ){
			OnVectorChanged( *pPanel.GetObject(), editVector->GetVector() );
		}
	}
	
	virtual void OnVectorChanged( igdeWObject &object, const decVector &vector ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	igdeWPWObject &pPanel;
	
public:
	cBaseAction( igdeWPWObject &panel, const char *text, const char *description ) :
	igdeAction( text, description ), pPanel( panel ){ }
	
	virtual void OnAction(){
		if( pPanel.GetObject() ){
			OnAction( *pPanel.GetObject() );
		}
	}
	
	virtual void OnAction( igdeWObject &object ) = 0;
};



class cTextClass : public igdeTextFieldListener{
	igdeWPWObject &pPanel;
	
public:
	cTextClass( igdeWPWObject &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		if( ! pPanel.GetObject() ){
			return;
		}
		
		igdeWObject &object = *pPanel.GetObject();
		igdeGDClass * const oldGDClass = object.GetGDClass();
		
		object.SetGDClassName( textField->GetText() );
		
		if( object.GetGDClass() != oldGDClass ){
			pPanel.OnAction();
		}
	}
};

class cActionSelectClass : public cBaseAction{
	igdeTextField &pTextField;
	
public:
	cActionSelectClass( igdeWPWObject &panel, igdeTextField &textField ) :
	cBaseAction( panel, "...", "Brings up a dialog to select the object class." ),
	pTextField( textField ){ }
	
	virtual void OnAction( igdeWObject &object ){
		igdeGDClass *gdClass = object.GetGDClass();
		if( igdeDialogBrowserObjectClass::SelectObjectClass( &pPanel, gdClass ) ){
			object.SetGDClassName( gdClass->GetName() );
			pTextField.SetText( gdClass->GetName() );
			pPanel.OnAction();
		}
	}
};


class cEditPosition : public cBaseEditVector{
public:
	cEditPosition( igdeWPWObject &panel ) : cBaseEditVector( panel ){ }
	
	virtual void OnVectorChanged( igdeWObject &object, const decVector &vector ){
		if( ! object.GetPosition().IsEqualTo( vector ) ){
			object.SetPosition( vector );
			pPanel.OnAction();
		}
	}
};

class cEditOrientation : public cBaseEditVector{
public:
	cEditOrientation( igdeWPWObject &panel ) : cBaseEditVector( panel ){ }
	
	virtual void OnVectorChanged( igdeWObject &object, const decVector &vector ){
		const decQuaternion orientation( decQuaternion::CreateFromEuler( vector * DEG2RAD ) );
		if( ! object.GetOrientation().IsEqualTo( orientation ) ){
			object.SetOrientation( orientation );
			pPanel.OnAction();
		}
	}
};

class cEditScaling : public cBaseEditVector{
public:
	cEditScaling( igdeWPWObject &panel ) : cBaseEditVector( panel ){ }
	
	virtual void OnVectorChanged( igdeWObject &object, const decVector &vector ){
		if( ! object.GetScaling().IsEqualTo( vector ) ){
			object.SetScaling( vector );
			pPanel.OnAction();
		}
	}
};


class cCheckVisible : public cBaseAction{
public:
	cCheckVisible( igdeWPWObject &panel ) :
	cBaseAction( panel, "Visible", "Object is visible" ){ }
	
	virtual void OnAction( igdeWObject &object ){
		object.SetVisible( ! object.GetVisible() );
		pPanel.OnAction();
	}
};

class cCheckDynamicCollider : public cBaseAction{
public:
	cCheckDynamicCollider( igdeWPWObject &panel ) :
	cBaseAction( panel, "Dynamic Collider", "Object is attached using a dynamic collider" ){ }
	
	virtual void OnAction( igdeWObject &object ){
		object.SetDynamicCollider( ! object.GetDynamicCollider() );
		pPanel.OnAction();
	}
};

}



// Class igdeWPWObject
////////////////////////

// Constructor, destructor
////////////////////////////

igdeWPWObject::igdeWPWObject( igdeEnvironment &environment ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pObject( NULL )
{
	pCreateContent();
}

igdeWPWObject::igdeWPWObject( igdeEnvironment &environment, igdeAction *action ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pObject( NULL )
{
	pCreateContent();
	SetAction( action );
}

igdeWPWObject::~igdeWPWObject(){
	DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeWPWObject::SetObject( igdeWObject *object ){
	if( object == pObject ){
		return;
	}
	
	pObject = object;
	UpdateObjectWrapper();
}

void igdeWPWObject::UpdateObject(){
	if( pObject ){
		if( pObject->GetGDClass() ){
			pEditClass->SetText( pObject->GetGDClass()->GetName() );
			
		}else{
			pEditClass->ClearText();
		}
		
		pEditPosition->SetVector( pObject->GetPosition() );
		pEditOrientation->SetVector( pObject->GetOrientation().GetEulerAngles() / DEG2RAD );
		pEditScaling->SetVector( pObject->GetScaling() );
		pChkVisible->SetChecked( pObject->GetVisible() );
		pChkDynamicCollider->SetChecked( pObject->GetDynamicCollider() );
		
	}else{
		pEditClass->ClearText();
		pEditPosition->SetVector( decVector() );
		pEditOrientation->SetVector( decVector() );
		pEditScaling->SetVector( decVector() );
		pChkVisible->SetChecked( false );
		pChkDynamicCollider->SetChecked( false );
	}
	
	const bool enabled = pObject != NULL;
	pEditClass->SetEnabled( enabled );
	pEditPosition->SetEnabled( enabled );
	pEditOrientation->SetEnabled( enabled );
	pEditScaling->SetEnabled( enabled );
	pChkVisible->SetEnabled( enabled );
	pChkDynamicCollider->SetEnabled( enabled );
}



void igdeWPWObject::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}

void igdeWPWObject::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeWPWObject::OnParameterChanged( igdeAction *action ){
	//SetEnabled( action->GetEnabled() );
}

void igdeWPWObject::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeWPWObject::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



// Private Functions
//////////////////////

void igdeWPWObject::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference form, frameLine;
	
	form.TakeOver( new igdeContainerForm( env ) );
	AddChild( form );
	
	helper.FormLineStretchFirst( form, "Class:",
		"Game definition class to use for the object.", frameLine );
	helper.EditString( frameLine, "Path to the sky to use.",
		pEditClass, new cTextClass( *this ) );
	helper.Button( frameLine, pBtnClass, new cActionSelectClass( *this, pEditClass ), true );
	
	helper.EditVector( form, "Position:", "Position of object in meters.",
		pEditPosition, new cEditPosition( *this ) );
	helper.EditVector( form, "Orientation:", "Orientation of object in euler coordinates.",
		pEditOrientation, new cEditOrientation( *this ) );
	helper.EditVector( form, "Scaling:", "Scaling of object.",
		pEditScaling, new cEditScaling( *this ) );
	
	helper.CheckBox( form, pChkVisible, new cCheckVisible( *this ), true );
	helper.CheckBox( form, pChkDynamicCollider, new cCheckDynamicCollider( *this ), true );
}
