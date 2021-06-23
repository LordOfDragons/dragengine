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

#include "igdeWPCamera.h"
#include "../igdeCamera.h"
#include "../igdeUIHelper.h"
#include "../igdeCheckBox.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../composed/igdeEditVector.h"
#include "../composed/igdeEditVectorListener.h"
#include "../event/igdeAction.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerFlow.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	igdeWPCamera &pPanel;
	
public:
	cBaseEditVectorListener( igdeWPCamera &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		if( pPanel.GetCamera() ){
			OnVectorChanged( *pPanel.GetCamera(), editVector->GetVector() );
		}
	}
	
	virtual void OnVectorChanged( igdeCamera &camera, const decVector &vector ) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	igdeWPCamera &pPanel;
	
public:
	cBaseTextFieldListener( igdeWPCamera &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		igdeCamera * const camera = pPanel.GetCamera();
		if( camera ){
			OnChanged( textField, *camera );
		}
	}
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	igdeWPCamera &pPanel;
	
public:
	cBaseAction( igdeWPCamera &panel, const char *text, const char *description = "" ) :
		igdeAction( text, description ), pPanel( panel ){ }
	
	virtual void OnAction(){
		if( pPanel.GetCamera() ){
			OnActionCamera( *pPanel.GetCamera() );
		}
	}
	
	virtual void OnActionCamera( igdeCamera &camera ) = 0;
};



class cEditCameraPosition : public cBaseEditVectorListener{
public:
	cEditCameraPosition( igdeWPCamera &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnVectorChanged( igdeCamera &camera, const decVector &vector ){
		if( camera.GetPosition().IsEqualTo( vector ) ){
			return;
		}
		
		camera.SetPosition( vector );
		pPanel.OnAction();
	}
};

class cEditCameraRotation : public cBaseEditVectorListener{
public:
	cEditCameraRotation( igdeWPCamera &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnVectorChanged( igdeCamera &camera, const decVector &vector ){
		if( camera.GetOrientation().IsEqualTo( vector ) ){
			return;
		}
		
		camera.SetOrientation( vector );
		pPanel.OnAction();
	}
};

class cTextOrbitDistance : public cBaseTextFieldListener{
public:
	cTextOrbitDistance( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetDistance() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetDistance( value );
		pPanel.OnAction();
	}
};

class cTextFov : public cBaseTextFieldListener{
public:
	cTextFov( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetFov() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetFov( value );
		pPanel.OnAction();
	}
};

class cTextFovRatio : public cBaseTextFieldListener{
public:
	cTextFovRatio( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetFovRatio() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetFovRatio( value );
		pPanel.OnAction();
	}
};

class cTextImageDistance : public cBaseTextFieldListener{
public:
	cTextImageDistance( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetImageDistance() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetImageDistance( value );
		pPanel.OnAction();
	}
};

class cTextViewDistance : public cBaseTextFieldListener{
public:
	cTextViewDistance( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetViewDistance() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetViewDistance( value );
		pPanel.OnAction();
	}
};

class cTextExposure : public cBaseTextFieldListener{
public:
	cTextExposure( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetExposure() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetExposure( value );
		pPanel.OnAction();
	}
};

class cTextAdaptionTime : public cBaseTextFieldListener{
public:
	cTextAdaptionTime( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetAdaptionTime() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetAdaptionTime( value );
		pPanel.OnAction();
	}
};

class cTextLowIntensity : public cBaseTextFieldListener{
public:
	cTextLowIntensity( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetLowestIntensity() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetLowestIntensity( value );
		pPanel.OnAction();
	}
};

class cTextHighIntensity : public cBaseTextFieldListener{
public:
	cTextHighIntensity( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, igdeCamera &camera ){
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetHighestIntensity() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetHighestIntensity( value );
		pPanel.OnAction();
	}
};

class cCheckEnableHDRR : public cBaseAction{
public:
	cCheckEnableHDRR( igdeWPCamera &panel ) : cBaseAction( panel, "Enable HDRR",
		"Enable high definition range rendering (HDRR) if supported" ){ }
	
	virtual void OnActionCamera( igdeCamera &camera ){
		camera.SetEnableHDRR( ! camera.GetEnableHDRR() );
		pPanel.OnAction();
	}
};

class cCheckEnableGI : public cBaseAction{
public:
	cCheckEnableGI( igdeWPCamera &panel ) : cBaseAction( panel, "Enable GI",
		"Enable global illumination (GI) if supported" ){ }
	
	virtual void OnActionCamera( igdeCamera &camera ){
		camera.SetEnableGI( ! camera.GetEnableGI() );
		pPanel.OnAction();
	}
};

}



// Class igdeWPCamera
///////////////////////

// Constructor, destructor
////////////////////////////

igdeWPCamera::igdeWPCamera( igdeEnvironment &environment ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pCamera( NULL )
{
	pCreateContent();
}

igdeWPCamera::igdeWPCamera( igdeEnvironment &environment, igdeAction *action ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pCamera( NULL )
{
	pCreateContent();
	SetAction( action );
}

igdeWPCamera::~igdeWPCamera(){
	DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeWPCamera::SetCamera( igdeCamera *camera ){
	if( camera == pCamera ){
		return;
	}
	
	pCamera = camera;
	
	UpdateCamera();
	UpdateViewDirection();
}

void igdeWPCamera::UpdateCamera(){
	if( pCamera ){
		pEditPosition->SetVector( pCamera->GetPosition() );
		pEditRotation->SetVector( pCamera->GetOrientation() );
		pEditOrbitDistance->SetFloat( pCamera->GetDistance() );
		pEditFov->SetFloat( pCamera->GetFov() );
		pEditFovRatio->SetFloat( pCamera->GetFovRatio() );
		pEditImageDist->SetFloat( pCamera->GetImageDistance() );
		pEditViewDist->SetFloat( pCamera->GetViewDistance() );
		pEditExposure->SetFloat( pCamera->GetExposure() );
		pEditAdaptTime->SetFloat( pCamera->GetAdaptionTime() );
		pEditLowInt->SetFloat( pCamera->GetLowestIntensity() );
		pEditHiInt->SetFloat( pCamera->GetHighestIntensity() );
		pChkEnableHDRR->SetChecked( pCamera->GetEnableHDRR() );
		pChkEnableGI->SetChecked( pCamera->GetEnableGI() );
		
	}else{
		pEditPosition->SetVector( decVector() );
		pEditRotation->SetVector( decVector() );
		pEditOrbitDistance->ClearText();
		pEditFov->ClearText();
		pEditFovRatio->ClearText();
		pEditImageDist->ClearText();
		pEditViewDist->ClearText();
		pEditExposure->ClearText();
		pEditAdaptTime->ClearText();
		pEditLowInt->ClearText();
		pEditHiInt->ClearText();
		pChkEnableHDRR->SetChecked( true );
		pChkEnableGI->SetChecked( false );
	}
	
	const bool enabled = pCamera != NULL;
	
	pEditPosition->SetEnabled( enabled );
	pEditRotation->SetEnabled( enabled );
	pEditOrbitDistance->SetEnabled( enabled );
	pEditFov->SetEnabled( enabled );
	pEditFovRatio->SetEnabled( enabled );
	pEditImageDist->SetEnabled( enabled );
	pEditViewDist->SetEnabled( enabled );
	pEditExposure->SetEnabled( enabled );
	pEditAdaptTime->SetEnabled( enabled );
	pEditLowInt->SetEnabled( enabled );
	pEditHiInt->SetEnabled( enabled );
	pChkEnableHDRR->SetEnabled( enabled );
	pChkEnableGI->SetEnabled( enabled );
}

void igdeWPCamera::UpdateViewDirection(){
	if( pCamera ){
		pEditViewDir->SetVector( pCamera->GetViewMatrix().TransformView() );
		
	}else{
		pEditViewDir->SetVector( decVector() );
	}
	
	pEditViewDir->SetEnabled( pCamera != NULL );
}



void igdeWPCamera::SetAction( igdeAction *action ){
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

void igdeWPCamera::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeWPCamera::OnParameterChanged( igdeAction *action ){
	//SetEnabled( action->GetEnabled() );
}

void igdeWPCamera::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeWPCamera::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



// Private Functions
//////////////////////

void igdeWPCamera::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference form, frameLine;
	
	form.TakeOver( new igdeContainerForm( env ) );
	AddChild( form );
	
	helper.EditVector( form, "Position:", "Position of the camera.",
		pEditPosition, new cEditCameraPosition( *this ) );
	helper.EditVector( form, "Rotation:", "Rotation of the camera.",
		pEditRotation, new cEditCameraRotation( *this ) );
	
	helper.EditFloat( form, "Orbit Distance:", "Distance to center point",
		pEditOrbitDistance, new cTextOrbitDistance( *this ) );
	
	helper.EditVector( form, "View:", "View direction of the camera.", pEditViewDir, NULL );
	pEditViewDir->SetEditable( false );
	
	helper.FormLine( form, "FoV:", "Field of view", frameLine );
	helper.EditFloat( frameLine, "Field of view in degrees", pEditFov,
		new cTextFov( *this ) );
	helper.EditFloat( frameLine, "Field of view ratio (height / width)", pEditFovRatio,
		new cTextFovRatio( *this ) );
	
	helper.FormLine( form, "Distance:", "Distances", frameLine );
	helper.EditFloat( frameLine, "Image distance (near clipping plane)", pEditImageDist,
		new cTextImageDistance( *this ) );
	helper.EditFloat( frameLine, "View distance (far clipping plane", pEditViewDist,
		new cTextViewDistance( *this ) );
	
	helper.FormLine( form, "Exposure:",
		"Exposure multiplier and adaption time", frameLine );
	helper.EditFloat( frameLine, "Exposure multiplier", pEditExposure,
		new cTextExposure( *this ) );
	helper.EditFloat( frameLine, "Adaption time in seconds", pEditAdaptTime,
		new cTextAdaptionTime( *this ) );
	
	helper.FormLine( form, "Adaption:",
		"Exposure multiplier and adaption time", frameLine );
	helper.EditFloat( frameLine, "Lower intensity to adapt to.", pEditLowInt,
		new cTextLowIntensity( *this ) );
	helper.EditFloat( frameLine, "Higher intensity to adapt to", pEditHiInt,
		new cTextHighIntensity( *this ) );
	
	helper.CheckBox( form, pChkEnableHDRR, new cCheckEnableHDRR( *this ), true );
	helper.CheckBox( form, pChkEnableGI, new cCheckEnableGI( *this ), true );
}
