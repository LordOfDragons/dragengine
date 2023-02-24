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
#include <string.h>
#include <stdlib.h>

#include "reWPView.h"
#include "reWPViewListener.h"
#include "reWindowProperties.h"
#include "../reWindowMain.h"
#include "../../configuration/reConfiguration.h"
#include "../../rig/reRig.h"
#include "../../rig/reCamera.h"
#include "../../rig/reRigTexture.h"
#include "../../rig/bone/reRigBone.h"
#include "../../loadsave/reLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
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
#include <deigde/gui/igdeSlider.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditSliderText.h>
#include <deigde/gui/composed/igdeEditSliderTextListener.h>
#include <deigde/gui/composed/igdeEditDVector.h>
#include <deigde/gui/composed/igdeEditDVectorListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSliderListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/rig/deRig.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	reWPView &pPanel;
	
public:
	cBaseTextFieldListener( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnChanged( textField, *rig );
		}
	}
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	reWPView &pPanel;
	
public:
	cBaseAction( reWPView &panel, const char *text, const char *description ) :
	igdeAction( text, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnAction( *rig );
		}
	}
	
	virtual void OnAction( reRig &rig ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	reWPView &pPanel;
	
public:
	cBaseComboBoxListener( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnTextChanged( comboBox, *rig );
		}
	}
	
	virtual void OnTextChanged( igdeComboBox *comboBox, reRig &rig ) = 0;
};

class cBaseSliderListener : public igdeEditSliderTextListener{
protected:
	reWPView &pPanel;
	
public:
	cBaseSliderListener( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnSliderTextValueChanging( igdeEditSliderText *sliderText ){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnChanging( sliderText->GetValue(), *rig );
		}
	}
	
	virtual void OnSliderTextValueChanged( igdeEditSliderText *sliderText ){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnChanged( sliderText->GetValue(), *rig );
		}
	}
	
	virtual void OnChanging( float value, reRig &rig ) = 0;
	virtual void OnChanged( float value, reRig &rig ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	reWPView &pPanel;
	
public:
	cBaseEditVectorListener( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnChanged( editVector->GetVector(), *rig );
		}
	}
	
	virtual void OnChanged( const decVector &vector, reRig &rig ) = 0;
};

class cBaseEditDVectorListener : public igdeEditDVectorListener{
protected:
	reWPView &pPanel;
	
public:
	cBaseEditDVectorListener( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnDVectorChanged( igdeEditDVector *editDVector ){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnChanged( editDVector->GetDVector(), *rig );
		}
	}
	
	virtual void OnChanged( const decDVector &vector, reRig &rig ) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	reWPView &pPanel;
	
public:
	cBaseEditPathListener( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		reRig * const rig = pPanel.GetRig();
		if( rig ){
			OnChanged( editPath->GetPath(), *rig );
		}
	}
	
	virtual void OnChanged( const decString &path, reRig &rig ) = 0;
};



class cEditModelPath : public cBaseEditPathListener{
public:
	cEditModelPath( reWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual void OnChanged( const decString &path, reRig &rig ){
		rig.SetModelPath( path );
	}
};

class cEditSkinPath : public cBaseEditPathListener{
public:
	cEditSkinPath( reWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual void OnChanged( const decString &path, reRig &rig ){
		rig.SetSkinPath( path );
	}
};

class cEditAnimationPath : public cBaseEditPathListener{
public:
	cEditAnimationPath( reWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual void OnChanged( const decString &path, reRig &rig ){
		rig.SetAnimationPath( path );
	}
};


class cComboTexture : public igdeComboBoxListener{
	reWPView &pPanel;
	
public:
	cComboTexture( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox* ){
		pPanel.UpdateTexture();
	}
};

class cEditTextureSkinPath : public cBaseEditPathListener{
public:
	cEditTextureSkinPath( reWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual void OnChanged( const decString &path, reRig &rig ){
		reRigTexture * const texture = pPanel.GetSelectedTexture();
		if( texture ){
			texture->SetSkinPath( path );
			rig.UpdateComponentTextures();
		}
	}
};


class cComboAnimationMove : public cBaseComboBoxListener{
public:
	cComboAnimationMove( reWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox, reRig &rig ){
		rig.SetMoveName( comboBox->GetText() );
	}
};

class cSliderMoveTime : public cBaseSliderListener{
public:
	cSliderMoveTime( reWPView &panel ) : cBaseSliderListener( panel ){ }
	
	virtual void OnChanging( float value, reRig &rig ){
		rig.SetMoveTime( value );
	}
	
	virtual void OnChanged( float value, reRig &rig ){
		rig.SetMoveTime( value );
	}
};

class cCheckUseRestPose : public cBaseAction{
public:
	cCheckUseRestPose( reWPView &panel ) : cBaseAction( panel,
		"Use rest pose", "Displays the rest pose instead of the animation frame" ){ }
	
	virtual void OnAction( reRig &rig ){
		rig.SetUseRestPose( ! rig.GetUseRestPose() );
	}
};

class cCheckPlaybackMove : public cBaseAction{
public:
	cCheckPlaybackMove( reWPView &panel ) : cBaseAction( panel,
		"Playback", "Determines if the animation move is played back" ){ }
	
	virtual void OnAction( reRig &rig ){
		rig.SetPlaybackMove( ! rig.GetPlaybackMove() );
	}
};


class cEditGravity : public cBaseEditVectorListener{
public:
	cEditGravity( reWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnChanged( const decVector &vector, reRig &rig ){
		rig.SetGravity( vector );
	}
};

class cEditLocalGravity : public cBaseEditVectorListener{
public:
	cEditLocalGravity( reWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnChanged( const decVector &vector, reRig &rig ){
		rig.SetLocalGravity( vector );
	}
};

class cSliderSlowMotion : public cBaseSliderListener{
public:
	cSliderSlowMotion( reWPView &panel ) : cBaseSliderListener( panel ){ }
	
	virtual void OnChanging( float value, reRig &rig ){
		rig.SetSlowmotion( value );
	}
	
	virtual void OnChanged( float value, reRig &rig ){
		rig.SetSlowmotion( value );
	}
};


class cTextGridSize : public igdeTextFieldListener{
	reWPView &pPanel;
public:
	cTextGridSize( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField ){
		pPanel.GetWindowProperties().GetWindowMain().GetConfiguration()
			.SetGridSize( textField->GetFloat() );
		if( pPanel.GetRig() ){
			pPanel.GetRig()->NotifyViewChanged();
		}
	}
};

class cCheckSnapToGrid : public cBaseAction{
public:
	cCheckSnapToGrid( reWPView &panel ) : cBaseAction( panel, "Snap", "Snap to grid" ){ }
	
	virtual void OnAction( reRig &rig ){
		reConfiguration &config = pPanel.GetWindowProperties().GetWindowMain().GetConfiguration();
		config.SetSnapToGrid( ! config.GetSnapToGrid() );
		rig.NotifyViewChanged();
	}
};

class cTextSensitivity : public igdeTextFieldListener{
	reWPView &pPanel;
public:
	cTextSensitivity( reWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField ){
		pPanel.GetWindowProperties().GetWindowMain().GetConfiguration()
			.SetSensitivity( textField->GetFloat() );
		if( pPanel.GetRig() ){
			pPanel.GetRig()->NotifyViewChanged();
		}
	}
};


class cEditCameraPosition : public cBaseEditDVectorListener{
public:
	cEditCameraPosition( reWPView &panel ) : cBaseEditDVectorListener( panel ){ }
	
	virtual void OnChanged( const decDVector &vector, reRig &rig ){
		rig.GetCamera()->SetFreePosition( vector );
	}
};

class cEditCameraRotation : public cBaseEditVectorListener{
public:
	cEditCameraRotation( reWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnChanged( const decVector &vector, reRig &rig ){
		rig.GetCamera()->SetFreeOrientation( vector );
	}
};

class cTextCameraFov : public cBaseTextFieldListener{
public:
	cTextCameraFov( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetFov( textField->GetFloat() );
	}
};

class cTextCameraFovRatio : public cBaseTextFieldListener{
public:
	cTextCameraFovRatio( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetFovRatio( textField->GetFloat() );
	}
};

class cTextCameraImageDistance : public cBaseTextFieldListener{
public:
	cTextCameraImageDistance( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetImageDistance( textField->GetFloat() );
	}
};

class cTextCameraViewDistance : public cBaseTextFieldListener{
public:
	cTextCameraViewDistance( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetViewDistance( textField->GetFloat() );
	}
};

class cTextCameraExposure : public cBaseTextFieldListener{
public:
	cTextCameraExposure( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetExposure( textField->GetFloat() );
	}
};

class cTextCameraAdaptionTime : public cBaseTextFieldListener{
public:
	cTextCameraAdaptionTime( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetAdaptionTime( textField->GetFloat() );
	}
};

class cTextCameraLowIntensity : public cBaseTextFieldListener{
public:
	cTextCameraLowIntensity( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetLowestIntensity( textField->GetFloat() );
	}
};

class cTextCameraHighIntensity : public cBaseTextFieldListener{
public:
	cTextCameraHighIntensity( reWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, reRig &rig ){
		rig.GetCamera()->SetHighestIntensity( textField->GetFloat() );
	}
};


class cCheckCameraAttach : public cBaseAction{
public:
	cCheckCameraAttach( reWPView &panel ) : cBaseAction( panel, "Attach camera to a bone",
		"Attaches the camera to a bone instead of roaming around freely" ){ }
	
	virtual void OnAction( reRig &rig ){
		reCamera &camera = *rig.GetCamera();
		camera.SetAttachToBone( ! camera.GetAttachToBone() );
	}
};

class cComboCameraBone : public cBaseComboBoxListener{
public:
	cComboCameraBone( reWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox, reRig &rig ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		reRigBone *bone = NULL;
		if( selection ){
			bone = ( reRigBone* )selection->GetData();
		}
		rig.GetCamera()->SetBone( bone );
	}
};

class cEditCameraRelativePosition : public cBaseEditVectorListener{
public:
	cEditCameraRelativePosition( reWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnChanged( const decVector &vector, reRig &rig ){
		rig.GetCamera()->SetRelativePosition( vector );
	}
};

class cEditCameraRelativeRotation : public cBaseEditVectorListener{
public:
	cEditCameraRelativeRotation( reWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual void OnChanged( const decVector &vector, reRig &rig ){
		rig.GetCamera()->SetRelativeOrientation( vector );
	}
};


class cActionSkyChanged : public cBaseAction{
public:
	cActionSkyChanged( reWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( reRig &rig ){
		rig.NotifySkyChanged();
	}
};

class cActionEnvObjChanged : public cBaseAction{
public:
	cActionEnvObjChanged( reWPView &panel ) : cBaseAction( panel, "", "" ){ }
	
	virtual void OnAction( reRig &rig ){
		rig.NotifyEnvObjectChanged();
	}
};

}



// Class reWPView
///////////////////

// Constructor, destructor
////////////////////////////

reWPView::reWPView( reWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pRig( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, groupBox2, frameLine, form;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeActionReference action;
	
	pListener = new reWPViewListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// resources
	helper.GroupBoxFlow( content, groupBox, "Resources:" );
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	
	helper.EditPath( form, "Model:", "Path to the model resource to use.",
		igdeEnvironment::efpltModel, pEditModelPath, new cEditModelPath( *this ) );
	helper.EditPath( form, "Skin:", "Path to the skin resource to use.",
		igdeEnvironment::efpltSkin, pEditSkinPath, new cEditSkinPath( *this ) );
	helper.EditPath( form, "Animation:", "Path to the animation resource to use.",
		igdeEnvironment::efpltAnimation, pEditAnimPath, new cEditAnimationPath( *this ) );
	
		// textures
		helper.GroupBox( groupBox, groupBox2, "Textures:" );
		
		helper.ComboBox( groupBox2, "Texture:", "Model texture to edit.",
			pCBTexture, new cComboTexture( *this ) );
		pCBTexture->SetDefaultSorter();
		
		helper.EditPath( groupBox2, "Skin:", "Skin to use for the texture.",
			igdeEnvironment::efpltSkin, pEditTexSkin, new cEditTextureSkinPath( *this ) );
	
	// animation move
	helper.GroupBox( content, groupBox, "Animation Move:" );
	
	helper.ComboBox( groupBox, "Move:", true, "Name of the animation move to use.",
		pCBAnimMoves, new cComboAnimationMove( *this ) );
	pCBAnimMoves->SetDefaultSorter();
	
	helper.EditSliderText( groupBox, "Time:", "Move time to display.",
		0.0f, 0.0f, 5, 2, 0.5f, pSldMoveTime, new cSliderMoveTime( *this ) );
	
	helper.CheckBox( groupBox, pChkUseRestPose, new cCheckUseRestPose( *this ), true );
	helper.CheckBox( groupBox, pChkPlaybackMove, new cCheckPlaybackMove( *this ), true );
	
	// physics
	helper.GroupBox( content, groupBox, "Physics:" );
	
	helper.EditVector( groupBox, "Gravity:", "Gravity magnitude and direction of the world.",
		pEditGravity, new cEditGravity( *this ) );
	helper.EditVector( groupBox, "Local Gravity:", "Magnitude and direction of the object local gravity.",
		pEditLocalGravity, new cEditLocalGravity( *this ) );
	helper.EditSliderText( groupBox, "Slowmotion:", "Sets the slowmotion factor.",
		0.0f, 1.0f, 5, 2, 0.1f, pSldSlowmotion, new cSliderSlowMotion( *this ) );
	pSldSlowmotion->SetValue( 1.0f );
	
	// editing
	helper.GroupBox( content, groupBox, "Editing:" );
	
	helper.FormLineStretchFirst( groupBox, "Grid:", "Snap to grid", frameLine );
	helper.EditFloat( frameLine, "Grid spacing in meters", pEditGridSize, new cTextGridSize( *this ) );
	helper.CheckBox( frameLine, pChkSnapToGrid, new cCheckSnapToGrid( *this ), true );
	
	helper.EditFloat( groupBox, "Sensitivity:",
		"Sets the sensivity of the mouse while moving but not rotation.",
		pEditSensitivity, new cTextSensitivity( *this ) );
	
	// property panels
	action.TakeOver( new cActionSkyChanged( *this ) );
	helper.WPSky( content, pWPSky, action, "Sky:", false, true );
	
	action.TakeOver( new cActionEnvObjChanged( *this ) );
	helper.WPWObject( content, pWPEnvObject, action, "Environment Object:", false, true );
	
	// camera
	helper.GroupBox( content, groupBox, "Camera:", true );
	
	helper.EditDVector( groupBox, "Position:", "Position of the camera.",
		pEditCamPosition, new cEditCameraPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Rotation of the camera.",
		pEditCamRotation, new cEditCameraRotation( *this ) );
	
	helper.EditVector( groupBox, "View:", "View direction of the camera.", pEditCamViewDir, NULL );
	pEditCamViewDir->SetEditable( false );
	
	helper.FormLine( groupBox, "FoV:", "Field of view", frameLine );
	helper.EditFloat( frameLine, "Field of view in degrees", pEditCamFov,
		new cTextCameraFov( *this ) );
	helper.EditFloat( frameLine, "Field of view ratio (height / width)", pEditCamFovRatio,
		new cTextCameraFovRatio( *this ) );
	
	helper.FormLine( groupBox, "Distance:", "Distances", frameLine );
	helper.EditFloat( frameLine, "Image distance (near clipping plane)", pEditCamImageDist,
		new cTextCameraImageDistance( *this ) );
	helper.EditFloat( frameLine, "View distance (far clipping plane", pEditCamViewDist,
		new cTextCameraViewDistance( *this ) );
	
	helper.FormLine( groupBox, "Exposure:",
		"Exposure multiplier and adaption time", frameLine );
	helper.EditFloat( frameLine, "Exposure multiplier", pEditCamExposure,
		new cTextCameraExposure( *this ) );
	helper.EditFloat( frameLine, "Adaption time in seconds", pEditCamAdaptTime,
		new cTextCameraAdaptionTime( *this ) );
	
	helper.FormLine( groupBox, "Adaption:",
		"Exposure multiplier and adaption time", frameLine );
	helper.EditFloat( frameLine, "Lower intensity to adapt to.", pEditCamLowInt,
		new cTextCameraLowIntensity( *this ) );
	helper.EditFloat( frameLine, "Higher intensity to adapt to", pEditCamHiInt,
		new cTextCameraHighIntensity( *this ) );
	
	// camera attaching
	helper.GroupBox( content, groupBox, "Camera Attaching:", true );
	
	helper.CheckBox( groupBox, pChkCamAttach, new cCheckCameraAttach( *this ), true );
	
	helper.ComboBox( groupBox, "Bone:", true, "Bone the camera is attached to.",
		pCBCamBone, new cComboCameraBone( *this ) );
	pCBCamBone->SetDefaultSorter();
	
	helper.EditVector( groupBox, "Position:",
		"Camera position relative to the attachment bone coordinate system.",
		pEditCamRelPosition, new cEditCameraRelativePosition( *this ) );
	helper.EditVector( groupBox, "Rotation:",
		"Camera rotation relative to the attachment bone coordinate system.",
		pEditCamRelRotation, new cEditCameraRelativeRotation( *this ) );
}

reWPView::~reWPView(){
	SetRig( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void reWPView::SetRig( reRig *rig ){
	if( rig == pRig ){
		return;
	}
	
	pWPEnvObject->SetObject( NULL );
	pWPSky->SetSky( NULL );
	
	if( pRig ){
		pRig->RemoveNotifier( pListener );
		pRig->FreeReference();
		pRig = NULL;
	}
	
	pRig = rig;
	
	decString defaultPath;
	
	if( rig ){
		rig->AddNotifier( pListener );
		rig->AddReference();
		
		pWPSky->SetSky( rig->GetSky() );
		pWPEnvObject->SetObject( rig->GetEnvObject() );
		
		defaultPath = rig->GetDirectoryPath();
	}
	
	pEditModelPath->SetDefaultPath( defaultPath );
	pEditSkinPath->SetDefaultPath( defaultPath );
	pEditAnimPath->SetDefaultPath( defaultPath );
	pEditTexSkin->SetDefaultPath( defaultPath );
	
	UpdateBoneList();
	UpdateMoveList();
	UpdateResources();
	UpdateCamera();
	UpdateCameraView();
	UpdateSky();
	UpdateEnvObject();
	UpdateView();
}



void reWPView::UpdateResources(){
	if( pRig ){
		pEditModelPath->SetPath( pRig->GetModelPath() );
		pEditSkinPath->SetPath( pRig->GetSkinPath() );
		pEditAnimPath->SetPath( pRig->GetAnimationPath() );
		
	}else{
		pEditModelPath->ClearPath();
		pEditSkinPath->ClearPath();
		pEditAnimPath->ClearPath();
	}
	
	const bool enabled = pRig != NULL;
	pEditModelPath->SetEnabled( enabled );
	pEditSkinPath->SetEnabled( enabled );
	pEditAnimPath->SetEnabled( enabled );
	
	UpdateTextureList();
	UpdateTexture();
}

void reWPView::UpdateCamera(){
	reCamera * const camera = pRig ? pRig->GetCamera() : NULL;
	const bool enableAttach = camera ? camera->GetAttachToBone() : false;
	
	if( camera ){
		pEditCamPosition->SetDVector( camera->GetFreePosition() );
		pEditCamRotation->SetVector( camera->GetFreeOrientation() );
		pEditCamFov->SetFloat( camera->GetFov() );
		pEditCamFovRatio->SetFloat( camera->GetFovRatio() );
		pEditCamImageDist->SetFloat( camera->GetImageDistance() );
		pEditCamViewDist->SetFloat( camera->GetViewDistance() );
		pEditCamExposure->SetFloat( camera->GetExposure() );
		pEditCamAdaptTime->SetFloat( camera->GetAdaptionTime() );
		pEditCamLowInt->SetFloat( camera->GetLowestIntensity() );
		pEditCamHiInt->SetFloat( camera->GetHighestIntensity() );
		
		pChkCamAttach->SetChecked( camera->GetAttachToBone() );
		pCBCamBone->SetText( camera->GetBone() ? camera->GetBone()->GetName() : decString() );
		pEditCamRelPosition->SetVector( camera->GetRelativePosition() );
		pEditCamRelRotation->SetVector( camera->GetRelativeOrientation() );
		
	}else{
		pEditCamPosition->SetDVector( decDVector() );
		pEditCamRotation->SetVector( decVector() );
		pEditCamFov->ClearText();
		pEditCamFovRatio->ClearText();
		pEditCamImageDist->ClearText();
		pEditCamViewDist->ClearText();
		pEditCamExposure->ClearText();
		pEditCamAdaptTime->ClearText();
		pEditCamLowInt->ClearText();
		pEditCamHiInt->ClearText();
		
		pChkCamAttach->SetChecked( false );
		pCBCamBone->SetText( "" );
		pEditCamRelPosition->SetVector( decVector() );
		pEditCamRelRotation->SetVector( decVector() );
	}
	
	const bool enabled = camera != NULL;
	
	pEditCamPosition->SetEnabled( ! enableAttach );
	pEditCamRotation->SetEnabled( ! enableAttach );
	pEditCamFov->SetEnabled( enabled );
	pEditCamFovRatio->SetEnabled( enabled );
	pEditCamImageDist->SetEnabled( enabled );
	pEditCamViewDist->SetEnabled( enabled );
	pEditCamExposure->SetEnabled( enabled );
	pEditCamAdaptTime->SetEnabled( enabled );
	pEditCamLowInt->SetEnabled( enabled );
	pEditCamHiInt->SetEnabled( enabled );
	
	pChkCamAttach->SetEnabled( enabled );
	pCBCamBone->SetEnabled( enableAttach );
	pEditCamRelPosition->SetEnabled( enableAttach );
	pEditCamRelRotation->SetEnabled( enableAttach );
}

void reWPView::UpdateCameraView(){
	reCamera * const camera = pRig ? pRig->GetCamera() : NULL;
	
	if( camera ){
		pEditCamViewDir->SetVector( camera->GetViewMatrix().TransformView() );
		
	}else{
		pEditCamViewDir->SetVector( decVector() );
	}
	
	pEditCamViewDir->SetEnabled( camera != NULL );
}

void reWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void reWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void reWPView::UpdateView(){
	if( pRig ){
		pCBAnimMoves->SetText( pRig->GetMoveName() );
		
		pSldMoveTime->SetRange( 0.0f, pRig->GetPlayTime() );
		pSldMoveTime->SetValue( pRig->GetMoveTime() );
		
		pChkUseRestPose->SetChecked( pRig->GetUseRestPose() );
		pChkPlaybackMove->SetChecked( pRig->GetPlaybackMove() );
		
		// physics
		pEditGravity->SetVector( pRig->GetGravity() );
		pEditLocalGravity->SetVector( pRig->GetLocalGravity() );
		
		pSldSlowmotion->SetValue( pRig->GetSlowmotion() );
		
	}else{
		pCBAnimMoves->SetText( "" );
		
		pSldMoveTime->SetRange( 0.0f, 0.0f );
		pSldMoveTime->SetValue( 0.0f );
		
		pChkUseRestPose->SetChecked( false );
		pChkPlaybackMove->SetChecked( false );
		
		pEditGravity->SetVector( decVector() );
		pEditLocalGravity->SetVector( decVector() );
	}
	
	const bool enabled = pRig != NULL;
	
	pCBAnimMoves->SetEnabled( enabled );
	pSldMoveTime->SetEnabled( enabled );
	pChkUseRestPose->SetEnabled( enabled );
	pChkPlaybackMove->SetEnabled( enabled );
	pEditGravity->SetEnabled( enabled );
	pEditLocalGravity->SetEnabled( enabled );
	
	// editing
	const reConfiguration &configuration = pWindowProperties.GetWindowMain().GetConfiguration();
	pChkSnapToGrid->SetChecked( configuration.GetSnapToGrid() );
	pEditGridSize->SetFloat( configuration.GetGridSize() );
	pEditSensitivity->SetFloat( configuration.GetSensitivity() );
}

void reWPView::UpdateMoveList(){
	const deAnimator * const engAnimator = pRig ? pRig->GetEngineAnimator() : NULL;
	const deAnimation * const animation = engAnimator ? engAnimator->GetAnimation() : NULL;
	
	pCBAnimMoves->RemoveAllItems();
	
	if( animation ){
		const int count = animation->GetMoveCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pCBAnimMoves->AddItem( animation->GetMove( i )->GetName() );
		}
	}
	
	pCBAnimMoves->SortItems();
}

void reWPView::UpdateBoneList(){
	pCBCamBone->RemoveAllItems();
	
	if( pRig ){
		const int count = pRig->GetBoneCount();
		int i;
		
		for( i=0; i<count; i++ ){
			reRigBone * const bone = pRig->GetBoneAt( i );
			pCBCamBone->AddItem( bone->GetName(), NULL, bone );
		}
	}
	
	pCBCamBone->SortItems();
}

void reWPView::UpdateTextureList(){
	const decString oldSelection( pCBTexture->GetText() );
	
	pCBTexture->RemoveAllItems();
	
	if( pRig ){
		const int count = pRig->GetComponentTextureCount();
		int i;
		
		for( i=0; i<count; i++ ){
			reRigTexture * const texture = pRig->GetComponentTextureAt( i );
			pCBTexture->AddItem( texture->GetName(), NULL, texture );
		}
	}
	
	pCBTexture->SortItems();
	pCBTexture->SetText( oldSelection );
}

void reWPView::UpdateTexture(){
	reRigTexture * const texture = GetSelectedTexture();
	
	if( texture ){
		pEditTexSkin->SetPath( texture->GetSkinPath() );
		
	}else{
		pEditTexSkin->ClearPath();
	}
	
	pEditTexSkin->SetEnabled( texture );
}



reRigTexture* reWPView::GetSelectedTexture() const{
	if( ! pRig ){
		return NULL;
	}
	
	const igdeListItem * const selection = pCBTexture->GetSelectedItem();
	if( ! selection ){
		return NULL;
	}
	
	return ( reRigTexture* )selection->GetData();
}
