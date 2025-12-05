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

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPConversation.h"
#include "ceWPConversationListener.h"
#include "ceWindowProperties.h"
#include "../ceWindowMain.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/actor/ceConversationActor.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/facepose/ceFacePose.h"
#include "../../conversation/gesture/ceGesture.h"
#include "../../conversation/target/ceTarget.h"
#include "../../configuration/ceConfiguration.h"
#include "../../loadsave/ceLoadSaveSystem.h"
#include "../../undosys/ceUConvoSetImportConvoPath.h"
#include "../../undosys/cameraShot/ceUCCShotAdd.h"
#include "../../undosys/cameraShot/ceUCCShotRemove.h"
#include "../../undosys/cameraShot/ceUCCShotSetActorCount.h"
#include "../../undosys/cameraShot/ceUCCShotSetCameraTarget.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamDistFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamDistTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffCamFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffCamTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamOrbitFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetCamOrbitTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetFovFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetFovTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetLookAtTarget.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffLookAtFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetOffLookAtTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetName.h"
#include "../../undosys/cameraShot/ceUCCShotSetPosFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetPosTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetRotFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetRotTo.h"
#include "../../undosys/cameraShot/ceUCCShotSetTiltFrom.h"
#include "../../undosys/cameraShot/ceUCCShotSetTiltTo.h"
#include "../../undosys/cameraShot/ceUCCShotToggleLockUpAxis.h"
#include "../../undosys/cameraShot/ceUCCShotToggleAlignTargets.h"
#include "../../undosys/cameraShot/ceUCCShotToggleRelativeToLookAt.h"
#include "../../undosys/cameraShot/ceUCCShotToggleLockCameraTarget.h"
#include "../../undosys/cameraShot/ceUCCShotToggleLockLookAtTarget.h"
#include "../../undosys/facePose/ceUCFacePoseAdd.h"
#include "../../undosys/facePose/ceUCFacePoseRemove.h"
#include "../../undosys/facePose/ceUCFacePoseSetName.h"
#include "../../undosys/facePose/controller/ceUCFPControllerAdd.h"
#include "../../undosys/facePose/controller/ceUCFPControllerRemove.h"
#include "../../undosys/facePose/controller/ceUCFPControllerSetValue.h"
#include "../../undosys/gesture/ceUCGestureAdd.h"
#include "../../undosys/gesture/ceUCGestureRemove.h"
#include "../../undosys/gesture/ceUCGestureSetName.h"
#include "../../undosys/gesture/ceUCGestureSetAnimator.h"
#include "../../undosys/gesture/ceUCGestureToggleHold.h"
#include "../../undosys/gesture/ceUCGestureSetDuration.h"
#include "../../undosys/target/ceUCTargetAdd.h"
#include "../../undosys/target/ceUCTargetRemove.h"
#include "../../undosys/target/ceUCTargetSetName.h"
#include "../../undosys/target/ceUCTargetSetActor.h"
#include "../../undosys/target/ceUCTargetSetEntityID.h"
#include "../../undosys/target/ceUCTargetSetPosition.h"
#include "../../undosys/target/ceUCTargetSetOrientation.h"
#include "../../undosys/target/ceUCTargetSetBone.h"
#include "../../utils/ceControllerValue.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/composed/igdeEditVectorListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	ceWPConversation &pPanel;
	
public:
	cBaseAction( ceWPConversation &panel, const char *text, igdeIcon *icon, const char *description ) :
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
	ceWPConversation &pPanel;
	
public:
	cBaseActionContextMenu( ceWPConversation &panel, const char *description ) :
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
	ceWPConversation &pPanel;
	
public:
	cBaseComboBoxListener( ceWPConversation &panel ) : pPanel( panel ){ }
	
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
	ceWPConversation &pPanel;
	
public:
	cBaseTextFieldListener( ceWPConversation &panel ) : pPanel( panel ){ }
	
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
	ceWPConversation &pPanel;
	
public:
	cBaseEditVectorListener( ceWPConversation &panel ) : pPanel( panel ){ }
	
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



class cActionImportConvoAdd : public cBaseAction{
public:
	cActionImportConvoAdd( ceWPConversation &panel ) : cBaseAction( panel, "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Import Conversation" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		const decString &path = pPanel.GetPathImportConvo();
		if( path.IsEmpty() || path == conversation->GetFilePath()
		|| conversation->GetImportConversationPath().Has( path ) ){
			return NULL;
		}
		
		decStringList list( pPanel.GetConversation()->GetImportConversationPath() );
		list.Add( path );
		return new ceUConvoSetImportConvoPath( pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list );
	}
};

class cActionImportConvoRemove : public cBaseAction{
public:
	cActionImportConvoRemove( ceWPConversation &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Import" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		const decString path( pPanel.GetImportConvo() );
		if( path.IsEmpty() ){
			return NULL;
		}
		
		decStringList list( pPanel.GetConversation()->GetImportConversationPath() );
		const int index = list.IndexOf( path );
		if( index == -1 ){
			return NULL;
		}
		
		list.RemoveFrom( index );
		return new ceUConvoSetImportConvoPath( pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list );
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( ! pPanel.GetImportConvo().IsEmpty() );
	}
};

class cActionImportConvoMoveUp : public cBaseAction{
public:
	cActionImportConvoMoveUp( ceWPConversation &panel ) : cBaseAction( panel, "Move Up",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ), "Move Up" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		const decString path( pPanel.GetImportConvo() );
		if( path.IsEmpty() ){
			return NULL;
		}
		
		decStringList list( pPanel.GetConversation()->GetImportConversationPath() );
		const int index = list.IndexOf( path );
		if( index < 1 ){
			return NULL;
		}
		
		list.Move( index, index - 1 );
		return new ceUConvoSetImportConvoPath( pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list );
	}
	
	virtual void Update( const ceConversation &conversation ){
		const int index = conversation.GetImportConversationPath().IndexOf( pPanel.GetImportConvo() );
		SetEnabled( ! pPanel.GetImportConvo().IsEmpty() && index > 0 );
	}
};

class cActionImportConvoMoveDown : public cBaseAction{
public:
	cActionImportConvoMoveDown( ceWPConversation &panel ) : cBaseAction( panel, "Move Down",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiUp ), "Move Down" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		const decString path( pPanel.GetImportConvo() );
		if( path.IsEmpty() ){
			return NULL;
		}
		
		decStringList list( pPanel.GetConversation()->GetImportConversationPath() );
		const int index = list.IndexOf( path );
		if( index == -1 || index < list.GetCount() - 1 ){
			return NULL;
		}
		
		list.Move( index, index + 1 );
		return new ceUConvoSetImportConvoPath( pPanel.GetWindowProperties().GetWindowMain()
			.GetLoadSaveSystem(), pPanel.GetConversation(), list );
	}
	
	virtual void Update( const ceConversation &conversation ){
		const decStringList &list = conversation.GetImportConversationPath();
		const int index = list.IndexOf( pPanel.GetImportConvo() );
		SetEnabled( ! pPanel.GetImportConvo().IsEmpty() && index != -1 && index < list.GetCount() - 1 );
	}
};

class cActionImportConvoClear : public cBaseAction{
public:
	cActionImportConvoClear( ceWPConversation &panel ) : cBaseAction( panel, "Remove All",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove All Imports" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		return pPanel.GetConversation() && pPanel.GetConversation()->GetImportConversationPath().GetCount() > 0
			? new ceUConvoSetImportConvoPath( pPanel.GetWindowProperties().GetWindowMain()
				.GetLoadSaveSystem(), pPanel.GetConversation(), decStringList() ) : NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetConversation() && pPanel.GetConversation()->GetImportConversationPath().GetCount() > 0 );
	}
};

class cListImportConvo : public igdeListBoxListener{
	ceWPConversation &pPanel;
public:
	cListImportConvo( ceWPConversation &panel ) : pPanel( panel ){ }
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( menu, new cActionImportConvoMoveUp( pPanel ), true );
		helper.MenuCommand( menu, new cActionImportConvoMoveDown( pPanel ), true );
		helper.MenuCommand( menu, new cActionImportConvoRemove( pPanel ), true );
		helper.MenuCommand( menu, new cActionImportConvoClear( pPanel ), true );
	}
};



class cComboTarget : public cBaseComboBoxListener{
public:
	cComboTarget( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation *conversation ){
		conversation->SetActiveTarget( comboBox.GetSelectedItem()
			? ( ceTarget* )comboBox.GetSelectedItem()->GetData() : NULL );
		return NULL;
	}
};

class cActionTargetAdd : public cBaseAction{
public:
	cActionTargetAdd( ceWPConversation &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Target" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		decString name( "Target" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Target", "Name:", name ) ){
			return NULL;
		}
		if( conversation->GetTargetList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Target", "Duplicate name" );
			return NULL;
		}
		
		const ceTarget::Ref target(ceTarget::Ref::NewWith(name));
		return new ceUCTargetAdd( conversation, target );
	}
};

class cActionTargetRemove : public cBaseAction{
public:
	cActionTargetRemove( ceWPConversation &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Target" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		return pPanel.GetTarget() ? new ceUCTargetRemove( pPanel.GetTarget() ) : NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetTarget() );
	}
};

class cActionTargetRename : public cBaseAction{
public:
	cActionTargetRename( ceWPConversation &panel ) : cBaseAction( panel,
	"Rename...", NULL, "Rename Target" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceTarget * const target = pPanel.GetTarget();
		if( ! target ){
			return NULL;
		}
		
		decString name( target->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Target", "Name:", name ) 
		|| name == target->GetName() ){
			return NULL;
		}
		if( conversation->GetTargetList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Target", "Duplicate name" );
			return NULL;
		}
		
		return new ceUCTargetSetName( target, name );
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetTarget() );
	}
};

class cActionTargetMenu : public cBaseActionContextMenu{
public:
	cActionTargetMenu( ceWPConversation &panel ) : cBaseActionContextMenu( panel, "Target menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionTargetAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionTargetRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionTargetRename( pPanel ), true );
	}
};

class cComboTargetActorID : public cBaseComboBoxListener{
public:
	cComboTargetActorID( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation* ){
		ceTarget * const target = pPanel.GetTarget();
		return target && comboBox.GetText() != target->GetActor()
			? new ceUCTargetSetActor( target, comboBox.GetText() ) : NULL;
	}
};

class cTextTargetEntityID : public cBaseTextFieldListener{
public:
	cTextTargetEntityID( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceTarget * const target = pPanel.GetTarget();
		return target && textField.GetText() != target->GetCoordSystem()
			? new ceUCTargetSetEntityID( target, textField.GetText() ) : NULL;
	}
};

class cTextTargetBone : public cBaseTextFieldListener{
public:
	cTextTargetBone( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceTarget * const target = pPanel.GetTarget();
		return target && textField.GetText() != target->GetBone()
			? new ceUCTargetSetBone( target, textField.GetText() ) : NULL;
	}
};

class cVectorTargetPosition : public cBaseEditVectorListener{
public:
	cVectorTargetPosition( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceTarget * const target = pPanel.GetTarget();
		return target && ! editVector.GetVector().IsEqualTo( target->GetPosition() )
			? new ceUCTargetSetPosition( target, editVector.GetVector() ) : NULL;
	}
};

class cVectorTargetOrientation : public cBaseEditVectorListener{
public:
	cVectorTargetOrientation( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceTarget * const target = pPanel.GetTarget();
		return target && ! editVector.GetVector().IsEqualTo( target->GetOrientation() )
			? new ceUCTargetSetOrientation( target, editVector.GetVector() ) : NULL;
	}
};



class cComboCameraShot : public cBaseComboBoxListener{
public:
	cComboCameraShot( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation *conversation ){
		conversation->SetActiveCameraShot( comboBox.GetSelectedItem()
			? ( ceCameraShot* )comboBox.GetSelectedItem()->GetData() : NULL );
		return NULL;
	}
};

class cActionCameraShotAdd : public cBaseAction{
public:
	cActionCameraShotAdd( ceWPConversation &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Camera Shot" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		decString name( "Camera Shot" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Camera Shot", "Name:", name ) ){
			return NULL;
		}
		if( conversation->GetCameraShotList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Camera Shot", "Duplicate name" );
			return NULL;
		}
		
		return new ceUCCShotAdd(conversation, ceCameraShot::Ref::NewWith(name));
	}
};

class cActionCameraShotRemove : public cBaseAction{
public:
	cActionCameraShotRemove( ceWPConversation &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Camera Shot" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		return pPanel.GetCameraShot() ? new ceUCCShotRemove( pPanel.GetCameraShot() ) : NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetCameraShot() );
	}
};

class cActionCameraShotRename : public cBaseAction{
public:
	cActionCameraShotRename( ceWPConversation &panel ) : cBaseAction( panel,
	"Rename...", NULL, "Rename Camera Shot" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		if( ! cameraShot ){
			return NULL;
		}
		
		decString name( cameraShot->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Camera Shot", "Name:", name ) 
		|| name == cameraShot->GetName() ){
			return NULL;
		}
		if( conversation->GetCameraShotList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Camera Shot", "Duplicate name" );
			return NULL;
		}
		
		return new ceUCCShotSetName( cameraShot, name );
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetCameraShot() );
	}
};

class cActionCameraDuplicate : public cBaseAction{
public:
	cActionCameraDuplicate( ceWPConversation &panel ) : cBaseAction( panel,
	"Duplicate...", NULL, "Duplicate Camera Shot" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		if( ! cameraShot ){
			return NULL;
		}
		
		decString name( cameraShot->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Duplicate Camera Shot", "Name:", name ) ){
			return NULL;
		}
		if( conversation->GetCameraShotList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Duplicate Camera Shot", "Duplicate name" );
			return NULL;
		}
		
		const ceCameraShot::Ref duplicate(ceCameraShot::Ref::NewWith(*cameraShot));
		( ( ceCameraShot& )( deObject& )duplicate ).SetName( name );
		return new ceUCCShotAdd( conversation, duplicate );
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetCameraShot() );
	}
};

class cActionCameraShotMenu : public cBaseActionContextMenu{
public:
	cActionCameraShotMenu( ceWPConversation &panel ) : cBaseActionContextMenu( panel, "Camera Shot menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionCameraShotAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionCameraShotRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionCameraShotRename( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionCameraDuplicate( pPanel ), true );
	}
};

class cTextCShotActorCount : public cBaseTextFieldListener{
public:
	cTextCShotActorCount( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const int value = decMath::max( textField.GetInteger(), 0 );
		return cameraShot && value != cameraShot->GetActorCount()
			? new ceUCCShotSetActorCount( cameraShot, value ) : NULL;
	}
};


class cComboCameraShotCameraTarget : public cBaseComboBoxListener{
public:
	cComboCameraShotCameraTarget( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && comboBox.GetText() != cameraShot->GetCameraTarget()
			? new ceUCCShotSetCameraTarget( cameraShot, comboBox.GetText() ) : NULL;
	}
};

class cVectorCShotOffCamFrom : public cBaseEditVectorListener{
public:
	cVectorCShotOffCamFrom( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetOffsetCameraFrom() )
			? new ceUCCShotSetOffCamFrom( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cVectorCShotOffCamTo : public cBaseEditVectorListener{
public:
	cVectorCShotOffCamTo( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetOffsetCameraTo() )
			? new ceUCCShotSetOffCamTo( cameraShot, editVector.GetVector() ) : NULL;
	}
};


class cComboCameraShotCameraLookAt : public cBaseComboBoxListener{
public:
	cComboCameraShotCameraLookAt( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && comboBox.GetText() != cameraShot->GetLookAtTarget()
			? new ceUCCShotSetLookAtTarget( cameraShot, comboBox.GetText() ) : NULL;
	}
};

class cVectorCShotOffLookAtFrom : public cBaseEditVectorListener{
public:
	cVectorCShotOffLookAtFrom( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetOffsetLookAtFrom() )
			? new ceUCCShotSetOffLookAtFrom( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cVectorCShotOffLookAtTo : public cBaseEditVectorListener{
public:
	cVectorCShotOffLookAtTo( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetOffsetLookAtTo() )
			? new ceUCCShotSetOffLookAtTo( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cVectorCShotCamOrbitFrom : public cBaseEditVectorListener{
public:
	cVectorCShotCamOrbitFrom( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetCameraOrbitFrom() )
			? new ceUCCShotSetCamOrbitFrom( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cVectorCShotCamOrbitTo : public cBaseEditVectorListener{
public:
	cVectorCShotCamOrbitTo( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetCameraOrbitTo() )
			? new ceUCCShotSetCamOrbitTo( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cTextCShotCamDistanceFrom : public cBaseTextFieldListener{
public:
	cTextCShotCamDistanceFrom( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf( value - cameraShot->GetCameraDistanceFrom() ) > FLOAT_SAFE_EPSILON
			? new ceUCCShotSetCamDistFrom( cameraShot, value ) : NULL;
	}
};

class cTextCShotCamDistanceTo : public cBaseTextFieldListener{
public:
	cTextCShotCamDistanceTo( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf( value - cameraShot->GetCameraDistanceTo() ) > FLOAT_SAFE_EPSILON
			? new ceUCCShotSetCamDistTo( cameraShot, value ) : NULL;
	}
};


class cVectorCShotPositionFrom : public cBaseEditVectorListener{
public:
	cVectorCShotPositionFrom( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetPositionFrom() )
			? new ceUCCShotSetPosFrom( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cVectorCShotPositionTo : public cBaseEditVectorListener{
public:
	cVectorCShotPositionTo( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetPositionTo() )
			? new ceUCCShotSetPosTo( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cVectorCShotRotationFrom : public cBaseEditVectorListener{
public:
	cVectorCShotRotationFrom( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetRotationFrom() )
			? new ceUCCShotSetRotFrom( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cVectorCShotRotationTo : public cBaseEditVectorListener{
public:
	cVectorCShotRotationTo( ceWPConversation &panel ) : cBaseEditVectorListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeEditVector &editVector, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot && ! editVector.GetVector().IsEqualTo( cameraShot->GetRotationTo() )
			? new ceUCCShotSetRotTo( cameraShot, editVector.GetVector() ) : NULL;
	}
};

class cTextCShotTiltFrom : public cBaseTextFieldListener{
public:
	cTextCShotTiltFrom( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf( value - cameraShot->GetTiltFrom() ) > FLOAT_SAFE_EPSILON
			? new ceUCCShotSetTiltFrom( cameraShot, value ) : NULL;
	}
};

class cTextCShotTiltTo : public cBaseTextFieldListener{
public:
	cTextCShotTiltTo( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf( value - cameraShot->GetTiltTo() ) > FLOAT_SAFE_EPSILON
			? new ceUCCShotSetTiltTo( cameraShot, value ) : NULL;
	}
};

class cTextCShotFovFrom : public cBaseTextFieldListener{
public:
	cTextCShotFovFrom( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf( value - cameraShot->GetFovFrom() ) > FLOAT_SAFE_EPSILON
			? new ceUCCShotSetFovFrom( cameraShot, value ) : NULL;
	}
};

class cTextCShotFovTo : public cBaseTextFieldListener{
public:
	cTextCShotFovTo( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		const float value = textField.GetFloat();
		return cameraShot && fabsf( value - cameraShot->GetFovTo() ) > FLOAT_SAFE_EPSILON
			? new ceUCCShotSetFovTo( cameraShot, value ) : NULL;
	}
};

class cActionCShotAlignTargets : public cBaseAction{
public:
	cActionCShotAlignTargets( ceWPConversation &panel ) : cBaseAction( panel,
		"Align Targets", NULL, "Targets are aligned to face each other" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? new ceUCCShotToggleAlignTargets( cameraShot ) : NULL;
	}
	
	virtual void Update(const ceConversation & ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled( cameraShot );
		SetSelected( cameraShot && cameraShot->GetAlignTargets() );
	}
};

class cActionCShotLockUpAxis : public cBaseAction{
public:
	cActionCShotLockUpAxis( ceWPConversation &panel ) : cBaseAction( panel,
		"Align Target Lock Up-Axis", NULL, "Target up axes are locked during aligning" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? new ceUCCShotToggleLockUpAxis( cameraShot ) : NULL;
	}
	
	virtual void Update(const ceConversation & ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled( cameraShot );
		SetSelected( cameraShot && cameraShot->GetLockUpAxis() );
	}
};

class cActionCShotRelativeToLookAt : public cBaseAction{
public:
	cActionCShotRelativeToLookAt( ceWPConversation &panel ) : cBaseAction( panel,
		"Anchor Relative to Look-At", NULL, "Camera is relative to the camera target or the look-at target" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? new ceUCCShotToggleRelativeToLookAt( cameraShot ) : NULL;
	}
	
	virtual void Update(const ceConversation & ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled( cameraShot );
		SetSelected( cameraShot && cameraShot->GetRelativeToLookAt() );
	}
};

class cActionCShotLockCameraTarget : public cBaseAction{
public:
	cActionCShotLockCameraTarget( ceWPConversation &panel ) : cBaseAction( panel,
		"Lock Target Camera", NULL, "Camera target position is locked during the entire camera shot" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? new ceUCCShotToggleLockCameraTarget( cameraShot ) : NULL;
	}
	
	virtual void Update(const ceConversation & ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled( cameraShot );
		SetSelected( cameraShot && cameraShot->GetLockCameraTarget() );
	}
};

class cActionCShotLockLookAtTarget : public cBaseAction{
public:
	cActionCShotLockLookAtTarget( ceWPConversation &panel ) : cBaseAction( panel,
		"Lock Target Look-At", NULL, "Look-at target position is locked during the entire camera shot" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		return cameraShot ? new ceUCCShotToggleLockLookAtTarget( cameraShot ) : NULL;
	}
	
	virtual void Update(const ceConversation & ){
		ceCameraShot * const cameraShot = pPanel.GetCameraShot();
		SetEnabled( cameraShot );
		SetSelected( cameraShot && cameraShot->GetLockLookAtTarget() );
	}
};



class cComboGesture : public cBaseComboBoxListener{
public:
	cComboGesture( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation *conversation ){
		conversation->SetActiveGesture( comboBox.GetSelectedItem()
			? ( ceGesture* )comboBox.GetSelectedItem()->GetData() : NULL );
		return NULL;
	}
};

class cActionGestureAdd : public cBaseAction{
public:
	cActionGestureAdd( ceWPConversation &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Gesture" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		decString name( "Gesture" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Gesture", "Name:", name ) ){
			return NULL;
		}
		if( conversation->GetGestureList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Gesture", "Duplicate name" );
			return NULL;
		}
		
		const ceGesture::Ref gesture(ceGesture::Ref::NewWith(name));
		return new ceUCGestureAdd( conversation, gesture );
	}
};

class cActionGestureRemove : public cBaseAction{
public:
	cActionGestureRemove( ceWPConversation &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Gesture" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		return pPanel.GetGesture() ? new ceUCGestureRemove( pPanel.GetGesture() ) : NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetGesture() );
	}
};

class cActionGestureRename : public cBaseAction{
public:
	cActionGestureRename( ceWPConversation &panel ) : cBaseAction( panel,
	"Rename...", NULL, "Rename Gesture" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceGesture * const gesture = pPanel.GetGesture();
		if( ! gesture ){
			return NULL;
		}
		
		decString name( gesture->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Gesture", "Name:", name ) 
		|| name == gesture->GetName() ){
			return NULL;
		}
		if( conversation->GetGestureList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Gesture", "Duplicate name" );
			return NULL;
		}
		
		return new ceUCGestureSetName( gesture, name );
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetGesture() );
	}
};

class cActionGestureMenu : public cBaseActionContextMenu{
public:
	cActionGestureMenu( ceWPConversation &panel ) : cBaseActionContextMenu( panel, "Gesture menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionGestureAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionGestureRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionGestureRename( pPanel ), true );
	}
};

class cTextGestureAnimator : public cBaseTextFieldListener{
public:
	cTextGestureAnimator( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceGesture * const gesture = pPanel.GetGesture();
		return gesture && textField.GetText() != gesture->GetAnimator()
			? new ceUCGestureSetAnimator( gesture, textField.GetText() ) : NULL;
	}
};

class cActionGestureHold : public cBaseAction{
public:
	cActionGestureHold( ceWPConversation &panel ) : cBaseAction( panel,
		"Hold Gesture", NULL, "Hold gesture after gesture finished" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceGesture * const gesture = pPanel.GetGesture();
		return gesture ? new ceUCGestureToggleHold( gesture ) : NULL;
	}
	
	virtual void Update(const ceConversation & ){
		ceGesture * const gesture = pPanel.GetGesture();
		SetEnabled( gesture );
		SetSelected( gesture && gesture->GetHold() );
	}
};

class cTextGestureDuration : public cBaseTextFieldListener{
public:
	cTextGestureDuration( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceGesture * const gesture = pPanel.GetGesture();
		const float value = textField.GetFloat();
		return gesture && fabsf( value - gesture->GetDuration() ) > FLOAT_SAFE_EPSILON
			? new ceUCGestureSetDuration( gesture, value ) : nullptr;
	}
};



class cComboFacePose : public cBaseComboBoxListener{
public:
	cComboFacePose( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox &comboBox, ceConversation *conversation ){
		conversation->SetActiveFacePose( comboBox.GetSelectedItem()
			? ( ceFacePose* )comboBox.GetSelectedItem()->GetData() : NULL );
		return NULL;
	}
};

class cActionFacePoseAdd : public cBaseAction{
public:
	cActionFacePoseAdd( ceWPConversation &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Face Pose" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		decString name( "FacePose" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Face Pose", "Name:", name ) ){
			return NULL;
		}
		if( conversation->GetFacePoseList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Face Pose", "Duplicate name" );
			return NULL;
		}
		
		const ceFacePose::Ref facePose(ceFacePose::Ref::NewWith(name));
		return new ceUCFacePoseAdd( conversation, facePose );
	}
};

class cActionFacePoseRemove : public cBaseAction{
public:
	cActionFacePoseRemove( ceWPConversation &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Face Pose" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		return pPanel.GetFacePose() ? new ceUCFacePoseRemove( pPanel.GetFacePose() ) : NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetFacePose() );
	}
};

class cActionFacePoseRename : public cBaseAction{
public:
	cActionFacePoseRename( ceWPConversation &panel ) : cBaseAction( panel,
	"Rename...", NULL, "Rename Face Pose" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceFacePose * const facePose = pPanel.GetFacePose();
		if( ! facePose ){
			return NULL;
		}
		
		decString name( facePose->GetName() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Face Pose", "Name:", name ) 
		|| name == facePose->GetName() ){
			return NULL;
		}
		if( conversation->GetFacePoseList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Face Pose", "Duplicate name" );
			return NULL;
		}
		
		return new ceUCFacePoseSetName( facePose, name );
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetFacePose() );
	}
};

class cActionFacePoseMenu : public cBaseActionContextMenu{
public:
	cActionFacePoseMenu( ceWPConversation &panel ) : cBaseActionContextMenu( panel, "Face Pose menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionFacePoseAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionFacePoseRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionFacePoseRename( pPanel ), true );
	}
};

class cComboFacePoseController : public cBaseComboBoxListener{
public:
	cComboFacePoseController( ceWPConversation &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeComboBox&, ceConversation* ){
		ceFacePose * const facePose = pPanel.GetFacePose();
		if( facePose ){
			pPanel.UpdateFPController();
		}
		return NULL;
	}
};

class cActionFacePoseControllerAdd : public cBaseAction{
public:
	cActionFacePoseControllerAdd( ceWPConversation &panel ) : cBaseAction( panel, "Add...",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add Face Pose Controller" ){ }
	
	virtual igdeUndo *OnAction( ceConversation *conversation ){
		ceFacePose * const facePose = pPanel.GetFacePose();
		if( ! facePose || conversation->GetFacePoseControllerNameList().GetCount() == 0 ){
			return NULL;
		}
		
		decStringList names( conversation->GetFacePoseControllerNameList() );
		names.SortAscending();
		decString name;
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Face Pose Controller", "Controller to add", name, names ) ){
			return NULL;
		}
		
		if( facePose->GetControllerList().HasNamed( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Face Pose Controller", "Duplicate controller" );
			return NULL;
		}
		
		const ceControllerValue::Ref controller(ceControllerValue::Ref::NewWith(name, 1.0f));
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCFPControllerAdd( facePose, controller ) );
		conversation->GetUndoSystem()->Add( undo );
		
		pPanel.SelectFacePoseController( controller );
		return NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetFacePose() );
	}
};

class cActionFacePoseControllerRemove : public cBaseAction{
public:
	cActionFacePoseControllerRemove( ceWPConversation &panel ) : cBaseAction( panel, "Remove",
	panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove Face Pose Controller" ){ }
	
	virtual igdeUndo *OnAction( ceConversation* ){
		ceFacePose * const facePose = pPanel.GetFacePose();
		ceControllerValue * const controller = pPanel.GetFacePoseController();
		return facePose && controller ? new ceUCFPControllerRemove( facePose, controller ) : NULL;
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pPanel.GetFacePoseController() );
	}
};

class cActionFacePoseControllerMenu : public cBaseActionContextMenu{
public:
	cActionFacePoseControllerMenu( ceWPConversation &panel ) :
	cBaseActionContextMenu( panel, "Face Pose Controller menu" ){ }
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu, ceConversation* ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		helper.MenuCommand( contextMenu, new cActionFacePoseControllerAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionFacePoseControllerRemove( pPanel ), true );
	}
};

class cTextFPControllerValue : public cBaseTextFieldListener{
public:
	cTextFPControllerValue( ceWPConversation &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, ceConversation* ){
		ceControllerValue * const controller = pPanel.GetFacePoseController();
		const float value = textField.GetFloat();
		return controller && fabsf( value - controller->GetValue() ) > FLOAT_SAFE_EPSILON
			? new ceUCFPControllerSetValue( pPanel.GetFacePose(), controller, value ) : NULL;
	}
};

}



// Class ceWPConversation
///////////////////////////

// Constructor, destructor
////////////////////////////

ceWPConversation::ceWPConversation( ceWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pConversation( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainer::Ref content, groupBox, groupBox2, form, formLine;
	igdeActionContextMenu *actionContextMenu;
	
	pListener = new ceWPConversationListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// import conversation
	helper.GroupBoxFlow( content, groupBox, "Import Conversation:", true, true );
	
	formLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	helper.EditPath( formLine, "Path to add to import list", *windowProperties.GetWindowMain()
		.GetLoadSaveSystem().GetConversationFilePatterns(), pPathImportConvo, NULL );
	helper.Button( formLine, new cActionImportConvoAdd( *this ), true );
	groupBox->AddChild( formLine );
	helper.ListBox( groupBox, 3, "Imported conversations", pListImportConvoPath, new cListImportConvo( *this ) );
	
	
	// targets
	helper.GroupBox( content, groupBox, "Targets:", true );
	
	helper.FormLineStretchFirst( groupBox, "Target:", "Target to edit", formLine );
	helper.ComboBoxFilter( formLine, "Target to edit", pCBTarget, new cComboTarget( *this ) );
	pCBTarget->SetDefaultSorter();
	actionContextMenu = new cActionTargetMenu( *this );
	helper.Button( formLine, pBtnTarget, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnTarget );
	
	helper.ComboBoxFilter( groupBox, "Actor:", true, "ID of the actor or empty to not use an actor by ID",
		pCBTargetActorID, new cComboTargetActorID( *this ) );
	pCBTargetActorID->SetDefaultSorter();
	
	helper.EditString( groupBox, "Coord-System:", "ID of coordinate system or empty to not use a coordinate system",
		pEditTargetEntityID, new cTextTargetEntityID( *this ) );
	helper.EditString( groupBox, "Bone:",
		"Name of the bone to use as coordinate frame or an empty string to use the origin",
		pEditTargetBone, new cTextTargetBone( *this ) );
	helper.EditVector( groupBox, "Position:", "Position relative to the target object",
		pEditTargetPosition, new cVectorTargetPosition( *this ) );
	helper.EditVector( groupBox, "Orientation:", "Orientation relative to the target object",
		pEditTargetOrientation, new cVectorTargetOrientation( *this ) );
	
	
	// camera shots
	helper.GroupBoxFlow( content, groupBox, "Camera Shots:", false, false );
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	helper.FormLineStretchFirst( form, "Camera Shot:", "Camera Shot to edit", formLine );
	helper.ComboBoxFilter( formLine, "Camera Shot to edit", pCBCameraShot, new cComboCameraShot( *this ) );
	pCBCameraShot->SetDefaultSorter();
	actionContextMenu = new cActionCameraShotMenu( *this );
	helper.Button( formLine, pBtnCameraShot, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnCameraShot );
	
	helper.EditInteger( form, "Actor Count:", "Number of actors required for this camera action",
		pEditCShotActorCount, new cTextCShotActorCount( *this ) );
	
	// camera shot camera target
	helper.GroupBox( groupBox, groupBox2, "Camera Target:", false );
	
	helper.ComboBoxFilter( groupBox2, "Target:", true,
		"Target the camera is attached to or empty string for static position",
		pCBCameraShotCameraTarget, new cComboCameraShotCameraTarget( *this ) );
	pCBCameraShotCameraTarget->SetDefaultSorter();
	
	helper.EditVector( groupBox2, "Offset From:", "Start offset relative to the target",
		pEditCShotOffCamFrom, new cVectorCShotOffCamFrom( *this ) );
	helper.EditVector( groupBox2, "Offset To:", "End offset relative to the target",
		pEditCShotOffCamTo, new cVectorCShotOffCamTo( *this ) );
	
	// camera shot look at target
	helper.GroupBox( groupBox, groupBox2, "Camera Look-At:", false );
	
	helper.ComboBoxFilter( groupBox2, "Look-At Target:", true,
		"Target to point the camera at or empty string for static position",
		pCBCameraShotLookAtTarget, new cComboCameraShotCameraLookAt( *this ) );
	pCBCameraShotLookAtTarget->SetDefaultSorter();
	
	helper.EditVector( groupBox2, "Offset From:", "Start offset relative to the look-at target",
		pEditCShotOffLookAtFrom, new cVectorCShotOffLookAtFrom( *this ) );
	helper.EditVector( groupBox2, "Offset To:", "End offset relative to the look-at target",
		pEditCShotOffLookAtTo, new cVectorCShotOffLookAtTo( *this ) );
	
	// camera shot camera target
	helper.GroupBox( groupBox, groupBox2, "Orbiting:", false );
	
	helper.EditVector( groupBox2, "Orbit From:", "Start rotation of the camera orbit",
		pEditCShotCamOrbitFrom, new cVectorCShotCamOrbitFrom( *this ) );
	helper.EditVector( groupBox2, "Orbit To:", "End rotation of the camera orbit",
		pEditCShotCamOrbitTo, new cVectorCShotCamOrbitTo( *this ) );
	
	helper.EditFloat( groupBox2, "Distance From:", "Start distance of the camera to the orbit position",
		pEditCShotCamDistFrom, new cTextCShotCamDistanceFrom( *this ) );
	helper.EditFloat( groupBox2, "Distance To:", "End distance of the camera to the orbit position",
		pEditCShotCamDistTo, new cTextCShotCamDistanceTo( *this ) );
	
	// camera shot camera target
	helper.GroupBox( groupBox, groupBox2, "Parameters:", false );
	
	helper.EditVector( groupBox2, "Position From:", "Start position",
		pEditCShotPosFrom, new cVectorCShotPositionFrom( *this ) );
	helper.EditVector( groupBox2, "Position To:", "End position",
		pEditCShotPosTo, new cVectorCShotPositionTo( *this ) );
	
	helper.EditVector( groupBox2, "Rotation From:", "Start rotation",
		pEditCShotRotFrom, new cVectorCShotRotationFrom( *this ) );
	helper.EditVector( groupBox2, "Rotation To:", "End rotation",
		pEditCShotRotTo, new cVectorCShotRotationTo( *this ) );
	
	helper.EditFloat( groupBox2, "Tilt From:", "Start tilt of the camera along the view axis",
		pEditCShotTiltFrom, new cTextCShotTiltFrom( *this ) );
	helper.EditFloat( groupBox2, "Tilt To:", "End tilt of the camera along the view axis",
		pEditCShotTiltTo, new cTextCShotTiltTo( *this ) );
	
	helper.EditFloat( groupBox2, "Fov From:", "Start fov of the camera along the view axis",
		pEditCShotFovFrom, new cTextCShotFovFrom( *this ) );
	helper.EditFloat( groupBox2, "Fov To:", "End fov of the camera along the view axis",
		pEditCShotFovTo, new cTextCShotFovTo( *this ) );
	
	helper.CheckBox( groupBox2, pChkCShotAlignTargets, new cActionCShotAlignTargets( *this ), true );
	helper.CheckBox( groupBox2, pChkCShotLockUpAxis, new cActionCShotLockUpAxis( *this ), true );
	helper.CheckBox( groupBox2, pChkCShotRelativeToLookAt, new cActionCShotRelativeToLookAt( *this ), true );
	helper.CheckBox( groupBox2, pChkCShotLockCameraTarget, new cActionCShotLockCameraTarget( *this ), true );
	helper.CheckBox( groupBox2, pChkCShotLockLookAtTarget, new cActionCShotLockLookAtTarget( *this ), true );
	
	
	// gestures
	helper.GroupBox( content, groupBox, "Gestures:", true );
	
	helper.FormLineStretchFirst( groupBox, "Gesture:", "Gesture to edit", formLine );
	helper.ComboBoxFilter( formLine, "Gesture to edit", pCBGesture, new cComboGesture( *this ) );
	pCBGesture->SetDefaultSorter();
	actionContextMenu = new cActionGestureMenu( *this );
	helper.Button( formLine, pBtnGesture, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnGesture );
	
	helper.EditString( groupBox, "Animator:", "Name of the animator to use for this gesture",
		pEditGestureAnimator, new cTextGestureAnimator( *this ) );
	
	helper.EditFloat( groupBox, "Duration:", "Duration of gesture. Used as default value in strips",
		pEditGestureDuration, new cTextGestureDuration ( *this ) );
	
	helper.CheckBox( groupBox, pChkGestureHold, new cActionGestureHold( *this ), true );
	
	
	// facePoses
	helper.GroupBox( content, groupBox, "Face Poses:", true );
	
	helper.FormLineStretchFirst( groupBox, "Face Pose:", "Face Pose to edit", formLine );
	helper.ComboBoxFilter( formLine, "Face Pose to edit", pCBFacePose, new cComboFacePose( *this ) );
	pCBFacePose->SetDefaultSorter();
	actionContextMenu = new cActionFacePoseMenu( *this );
	helper.Button( formLine, pBtnFacePose, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnFacePose );
	
	helper.FormLineStretchFirst( groupBox, "Controller:", "Face pose controller to edit", formLine );
	helper.ComboBox( formLine, "Face pose controller to edit", pCBFPController, new cComboFacePoseController( *this ) );
	pCBFPController->SetDefaultSorter();
	actionContextMenu = new cActionFacePoseControllerMenu( *this );
	helper.Button( formLine, pBtnFPController, actionContextMenu, true );
	actionContextMenu->SetWidget( pBtnFPController );
	
	helper.EditFloat( groupBox, "Value:", "Value of face pose controller",
		pEditFPControllerValue, new cTextFPControllerValue( *this ) );
}

ceWPConversation::~ceWPConversation(){
	SetConversation( NULL );
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void ceWPConversation::SetConversation( ceConversation *conversation ){
	if( conversation == pConversation ){
		return;
	}
	
	if( pConversation ){
		pConversation->RemoveListener( pListener );
		pConversation->FreeReference();
	}
	
	pConversation = conversation;
	
	if( conversation ){
		conversation->AddListener( pListener );
		conversation->AddReference();
	}
	
	const bool enabled = conversation;
	pCBGesture->SetEnabled( enabled );
	pCBFacePose->SetEnabled( enabled );
	
	pBtnGesture->GetAction()->Update();
	pBtnFacePose->GetAction()->Update();
	
	if( ! enabled ){
		pCBGesture->ClearText();
		pCBFacePose->ClearText();
	}
	
	UpdateTargetList();
	UpdateCameraShotList();
	UpdateConversation();
	UpdateGestureList();
	UpdateFacePoseList();
	
	UpdateActorIDLists();
	OnConversationPathChanged();
}



void ceWPConversation::UpdateConversation(){
	UpdateImportConvoPathList();
}

void ceWPConversation::OnConversationPathChanged(){
	if( pConversation ){
		pPathImportConvo->SetBasePath( pConversation->GetDirectoryPath() );
		
	}else{
		pPathImportConvo->SetBasePath( "" );
	}
}


void ceWPConversation::UpdateImportConvoPathList(){
	const decString selection( GetImportConvo() );
	
	pListImportConvoPath->RemoveAllItems();
	
	if( pConversation ){
		const decStringList &list = pConversation->GetImportConversationPath();
		const int count = list.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			pListImportConvoPath->AddItem( list.GetAt( i ) );
		}
	}
	
	if( ! selection.IsEmpty() ){
		pListImportConvoPath->SetSelection( pListImportConvoPath->IndexOfItem( selection ) );
	}
	if( pListImportConvoPath->GetSelection() == -1 && pListImportConvoPath->GetItemCount() > 0 ){
		pListImportConvoPath->SetSelection( 0 );
	}
}

const decString & ceWPConversation::GetPathImportConvo() const{
	return pPathImportConvo->GetPath();
}

decString ceWPConversation::GetImportConvo() const{
	return pListImportConvoPath->GetSelectedItem()
		? pListImportConvoPath->GetSelectedItem()->GetText() : decString();
}



ceGesture *ceWPConversation::GetGesture() const{
	return pConversation ? pConversation->GetActiveGesture() : NULL;
}

void ceWPConversation::UpdateGestureList(){
	ceGesture * const selection = GetGesture();
	
	pCBGesture->RemoveAllItems();
	
	if( pConversation ){
		const ceGestureList &list = pConversation->GetGestureList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceGesture * const gesture = list.GetAt( i );
			pCBGesture->AddItem( gesture->GetName(), NULL, gesture );
		}
		
		pCBGesture->SortItems();
		pCBGesture->StoreFilterItems();
	}
	
	if( pConversation ){
		pConversation->SetActiveGesture( selection );
	}
}

void ceWPConversation::SelectActiveGesture(){
	pCBGesture->SetSelectionWithData( GetGesture() );
	
	const bool enabled = GetGesture();
	pEditGestureAnimator->SetEnabled( enabled );
	
	if( ! enabled ){
		pEditGestureAnimator->ClearText();
	}
	
	UpdateGesture();
}

void ceWPConversation::UpdateGesture(){
	ceGesture * const gesture = GetGesture();
	if( gesture ){
		pEditGestureAnimator->SetText( gesture->GetAnimator() );
		pEditGestureDuration->SetFloat( gesture->GetDuration() );
	}
	
	pChkGestureHold->GetAction()->Update();
}



ceFacePose *ceWPConversation::GetFacePose() const{
	return pConversation ? pConversation->GetActiveFacePose() : NULL;
}

void ceWPConversation::UpdateFacePoseList(){
	ceFacePose * const selection = GetFacePose();
	
	pCBFacePose->RemoveAllItems();
	
	if( pConversation ){
		const ceFacePoseList &list = pConversation->GetFacePoseList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceFacePose * const facePose = list.GetAt( i );
			pCBFacePose->AddItem( facePose->GetName(), NULL, facePose );
		}
		
		pCBFacePose->SortItems();
		pCBFacePose->StoreFilterItems();
	}
	
	if( pConversation ){
		pConversation->SetActiveFacePose( selection );
	}
}

void ceWPConversation::SelectActiveFacePose(){
	pCBFacePose->SetSelectionWithData( GetFacePose() );
	
	const bool enabled = GetFacePose();
	pCBFPController->SetEnabled( enabled );
	
	if( ! enabled ){
		pCBFPController->ClearText();
	}
	
	UpdateFacePose();
}

void ceWPConversation::UpdateFacePose(){
	UpdateFPControllerList();
}

ceControllerValue *ceWPConversation::GetFacePoseController() const{
	ceFacePose * const facePose = GetFacePose();
	return facePose && pCBFPController->GetSelectedItem()
		? ( ceControllerValue* )pCBFPController->GetSelectedItem()->GetData() : NULL;
}

void ceWPConversation::UpdateFPControllerList(){
	ceFacePose * const facePose = GetFacePose();
	ceControllerValue * const controller = GetFacePoseController();
	
	pCBFPController->RemoveAllItems();
	
	if( facePose ){
		const decStringList &controllerNames = pConversation->GetFacePoseControllerNameList();
		const ceControllerValueList &list = facePose->GetControllerList();
		const int count = list.GetCount();
		decString text;
		int i;
		
		for( i=0; i<count; i++ ){
			ceControllerValue * const entry = list.GetAt( i );
			
			if( entry->GetControllerIndex() == -1 ){
				if( controllerNames.Has( entry->GetController() ) ){
					text = entry->GetController();
					
				}else{
					text.Format( "%s (missing)", entry->GetController().GetString() );
				}
				
			}else{ // deprecated
				text.Format( "%d (deprecated)", entry->GetControllerIndex() );
			}
			
			pCBFPController->AddItem( text, NULL, entry );
		}
	}
	
	pCBFPController->SetSelectionWithData( controller );
	if( pCBFPController->GetSelection() == -1 && pCBFPController->GetItemCount() > 0 ){
		pCBFPController->SetSelection( 0 );
	}
	UpdateFPController();
}

void ceWPConversation::SelectFacePoseController( ceControllerValue *controller ){
	pCBFPController->SetSelectionWithData( controller );
}

void ceWPConversation::UpdateFPController(){
	ceControllerValue * const controller = GetFacePoseController();
	
	if( controller ){
		pEditFPControllerValue->SetFloat( controller->GetValue() );
		
	}else{
		pEditFPControllerValue->ClearText();
	}
	
	pEditFPControllerValue->SetEnabled( controller );
}



ceCameraShot *ceWPConversation::GetCameraShot() const{
	return pConversation ? pConversation->GetActiveCameraShot() : NULL;
}

void ceWPConversation::UpdateCameraShotList(){
	ceCameraShot * const selection = GetCameraShot();
	
	pCBCameraShot->RemoveAllItems();
	
	if( pConversation ){
		const ceCameraShotList &list = pConversation->GetCameraShotList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceCameraShot * const cameraShot = list.GetAt( i );
			pCBCameraShot->AddItem( cameraShot->GetName(), NULL, cameraShot );
		}
		
		pCBCameraShot->SortItems();
		pCBCameraShot->StoreFilterItems();
	}
	
	if( pConversation ){
		pConversation->SetActiveCameraShot( selection );
	}
}

void ceWPConversation::SelectActiveCameraShot(){
	ceCameraShot * const cameraShot = GetCameraShot();
	
	pCBCameraShot->SetSelectionWithData( cameraShot );
	
	const bool enabled = cameraShot;
	pEditCShotActorCount->SetEnabled( enabled );
	pCBCameraShotCameraTarget->SetEnabled( enabled );
	pEditCShotOffCamFrom->SetEnabled( enabled );
	pEditCShotOffCamTo->SetEnabled( enabled );
	pEditCShotCamOrbitFrom->SetEnabled( enabled );
	pEditCShotCamOrbitTo->SetEnabled( enabled );
	pEditCShotCamDistFrom->SetEnabled( enabled );
	pEditCShotCamDistTo->SetEnabled( enabled );
	pCBCameraShotLookAtTarget->SetEnabled( enabled );
	pEditCShotOffLookAtFrom->SetEnabled( enabled );
	pEditCShotOffLookAtTo->SetEnabled( enabled );
	pEditCShotPosFrom->SetEnabled( enabled );
	pEditCShotPosTo->SetEnabled( enabled );
	pEditCShotRotFrom->SetEnabled( enabled );
	pEditCShotRotTo->SetEnabled( enabled );
	pEditCShotTiltFrom->SetEnabled( enabled );
	pEditCShotTiltTo->SetEnabled( enabled );
	pEditCShotFovFrom->SetEnabled( enabled );
	pEditCShotFovTo->SetEnabled( enabled );
	pChkCShotLockUpAxis->SetEnabled( enabled );
	pChkCShotAlignTargets->SetEnabled( enabled );
	pChkCShotRelativeToLookAt->SetEnabled( enabled );
	pChkCShotLockCameraTarget->SetEnabled( enabled );
	pChkCShotLockLookAtTarget->SetEnabled( enabled );
	
	if( ! enabled ){
		pEditCShotActorCount->ClearText();
		pEditCShotOffCamFrom->SetVector( decVector() );
		pEditCShotOffCamTo->SetVector( decVector() );
		pEditCShotCamOrbitFrom->SetVector( decVector() );
		pEditCShotCamOrbitTo->SetVector( decVector() );
		pEditCShotCamDistFrom->ClearText();
		pEditCShotCamDistTo->ClearText();
		pEditCShotOffLookAtFrom->SetVector( decVector() );
		pEditCShotOffLookAtTo->SetVector( decVector() );
		pEditCShotPosFrom->SetVector( decVector() );
		pEditCShotPosTo->SetVector( decVector() );
		pEditCShotRotFrom->SetVector( decVector() );
		pEditCShotRotTo->SetVector( decVector() );
		pEditCShotTiltFrom->ClearText();
		pEditCShotTiltTo->ClearText();
		pEditCShotFovFrom->ClearText();
		pEditCShotFovTo->ClearText();
	}
	
	UpdateCameraShot();
}

void ceWPConversation::UpdateCameraShot(){
	const ceCameraShot * const cameraShot = GetCameraShot();
	
	if( cameraShot ){
		pEditCShotActorCount->SetInteger( cameraShot->GetActorCount() );
		pCBCameraShotCameraTarget->SetText( cameraShot->GetCameraTarget() );
		pEditCShotOffCamFrom->SetVector( cameraShot->GetOffsetCameraFrom() );
		pEditCShotOffCamTo->SetVector( cameraShot->GetOffsetCameraTo() );
		pEditCShotCamOrbitFrom->SetVector( cameraShot->GetCameraOrbitFrom() );
		pEditCShotCamOrbitTo->SetVector( cameraShot->GetCameraOrbitTo() );
		pEditCShotCamDistFrom->SetFloat( cameraShot->GetCameraDistanceFrom() );
		pEditCShotCamDistTo->SetFloat( cameraShot->GetCameraDistanceTo() );
		pCBCameraShotLookAtTarget->SetText( cameraShot->GetLookAtTarget() );
		pEditCShotOffLookAtFrom->SetVector( cameraShot->GetOffsetLookAtFrom() );
		pEditCShotOffLookAtTo->SetVector( cameraShot->GetOffsetLookAtTo() );
		pEditCShotPosFrom->SetVector( cameraShot->GetPositionFrom() );
		pEditCShotPosTo->SetVector( cameraShot->GetPositionTo() );
		pEditCShotRotFrom->SetVector( cameraShot->GetRotationFrom() );
		pEditCShotRotTo->SetVector( cameraShot->GetRotationTo() );
		pEditCShotTiltFrom->SetFloat( cameraShot->GetTiltFrom() );
		pEditCShotTiltTo->SetFloat( cameraShot->GetTiltTo() );
		pEditCShotFovFrom->SetFloat( cameraShot->GetFovFrom() );
		pEditCShotFovTo->SetFloat( cameraShot->GetFovTo() );
	}
	
	pChkCShotLockUpAxis->GetAction()->Update();
	pChkCShotAlignTargets->GetAction()->Update();
	pChkCShotRelativeToLookAt->GetAction()->Update();
	pChkCShotLockCameraTarget->GetAction()->Update();
	pChkCShotLockLookAtTarget->GetAction()->Update();
}



ceTarget *ceWPConversation::GetTarget() const{
	return pConversation ? pConversation->GetActiveTarget() : NULL;
}

void ceWPConversation::UpdateTargetList(){
	ceTarget * const selection = GetTarget();
	
	pCBTarget->RemoveAllItems();
	
	if( pConversation ){
		const ceTargetList &list = pConversation->GetTargetList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceTarget * const target = list.GetAt( i );
			pCBTarget->AddItem( target->GetName(), NULL, target );
		}
		
		pCBTarget->SortItems();
		pCBTarget->StoreFilterItems();
	}
	
	if( pConversation ){
		pConversation->SetActiveTarget( selection );
	}
	
	// update lists containing targets
	const decString selCShotCamera( pCBCameraShotCameraTarget->GetText() );
	const decString selCShotLookAt( pCBCameraShotLookAtTarget->GetText() );
	
	pCBCameraShotCameraTarget->RemoveAllItems();
	pCBCameraShotLookAtTarget->RemoveAllItems();
	
	if( pConversation ){
		const ceTargetList list( pConversation->AllTargets() );
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( list.GetAt( i )->GetName().IsEmpty() ){
				continue;
			}
			pCBCameraShotCameraTarget->AddItem( list.GetAt( i )->GetName() );
			pCBCameraShotLookAtTarget->AddItem( list.GetAt( i )->GetName() );
		}
		
		pCBCameraShotCameraTarget->SortItems();
		pCBCameraShotLookAtTarget->SortItems();
	}
	
	pCBCameraShotCameraTarget->StoreFilterItems();
	pCBCameraShotLookAtTarget->StoreFilterItems();
	
	pCBCameraShotCameraTarget->SetText( selCShotCamera );
	pCBCameraShotLookAtTarget->SetText( selCShotLookAt );
}

void ceWPConversation::SelectActiveTarget(){
	pCBTarget->SetSelectionWithData( GetTarget() );
	
	const bool enabled = GetTarget();
	pCBTargetActorID->SetEnabled( enabled );
	pEditTargetEntityID->SetEnabled( enabled );
	pEditTargetBone->SetEnabled( enabled );
	pEditTargetPosition->SetEnabled( enabled );
	pEditTargetOrientation->SetEnabled( enabled );
	
	if( ! enabled ){
		pEditTargetEntityID->ClearText();
		pEditTargetBone->ClearText();
		pEditTargetPosition->SetVector( decVector() );
		pEditTargetOrientation->SetVector( decVector() );
	}
	
	UpdateTarget();
}

void ceWPConversation::UpdateTarget(){
	const ceTarget * const target = GetTarget();
	
	if( target ){
		pCBTargetActorID->SetText( target->GetActor() );
		pEditTargetEntityID->SetText( target->GetCoordSystem() );
		pEditTargetBone->SetText( target->GetBone() );
		pEditTargetPosition->SetVector( target->GetPosition() );
		pEditTargetOrientation->SetVector( target->GetOrientation() );
	}
}



void ceWPConversation::UpdateActorIDLists(){
	const decString selection( pCBTargetActorID->GetText() );
	
	pCBTargetActorID->RemoveAllItems();
	
	if( pConversation ){
		const ceConversationActorList &list = pConversation->GetActorList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( list.GetAt( i )->GetID().IsEmpty() ){
				continue;
			}
			
			const decString &id = list.GetAt( i )->GetID();
			if( ! pCBTargetActorID->HasItem( id ) ){
				pCBTargetActorID->AddItem( id );
			}
			
			const decString &aliasID = list.GetAt( i )->GetAliasID();
			if( ! aliasID.IsEmpty() && ! pCBTargetActorID->HasItem( aliasID ) ){
				pCBTargetActorID->AddItem( aliasID );
			}
		}
		
		pCBTargetActorID->SortItems();
		pCBTargetActorID->StoreFilterItems();
	}
	
	pCBTargetActorID->SetText( selection );
}
