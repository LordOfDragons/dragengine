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

#include <stdio.h>
#include <stdlib.h>

#include "igdeWPCamera.h"
#include "../igdeCamera.h"
#include "../igdeUIHelper.h"
#include "../igdeCheckBox.h"
#include "../igdeTextField.h"
#include "../igdeContainer.h"
#include "../igdeCommonDialogs.h"
#include "../composed/igdeEditVector.h"
#include "../composed/igdeEditVectorListener.h"
#include "../composed/igdeEditSliderText.h"
#include "../composed/igdeEditSliderTextListener.h"
#include "../curveedit/igdeViewCurveBezier.h"
#include "../curveedit/igdeViewCurveBezierListener.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionContextMenu.h"
#include "../event/igdeActionContextMenu.h"
#include "../event/igdeTextFieldListener.h"
#include "../filedialog/igdeFilePattern.h"
#include "../filedialog/igdeFilePatternList.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerFlow.h"
#include "../menu/igdeMenuCascade.h"
#include "../../environment/igdeEnvironment.h"
#include "../../loadsave/igdeLoadSaveCamera.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

namespace{

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	igdeWPCamera &pPanel;
	
public:
	cBaseEditVectorListener( igdeWPCamera &panel ) : pPanel( panel ){ }
	
	void OnVectorChanged( igdeEditVector *editVector ) override{
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
	
	void OnTextChanged( igdeTextField *textField ) override{
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
	cBaseAction( igdeWPCamera &panel, const char *text, const char *description = "",
		igdeIcon *icon = nullptr ) : igdeAction( text, icon, description ), pPanel( panel ){ }
	
	void OnAction() override{
		if( pPanel.GetCamera() ){
			OnActionCamera( *pPanel.GetCamera() );
		}
	}
	
	virtual void OnActionCamera( igdeCamera &camera ) = 0;
};



class cEditCameraPosition : public cBaseEditVectorListener{
public:
	cEditCameraPosition( igdeWPCamera &panel ) : cBaseEditVectorListener( panel ){ }
	
	void OnVectorChanged( igdeCamera &camera, const decVector &vector ) override{
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
	
	void OnVectorChanged( igdeCamera &camera, const decVector &vector ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
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
	
	void OnActionCamera( igdeCamera &camera ) override{
		camera.SetEnableHDRR( ! camera.GetEnableHDRR() );
		pPanel.OnAction();
	}
};

class cCheckEnableGI : public cBaseAction{
public:
	cCheckEnableGI( igdeWPCamera &panel ) : cBaseAction( panel, "Enable GI",
		"Enable global illumination (GI) if supported" ){ }
	
	void OnActionCamera( igdeCamera &camera ) override{
		camera.SetEnableGI( ! camera.GetEnableGI() );
		pPanel.OnAction();
	}
};

class cTextWhiteIntensity : public cBaseTextFieldListener{
public:
	cTextWhiteIntensity( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetWhiteIntensity() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetWhiteIntensity( value );
		pPanel.SetToneMapCurveRangeFromWhiteIntensity();
		pPanel.OnAction();
	}
};

class cTextBloomIntensity : public cBaseTextFieldListener{
public:
	cTextBloomIntensity( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetBloomIntensity() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetBloomIntensity( value );
		pPanel.OnAction();
	}
};

class cTextBloomStrength : public cBaseTextFieldListener{
public:
	cTextBloomStrength( igdeWPCamera &panel ) : cBaseTextFieldListener( panel ){ }
	
	void OnChanged( igdeTextField *textField, igdeCamera &camera ) override{
		const float value = textField->GetFloat();
		if( fabsf( value - camera.GetBloomStrength() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera.SetBloomStrength( value );
		pPanel.OnAction();
	}
};

class cSliderBloomSize : public igdeEditSliderTextListener{
	igdeWPCamera &pPanel;
public:
	cSliderBloomSize( igdeWPCamera &panel ) : igdeEditSliderTextListener(), pPanel( panel ){ }
	
	void OnSliderTextValueChanging( igdeEditSliderText *sliderText ) override{
		OnSliderTextValueChanged( sliderText );
	}
	
	void OnSliderTextValueChanged( igdeEditSliderText *sliderText ) override{
		igdeCamera * const camera = pPanel.GetCamera();
		if( ! camera ){
			return;
		}
		
		const float value = sliderText->GetValue();
		if( fabsf( value - camera->GetBloomSize() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera->SetBloomSize( value );
		pPanel.OnAction();
	}
};

class cSliderBloomBlend : public igdeEditSliderTextListener{
	igdeWPCamera &pPanel;
public:
	cSliderBloomBlend( igdeWPCamera &panel ) : igdeEditSliderTextListener(), pPanel( panel ){ }
	
	void OnSliderTextValueChanging(igdeEditSliderText *sliderText ) override{
		OnSliderTextValueChanged( sliderText );
	}
	
	void OnSliderTextValueChanged(igdeEditSliderText *sliderText ) override{
		igdeCamera * const camera = pPanel.GetCamera();
		if( ! camera ){
			return;
		}
		
		const float value = sliderText->GetValue();
		if( fabsf( value - camera->GetBloomBlend() ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		camera->SetBloomBlend( value );
		pPanel.OnAction();
	}
};

class cEditToneMapCurve : public igdeViewCurveBezierListener{
	igdeWPCamera &pPanel;
public:
	cEditToneMapCurve( igdeWPCamera &panel ) : igdeViewCurveBezierListener(), pPanel( panel ){ }
	
	void OnCurveChanging(igdeViewCurveBezier *viewCurveBezier ) override{
		OnCurveChanged( viewCurveBezier );
	}
	
	void OnCurveChanged(igdeViewCurveBezier *viewCurveBezier ) override{
		igdeCamera * const camera = pPanel.GetCamera();
		if( ! camera ){
			return;
		}
		
		const decCurveBezier &curve = viewCurveBezier->GetCurve();
		if( curve == camera->GetToneMapCurve() ){
			return;
		}
		
		camera->SetToneMapCurve( curve );
		pPanel.OnAction();
	}
};

class cActionCameraSetDefaultParams : public cBaseAction{
	const float pLowestIntensity, pHighestIntensity, pAdaptionTime;
public:
	cActionCameraSetDefaultParams( igdeWPCamera &panel, float lowestIntensity,
		float highestIntensity, float adaptionTime, const char *text ) : cBaseAction( panel,
		text, "Set default parameters" ), pLowestIntensity( lowestIntensity ),
		pHighestIntensity( highestIntensity ), pAdaptionTime( adaptionTime ){ }
	
	void OnActionCamera( igdeCamera &camera ) override{
		camera.SetDefaultParameters( pLowestIntensity, pHighestIntensity, pAdaptionTime );
		pPanel.OnAction();
	}
};

class cActionCameraLoad : public cBaseAction{
public:
	cActionCameraLoad( igdeWPCamera &panel ) : cBaseAction( panel, "Load...", "Load parameters from file",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ) ){ }
	
	void OnActionCamera( igdeCamera &camera ) override{
		if( ! igdeCommonDialogs::GetFileOpen( &pPanel, "Open Camera",
		*pPanel.GetEnvironment().GetFileSystemGame(), igdeWPCamera::patternCamera,
		igdeWPCamera::lastCameraFile ) ){
			return;
		}
		
		igdeLoadSaveCamera lscamera( pPanel.GetEnvironment(), pPanel.GetLogger(), "IGDE" );
		lscamera.Load( igdeWPCamera::lastCameraFile, camera, decBaseFileReader::Ref::New(
			pPanel.GetEnvironment().GetFileSystemGame()->OpenFileForReading(
			decPath::CreatePathUnix( igdeWPCamera::lastCameraFile ) ) ) );
	}
};

class cActionCameraSave : public cBaseAction{
public:
	cActionCameraSave( igdeWPCamera &panel ) : cBaseAction( panel, "Save...", "Save parameters to file",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ) ){ }
	
	void OnActionCamera( igdeCamera &camera ) override{
		if( ! igdeCommonDialogs::GetFileSave( &pPanel, "Save Camera",
		*pPanel.GetEnvironment().GetFileSystemGame(), igdeWPCamera::patternCamera,
		igdeWPCamera::lastCameraFile ) ){
			return;
		}
		
		igdeLoadSaveCamera lscamera( pPanel.GetEnvironment(), pPanel.GetLogger(), "IGDE" );
		lscamera.Save( camera, decBaseFileWriter::Ref::New(
			pPanel.GetEnvironment().GetFileSystemGame()->OpenFileForWriting(
			decPath::CreatePathUnix( igdeWPCamera::lastCameraFile ) ) ) );
	}
};

class cActionMenuCamera : public igdeActionContextMenu{
	igdeWPCamera &pPanel;
public:
	cActionMenuCamera( igdeWPCamera &panel ) : igdeActionContextMenu(
		"...", nullptr, "Camera menu" ), pPanel( panel ){ }
	
	void AddContextMenuEntries( igdeMenuCascade &contextMenu ) override{
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelperProperties();
		helper.MenuCommand( contextMenu, new cActionCameraSetDefaultParams(
			pPanel, 1.0f, 1.0f, 0.1f, "Set Default Indoor" ), true );
		helper.MenuCommand( contextMenu, new cActionCameraSetDefaultParams(
			pPanel, 20.0f, 20.0f, 0.1f, "Set Default Day" ), true );
		helper.MenuCommand( contextMenu, new cActionCameraSetDefaultParams(
			pPanel, 0.1f, 0.1f, 0.1f, "Set Default Night" ), true );
		helper.MenuCommand( contextMenu, new cActionCameraSetDefaultParams(
			pPanel, 1.0f, 20.0f, 0.1f, "Set Default Dynamic" ), true );
		
		helper.Separator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionCameraLoad( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionCameraSave( pPanel ), true );
	}
	
	void Update() override{
		SetEnabled( pPanel.GetCamera() );
	}
};

}



// Class igdeWPCamera
///////////////////////

decString igdeWPCamera::lastCameraFile( "Camera.decamera" );

static igdeFilePatternList sCreateFilePatternList(){
	igdeFilePatternList list;
	list.AddFilePattern( new igdeFilePattern( "Drag[en]gine Camera", "*.decamera", ".decamera" ) );
	return list;
}

const igdeFilePatternList igdeWPCamera::patternCamera( sCreateFilePatternList() );



// Constructor, destructor
////////////////////////////

igdeWPCamera::igdeWPCamera( igdeEnvironment &environment ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pCamera( nullptr )
{
	pCreateContent();
}

igdeWPCamera::igdeWPCamera( igdeEnvironment &environment, igdeAction *action ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY ),
pCamera( nullptr )
{
	pCreateContent();
	SetAction( action );
}

igdeWPCamera::~igdeWPCamera(){
	DestroyNativeWidget();
	SetAction( nullptr );
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
	SetToneMapCurveRangeFromWhiteIntensity();
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
		pEditWhiteIntensity->SetFloat( pCamera->GetWhiteIntensity() );
		pEditToneMapCurve->SetCurve( pCamera->GetToneMapCurve() );
		pSldBloomBlend->SetValue( pCamera->GetBloomBlend() );
		pEditBloomIntensity->SetFloat( pCamera->GetBloomIntensity() );
		pEditBloomStrength->SetFloat( pCamera->GetBloomStrength() );
		pSldBloomSize->SetValue( pCamera->GetBloomSize() );
		
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
		pEditWhiteIntensity->ClearText();
		pEditToneMapCurve->ClearCurve();
		pSldBloomBlend->SetValue( 1.0f );
		pEditBloomIntensity->ClearText();
		pEditBloomStrength->ClearText();
		pSldBloomSize->SetValue( 0.1f );
	}
	
	const bool enabled = pCamera != nullptr;
	
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
	pEditWhiteIntensity->SetEnabled( enabled );
	pEditToneMapCurve->SetEnabled( enabled );
	pSldBloomBlend->SetEnabled( enabled );
	pEditBloomIntensity->SetEnabled( enabled );
	pEditBloomStrength->SetEnabled( enabled );
	pSldBloomSize->SetEnabled( enabled );
}

void igdeWPCamera::UpdateViewDirection(){
	if( pCamera ){
		pEditViewDir->SetVector( pCamera->GetViewMatrix().TransformView() );
		
	}else{
		pEditViewDir->SetVector( decVector() );
	}
	
	pEditViewDir->SetEnabled( pCamera != nullptr );
}

void igdeWPCamera::SetToneMapCurveRangeFromWhiteIntensity(){
	if( pCamera ){
		pEditToneMapCurve->SetClampMax( decVector2( pCamera->GetWhiteIntensity(), 1.0f ) );
		
	}else{
		pEditToneMapCurve->SetClampMax( decVector2( 4.0f, 1.0f ) );
	}
	
	pEditToneMapCurve->SetClampMin( decVector2( 0.0f, 0.0f ) );
	pEditToneMapCurve->ResetView();
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

void igdeWPCamera::OnParameterChanged( igdeAction* ){
	//SetEnabled( action->GetEnabled() );
}

void igdeWPCamera::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeWPCamera::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = nullptr;
}



// Private Functions
//////////////////////

void igdeWPCamera::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref form, frameLine, group;
	
	
	form.TakeOver( new igdeContainerForm( env ) );
	AddChild( form );
	
	helper.EditVector( form, "Position:", "Position of the camera",
		pEditPosition, new cEditCameraPosition( *this ) );
	helper.EditVector( form, "Rotation:", "Rotation of the camera",
		pEditRotation, new cEditCameraRotation( *this ) );
	
	helper.EditFloat( form, "Orbit Distance:", "Distance to center point",
		pEditOrbitDistance, new cTextOrbitDistance( *this ) );
	
	helper.EditVector( form, "View:", "View direction of the camera", pEditViewDir, nullptr );
	pEditViewDir->SetEditable( false );
	
	helper.FormLineStretchFirst( form, "", "", frameLine );
	
	helper.CheckBoxOnly( frameLine, pChkEnableHDRR, new cCheckEnableHDRR( *this ), true );
	
	igdeActionContextMenu::Ref actionMenuCamera;
	actionMenuCamera.TakeOver( new cActionMenuCamera( *this ) );
	helper.Button( frameLine, pBtnCamera, actionMenuCamera );
	actionMenuCamera->SetWidget( pBtnCamera );
	
	helper.CheckBox( form, pChkEnableGI, new cCheckEnableGI( *this ), true );
	
	
	helper.GroupBox( *this, group, "Internal parameters:", true );
	helper.EditFloat( group, "Field of view:", "Field of view in degrees", pEditFov,
		new cTextFov( *this ) );
	helper.EditFloat( group, "Field of view ratio:", "Field of view ratio (height / width)",
		pEditFovRatio, new cTextFovRatio( *this ) );
	
	helper.EditFloat( group, "Image distance:", "Image distance in meters (near clipping plane)",
		pEditImageDist, new cTextImageDistance( *this ) );
	helper.EditFloat( group, "View distance:", "View distance in meters (far clipping plane)",
		pEditViewDist, new cTextViewDistance( *this ) );
	
	
	helper.GroupBox( *this, group, "Exposure controls:", true );
	helper.EditFloat( group, "Lower intensity:", "Lower intensity to adapt to",
		pEditLowInt, new cTextLowIntensity( *this ) );
	helper.EditFloat( group, "Higher intensity:", "Higher intensity to adapt to",
		pEditHiInt, new cTextHighIntensity( *this ) );
	helper.EditFloat( group, "Exposure:", "Exposure multiplier", pEditExposure,
		new cTextExposure( *this ) );
	helper.EditFloat( group, "Adaption time:", "Adaption time in seconds", pEditAdaptTime,
		new cTextAdaptionTime( *this ) );
	
	
	helper.GroupBoxFlow( *this, group, "Tone mapping:", true, true );
	form.TakeOver( new igdeContainerForm( env ) );
	group->AddChild( form );
	
	helper.EditFloat( form, "White intensity:",
		"White intensity multiplier (avoid bright parts wash out to white)",
		pEditWhiteIntensity, new cTextWhiteIntensity( *this ) );
	
	helper.Label( group, "Custom Curve:" );
	helper.ViewCurveBezier( group, pEditToneMapCurve, new cEditToneMapCurve( *this ) );
	pEditToneMapCurve->SetDefaultSize( decPoint( 200, 150 ) );
	pEditToneMapCurve->ClearCurve();
	pEditToneMapCurve->SetClamp( true );
	pEditToneMapCurve->SetClampMin( decVector2( 0.0f, 0.0f ) );
	pEditToneMapCurve->SetClampMax( decVector2( 3.0f, 1.0f ) );
	
	
	helper.GroupBox( *this, group, "Bloom / Overbright:", true );
	helper.EditSliderText( group, "Blend:", "Bloom blend factor (enable/disable bloom)",
		0.0f, 1.0f, 4, 3, 0.1f, pSldBloomBlend, new cSliderBloomBlend( *this ) );
	helper.EditFloat( group, "Intensity:",
		"Bloom intensity multiplier (overbright begin threshold)",
		pEditBloomIntensity, new cTextBloomIntensity( *this ) );
	helper.EditFloat( group, "Strength:",
		"Bloom strength (scale intensity above threshold)",
		pEditBloomStrength, new cTextBloomStrength( *this ) );
	helper.EditSliderText( group, "Size:", "Bloom size (blur size relative to screen width)",
		0.0f, 1.0f, 4, 3, 0.1f, pSldBloomSize, new cSliderBloomSize( *this ) );
}
