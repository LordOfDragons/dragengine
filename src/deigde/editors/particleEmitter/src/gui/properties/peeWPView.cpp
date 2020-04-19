/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeWPView.h"
#include "peeWPViewListener.h"
#include "peeWindowProperties.h"
#include "../peeWindowMain.h"
#include "../../emitter/peeEmitter.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCamera.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	peeWPView &pPanel;
public:
	cBaseAction( peeWPView &panel, const char *text, const char *description ) :
	igdeAction( text, description ), pPanel( panel ){ }
	
	virtual void OnAction(){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if( emitter ){
			OnAction( *emitter );
		}
	}
	
	virtual void OnAction( peeEmitter &emitter ) = 0;
};

class cActionSkyChanged : public cBaseAction{
public:
	cActionSkyChanged( peeWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( peeEmitter &emitter ){
		emitter.NotifySkyChanged();
	}
};

class cActionCameraChanged : public cBaseAction{
public:
	cActionCameraChanged( peeWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( peeEmitter &emitter ){
		emitter.NotifyCameraChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	cActionEnvObjChanged( peeWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( peeEmitter &emitter ){
		emitter.NotifyEnvObjectChanged();
	}
};

class cEditPosition : public igdeEditVectorListener{
	peeWPView &pPanel;
public:
	cEditPosition( peeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if( emitter ){
			emitter->SetPosition( editVector->GetVector() );
		}
	}
};

class cEditRotation : public igdeEditVectorListener{
	peeWPView &pPanel;
public:
	cEditRotation( peeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if( emitter ){
			emitter->SetOrientation( editVector->GetVector() );
		}
	}
};

class cEditBurstInterval : public igdeTextFieldListener{
	peeWPView &pPanel;
public:
	cEditBurstInterval( peeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if( emitter ){
			emitter->SetBurstInterval( textField->GetFloat() );
		}
	}
};

class cEditWarmUpTime : public igdeTextFieldListener{
	peeWPView &pPanel;
public:
	cEditWarmUpTime( peeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		peeEmitter * const emitter = pPanel.GetEmitter();
		if( emitter ){
			emitter->SetWarmUpTime( textField->GetFloat() );
		}
	}
};

class cActionEnableCasting : public cBaseAction{
public:
	cActionEnableCasting( peeWPView &panel ) :
	cBaseAction( panel, "Enable Casting", "Determines if casting is enabled" ){ }
	
	virtual void OnAction( peeEmitter &emitter ){
		emitter.SetEnableCasting( ! emitter.GetEnableCasting() );
	}
};

}



// Class peeWPView
///////////////////

// Constructor, destructor
////////////////////////////

peeWPView::peeWPView( peeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pEmitter( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox;
	
	pListener = new peeWPViewListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	helper.WPCamera( content, pWPCamera, new cActionCameraChanged( *this ),
		"Camera:", false, false, true );
	helper.WPSky( content, pWPSky, new cActionSkyChanged( *this ),
		"Sky:", false, false, true );
	helper.WPWObject( content, pWPEnvObject, new cActionEnvObjChanged( *this ),
		"Environment Object:", false, false, true );
	
	// preview settings
	helper.GroupBox( content, groupBox, "Preview:" );
	helper.EditVector( groupBox, "Position:", "Position of the emitter in meters.",
		pEditEmitterPosition, new cEditPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation of the emitter in degrees",
		pEditEmitterRotation, new cEditRotation( *this ) );
	helper.EditFloat( groupBox, "Burst Interval:", "Interval in seconds between bursts",
		pEditEmitterBurstInterval, new cEditBurstInterval( *this ) );
	helper.EditFloat( groupBox, "Warm-Up Time:",
		"Warm-Up time in seconds for the first enabling casting",
		pEditEmitterWarmUpTime, new cEditWarmUpTime( *this ) );
	helper.CheckBox( groupBox, pChkEmitterEnableCasting, new cActionEnableCasting( *this ), true );
}

peeWPView::~peeWPView(){
	SetEmitter( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void peeWPView::SetEmitter( peeEmitter *emitter ){
	if( emitter == pEmitter ){
		return;
	}
	
	pWPEnvObject->SetObjectWrapper( NULL );
	pWPSky->SetSky( NULL );
	pWPCamera->SetCamera( NULL );
	
	if( pEmitter ){
		pEmitter->RemoveListener( pListener );
		pEmitter->FreeReference();
		pEmitter = NULL;
	}
	
	pEmitter = emitter;
	
	if( emitter ){
		emitter->AddListener( pListener );
		emitter->AddReference();
		
		pWPSky->SetSky( emitter->GetSky() );
		pWPEnvObject->SetObjectWrapper( emitter->GetEnvObject() );
		pWPCamera->SetCamera( emitter->GetCamera() );
	}
	
	UpdateView();
	UpdateSky();
	UpdateEnvObject();
	UpdateCamera();
}



void peeWPView::UpdateView(){
	if( pEmitter ){
		pEditEmitterPosition->SetVector( pEmitter->GetPosition() );
		pEditEmitterRotation->SetVector( pEmitter->GetOrientation() );
		pEditEmitterBurstInterval->SetFloat( pEmitter->GetBurstInterval() );
		pEditEmitterWarmUpTime->SetFloat( pEmitter->GetWarmUpTime() );
		pChkEmitterEnableCasting->SetChecked( pEmitter->GetEnableCasting() );
		
	}else{
		pEditEmitterPosition->SetVector( decVector() );
		pEditEmitterRotation->SetVector( decVector() );
		pEditEmitterBurstInterval->ClearText();
		pEditEmitterWarmUpTime->ClearText();
		pChkEmitterEnableCasting->SetChecked( false );
	}
	
	const bool enabled = pEmitter != NULL;
	pEditEmitterPosition->SetEnabled( enabled );
	pEditEmitterRotation->SetEnabled( enabled );
	pEditEmitterBurstInterval->SetEnabled( enabled );
	pEditEmitterWarmUpTime->SetEnabled( enabled );
	pChkEmitterEnableCasting->SetEnabled( enabled );
}

void peeWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void peeWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObjectWrapper();
}

void peeWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
}
