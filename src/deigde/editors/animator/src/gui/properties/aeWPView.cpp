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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aeWPView.h"
#include "aeWPViewListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeCamera.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/attachment/aeAttachment.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/wakeboard/aeWakeboard.h"
#include "../../animator/aeSubAnimator.h"
#include "../../configuration/aeConfiguration.h"
#include "../../loadsave/aeLoadSaveSystem.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPCamera.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPView &pPanel;
	
public:
	cBaseAction( aeWPView &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			OnAction( animator );
		}
	}
	
	virtual void OnAction( aeAnimator *animator ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			Update( *animator );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseTextField : public igdeTextFieldListener{
protected:
	aeWPView &pPanel;
	
public:
	cBaseTextField( aeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			OnChanged( textField, animator );
		}
	}
	
	virtual void OnChanged( igdeTextField *textField, aeAnimator *animator ) = 0;
};

class cBaseComboBox : public igdeComboBoxListener{
protected:
	aeWPView &pPanel;
	
public:
	cBaseComboBox( aeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			OnChanged( comboBox, animator );
		}
	}
	
	virtual void OnChanged( igdeComboBox *comboBox, aeAnimator *animator ) = 0;
};

class cBaseEditPath : public igdeEditPathListener{
protected:
	aeWPView &pPanel;
	
public:
	cBaseEditPath( aeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			OnChanged( editPath, animator );
		}
	}
	
	virtual void OnChanged( igdeEditPath *editPath, aeAnimator *animator ) = 0;
};



class cPathDisplayModel : public cBaseEditPath{
public:
	cPathDisplayModel( aeWPView &panel ) : cBaseEditPath( panel ){ }
	
	virtual void OnChanged( igdeEditPath *editPath, aeAnimator *animator ){
		animator->SetDisplayModelPath( editPath->GetPath() );
	}
};

class cPathDisplaySkin : public cBaseEditPath{
public:
	cPathDisplaySkin( aeWPView &panel ) : cBaseEditPath( panel ){ }
	
	virtual void OnChanged( igdeEditPath *editPath, aeAnimator *animator ){
		animator->SetDisplaySkinPath( editPath->GetPath() );
	}
};

class cPathDisplayRig : public cBaseEditPath{
public:
	cPathDisplayRig( aeWPView &panel ) : cBaseEditPath( panel ){ }
	
	virtual void OnChanged( igdeEditPath *editPath, aeAnimator *animator ){
		animator->SetDisplayRigPath( editPath->GetPath() );
	}
};


class cPathTestingAnimator : public cBaseEditPath{
public:
	cPathTestingAnimator( aeWPView &panel ) : cBaseEditPath( panel ){ }
	
	virtual void OnChanged( igdeEditPath *editPath, aeAnimator *animator ){
		if( animator->GetTestingSubAnimator()->GetPathAnimator().Equals( editPath->GetPath() ) ){
			return;
		}
		
		animator->GetTestingSubAnimator()->SetPathAnimator( editPath->GetPath() );
		animator->GetTestingSubAnimator()->LoadAnimator(
			pPanel.GetWindowProperties().GetWindowMain().GetLoadSaveSystem() );
		animator->NotifyViewChanged();
	}
};

class cActionResetState : public cBaseAction{
public:
	cActionResetState( aeWPView &panel ) : cBaseAction( panel, "Reset Animation State", NULL,
		"Animation state is reset before each frame update for testing relative animators" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		animator->SetResetState( ! animator->GetResetState() );
	}
	
	virtual void Update( const aeAnimator &animator ){
		SetEnabled( true );
		SetSelected( animator.GetResetState() );
	}
};


class cEditSky : public cBaseAction{
public:
	cEditSky( aeWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		animator->NotifySkyChanged();
	}
};

class cEditEnvObject : public cBaseAction{
public:
	cEditEnvObject( aeWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		animator->NotifyEnvObjectChanged();
	}
};

class cEditCamera : public cBaseAction{
public:
	cEditCamera( aeWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		animator->NotifyCameraChanged();
	}
};


class cActionCamAttach : public cBaseAction{
public:
	cActionCamAttach( aeWPView &panel ) : cBaseAction( panel, "Attach camera to a bone", NULL,
		"Attaches the camera to a bone instead of roaming around freely" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		animator->GetCamera()->SetAttachToBone( ! animator->GetCamera()->GetAttachToBone() );
	}
	
	virtual void Update( const aeAnimator &animator ){
		SetEnabled( true );
		SetSelected( animator.GetCamera()->GetAttachToBone() );
	}
};

class cComboCamAttachBone : public cBaseComboBox{
public:
	cComboCamAttachBone( aeWPView &panel ) : cBaseComboBox( panel ){ }
	
	virtual void OnChanged( igdeComboBox *comboBox, aeAnimator *animator ){
		animator->GetCamera()->SetBone( comboBox->GetText() );
	}
};

class cEditCamRelPos : public igdeEditVectorListener{
	aeWPView &pPanel;
public:
	cEditCamRelPos( aeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			animator->GetCamera()->SetRelativePosition( editVector->GetVector() );
		}
	}
};

class cEditCamRelRot : public igdeEditVectorListener{
	aeWPView &pPanel;
public:
	cEditCamRelRot( aeWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( animator ){
			animator->GetCamera()->SetRelativeOrientation( editVector->GetVector() );
		}
	}
};


class cTextPlaySpeed : public cBaseTextField{
public:
	cTextPlaySpeed( aeWPView &panel ) : cBaseTextField( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, aeAnimator *animator ){
		animator->SetPlaySpeed( textField->GetFloat() );
	}
};

class cTextTimeStep : public cBaseTextField{
public:
	cTextTimeStep( aeWPView &panel ) : cBaseTextField( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, aeAnimator *animator ){
		animator->SetTimeStep( textField->GetFloat() );
	}
};

class cActionFrameStep : public cBaseAction{
	const float pFactor;
public:
	cActionFrameStep( aeWPView &panel, igdeIcon *icon, const char *description, float factor ) :
		cBaseAction( panel, "", icon, description ), pFactor( factor ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		const aeControllerList &list = animator->GetControllers();
		const float timeStep = animator->GetTimeStep() * pFactor;
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			list.GetAt( i )->UpdateValue( timeStep );
		}
	}
};

class cActionFramePrev : public cActionFrameStep{
public:
	cActionFramePrev( aeWPView &panel ) : cActionFrameStep( panel,
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiLeft ),
		"Step animation backward by the given time step", -1.0f ){ }
};

class cActionFrameNext : public cActionFrameStep{
public:
	cActionFrameNext( aeWPView &panel ) : cActionFrameStep( panel,
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiRight ),
		"Step animation forward by the given time step", 1.0f ){ }
};

class cActionPaused : public cBaseAction{
public:
	cActionPaused( aeWPView &panel ) : cBaseAction( panel, "Pause", NULL, "Paus animation playback" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		animator->SetPaused( ! animator->GetPaused() );
	}
	
	virtual void Update( const aeAnimator &animator ){
		SetEnabled( true );
		SetSelected( animator.GetPaused() );
	}
};

class cActionReset : public cBaseAction{
public:
	cActionReset( aeWPView &panel ) : cBaseAction( panel, "Reset", NULL, "Reset animation" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		const aeControllerList &list = animator->GetControllers();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			list.GetAt( i )->ResetValue();
		}
	}
};


class cComboAttachment : public cBaseComboBox{
public:
	cComboAttachment( aeWPView &panel ) : cBaseComboBox( panel ){ }
	
	virtual void OnChanged( igdeComboBox *comboBox, aeAnimator *animator ){
		animator->SetActiveAttachment( comboBox->GetSelectedItem()
			? ( aeAttachment* )comboBox->GetSelectedItem()->GetData() : NULL );
	}
};

class cActionAttachmentAdd : public cBaseAction{
public:
	cActionAttachmentAdd( aeWPView &panel ) : cBaseAction( panel, "Add",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add attachment" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		const decString baseName( "Attachment" );
		decString name( baseName );
		int number = 1;
		while( animator->HasAttachmentNamed( name ) ){
			name.Format( "%s #%d", baseName.GetString(), number );
		}
		
		deObjectReference attachment;
		attachment.TakeOver( new aeAttachment( &pPanel.GetEnvironment(), name ) );
		animator->AddAttachment( ( aeAttachment* )( deObject* )attachment );
		animator->SetActiveAttachment( ( aeAttachment* )( deObject* )attachment );
	}
};

class cActionAttachmentRemove : public cBaseAction{
public:
	cActionAttachmentRemove( aeWPView &panel ) : cBaseAction( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove attachment" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		aeAttachment * const attachment = pPanel.GetAttachment();
		if( ! attachment ){
			return;
		}
		
		animator->RemoveAttachment( attachment );
		if( animator->GetAttachmentCount() > 0 ){
			animator->SetActiveAttachment( animator->GetAttachmentAt( 0 ) );
		}
	}
	
	virtual void Update( const aeAnimator & ){
		SetEnabled( pPanel.GetAttachment() );
	}
};

class cActionAttachmentClear : public cBaseAction{
public:
	cActionAttachmentClear( aeWPView &panel ) : cBaseAction( panel, "Clear",
		NULL, "Remove all attachment" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		if( animator->GetAttachmentCount() > 0 ){
			animator->RemoveAllAttachments();
		}
	}
	
	virtual void Update( const aeAnimator &animator ){
		SetEnabled( animator.GetAttachmentCount() > 0 );
	}
};

class cActionAttachmentLoadConfig : public cBaseAction{
public:
	cActionAttachmentLoadConfig( aeWPView &panel ) : cBaseAction( panel, "Load configuration...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Load configuration" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		aeLoadSaveSystem &lssys = pPanel.GetWindowProperties().GetWindowMain().GetLoadSaveSystem();
		decString filename( animator->GetPathAttachmentConfig() );
		if( ! igdeCommonDialogs::GetFileOpen( &pPanel, "Open Attachment Configuration",
		*pPanel.GetEnvironment().GetFileSystemGame(), lssys.GetAttConfigFilePatterns(), filename ) ){
			return;
		}
		
		animator->SetPathAttachmentConfig( filename );
		lssys.LoadAttConfig( filename, *animator );
	}
};

class cActionAttachmentSaveConfig : public cBaseAction{
public:
	cActionAttachmentSaveConfig( aeWPView &panel ) : cBaseAction( panel, "Save configuration...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ), "Save configuration" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		aeLoadSaveSystem &lssys = pPanel.GetWindowProperties().GetWindowMain().GetLoadSaveSystem();
		decString filename( animator->GetPathAttachmentConfig() );
		if( ! igdeCommonDialogs::GetFileSave( &pPanel, "Save Attachment Configuration",
		*pPanel.GetEnvironment().GetFileSystemGame(), lssys.GetAttConfigFilePatterns(), filename ) ){
			return;
		}
		
		animator->SetPathAttachmentConfig( filename );
		lssys.SaveAttConfig( filename, *animator );
	}
};

class cActionMenuAttach : public igdeActionContextMenu{
	aeWPView &pPanel;
public:
	cActionMenuAttach( aeWPView &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Reset animation" ),
	pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( contextMenu, new cActionAttachmentAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionAttachmentRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionAttachmentClear( pPanel ), true );
		
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionAttachmentLoadConfig( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionAttachmentSaveConfig( pPanel ), true );
	}
};

class cTextAttachmentName : public cBaseTextField{
public:
	cTextAttachmentName( aeWPView &panel ) : cBaseTextField( panel ){ }
	
	virtual void OnChanged( igdeTextField *textField, aeAnimator *animator ){
		aeAttachment * const attachment = pPanel.GetAttachment();
		if( ! attachment || textField->GetText() == attachment->GetName() ){
			return;
		}
		
		if( animator->HasAttachmentNamed( textField->GetText() ) ){
			textField->SetText( attachment->GetName() );
			
		}else{
			attachment->SetName( textField->GetText() );
		}
	}
};

class cComboAttachmentBone : public cBaseComboBox{
public:
	cComboAttachmentBone( aeWPView &panel ) : cBaseComboBox( panel ){ }
	
	virtual void OnChanged( igdeComboBox *comboBox, aeAnimator* ){
		aeAttachment * const attachment = pPanel.GetAttachment();
		if( attachment ){
			attachment->SetBoneName( comboBox->GetText() );
		}
	}
};

class cComboAttachmentType : public cBaseComboBox{
public:
	cComboAttachmentType( aeWPView &panel ) : cBaseComboBox( panel ){ }
	
	virtual void OnChanged( igdeComboBox *comboBox, aeAnimator* ){
		aeAttachment * const attachment = pPanel.GetAttachment();
		if( attachment && comboBox->GetSelectedItem() ){
			attachment->SetAttachType( ( aeAttachment::eAttachTypes )
				( intptr_t )comboBox->GetSelectedItem()->GetData() );
		}
	}
};

class cEditAttachmentObject : public cBaseAction{
public:
	cEditAttachmentObject( aeWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual void OnAction( aeAnimator *animator ){
		if( pPanel.GetAttachment() ){
			animator->NotifyAttachmentChanged( pPanel.GetAttachment() );
		}
	}
};

}



// Class aeWPView
///////////////////

// Constructor, destructor
////////////////////////////

aeWPView::aeWPView( aeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pAnimator( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, formLine;
	
	pListener = new aeWPViewListener( *this );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// controllers
	helper.GroupBox( content, groupBox, "Display File Path:" );
	
	helper.EditPath( groupBox, "Model:", "Sets the model to use", igdeEnvironment::efpltModel,
		pEditDisplayModelPath, new cPathDisplayModel( *this ) );
	helper.EditPath( groupBox, "Skin:", "Sets the skin to use", igdeEnvironment::efpltSkin,
		pEditDisplaySkinPath, new cPathDisplaySkin( *this ) );
	helper.EditPath( groupBox, "Rig:", "Sets the rig to use", igdeEnvironment::efpltRig,
		pEditDisplayRigPath, new cPathDisplayRig( *this ) );
	
	
	// testing animator
	helper.GroupBox( content, groupBox, "Testing Base Animator:", true );
	
	helper.EditPath( groupBox, "Animator:",
		"Path to a testing animator to use as the base state for the editing animator",
		igdeEnvironment::efpltAnimator, pEditTestingAnimatorPath, new cPathTestingAnimator( *this ) );
	
	helper.CheckBox( groupBox, pChkResetState, new cActionResetState( *this ), true );
	
	
	// sky, environment object, camera
	helper.WPSky( content, pWPSky, new cEditSky( *this ), "Sky:", false, true, true );
	helper.WPWObject( content, pWPEnvObject, new cEditEnvObject( *this ), "Environment Object:", false, true, true );
	helper.WPCamera( content, pWPCamera, new cEditCamera( *this ), "Camera:", false, true, true );
	
	
	// camera attaching
	helper.GroupBox( content, groupBox, "Camera Attaching:" );
	
	helper.CheckBox( groupBox, pChkCamAttach, new cActionCamAttach( *this ), true );
	helper.ComboBoxFilter( groupBox, "Bone:", true, "Bone the camera is attached to",
		pCBCamBone, new cComboCamAttachBone( *this ) );
	pCBCamBone->SetDefaultSorter();
	
	helper.EditVector( groupBox, "Position:",
		"Camera position relative to the attachment bone coordinate system",
		pEditCamRelPos, new cEditCamRelPos( *this ) );
	helper.EditVector( groupBox, "Rotation:",
		"Camera rotation relative to the attachment bone coordinate system",
		pEditCamRelRot, new cEditCamRelRot( *this ) );
	
	
	// playback
	helper.GroupBox( content, groupBox, "Playback:" );
	
	helper.EditFloat( groupBox, "Play Speed:", "Sets the playback speed",
		pEditPlaySpeed, new cTextPlaySpeed( *this ) );
	helper.EditFloat( groupBox, "Time Step:", "Provides fine grained animation stepping",
		pEditTimeStep, new cTextTimeStep( *this ) );
	
	helper.FormLine( groupBox, "", "", formLine );
	helper.Button( formLine, pBtnFramePrev, new cActionFramePrev( *this ), true );
	helper.Button( formLine, pBtnPauseAnimation, new cActionPaused( *this ), true );
	helper.Button( formLine, pBtnFrameNext, new cActionFrameNext( *this ), true );
	helper.Button( formLine, pBtnResetAnimation, new cActionReset( *this ), true );
	
	
	// attachments
	helper.GroupBox( content, groupBox, "Attachments:", true );
	
	helper.FormLineStretchFirst( groupBox, "Attachment:", "Attachment to edit", formLine );
	helper.ComboBox( formLine, "Attachment to edit", pCBAttachments, new cComboAttachment( *this ) );
	
	igdeActionContextMenuReference actionMenuAttachment;
	actionMenuAttachment.TakeOver( new cActionMenuAttach( *this ) );
	helper.Button( formLine, pBtnAttMenu, actionMenuAttachment );
	actionMenuAttachment->SetWidget( pBtnAttMenu );
	
	helper.EditString( groupBox, "Name:", "Sets the name of the attachment",
		pEditAttName, new cTextAttachmentName( *this ) );
	
	helper.ComboBoxFilter( groupBox, "Attach Bone:", true, "Bone to attach to instead of the entire rig",
		pCBAttBoneName, new cComboAttachmentBone( *this ) );
	pCBAttBoneName->SetDefaultSorter();
	
	helper.ComboBox( groupBox, "Attach:", "Attach type", pCBAttAttachType, new cComboAttachmentType( *this ) );
	pCBAttAttachType->AddItem( "None", NULL, ( void* )( intptr_t )aeAttachment::eatNone );
	pCBAttAttachType->AddItem( "Bone", NULL, ( void* )( intptr_t )aeAttachment::eatBone );
	pCBAttAttachType->AddItem( "Rig", NULL, ( void* )( intptr_t )aeAttachment::eatRig );
	
	helper.WPWObject( content, pWPAttachment, new cEditAttachmentObject( *this ),
		"Attachment Object:", false, true, true );
}

aeWPView::~aeWPView(){
	SetAnimator( NULL );
	
	if( pAnimator ){
		pAnimator->RemoveNotifier( pListener );
		pAnimator->FreeReference();
		pAnimator = NULL;
	}
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void aeWPView::SetAnimator( aeAnimator *animator ){
	if( animator == pAnimator ){
		return;
	}
	
	pWPSky->SetSky( NULL );
	pWPEnvObject->SetObjectWrapper( NULL );
	pWPCamera->SetCamera( NULL );
	
	if( pAnimator ){
		pAnimator->RemoveNotifier( pListener );
		pAnimator->FreeReference();
	}
	
	pAnimator = animator;
	
	if( animator ){
		animator->AddNotifier( pListener );
		animator->AddReference();
		
		pWPSky->SetSky( animator->GetSky() );
		pWPEnvObject->SetObjectWrapper( animator->GetEnvObject() );
		pWPCamera->SetCamera( animator->GetCamera() );
	}
	
	UpdateBoneList();
	UpdatePath();
	UpdateCamera();
	UpdateCameraView();
	UpdateView();
	UpdateSky();
	UpdateEnvObject();
	UpdatePlayback();
	UpdateAttachmentList();
	OnAnimatorPathChanged();
}

aeAttachment *aeWPView::GetAttachment() const{
	return pAnimator ? pAnimator->GetActiveAttachment() : NULL;
}

void aeWPView::SelectActiveAttachment(){
	pCBAttachments->SetSelectionWithData( GetAttachment() );
}

void aeWPView::UpdateBoneList(){
	const decString cameraBone( pCBCamBone->GetText() );
	const decString attachBone( pCBAttBoneName->GetText() );
	
	pCBCamBone->RemoveAllItems();
	pCBAttBoneName->RemoveAllItems();
	
	if( pAnimator ){
		const deRig * const engRig = pAnimator->GetEngineComponent()
			? pAnimator->GetEngineComponent()->GetRig() : NULL;
		if( engRig ){
			const int count = engRig->GetBoneCount();
			int i;
			
			for( i=0; i<count; i++ ){
				const decString &name = engRig->GetBoneAt( i ).GetName();
				pCBCamBone->AddItem( name );
				pCBAttBoneName->AddItem( name );
			}
		}
		
		pCBCamBone->SortItems();
		pCBAttBoneName->SortItems();
	}
	
	pCBCamBone->StoreFilterItems();
	pCBAttBoneName->StoreFilterItems();
	
	pCBCamBone->SetText( cameraBone );
	pCBAttBoneName->SetText( attachBone );
}

void aeWPView::UpdatePath(){
	if( pAnimator ){
		pEditDisplayModelPath->SetPath( pAnimator->GetDisplayModelPath() );
		pEditDisplaySkinPath->SetPath( pAnimator->GetDisplaySkinPath() );
		pEditDisplayRigPath->SetPath( pAnimator->GetDisplayRigPath() );
		
	}else{
		pEditDisplayModelPath->ClearPath();
		pEditDisplaySkinPath->ClearPath();
		pEditDisplayRigPath->ClearPath();
	}
	
	const bool enabled = pAnimator;
	pEditDisplayModelPath->SetEnabled( enabled );
	pEditDisplaySkinPath->SetEnabled( enabled );
	pEditDisplayRigPath->SetEnabled( enabled );
}

void aeWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void aeWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObjectWrapper();
}

void aeWPView::UpdateCamera(){
	const aeCamera * const camera = pAnimator ? pAnimator->GetCamera() : NULL;
	
	pWPCamera->UpdateCamera();
	
	if( camera ){
		pCBCamBone->SetText( camera->GetBone() );
		pEditCamRelPos->SetVector( camera->GetRelativePosition() );
		pEditCamRelRot->SetVector( camera->GetRelativeOrientation() );
		
	}else{
		pCBCamBone->ClearText();
		pEditCamRelPos->SetVector( decVector() );
		pEditCamRelRot->SetVector( decVector() );
	}
	
	const bool enabled = camera;
	pCBCamBone->SetEnabled( enabled );
	pEditCamRelPos->SetEnabled( enabled );
	pEditCamRelRot->SetEnabled( enabled );
	
	pChkCamAttach->GetAction()->Update();
}

void aeWPView::UpdateCameraView(){
	pWPCamera->UpdateViewDirection();
}

void aeWPView::UpdateView(){
	if( pAnimator ){
		pEditTestingAnimatorPath->SetPath( pAnimator->GetTestingSubAnimator()->GetPathAnimator() );
		
	}else{
		pEditTestingAnimatorPath->ClearPath();
	}
	
	const bool enabled = pAnimator;
	pEditTestingAnimatorPath->SetEnabled( enabled );
	
	pChkResetState->GetAction()->Update();
}

void aeWPView::UpdatePlayback(){
	if( pAnimator ){
		pEditPlaySpeed->SetFloat( pAnimator->GetPlaySpeed() );
		pEditTimeStep->SetFloat( pAnimator->GetTimeStep() );
		
	}else{
		pEditPlaySpeed->ClearText();
		pEditTimeStep->ClearText();
	}
	
	const bool enabled = pAnimator;
	pEditPlaySpeed->SetEnabled( enabled );
	pEditTimeStep->SetEnabled( enabled );
}

void aeWPView::UpdateAttachmentList(){
	aeAttachment * const selection = GetAttachment();
	
	pCBAttachments->RemoveAllItems();
	
	if( pAnimator ){
		const int count = pAnimator->GetAttachmentCount();
		int i;
		
		for( i=0; i<count; i++ ){
			aeAttachment * const attachment = pAnimator->GetAttachmentAt( i );
			pCBAttachments->AddItem( attachment->GetName(), NULL, attachment );
		}
		
		pCBAttachments->SortItems();
	}
	
	pCBAttachments->SetSelectionWithData( selection );
	UpdateAttachment();
}

void aeWPView::UpdateAttachment(){
	const aeAttachment * const attachment = GetAttachment();
	
	if( attachment ){
		pEditAttName->SetText( attachment->GetName() );
		pWPAttachment->SetObjectWrapper( attachment->GetObjectWrapper() );
		pCBAttAttachType->SetSelectionWithData( ( void* )( intptr_t )attachment->GetAttachType() );
		pCBAttBoneName->SetText( attachment->GetBoneName() );
		
	}else{
		pEditAttName->ClearText();
		pWPAttachment->SetObjectWrapper( NULL );
		pCBAttAttachType->SetSelectionWithData( ( void* )( intptr_t )aeAttachment::eatNone );
		pCBAttBoneName->ClearText();
	}
	
	const bool enabled = attachment;
	pEditAttName->SetEnabled( enabled );
	pCBAttAttachType->SetEnabled( enabled );
	pCBAttBoneName->SetEnabled( enabled );
}

void aeWPView::OnAnimatorPathChanged(){
	if( pAnimator ){
		pEditDisplayModelPath->SetBasePath( pAnimator->GetDirectoryPath() );
		pEditDisplaySkinPath->SetBasePath( pAnimator->GetDirectoryPath() );
		pEditDisplayRigPath->SetBasePath( pAnimator->GetDirectoryPath() );
		
	}else{
		pEditDisplayModelPath->SetBasePath( "" );
		pEditDisplaySkinPath->SetBasePath( "" );
		pEditDisplayRigPath->SetBasePath( "" );
	}
}
