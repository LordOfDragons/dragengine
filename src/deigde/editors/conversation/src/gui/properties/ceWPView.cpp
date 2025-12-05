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

#include "ceWPView.h"
#include "ceWPViewListener.h"
#include "ceWindowProperties.h"
#include "../ceWindowMain.h"
#include "../../configuration/ceConfiguration.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/actor/ceConversationActor.h"
#include "../../conversation/actor/parameters/ceActorParameter.h"
#include "../../conversation/actor/controller/ceActorController.h"
#include "../../conversation/actor/pose/ceActorPose.h"
#include "../../conversation/actor/gesture/ceActorGesture.h"
#include "../../conversation/coordsystem/ceCoordSystem.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../conversation/playback/cePlayback.h"
#include "../../conversation/playback/command/cePlaybackCommand.h"
#include "../../conversation/playback/command/cePlaybackCommandList.h"
#include "../../conversation/playback/variable/cePlaybackVariable.h"
#include "../../conversation/playback/variable/cePlaybackVariableList.h"
#include "../../conversation/prop/ceProp.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/browse/igdeDialogBrowserObjectClass.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/properties/igdeWPCamera.h>
#include <deigde/gui/properties/igdeWPSky.h>
#include <deigde/gui/properties/igdeWPWObject.h>
#include <deigde/gui/properties/igdeWPTriggerTable.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	ceWPView &pPanel;
	
public:
	cBaseAction( ceWPView &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetConversation();
		if( ! conversation ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnAction( conversation ) );
		if( undo ){
			conversation->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( ceConversation *conversation ) = 0;
	
	virtual void Update(){
		const ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			Update( *conversation );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseActionContextMenu : public igdeActionContextMenu{
protected:
	ceWPView &pPanel;
	
public:
	cBaseActionContextMenu( ceWPView &panel, const char *description ) :
	igdeActionContextMenu( "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
	description ), pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			AddContextMenuEntries( contextMenu, conversation );
		}
	}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation *conversation ) = 0;
	
	virtual void Update(){
		const ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			Update( *conversation );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	ceWPView &pPanel;
	
public:
	cBaseComboBoxListener( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			igdeUndo::Ref undo;
			undo.TakeOver( OnChanged( *comboBox, conversation ) );
			if( undo ){
				conversation->GetUndoSystem()->Add( undo );
			}
		}
	}
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation *conversation ) = 0;
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	ceWPView &pPanel;
	
public:
	cBaseTextFieldListener( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			igdeUndo::Ref undo;
			undo.TakeOver( OnChanged( *textField, conversation ) );
			if( undo ){
				conversation->GetUndoSystem()->Add( undo );
			}
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation *conversation ) = 0;
};

class cBaseEditVectorListener : public igdeEditVectorListener{
protected:
	ceWPView &pPanel;
	
public:
	cBaseEditVectorListener( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnVectorChanged( igdeEditVector *editVector ){
		ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			igdeUndo::Ref undo;
			undo.TakeOver( OnChanged( *editVector, conversation ) );
			if( undo ){
				conversation->GetUndoSystem()->Add( undo );
			}
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation *conversation ) = 0;
};

class cBaseSpinTextFieldListener : public igdeSpinTextFieldListener{
protected:
	ceWPView &pPanel;
	
public:
	cBaseSpinTextFieldListener( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnValueChanged( igdeSpinTextField *textField ){
		ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			igdeUndo::Ref undo;
			undo.TakeOver( OnChanged( *textField, conversation ) );
			if( undo ){
				conversation->GetUndoSystem()->Add( undo );
			}
		}
	}
	
	virtual igdeUndo *OnChanged( igdeSpinTextField &textField, ceConversation *conversation ) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	ceWPView &pPanel;
	
public:
	cBaseEditPathListener( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			igdeUndo::Ref undo;
			undo.TakeOver( OnChanged( *editPath, conversation ) );
			if( undo ){
				conversation->GetUndoSystem()->Add( undo );
			}
		}
	}
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation *conversation ) = 0;
};



class cActionWPCamera : public cBaseAction{
public:
	cActionWPCamera( ceWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->NotifyCameraChanged();
		return NULL;
	}
};

class cActionWPSky : public cBaseAction{
public:
	cActionWPSky( ceWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->NotifySkyChanged();
		return NULL;
	}
};

class cActionWPEnvObject : public cBaseAction{
public:
	cActionWPEnvObject( ceWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->NotifyEnvObjectChanged();
		return NULL;
	}
};



class cComboProp : public cBaseComboBoxListener{
public:
	cComboProp( ceWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation *conversation ){
		conversation->SetActiveProp( comboBox.GetSelectedItem()
			? ( ceProp* )comboBox.GetSelectedItem()->GetData() : NULL );
		return NULL;
	}
};

class cActionPropAdd : public cBaseAction{
public:
	cActionPropAdd( ceWPView &panel ) : cBaseAction( panel, "Add",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Prop" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		deObjectReference prop;
		prop.TakeOver( new ceProp );
		conversation->AddProp( prop );
		conversation->SetActiveProp( prop );
		return NULL;
	}
};

class cActionPropRemove : public cBaseAction{
public:
	cActionPropRemove( ceWPView &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Prop" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetProp() ){
			conversation->RemoveProp( pPanel.GetProp() );
			if( conversation->GetPropList().GetCount() > 0 ){
				conversation->SetActiveProp( conversation->GetPropList().GetAt( 0 ) );
			}
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetProp() );
	}
};

class cActionPropMenu : public cBaseActionContextMenu{
public:
	cActionPropMenu( ceWPView &panel ) : cBaseActionContextMenu( panel, "Prop menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionPropAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionPropRemove( pPanel ), true );
	}
};

class cTextPropName : public cBaseTextFieldListener{
public:
	cTextPropName( ceWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		if( pPanel.GetProp() ){
			pPanel.GetProp()->SetName( textField.GetText() );
		}
		return NULL;
	}
};

class cTextPropClass : public cBaseTextFieldListener{
public:
	cTextPropClass( ceWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		if( pPanel.GetProp() ){
			pPanel.GetProp()->SetObjectClass( textField.GetText() );
		}
		return NULL;
	}
};

class cActionPropClass : public cBaseAction{
	igdeTextField::Ref &pTextField;
	
public:
	cActionPropClass( ceWPView &panel, igdeTextField::Ref &textField ) : cBaseAction( panel, "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ),
		"Brings up a dialog to select the prop class" ), pTextField( textField ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		if( ! pPanel.GetProp() || ! pPanel.GetGameDefinition() ){
			return NULL;
		}
		
		igdeGDClass *gdClass = pPanel.GetGameDefinition()->GetClassManager()->GetNamed( pPanel.GetProp()->GetObjectClass() );
		if( igdeDialogBrowserObjectClass::SelectObjectClass( &pPanel, gdClass ) ){
			pPanel.GetProp()->SetObjectClass( gdClass->GetName() );
		}
		return NULL;
	}
};

class cActionPropVisible : public cBaseAction{
public:
	cActionPropVisible( ceWPView &panel ) : cBaseAction( panel, "Visible", NULL, "Determines if the prop is visible" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		if( pPanel.GetProp() ){
			pPanel.GetProp()->SetVisible( ! pPanel.GetProp()->GetVisible() );
		}
		return NULL;
	}
};

class cVectorPropPosition : public cBaseEditVectorListener{
public:
	cVectorPropPosition( ceWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		if( pPanel.GetProp() ){
			pPanel.GetProp()->SetPosition( editVector.GetVector() );
		}
		return NULL;
	}
};

class cVectorPropRotation : public cBaseEditVectorListener{
public:
	cVectorPropRotation( ceWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		if( pPanel.GetProp() ){
			pPanel.GetProp()->SetOrientation( editVector.GetVector() );
		}
		return NULL;
	}
};



class cSpinActor : public cBaseSpinTextFieldListener{
public:
	cSpinActor( ceWPView &panel ) : cBaseSpinTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeSpinTextField &textField, ceConversation *conversation ){
		conversation->SetActiveActor( conversation->GetActorList().GetAt( textField.GetValue() ) );
		return NULL;
	}
};

class cActionActorAdd : public cBaseAction{
public:
	cActionActorAdd( ceWPView &panel ) : cBaseAction( panel, "Add",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Actor" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		const ceConversationActor::Ref actor(ceConversationActor::Ref::NewWith(pPanel.GetEnvironment()));
		conversation->AddActor( actor );
		conversation->SetActiveActor( actor );
		return NULL;
	}
};

class cActionActorRemove : public cBaseAction{
public:
	cActionActorRemove( ceWPView &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Actor" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActor() ){
			conversation->RemoveActor( pPanel.GetActor() );
			if( conversation->GetActorList().GetCount() > 0 ){
				conversation->SetActiveActor( conversation->GetActorList().GetAt( 0 ) );
			}
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActor() );
	}
};

class cActionActorMenu : public cBaseActionContextMenu{
public:
	cActionActorMenu( ceWPView &panel ) : cBaseActionContextMenu( panel, "Actor menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionActorAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionActorRemove( pPanel ), true );
	}
};

class cTextActorID : public cBaseTextFieldListener{
public:
	cTextActorID( ceWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetID( textField.GetText() );
		}
		return NULL;
	}
};

class cTextActorAliasID : public cBaseTextFieldListener{
public:
	cTextActorAliasID( ceWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetAliasID( textField.GetText() );
		}
		return NULL;
	}
};

class cVectorActorPosition : public cBaseEditVectorListener{
public:
	cVectorActorPosition( ceWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetPosition( editVector.GetVector() );
		}
		return NULL;
	}
};

class cVectorActorRotation : public cBaseEditVectorListener{
public:
	cVectorActorRotation( ceWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetOrientation( editVector.GetVector() );
		}
		return NULL;
	}
};

class cPathActorModel : public cBaseEditPathListener{
public:
	cPathActorModel( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetPathModel( editPath.GetPath() );
		}
		return NULL;
	}
};

class cPathActorSkin : public cBaseEditPathListener{
public:
	cPathActorSkin( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetPathSkin( editPath.GetPath() );
		}
		return NULL;
	}
};

class cPathActorRig : public cBaseEditPathListener{
public:
	cPathActorRig( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetPathRig( editPath.GetPath() );
		}
		return NULL;
	}
};

class cPathActorSpeechAnimation : public cBaseEditPathListener{
public:
	cPathActorSpeechAnimation( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetPathSpeechAnimation( editPath.GetPath() );
		}
		return NULL;
	}
};

class cPathActorEyeAnimator : public cBaseEditPathListener{
public:
	cPathActorEyeAnimator( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetPathEyesAnimator( editPath.GetPath() );
		}
		return NULL;
	}
};

class cPathActorFacePoseAnimator : public cBaseEditPathListener{
public:
	cPathActorFacePoseAnimator( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetPathFacePoseAnimator( editPath.GetPath() );
		}
		return NULL;
	}
};

class cActionActorWaiting : public cBaseAction{
public:
	cActionActorWaiting( ceWPView &panel ) : cBaseAction( panel, "Waiting", NULL,
	"Simulates the actor game scripts answer to the question if conversation should be waiting" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetWaiting( ! pPanel.GetActor()->GetWaiting() );
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActor() );
		SetSelected( pPanel.GetActor() && pPanel.GetActor()->GetWaiting() );
	}
};

class cTextActorBoneHeadRotator : public cBaseTextFieldListener{
public:
	cTextActorBoneHeadRotator( ceWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		if( pPanel.GetActor() ){
			pPanel.GetActor()->SetBoneHeadRotator( textField.GetText() );
		}
		return NULL;
	}
};


class cComboActorPose : public cBaseComboBoxListener{
public:
	cComboActorPose( ceWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation* ){
		if( pPanel.GetActor() && comboBox.GetSelectedItem() ){
			pPanel.GetActor()->SetActivePose( ( ceActorPose* )comboBox.GetSelectedItem()->GetData() );
		}
		return NULL;
	}
};

class cActionActorPoseAdd : public cBaseAction{
public:
	cActionActorPoseAdd( ceWPView &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Actor Pose" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceConversationActor * const actor = pPanel.GetActor();
		if( ! actor ){
			return NULL;
		}
		
		decString name;
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Pose", "Pose:", name ) ){
			return NULL;
		}
		
		if( actor->GetPoses().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Pose", "A pose with this name exists already." );
			return NULL;
		}
		
		const ceActorPose::Ref pose(ceActorPose::Ref::NewWith(pPanel.GetEnvironment(), name));
		actor->GetPoses().Add( pose );
		conversation->NotifyActorPosesChanged( actor );
		actor->SetActivePose( pose );
		conversation->NotifyActorActivePoseChanged( actor );
		return NULL;
	}
};

class cActionActorPoseRemove : public cBaseAction{
public:
	cActionActorPoseRemove( ceWPView &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Actor Pose" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActorPose() ){
			pPanel.GetActor()->GetPoses().Remove( pPanel.GetActorPose() );
			conversation->NotifyActorPosesChanged( pPanel.GetActor() );
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorPose() );
	}
};

class cActionActorPoseRename : public cBaseAction{
public:
	cActionActorPoseRename( ceWPView &panel ) : cBaseAction( panel, "Rename...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Rename Actor Pose" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceConversationActor * const actor = pPanel.GetActor();
		ceActorPose * const pose = pPanel.GetActorPose();
		if( ! pose || ! actor ){
			return NULL;
		}
		
		decString name( pose->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Pose", "Pose:", name ) || name == pose->GetName() ){
			return NULL;
		}
		
		if( actor->GetPoses().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Pose", "A pose with this name exists already." );
			
		}else{
			pose->SetName( name );
			actor->NotifyPosesChanged();
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorPose() );
	}
};

class cActionActorPoseMenu : public cBaseActionContextMenu{
public:
	cActionActorPoseMenu( ceWPView &panel ) : cBaseActionContextMenu( panel, "ActorPose menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionActorPoseAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionActorPoseRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionActorPoseRename( pPanel ), true );
	}
};

class cPathActorPoseAnimator : public cBaseEditPathListener{
public:
	cPathActorPoseAnimator( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActorPose() ){
			pPanel.GetActorPose()->SetPathAnimator( editPath.GetPath() );
		}
		return NULL;
	}
};


class cComboActorPoseController : public cBaseComboBoxListener{
public:
	cComboActorPoseController( ceWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox&, ceConversation* ){
		pPanel.UpdateActorController();
		return nullptr;
	}
};

class cActionActorPoseControllerAdd : public cBaseAction{
public:
	cActionActorPoseControllerAdd( ceWPView &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add actor pose controller" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceActorPose * const pose = pPanel.GetActorPose();
		if( ! pose ){
			return NULL;
		}
		
		decString name;
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Controller", "Controller:", name, pose->GetControllerNames() ) ){
			return nullptr;
		}
		
		if( pose->GetControllers().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Controller", "A controller with this name exists already." );
			return nullptr;
		}
		
		const ceActorController::Ref controller( ceActorController::Ref::New( new ceActorController ) );
		controller->SetName( name );
		pose->GetControllers().Add( controller );
		conversation->NotifyActorPosesChanged( pPanel.GetActor() );
		pPanel.SelectActorPoseController( controller );
		return nullptr;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorPose() );
	}
};

class cActionActorPoseControllerRemove : public cBaseAction{
public:
	cActionActorPoseControllerRemove( ceWPView &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove actor pose controller" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActorPoseController() ){
			pPanel.GetActorPose()->GetControllers().Remove( pPanel.GetActorPoseController() );
			conversation->NotifyActorPosesChanged( pPanel.GetActor() );
		}
		return nullptr;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorPoseController() );
	}
};

class cActionActorPoseControllerRemoveAll : public cBaseAction{
public:
	cActionActorPoseControllerRemoveAll( ceWPView &panel ) : cBaseAction( panel, "Remove All",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove all actor pose controllers" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActorPose() && pPanel.GetActorPose()->GetControllers().GetCount() > 0 ){
			pPanel.GetActorPose()->GetControllers().RemoveAll();
			conversation->NotifyActorPosesChanged( pPanel.GetActor() );
		}
		return nullptr;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorPose() && pPanel.GetActorPose()->GetControllers().GetCount() > 0 );
	}
};

class cActionActorPoseControllerRename : public cBaseAction{
public:
	cActionActorPoseControllerRename( ceWPView &panel ) : cBaseAction( panel, "Rename...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Rename Actor Pose Controller" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		if( ! pPanel.GetActorPoseController() ){
			return nullptr;
		}
		
		decString name( pPanel.GetActorPoseController()->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Controller", "Controller:", name )
		|| name == pPanel.GetActorPoseController()->GetName() ){
			return nullptr;
		}
		
		if( pPanel.GetActorPose()->GetControllers().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Controller", "A controller with this name exists already." );
			
		}else{
			pPanel.GetActorPoseController()->SetName( name );
			pPanel.GetActor()->NotifyPosesChanged();
		}
		return nullptr;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorPoseController() );
	}
};

class cActionActorPoseControllerMenu : public cBaseActionContextMenu{
public:
	cActionActorPoseControllerMenu( ceWPView &panel ) : cBaseActionContextMenu( panel, "ActorPoseController menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionActorPoseControllerAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionActorPoseControllerRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionActorPoseControllerRemoveAll( pPanel ), true );
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionActorPoseControllerRename( pPanel ), true );
	}
};

class cComboActorPoseControllerUpdateType : public cBaseComboBoxListener{
	igdeComboBox::Ref &pCBController;
	
public:
	cComboActorPoseControllerUpdateType( ceWPView &panel, igdeComboBox::Ref &cbController ) :
	cBaseComboBoxListener( panel ), pCBController( cbController ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation* ){
		ceActorController * const controller = pCBController->GetSelectedItem()
			? ( ceActorController* )pCBController->GetSelectedItem()->GetData() : nullptr;
		
		if( controller && comboBox.GetSelectedItem() ){
			controller->SetUpdateType( ( ceActorController::eUpdateTypes )( intptr_t )comboBox.GetSelectedItem()->GetData() );
		}
		return nullptr;
	}
};

class cTextActorPoseControllerValue : public cBaseTextFieldListener{
	igdeComboBox::Ref &pCBController;
	
public:
	cTextActorPoseControllerValue( ceWPView &panel, igdeComboBox::Ref &cbController ) :
	cBaseTextFieldListener( panel ), pCBController( cbController ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceActorController * const controller = pCBController->GetSelectedItem()
			? ( ceActorController* )pCBController->GetSelectedItem()->GetData() : nullptr;
		
		if( controller ){
			controller->SetValue( textField.GetFloat() );
		}
		return nullptr;
	}
};

class cTextActorPoseControllerVector : public cBaseEditVectorListener{
	igdeComboBox::Ref &pCBController;
	
public:
	cTextActorPoseControllerVector( ceWPView &panel, igdeComboBox::Ref &cbController ) :
	cBaseEditVectorListener( panel ), pCBController( cbController ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceActorController * const controller = pCBController->GetSelectedItem()
			? ( ceActorController* )pCBController->GetSelectedItem()->GetData() : nullptr;
		
		if( controller ){
			controller->SetVector( editVector.GetVector() );
		}
		return nullptr;
	}
};


class cComboActorGesture : public cBaseComboBoxListener{
public:
	cComboActorGesture( ceWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation* ){
		pPanel.SelectActorPoseGesture( comboBox.GetSelectedItem()
			? ( ceActorGesture* )comboBox.GetSelectedItem()->GetData() : NULL );
		return NULL;
	}
};

class cActionActorGestureAdd : public cBaseAction{
public:
	cActionActorGestureAdd( ceWPView &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Actor Gesture" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceActorPose * const pose = pPanel.GetActorPose();
		if( ! pose ){
			return NULL;
		}
		
		decString name;
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Gesture", "Gesture:", name ) ){
			return NULL;
		}
		
		if( pose->GetGestures().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Gesture", "A gesture with this name exists already." );
			return NULL;
		}
		
		const ceActorGesture::Ref gesture(ceActorGesture::Ref::NewWith(pPanel.GetEnvironment(), name));
		pose->GetGestures().Add( gesture );
		pPanel.GetActor()->NotifyPoseGesturesChanged( pose );
		pPanel.UpdateActorGestures();
		pPanel.SelectActorPoseGesture( gesture );
		return NULL;
	}
};

class cActionActorGestureRemove : public cBaseAction{
public:
	cActionActorGestureRemove( ceWPView &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Actor Gesture" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		if( pPanel.GetActorGesture() ){
			pPanel.GetActorPose()->GetGestures().Remove( pPanel.GetActorGesture() );
			pPanel.GetActor()->NotifyPoseGesturesChanged( pPanel.GetActorPose() );
			pPanel.UpdateActorGestures();
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorGesture() );
	}
};

class cActionActorGestureRename : public cBaseAction{
public:
	cActionActorGestureRename( ceWPView &panel ) : cBaseAction( panel, "Rename...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Rename Actor Gesture" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceActorPose * const pose = pPanel.GetActorPose();
		ceActorGesture * const gesture = pPanel.GetActorGesture();
		if( ! gesture || ! pose ){
			return NULL;
		}
		
		decString name( gesture->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Gesture", "Gesture:", name ) || name == gesture->GetName() ){
			return NULL;
		}
		
		if( pose->GetGestures().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Gesture", "A gesture with this name exists already." );
			
		}else{
			gesture->SetName( name  );
			pPanel.GetActor()->NotifyPoseGesturesChanged( pose );
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActorGesture() );
	}
};

class cActionActorGestureMenu : public cBaseActionContextMenu{
public:
	cActionActorGestureMenu( ceWPView &panel ) : cBaseActionContextMenu( panel, "ActorGesture menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionActorGestureAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionActorGestureRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionActorGestureRename( pPanel ), true );
	}
};

class cPathActorGestureAnimator : public cBaseEditPathListener{
public:
	cPathActorGestureAnimator( ceWPView &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditPath &editPath, ceConversation* ){
		if( pPanel.GetActorGesture() && pPanel.GetActorGesture()->GetPathAnimator() != editPath.GetPath() ){
			pPanel.GetActorGesture()->SetPathAnimator( editPath.GetPath() );
			pPanel.GetActor()->NotifyPoseGesturesChanged( pPanel.GetActorPose() );
		}
		return NULL;
	}
};


class cActionActorCommandAdd : public cBaseAction{
public:
	cActionActorCommandAdd( ceWPView &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Actor Command" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceConversationActor * const actor = pPanel.GetActor();
		if( ! actor ){
			return NULL;
		}
		
		decString command;
		if( ! igdeCommonDialogs::GetMultilineString( &pPanel.GetWindowProperties().GetWindowMain(),
			"Add Command", "Command:", command )
		|| actor->GetCommands().HasWith( command ) ){
			return NULL;
		}
		
		const cePlaybackCommand::Ref pcommand(cePlaybackCommand::Ref::NewWith(command, false));
		actor->GetCommands().Add( pcommand );
		conversation->NotifyActorCommandsChanged( actor );
		return NULL;
	}
};

class cActionActorCommandRemove : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionActorCommandRemove( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove Actor Command" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActor() && pListBox.GetSelectedItem() ){
			pPanel.GetActor()->GetCommands().Remove( ( cePlaybackCommand* )pListBox.GetSelectedItem()->GetData() );
			conversation->NotifyActorCommandsChanged( pPanel.GetActor() );
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActor() && pListBox.GetSelectedItem() );
	}
};

class cActionActorCommandClear : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionActorCommandClear( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Remove All", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove All Actor Commands" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActor() && pListBox.GetItemCount() > 0 ){
			pPanel.GetActor()->GetCommands().RemoveAll();
			conversation->NotifyActorCommandsChanged( pPanel.GetActor() );
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActor() && pListBox.GetItemCount() > 0 );
	}
};

class cListActorCommands: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	cListActorCommands( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeListBox *listBox, int index ){
		if( pPanel.GetActor() ){
			cePlaybackCommand &pcommand = *( ( cePlaybackCommand* )listBox->GetItemAt( index )->GetData() );
			pcommand.SetValue( ! pcommand.GetValue() );
			pPanel.GetConversation()->NotifyActorCommandsChanged( pPanel.GetActor() );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox *listBox, igdeMenuCascade &menu ){
		if( pPanel.GetActor() ){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand( menu, new cActionActorCommandAdd( pPanel ), true );
			helper.MenuCommand( menu, new cActionActorCommandRemove( pPanel, *listBox ), true );
			helper.MenuCommand( menu, new cActionActorCommandClear( pPanel, *listBox ), true );
		}
	}
};


class cActionActorParameterSet : public cBaseAction{
public:
	cActionActorParameterSet( ceWPView &panel ) : cBaseAction( panel, "Set...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Set Actor Parameter" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceConversationActor * const actor = pPanel.GetActor();
		if( ! actor ){
			return NULL;
		}
		
		decString name;
		if( ! igdeCommonDialogs::GetString( &pPanel, "Set Parameter", "Parameter:", name ) ){
			return NULL;
		}
		
		ceActorParameter * const parameter = actor->GetParameters().GetNamed( name );
		int value = parameter ? parameter->GetValue() : 0;
		if( ! igdeCommonDialogs::GetInteger( &pPanel, "Set Parameter", "Value:", value ) ){
			return NULL;
		}
		
		actor->GetParameters().Set( name, value );
		conversation->NotifyActorParametersChanged( actor );
		return NULL;
	}
};

class cActionActorParameterRemove : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionActorParameterRemove( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove Actor Parameter" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActor() && pListBox.GetSelectedItem() ){
			ceActorParameter * const parameter = ( ceActorParameter* )pListBox.GetSelectedItem()->GetData();
			if( parameter ){
				pPanel.GetActor()->GetParameters().Remove( parameter );
				conversation->NotifyActorParametersChanged( pPanel.GetActor() );
			}
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActor() && pListBox.GetSelectedItem() );
	}
};

class cActionActorParameterClear : public cBaseAction{
public:
	cActionActorParameterClear( ceWPView &panel ) : cBaseAction( panel, "Remove All",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove All Actor Parameters" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetActor() && pPanel.GetActor()->GetParameters().GetCount() > 0 ){
			pPanel.GetActor()->GetParameters().RemoveAll();
			conversation->NotifyActorParametersChanged( pPanel.GetActor() );
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetActor() && pPanel.GetActor()->GetParameters().GetCount() > 0 );
	}
};

class cListActorParameters: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	cListActorParameters( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeListBox *listBox, int ){
		if( ! pPanel.GetActor() ){
			return;
		}
		
		ceActorParameter &parameter = *( ( ceActorParameter* )listBox->GetSelectedItem()->GetData() );
		int value = parameter.GetValue();
		if( igdeCommonDialogs::GetInteger( &pPanel, "Set Parameter Value", "Value:", value ) ){
			parameter.SetValue( value );
			pPanel.GetConversation()->NotifyActorParametersChanged( pPanel.GetActor() );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox *listBox, igdeMenuCascade &menu ){
		if( pPanel.GetActor() ){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand( menu, new cActionActorParameterSet( pPanel ), true );
			helper.MenuCommand( menu, new cActionActorParameterRemove( pPanel, *listBox ), true );
			helper.MenuCommand( menu, new cActionActorParameterClear( pPanel ), true );
		}
	}
};


class cSpinCoordSys : public cBaseSpinTextFieldListener{
public:
	cSpinCoordSys( ceWPView &panel ) : cBaseSpinTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeSpinTextField &textField, ceConversation *conversation ){
		conversation->SetActiveCoordSystem( conversation->GetCoordSystemList().GetAt( textField.GetValue() ) );
		return NULL;
	}
};

class cActionCoordSysAdd : public cBaseAction{
public:
	cActionCoordSysAdd( ceWPView &panel ) : cBaseAction( panel, "Add",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Coord-System" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		deObjectReference actor;
		actor.TakeOver( new ceCoordSystem );
		conversation->AddCoordSystem( actor );
		conversation->SetActiveCoordSystem( actor );
		return NULL;
	}
};

class cActionCoordSysRemove : public cBaseAction{
public:
	cActionCoordSysRemove( ceWPView &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Coord-System" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pPanel.GetCoordSys() ){
			conversation->RemoveCoordSystem( pPanel.GetCoordSys() );
			if( conversation->GetCoordSystemList().GetCount() > 0 ){
				conversation->SetActiveCoordSystem( conversation->GetCoordSystemList().GetAt( 0 ) );
			}
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetCoordSys() );
	}
};

class cActionCoordSysMenu : public cBaseActionContextMenu{
public:
	cActionCoordSysMenu( ceWPView &panel ) : cBaseActionContextMenu( panel, "Coord-System menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionCoordSysAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionCoordSysRemove( pPanel ), true );
	}
};

class cTextCoordSysID : public cBaseTextFieldListener{
public:
	cTextCoordSysID( ceWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		if( pPanel.GetCoordSys() ){
			pPanel.GetCoordSys()->SetID( textField.GetText() );
		}
		return NULL;
	}
};

class cTextCoordSysAliasID : public cBaseTextFieldListener{
public:
	cTextCoordSysAliasID( ceWPView &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		if( pPanel.GetCoordSys() ){
			pPanel.GetCoordSys()->SetAliasID( textField.GetText() );
		}
		return NULL;
	}
};

class cVectorCoordSysPosition : public cBaseEditVectorListener{
public:
	cVectorCoordSysPosition( ceWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		if( pPanel.GetCoordSys() ){
			pPanel.GetCoordSys()->SetPosition( editVector.GetVector() );
		}
		return NULL;
	}
};

class cVectorCoordSysRotation : public cBaseEditVectorListener{
public:
	cVectorCoordSysRotation( ceWPView &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		if( pPanel.GetCoordSys() ){
			pPanel.GetCoordSys()->SetOrientation( editVector.GetVector() );
		}
		return NULL;
	}
};



class cComboPlaybackFile : public cBaseComboBoxListener{
public:
	cComboPlaybackFile( ceWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox&, ceConversation* ){
		pPanel.UpdatePlaybackTopicList();
		return NULL;
	}
};

class cActionPlaybackSelectTopic : public cBaseAction{
public:
	cActionPlaybackSelectTopic( ceWPView &panel ) : cBaseAction( panel, "Set",
	NULL, "Set this topic as the one to play back" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		cePlayback &playback = *conversation->GetPlayback();
		playback.SetTopic( pPanel.GetSelectedPlaybackTopic() );
		playback.Rewind();
		playback.SetRunning( false );
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetSelectedPlaybackTopic() );
	}
};

class cActionPlaybackRewind : public cBaseAction{
public:
	cActionPlaybackRewind( ceWPView &panel ) : cBaseAction( panel, "Rewind",
	NULL, "Rewind the playback to the beginning of the selected topic" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->GetPlayback()->Rewind();
		return NULL;
	}
};

class cActionPlaybackRunning : public cBaseAction{
public:
	cActionPlaybackRunning( ceWPView &panel ) : cBaseAction( panel, "Running",
	NULL, "Determines if the topic is currently played back" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->GetPlayback()->SetRunning( ! conversation->GetPlayback()->GetRunning() );
		return NULL;
	}
	
	virtual void Update( const ceConversation &conversation ){
		SetEnabled( conversation.GetPlayback()->GetTopic() );
		SetSelected( conversation.GetPlayback()->GetRunning() );
	}
};

class cActionPlaybackPaused : public cBaseAction{
public:
	cActionPlaybackPaused( ceWPView &panel ) : cBaseAction( panel, "Paused",
	NULL, "Determines if the played back is paused" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->GetPlayback()->SetPaused( ! conversation->GetPlayback()->GetPaused() );
		return NULL;
	}
	
	virtual void Update( const ceConversation &conversation ){
		SetEnabled( conversation.GetPlayback()->GetTopic() );
		SetSelected( conversation.GetPlayback()->GetPaused() );
	}
};

class cActionPlaybackAutoAdvanceCommands : public cBaseAction{
public:
	cActionPlaybackAutoAdvanceCommands( ceWPView &panel ) : cBaseAction( panel, "Auto Advance",
	nullptr, "Auto advance certain commands (game/actor commands, trigger, add/remove actor/coordsystem)" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->GetPlayback()->SetAutoAdvanceCommands( ! conversation->GetPlayback()->GetAutoAdvanceCommands() );
		return NULL;
	}
	
	virtual void Update( const ceConversation &conversation ){
		SetEnabled( true );
		SetSelected( conversation.GetPlayback()->GetAutoAdvanceCommands() );
	}
};

class cComboPlaybackCameraHandling : public cBaseComboBoxListener{
public:
	cComboPlaybackCameraHandling( ceWPView &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation *conversation ){
		if( comboBox.GetSelectedItem() ){
			conversation->GetPlayback()->SetCameraHandling(
				( cePlayback::eCameraHandling )( intptr_t )comboBox.GetSelectedItem()->GetData() );
		}
		return NULL;
	}
};


class cActionPlaybackCommandAdd : public cBaseAction{
public:
	cActionPlaybackCommandAdd( ceWPView &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Playback Command" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		decString command;
		if( ! igdeCommonDialogs::GetMultilineString( &pPanel.GetWindowProperties().GetWindowMain(),
			"Add Command", "Command:", command )
		|| conversation->GetPlayback()->GetCommands().HasWith( command ) ){
			return NULL;
		}
		
		const cePlaybackCommand::Ref pcommand(cePlaybackCommand::Ref::NewWith(command, false));
		conversation->GetPlayback()->GetCommands().Add( pcommand );
		conversation->NotifyPlaybackCommandListChanged();
		return NULL;
	}
};

class cActionPlaybackCommandRemove : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionPlaybackCommandRemove( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove Playback Command" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pListBox.GetSelectedItem() ){
			conversation->GetPlayback()->GetCommands().Remove(
				( cePlaybackCommand* )pListBox.GetSelectedItem()->GetData() );
			conversation->NotifyPlaybackCommandListChanged();
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pListBox.GetSelectedItem() );
	}
};

class cActionPlaybackCommandClear : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionPlaybackCommandClear( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Remove All", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove All Playback Commands" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pListBox.GetItemCount() > 0 ){
			conversation->GetPlayback()->GetCommands().RemoveAll();
			conversation->NotifyPlaybackCommandListChanged();
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pListBox.GetItemCount() > 0 );
	}
};

class cListPlaybackCommands: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	cListPlaybackCommands( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeListBox *listBox, int index ){
		if( pPanel.GetConversation() ){
			cePlaybackCommand &pcommand = *( ( cePlaybackCommand* )listBox->GetItemAt( index )->GetData() );
			pcommand.SetValue( ! pcommand.GetValue() );
			pPanel.GetConversation()->NotifyPlaybackCommandListChanged();
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox *listBox, igdeMenuCascade &menu ){
		if( pPanel.GetConversation() ){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand( menu, new cActionPlaybackCommandAdd( pPanel ), true );
			helper.MenuCommand( menu, new cActionPlaybackCommandRemove( pPanel, *listBox ), true );
			helper.MenuCommand( menu, new cActionPlaybackCommandClear( pPanel, *listBox ), true );
		}
	}
};


class cActionPlaybackVariableAdd : public cBaseAction{
public:
	cActionPlaybackVariableAdd( ceWPView &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Playback Variable" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		decString variable;
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Variable", "Variable:", variable )
		|| conversation->GetPlayback()->GetVariables().HasNamed( variable ) ){
			return NULL;
		}
		
		conversation->GetPlayback()->GetVariables().Set( variable, 0 );
		conversation->NotifyPlaybackVarListChanged();
		return NULL;
	}
};

class cActionPlaybackVariableSet : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionPlaybackVariableSet( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Set...", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
	"Set Playback Variable" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( ! pListBox.GetSelectedItem() ){
			return NULL;
		}
		
		cePlaybackVariable &variable = *( ( cePlaybackVariable* )pListBox.GetSelectedItem()->GetData() );
		int value = variable.GetValue();
		if( ! igdeCommonDialogs::GetInteger( &pPanel, "Set Variable", "Value:", value )
		|| variable.GetValue() == value ){
			return NULL;
		}
		
		variable.SetValue( value );
		conversation->NotifyPlaybackVarListChanged();
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pListBox.GetSelectedItem() );
	}
};

class cActionPlaybackVariableRemove : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionPlaybackVariableRemove( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Remove", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove Playback Variable" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pListBox.GetSelectedItem() ){
			conversation->GetPlayback()->GetVariables().Remove(
				( cePlaybackVariable* )pListBox.GetSelectedItem()->GetData() );
			conversation->NotifyPlaybackVarListChanged();
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pListBox.GetSelectedItem() );
	}
};

class cActionPlaybackVariableClear : public cBaseAction{
	igdeListBox &pListBox;
	
public:
	cActionPlaybackVariableClear( ceWPView &panel, igdeListBox &listBox ) :
	cBaseAction( panel, "Remove All", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove All Playback Variables" ), pListBox( listBox ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		if( pListBox.GetItemCount() > 0 ){
			conversation->GetPlayback()->GetVariables().RemoveAll();
			conversation->NotifyPlaybackVarListChanged();
		}
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pListBox.GetItemCount() > 0 );
	}
};

class cListPlaybackVariables: public igdeListBoxListener{
	ceWPView &pPanel;
public:
	cListPlaybackVariables( ceWPView &panel ) : pPanel( panel ){ }
	
	virtual void OnDoubleClickItem( igdeListBox *listBox, int ){
		if( ! pPanel.GetConversation() || ! listBox->GetSelectedItem() ){
			return;
		}
		
		cePlaybackVariable &variable = *( ( cePlaybackVariable* )listBox->GetSelectedItem()->GetData() );
		int value = variable.GetValue();
		if( igdeCommonDialogs::GetInteger( &pPanel, "Set Variable", "Value:", value )
		&& variable.GetValue() != value ){
			variable.SetValue( value );
			pPanel.GetConversation()->NotifyPlaybackVarListChanged();
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox *listBox, igdeMenuCascade &menu ){
		if( pPanel.GetConversation() ){
			igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
			helper.MenuCommand( menu, new cActionPlaybackVariableAdd( pPanel ), true );
			helper.MenuCommand( menu, new cActionPlaybackVariableSet( pPanel, *listBox ), true );
			helper.MenuCommand( menu, new cActionPlaybackVariableRemove( pPanel, *listBox ), true );
			helper.MenuCommand( menu, new cActionPlaybackVariableClear( pPanel, *listBox ), true );
		}
	}
};



class cActionPlaybackTriggerTable : public cBaseAction{
public:
	cActionPlaybackTriggerTable( ceWPView &panel ) : cBaseAction( panel, "", NULL, "" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->NotifyPlaybackTriggerTableChanged();
		return NULL;
	}
};



class cActionMissingWordsCopy : public cBaseAction{
public:
	cActionMissingWordsCopy( ceWPView &panel ) : cBaseAction( panel, "Copy To Clipboard",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy missing words found during playback to clipboard" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		const decStringSet &list = conversation->GetPlayback()->GetMissingWords();
		const int count = list.GetCount();
		decStringList ordered;
		int i;
		for( i=0; i<count; i++ ){
			ordered.Add( list.GetAt( i ) );
		}
		ordered.SortAscending();
		
		decString text( ordered.Join( "\n" ) );
		igdeCommonDialogs::GetMultilineString( &pPanel.GetWindowProperties().GetWindowMain(),
			"Copy To Clipboard", "Text for you to copy to the clipboard", text );
		return NULL;
	}
};

class cActionMissingWordsClear : public cBaseAction{
public:
	cActionMissingWordsClear( ceWPView &panel ) : cBaseAction( panel, "Clear",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiDelete ),
		"Clear list of missing words found during playback" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		conversation->GetPlayback()->GetMissingWords().RemoveAll();
		conversation->NotifyPlaybackMissingWordsChanged();
		return NULL;
	}
};

class cActionPlaybackMissingWordsMenu : public cBaseActionContextMenu{
public:
	cActionPlaybackMissingWordsMenu( ceWPView &panel ) : cBaseActionContextMenu( panel, "Missing Words menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionMissingWordsCopy( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionMissingWordsClear( pPanel ), true );
	}
};

}



// Class ceWPView
///////////////////

// Constructor, destructor
////////////////////////////

ceWPView::ceWPView( ceWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pConversation( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, groupBox2, form, formLine;
	igdeActionContextMenu *actionContextMenu;
	
	pListener = new ceWPViewListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// wrapper properties
	helper.WPCamera( content, pWPCamera, new cActionWPCamera( *this ), "Camera:", false, true, true );
	helper.WPCamera( content, pWPCameraFree, new cActionWPCamera( *this ), "Camera Free:", false, true, true );
	helper.WPSky( content, pWPSky, new cActionWPSky( *this ), "Sky:", false, true, true );
	helper.WPWObject( content, pWPEnvObject, new cActionWPEnvObject( *this ), "Environment Object:", false, true, true );
	
	
	// props
	helper.GroupBox( content, groupBox, "Props:", true );
	
	helper.FormLineStretchFirst( groupBox, "Prop:", "Prop to edit", formLine );
	helper.ComboBox( formLine, "Prop to edit", pCBProps, new cComboProp( *this ) );
	pCBProps->SetDefaultSorter();
	actionContextMenu = new cActionPropMenu( *this );
	helper.Button( formLine, pBtnPropMenu, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnPropMenu );
	
	helper.EditString( groupBox, "Name:", "Sets the name of the prop", pEditPropName, new cTextPropName( *this ) );
	
	helper.FormLineStretchFirst( groupBox, "Object:", "Class of the prop", formLine );
	helper.EditString( formLine, "Class of the prop", pEditPropClass, new cTextPropClass( *this ) );
	helper.Button( formLine, pBtnPropMenu, new cActionPropClass( *this, pEditPropClass ), true );
	
	helper.CheckBox( groupBox, pChkPropVisible, new cActionPropVisible( *this ), true );
	helper.EditVector( groupBox, "Position:", "Prop position", pEditPropPos, new cVectorPropPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Prop rotation", pEditPropRot, new cVectorPropRotation( *this ) );
	
	
	// props
	helper.GroupBoxFlow( content, groupBox, "Actors:", false );
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	
	helper.FormLineStretchFirst( form, "Actor:", "Actor to edit", formLine );
	helper.EditSpinInteger( formLine, "Actor to edit", 0, 0, pSpinActor, new cSpinActor( *this ) );
	actionContextMenu = new cActionActorMenu( *this );
	helper.Button( formLine, pBtnActorMenu, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnActorMenu );
	
	helper.EditString( form, "ID:", "Unique actor ID in the conversation",
		pEditActorID, new cTextActorID( *this ) );
	helper.EditString( form, "Alias ID:", "Optional unique actor alias ID",
		pEditActorAliasID, new cTextActorAliasID( *this ) );
	helper.EditVector( form, "Position:", "Actor position", pEditActorPos, new cVectorActorPosition( *this ) );
	helper.EditVector( form, "Rotation:", "Actor rotation", pEditActorOri, new cVectorActorRotation( *this ) );
	helper.EditPath( form, "Model:", "Actor model to use", igdeEnvironment::efpltModel,
		pEditActorPathModel, new cPathActorModel( *this ) );
	helper.EditPath( form, "Skin:", "Actor skin to use", igdeEnvironment::efpltSkin,
		pEditActorPathSkin, new cPathActorSkin( *this ) );
	helper.EditPath( form, "Rig:", "Actor rig to use", igdeEnvironment::efpltRig,
		pEditActorPathRig, new cPathActorRig( *this ) );
	helper.EditPath( form, "Speech Animation:", "Speech animation to use",
		igdeEnvironment::efpltSpeechAnimation, pEditActorPathSpeechAnim, new cPathActorSpeechAnimation( *this ) );
	helper.EditPath( form, "Eye Animator:", "Eye animator to use",
		igdeEnvironment::efpltAnimator, pEditActorPathEyeAnimator, new cPathActorEyeAnimator( *this ) );
	helper.EditPath( form, "Face Pose Animator:", "Face pose animator to use",
		igdeEnvironment::efpltAnimator, pEditActorPathFaceAnimator, new cPathActorFacePoseAnimator( *this ) );
	helper.CheckBox( form, pChkActorWaiting, new cActionActorWaiting( *this ), true );
	helper.EditString( form, "Head Rotator:", "Name of head rotator bone or empty string if not used",
		pEditActorBoneHeadRotator, new cTextActorBoneHeadRotator( *this ) );
	
	helper.FormLineStretchFirst( form, "Actor Pose:", "Actor pose to edit", formLine );
	helper.ComboBox( formLine, "Actor pose to edit", pCBActorPose, new cComboActorPose( *this ) );
	actionContextMenu = new cActionActorPoseMenu( *this );
	helper.Button( formLine, pBtnActorPoseMenu, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnActorPoseMenu );
	
	helper.EditPath( form, "Animator:", "Animator to use",
		igdeEnvironment::efpltAnimator, pEditActorPosePathAnimator, new cPathActorPoseAnimator( *this ) );
	
	
	helper.GroupBox( groupBox, groupBox2, "Animator Controllers:", true );
	
	helper.FormLineStretchFirst( groupBox2, "Controller:", "Controller to edit", formLine );
	helper.ComboBox( formLine, "Controller to edit", pCBActorPoseController, new cComboActorPoseController( *this ) );
	actionContextMenu = new cActionActorPoseControllerMenu( *this );
	helper.Button( formLine, pBtnActorPoseControllerMenu, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnActorPoseControllerMenu );
	
	helper.ComboBox( groupBox2, "Update Type:", "How to update the controller", pCBActorPoseControllerUpdateType,
		new cComboActorPoseControllerUpdateType( *this, pCBActorPoseController ) );
	pCBActorPoseControllerUpdateType->AddItem( "Constant Value", NULL,
		( void* )( intptr_t )ceActorController::eutConstant );
	pCBActorPoseControllerUpdateType->AddItem( "Elapsed Time", NULL,
		( void* )( intptr_t )ceActorController::eutElapsedTime );
	pCBActorPoseControllerUpdateType->AddItem( "Head Left-Right", NULL,
		( void* )( intptr_t )ceActorController::eutHeadLeftRight );
	pCBActorPoseControllerUpdateType->AddItem( "Head Up-Down", NULL,
		( void* )( intptr_t )ceActorController::eutHeadUpDown );
	pCBActorPoseControllerUpdateType->AddItem( "Eyes Left-Right", NULL,
		( void* )( intptr_t )ceActorController::eutEyesLeftRight );
	pCBActorPoseControllerUpdateType->AddItem( "Eyes Up-Down", NULL,
		( void* )( intptr_t )ceActorController::eutEyesUpDown );
	
	helper.EditFloat( groupBox2, "Value:", "Constant value to use if the update type is constant",
		pEditActorPoseControllerValue, new cTextActorPoseControllerValue( *this, pCBActorPoseController ) );
	helper.EditVector( groupBox2, "Vector:", "Constant vector to use if the update type is constant",
		pEditActorPoseControllerVector, new cTextActorPoseControllerVector( *this, pCBActorPoseController ) );
	
	
	helper.GroupBox( groupBox, groupBox2, "Actor Gestures:", true );
	
	helper.FormLineStretchFirst( groupBox2, "Gesture:", "Actor gesture to edit", formLine );
	helper.ComboBox( formLine, "Actor gesture to edit", pCBActorGesture, new cComboActorGesture( *this ) );
	actionContextMenu = new cActionActorGestureMenu( *this );
	helper.Button( formLine, pBtnActorGestureMenu, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnActorGestureMenu );
	
	helper.EditPath( groupBox2, "Animator:", "Animator to use", igdeEnvironment::efpltAnimator,
		pEditActorGesturePathAnimator, new cPathActorGestureAnimator( *this ) );
	
	
	helper.GroupBoxFlow( groupBox, groupBox2, "Actor Command Simulation:", false, true );
	helper.ListBox( groupBox2, 5, "Actor Commands", pListActorCommands, new cListActorCommands( *this ) );
	pListActorCommands->SetDefaultSorter();
	
	helper.GroupBoxFlow( groupBox, groupBox2, "Actor Parameters:", false, true );
	helper.ListBox( groupBox2, 5, "Actor Parameters", pListActorParameters, new cListActorParameters( *this ) );
	pListActorParameters->SetDefaultSorter();
	
	
	// coordinate system
	helper.GroupBox( content, groupBox, "Coordinate Systems:", true );
	
	helper.FormLineStretchFirst( groupBox, "Coord-System:", "Coordinate system to edit", formLine );
	helper.EditSpinInteger( formLine, "Coordinate system to edit", 0, 0, pSpinCoordSys, new cSpinCoordSys( *this ) );
	actionContextMenu = new cActionCoordSysMenu( *this );
	helper.Button( formLine, pBtnCoordSysMenu, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnCoordSysMenu );
	
	helper.EditString( groupBox, "ID:", "Unique coordinate system ID in the conversation",
		pEditCoordSysID, new cTextCoordSysID( *this ) );
	helper.EditString( groupBox, "Alias ID:", "Optional unique coordinate system alias ID",
		pEditCoordSysAliasID, new cTextCoordSysAliasID( *this ) );
	helper.EditVector( groupBox, "Position:", "Position of the coordinate system",
		pEditCoordSysPosition, new cVectorCoordSysPosition( *this ) );
	helper.EditVector( groupBox, "Rotation:", "Orientation of the coordinate system",
		pEditCoordSysRotation, new cVectorCoordSysRotation( *this ) );
	
	
	// playback
	helper.GroupBoxFlow( content, groupBox, "Playback:", false );
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	
	helper.ComboBoxFilter( form, "File:", "File to show topics for",
		pCBPlaybackFile, new cComboPlaybackFile( *this ) );
	pCBPlaybackFile->SetDefaultSorter();
	
	helper.ComboBoxFilter( form, "Topic:", "Topic to play back", pCBPlaybackTopic, NULL );
	pCBPlaybackTopic->SetDefaultSorter();
	
	helper.FormLine( form, "", "", formLine );
	helper.Button( formLine, pBtnPlaybackSelectTopic, new cActionPlaybackSelectTopic( *this ), true );
	helper.Button( formLine, pBtnPlaybackRewind, new cActionPlaybackRewind( *this ), true );
	
	helper.FormLine( form, "", "", formLine );
	helper.CheckBoxOnly( formLine, pChkPlaybackRunning, new cActionPlaybackRunning( *this ), true );
	helper.CheckBoxOnly( formLine, pChkPlaybackPaused, new cActionPlaybackPaused( *this ), true );
	helper.CheckBoxOnly( formLine, pChkPlaybackAutoAdvanceCommands, new cActionPlaybackAutoAdvanceCommands( *this ), true );
	
	helper.ComboBox( form, "Camera Handling:", "How camera is handled",
		pCBPlaybackCameraHandling, new cComboPlaybackCameraHandling( *this ) );
	pCBPlaybackCameraHandling->AddItem( "Conversation", NULL, ( void* )( intptr_t )cePlayback::echConversation );
	pCBPlaybackCameraHandling->AddItem( "Camera Shot", NULL, ( void* )( intptr_t )cePlayback::echCameraShot );
	pCBPlaybackCameraHandling->AddItem( "Free", NULL, ( void* )( intptr_t )cePlayback::echFree );
	pCBPlaybackCameraHandling->SetSelectionWithData( ( void* )( intptr_t )cePlayback::echFree );
	
	
	helper.GroupBoxFlow( groupBox, groupBox2, "Game Command:", false, true );
	helper.ListBox( groupBox2, 5, "Game Commands", pListPlaybackCommands, new cListPlaybackCommands( *this ) );
	pListPlaybackCommands->SetDefaultSorter();
	
	helper.GroupBoxFlow( groupBox, groupBox2, "Variables:", false, true );
	helper.ListBox( groupBox2, 5, "Variables", pListPlaybackVars, new cListPlaybackVariables( *this ) );
	pListPlaybackVars->SetDefaultSorter();
	
	
	helper.WPTriggerTable( groupBox, pWPPlaybackTriggerTable,
		new cActionPlaybackTriggerTable( *this ), "Trigger Table:", false, true, true );
	
	
	helper.GroupBoxFlow( groupBox, groupBox2, "Debug:", false, false );
	
	helper.FormLineStretchFirst( groupBox2, "Missing Words:", "Missing words found during playback", formLine );
	helper.ComboBox( formLine, "Missing words found during playback", pCBPlaybackMissingWords, NULL );
	pCBPlaybackMissingWords->SetDefaultSorter();
	actionContextMenu = new cActionPlaybackMissingWordsMenu( *this );
	helper.Button( formLine, pBtnPlaybackMissingWordsMenu, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnPlaybackMissingWordsMenu );
}

ceWPView::~ceWPView(){
	SetConversation( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void ceWPView::SetConversation( ceConversation *conversation ){
	if( conversation == pConversation ){
		return;
	}
	
	pWPEnvObject->SetObject(nullptr);
	pWPSky->SetSky( NULL );
	pWPCamera->SetCamera( NULL );
	pWPCameraFree->SetCamera( NULL );
	pWPPlaybackTriggerTable->SetTriggerTargetList( NULL );
	
	if( pConversation ){
		pConversation->RemoveListener( pListener );
		pConversation->FreeReference();
	}
	
	pConversation = conversation;
	
	if( conversation ){
		conversation->AddListener( pListener );
		conversation->AddReference();
		
		pWPSky->SetSky( conversation->GetSky() );
		pWPEnvObject->SetObject(conversation->GetEnvObject());
		pWPCamera->SetCamera( conversation->GetCamera() );
		pWPCameraFree->SetCamera( conversation->GetCameraFree() );
		pWPPlaybackTriggerTable->SetTriggerTargetList( &conversation->GetPlayback()->GetTriggerTable() );
	}
	
	UpdateView();
	UpdateSky();
	UpdateEnvObject();
	UpdateCamera();
	UpdatePropList();
	UpdateActors();
	UpdateCoordSystems();
	UpdatePlayback();
	UpdatePlaybackFileList();
	UpdatePlaybackTopicList();
	UpdatePlaybackCommands();
	UpdatePlaybackVariables();
	UpdatePlaybackTriggerTable();
}

void ceWPView::UpdateView(){
}

void ceWPView::UpdateSky(){
	pWPSky->UpdateSky();
}

void ceWPView::UpdateEnvObject(){
	pWPEnvObject->UpdateObject();
}

void ceWPView::UpdateCamera(){
	pWPCamera->UpdateCamera();
	pWPCamera->UpdateViewDirection();
	pWPCameraFree->UpdateCamera();
	pWPCameraFree->UpdateViewDirection();
}



ceConversationActor *ceWPView::GetActor() const{
	return pConversation ? pConversation->GetActiveActor() : NULL;
}

ceActorPose *ceWPView::GetActorPose() const{
	const ceConversationActor * const actor = GetActor();
	return actor ? actor->GetActivePose() : NULL;
}

ceActorController *ceWPView::GetActorPoseController() const{
	return pCBActorPoseController->GetSelectedItem()
		? ( ceActorController* )pCBActorPoseController->GetSelectedItem()->GetData() : nullptr;
}

ceActorGesture *ceWPView::GetActorGesture() const{
	return GetActorPose() && pCBActorGesture->GetSelectedItem()
		? ( ceActorGesture* )pCBActorGesture->GetSelectedItem()->GetData() : NULL;
}

ceProp *ceWPView::GetProp() const{
	return pConversation ? pConversation->GetActiveProp() : NULL;
}

void ceWPView::UpdatePropList(){
	pCBProps->RemoveAllItems();
	
	if( pConversation ){
		const cePropList &list = pConversation->GetPropList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceProp * const prop = list.GetAt( i );
			pCBProps->AddItem( prop->GetName(), NULL, prop );
		}
	}
	
	pCBProps->SortItems();
	pCBProps->SetSelectionWithData( GetProp() );
	UpdateProp();
}

void ceWPView::UpdateProp(){
	const ceProp * const prop = GetProp();
	
	if( prop ){
		pEditPropName->SetText( prop->GetName() );
		pEditPropClass->SetText( prop->GetObjectClass() );
		pEditPropPos->SetVector( prop->GetPosition() );
		pEditPropRot->SetVector( prop->GetOrientation() );
		
	}else{
		pEditPropName->ClearText();
		pEditPropClass->ClearText();
		pEditPropPos->SetVector( decVector() );
		pEditPropRot->SetVector( decVector() );
	}
	
	const bool enabled = prop;
	pEditPropName->SetEnabled( enabled );
	pEditPropClass->SetEnabled( enabled );
	pEditPropPos->SetEnabled( enabled );
	pEditPropRot->SetEnabled( enabled );
	
	pChkPropVisible->GetAction()->Update();
}

void ceWPView::UpdateActors(){
	const ceConversationActor * const actor = GetActor();
	
	// actor list
	if( pConversation ){
		const ceConversationActorList &list = pConversation->GetActorList();
		pSpinActor->SetRange( 0, decMath::max( list.GetCount() - 1, 0 ) );
		pSpinActor->SetValue( actor ? list.IndexOf( ( ceConversationActor* )actor ) : 0 );
		
	}else{
		pSpinActor->SetRange( 0, 0 );
	}
	
	// actor
	if( actor ){
		pEditActorID->SetText( actor->GetID() );
		pEditActorAliasID->SetText( actor->GetAliasID() );
		pEditActorPos->SetVector( actor->GetPosition() );
		pEditActorOri->SetVector( actor->GetOrientation() );
		pEditActorPathModel->SetPath( actor->GetPathModel() );
		pEditActorPathSkin->SetPath( actor->GetPathSkin() );
		pEditActorPathRig->SetPath( actor->GetPathRig() );
		pEditActorPathSpeechAnim->SetPath( actor->GetPathSpeechAnimation() );
		pEditActorPathEyeAnimator->SetPath( actor->GetPathEyesAnimator() );
		pEditActorPathFaceAnimator->SetPath( actor->GetPathFacePoseAnimator() );
		pEditActorBoneHeadRotator->SetText( actor->GetBoneHeadRotator() );
		
	}else{
		pEditActorID->ClearText();
		pEditActorAliasID->ClearText();
		pEditActorPos->SetVector( decVector() );
		pEditActorOri->SetVector( decVector() );
		pEditActorPathModel->ClearPath();
		pEditActorPathSkin->ClearPath();
		pEditActorPathRig->ClearPath();
		pEditActorPathSpeechAnim->ClearPath();
		pEditActorPathEyeAnimator->ClearPath();
		pEditActorPathFaceAnimator->ClearPath();
		pEditActorBoneHeadRotator->ClearText();
	}
	
	const bool enabled = actor;
	pEditActorID->SetEnabled( enabled );
	pEditActorAliasID->SetEnabled( enabled );
	pEditActorPos->SetEnabled( enabled );
	pEditActorOri->SetEnabled( enabled );
	pEditActorPathModel->SetEnabled( enabled );
	pEditActorPathSkin->SetEnabled( enabled );
	pEditActorPathRig->SetEnabled( enabled );
	pEditActorPathSpeechAnim->SetEnabled( enabled );
	pEditActorPathEyeAnimator->SetEnabled( enabled );
	pEditActorPathFaceAnimator->SetEnabled( enabled );
	pEditActorBoneHeadRotator->SetEnabled( enabled );
	
	pChkActorWaiting->GetAction()->Update();
	
	UpdateActorPoses();
	UpdateActorCommands();
	UpdateActorParameters();
}

void ceWPView::UpdateActorPoses(){
	ceConversationActor * const actor = GetActor();
	ceActorPose *activePose = actor ? actor->GetActivePose() : NULL;
	
	pCBActorPose->RemoveAllItems();
	
	if( actor ){
		const ceActorPoseList &poses = actor->GetPoses();
		const int poseCount = poses.GetCount();
		int i;
		
		for( i=0; i<poseCount; i++ ){
			ceActorPose * const pose = poses.GetAt( i );
			pCBActorPose->AddItem( pose->GetName(), NULL, pose );
		}
		
		if( ! activePose && poseCount > 0 ){
			activePose = poses.GetAt( 0 );
		}
		actor->SetActivePose( activePose );
	}
	
	SelectActiveActorPose();
}

void ceWPView::SelectActiveActorPose(){
	pCBActorPose->SetSelectionWithData( GetActorPose() );
	UpdateActorPose();
}

void ceWPView::UpdateActorPose(){
	const ceActorPose * const pose = GetActorPose();
	
	if( pose ){
		pEditActorPosePathAnimator->SetPath( pose->GetPathAnimator() );
		
	}else{
		pEditActorPosePathAnimator->ClearPath();
	}
	
	const bool enabled = pose;
	pEditActorPosePathAnimator->SetEnabled( enabled );
	
	UpdateActorControllers();
	UpdateActorGestures();
}

void ceWPView::UpdateActorGestures(){
	const ceActorPose * const pose = GetActorPose();
	ceActorGesture *gesture = GetActorGesture();
	pCBActorGesture->RemoveAllItems();
	
	if( pose ){
		const ceActorGestureList &gestures = pose->GetGestures();
		const int gestureCount = gestures.GetCount();
		int i;
		
		for( i=0; i<gestureCount; i++ ){
			ceActorGesture * const poseGesture = gestures.GetAt( i );
			pCBActorGesture->AddItem( poseGesture->GetName(), NULL, poseGesture );
		}
		
		pCBActorGesture->SortItems();
	}
	
	pCBActorGesture->SetSelectionWithData( gesture );
	if( ! pCBActorGesture->GetSelectedItem() && pCBActorGesture->GetItemCount() > 0 ){
		pCBActorGesture->SetSelection( 0 );
	}
	gesture = GetActorGesture();
	
	if( gesture ){
		pEditActorGesturePathAnimator->SetPath( gesture->GetPathAnimator() );
		
	}else{
		pEditActorGesturePathAnimator->ClearPath();
	}
	
	const bool enabled = gesture;
	pEditActorGesturePathAnimator->SetEnabled( enabled );
}

void ceWPView::SelectActorPoseGesture( ceActorGesture *gesture ){
	pCBActorGesture->SetSelectionWithData( gesture );
	UpdateActorGesture();
}

void ceWPView::UpdateActorGesture(){
	const ceActorGesture * const gesture = GetActorGesture();
	
	if( gesture ){
		pEditActorGesturePathAnimator->SetPath( gesture->GetPathAnimator() );
		
	}else{
		pEditActorGesturePathAnimator->ClearPath();
	}
	
	pEditActorGesturePathAnimator->SetEnabled( gesture );
}

void ceWPView::UpdateActorControllers(){
	const ceActorPose * const pose = GetActorPose();
	ceActorController * const selection = pCBActorPoseController->GetSelectedItem()
		? ( ceActorController* )pCBActorPoseController->GetSelectedItem()->GetData() : nullptr;
	
	if( pose ){
		const int count = pose->GetControllers().GetCount();
		decString text;
		int i;
		
		pCBActorPoseController->RemoveAllItems();
		for( i=0; i<count; i++ ){
			ceActorController * const controller = pose->GetControllers().GetAt( i );
			pCBActorPoseController->AddItem( controller->GetName(), nullptr, controller );
		}
		
		pCBActorPoseController->SetSelectionWithData( selection );
		
	}else{
		pCBActorPoseController->RemoveAllItems();
	}
	
	pCBActorPoseController->SetEnabled( pose );
	
	UpdateActorController();
}

void ceWPView::UpdateActorController(){
	const ceActorPose * const pose = GetActorPose();
	const ceActorController * const controller = pCBActorPoseController->GetSelectedItem()
		? ( ceActorController* )pCBActorPoseController->GetSelectedItem()->GetData() : nullptr;
	
	if( pose && controller ){
		pCBActorPoseControllerUpdateType->SetSelectionWithData(
			( void* )( intptr_t )controller->GetUpdateType() );
		pEditActorPoseControllerValue->SetFloat( controller->GetValue() );
		pEditActorPoseControllerVector->SetVector( controller->GetVector() );
		
	}else{
		pCBActorPoseControllerUpdateType->SetSelectionWithData(
			( void* )( intptr_t )ceActorController::eutElapsedTime );
		pEditActorPoseControllerValue->ClearText();
		pEditActorPoseControllerVector->SetVector( decVector() );
	}
	
	const bool enabled = pose && controller;
	pCBActorPoseControllerUpdateType->SetEnabled( enabled );
	pEditActorPoseControllerValue->SetEnabled( enabled );
	pEditActorPoseControllerVector->SetEnabled( enabled );
}

void ceWPView::SelectActorPoseController( ceActorController *controller ){
	pCBActorPoseController->SetSelectionWithData( controller );
}

void ceWPView::UpdateActorCommands(){
	const ceConversationActor * const actor = GetActor();
	cePlaybackCommand * const selection = pListActorCommands->GetSelectedItem()
		? ( cePlaybackCommand* )pListActorCommands->GetSelectedItem()->GetData() : NULL;
	
	pListActorCommands->RemoveAllItems();
	
	if( actor ){
		igdeIcon * const iconTrue = GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus );
		igdeIcon * const iconFalse = GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus );
		const cePlaybackCommandList &list = actor->GetCommands();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			cePlaybackCommand * const entry = list.GetAt( i );
			pListActorCommands->AddItem( entry->GetCommand(),
				entry->GetValue() ? iconTrue : iconFalse, entry );
		}
	}
	
	pListActorCommands->SortItems();
	
	pListActorCommands->SetSelectionWithData( selection );
	if( ! pListActorCommands->GetSelectedItem() && pListActorCommands->GetItemCount() > 0 ){
		pListActorCommands->SetSelection( 0 );
	}
}

void ceWPView::UpdateActorParameters(){
	const ceConversationActor * const actor = GetActor();
	ceActorParameter * const selection = pListActorParameters->GetSelectedItem()
		? ( ceActorParameter* )pListActorParameters->GetSelectedItem()->GetData() : NULL;
	
	pListActorParameters->RemoveAllItems();
	
	if( actor ){
		const ceActorParameterList &list = actor->GetParameter();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			ceActorParameter * const parameter = list.GetAt( i );
			text.Format( "%s = %d", parameter->GetName().GetString(), parameter->GetValue() );
			pListActorParameters->AddItem( text, NULL, parameter );
		}
	}
	
	pListActorParameters->SortItems();
	
	pListActorParameters->SetSelectionWithData( selection );
	if( ! pListActorParameters->GetSelectedItem() && pListActorParameters->GetItemCount() > 0 ){
		pListActorParameters->SetSelection( 0 );
	}
}



ceCoordSystem *ceWPView::GetCoordSys() const{
	return pConversation ? pConversation->GetActiveCoordSystem() : NULL;
}

void ceWPView::UpdateCoordSystems(){
	if( pConversation ){
		const ceCoordSystemList &coordSystemList = pConversation->GetCoordSystemList();
		pSpinCoordSys->SetRange( 0, decMath::max( coordSystemList.GetCount() - 1, 0 ) );
		
		ceCoordSystem * const coordsys = pConversation->GetActiveCoordSystem();
		if( coordsys ){
			pSpinCoordSys->SetValue( coordSystemList.IndexOf( coordsys ) );
			pEditCoordSysID->SetText( coordsys->GetID() );
			pEditCoordSysAliasID->SetText( coordsys->GetAliasID() );
			pEditCoordSysPosition->SetVector( coordsys->GetPosition() );
			pEditCoordSysRotation->SetVector( coordsys->GetOrientation() );
			
		}else{
			pEditCoordSysID->ClearText();
			pEditCoordSysAliasID->ClearText();
			pEditCoordSysPosition->SetVector( decVector() );
			pEditCoordSysRotation->SetVector( decVector() );
		}
		
	}else{
		pEditCoordSysID->ClearText();
		pEditCoordSysAliasID->ClearText();
		pEditCoordSysPosition->SetVector( decVector() );
		pEditCoordSysRotation->SetVector( decVector() );
	}
	
	const bool enabled = GetCoordSys();
	pEditCoordSysID->SetEnabled( enabled );
	pEditCoordSysAliasID->SetEnabled( enabled );
	pEditCoordSysPosition->SetEnabled( enabled );
	pEditCoordSysRotation->SetEnabled( enabled );
}



void ceWPView::UpdatePlaybackFileList(){
	ceConversationFile * const selectedFile = GetSelectedPlaybackFile();
	pCBPlaybackFile->RemoveAllItems();
	
	if( pConversation ){
		const ceConversationFileList list( pConversation->AllFiles() );
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceConversationFile * const file = list.GetAt( i );
			pCBPlaybackFile->AddItem( file->GetID(), NULL, file );
		}
		
		pCBPlaybackFile->SortItems();
		pCBPlaybackFile->StoreFilterItems();
	}
	
	pCBPlaybackFile->SetSelectionWithData( selectedFile );
}

ceConversationFile *ceWPView::GetSelectedPlaybackFile() const{
	return pCBPlaybackFile->GetSelectedItem()
		? ( ceConversationFile* )pCBPlaybackFile->GetSelectedItem()->GetData() : NULL;
}

void ceWPView::UpdatePlaybackTopicList(){
	ceConversationFile * const selectedFile = GetSelectedPlaybackFile();
	ceConversationTopic * const selectedTopic = GetSelectedPlaybackTopic();
	
	pCBPlaybackTopic->RemoveAllItems();
	
	if( selectedFile ){
		const ceConversationTopicList list( pConversation->AllTopics( selectedFile->GetID() ) );
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceConversationTopic * const topic = list.GetAt( i );
			pCBPlaybackTopic->AddItem( topic->GetID(), NULL, topic );
		}
		
		pCBPlaybackTopic->SortItems();
		pCBPlaybackTopic->StoreFilterItems();
	}
	
	pCBPlaybackTopic->SetSelectionWithData( selectedTopic );
}

ceConversationTopic *ceWPView::GetSelectedPlaybackTopic() const{
	return pCBPlaybackTopic->GetSelectedItem()
		? ( ceConversationTopic* )pCBPlaybackTopic->GetSelectedItem()->GetData() : NULL;
}

void ceWPView::UpdatePlayback(){
	if( pConversation ){
		pCBPlaybackCameraHandling->SetSelectionWithData(
			( void* )( intptr_t )pConversation->GetPlayback()->GetCameraHandling() );
		
	}else{
		pCBPlaybackCameraHandling->SetSelectionWithData( ( void* )( intptr_t )cePlayback::echFree );
	}
	
	pCBPlaybackCameraHandling->SetEnabled( pConversation );
	
	pChkPlaybackRunning->GetAction()->Update();
	pChkPlaybackPaused->GetAction()->Update();
	pChkPlaybackAutoAdvanceCommands->GetAction()->Update();
}

void ceWPView::UpdatePlaybackCommands(){
	cePlaybackCommand * const selectedEntry = pListPlaybackCommands->GetSelectedItem()
		? ( cePlaybackCommand* )pListPlaybackCommands->GetSelectedItem()->GetData() : NULL;
	
	pListPlaybackCommands->RemoveAllItems();
	
	if( pConversation ){
		igdeIcon * const iconTrue = GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus );
		igdeIcon * const iconFalse = GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus );
		const cePlaybackCommandList &list = pConversation->GetPlayback()->GetCommands();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			cePlaybackCommand * const entry = list.GetAt( i );
			pListPlaybackCommands->AddItem( entry->GetCommand(),
				entry->GetValue() ? iconTrue : iconFalse, entry );
		}
	}
	
	pListPlaybackCommands->SortItems();
	
	pListPlaybackCommands->SetSelectionWithData( selectedEntry );
	if( ! pListPlaybackCommands->GetSelectedItem() && pListPlaybackCommands->GetItemCount() > 0 ){
		pListPlaybackCommands->SetSelection( 0 );
	}
}

void ceWPView::UpdatePlaybackVariables(){
	cePlaybackVariable * const selectedVariable = pListPlaybackVars->GetSelectedItem()
		? ( cePlaybackVariable* )pListPlaybackVars->GetSelectedItem()->GetData() : NULL;
	
	pListPlaybackVars->RemoveAllItems();
	
	if( pConversation ){
		const cePlaybackVariableList &list = pConversation->GetPlayback()->GetVariables();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			cePlaybackVariable * const variable = list.GetAt( i );
			text.Format( "%s = %d", variable->GetName().GetString(), variable->GetValue() );
			pListPlaybackVars->AddItem( text, NULL, variable );
		}
	}
	
	pListPlaybackVars->SortItems();
	
	pListPlaybackVars->SetSelectionWithData( selectedVariable );
	if( ! pListPlaybackVars->GetSelectedItem() && pListPlaybackVars->GetItemCount() > 0 ){
		pListPlaybackVars->SetSelection( 0 );
	}
}

void ceWPView::UpdatePlaybackTriggerTable(){
	pWPPlaybackTriggerTable->UpdateTable();
}

void ceWPView::UpdatePlaybackMissingWords(){
	const decString selectedWord( pCBPlaybackMissingWords->GetText() );
	
	pCBPlaybackMissingWords->RemoveAllItems();
	
	if( pConversation ){
		const decStringSet &list = pConversation->GetPlayback()->GetMissingWords();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pCBPlaybackMissingWords->AddItem( list.GetAt( i ) );
		}
	}
	
	pCBPlaybackMissingWords->SortItems();
	
	pCBPlaybackMissingWords->SetText( selectedWord );
	if( ! pCBPlaybackMissingWords->GetSelectedItem() && pCBPlaybackMissingWords->GetItemCount() > 0 ){
		pCBPlaybackMissingWords->SetSelection( 0 );
	}
}
