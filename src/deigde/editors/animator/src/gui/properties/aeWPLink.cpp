/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeWPLink.h"
#include "aeWPLinkListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"
#include "../../animator/link/aeLink.h"
#include "../../clipboard/aeClipboardDataLink.h"
#include "../../undosys/link/aeULinkSetName.h"
#include "../../undosys/link/aeULinkSetController.h"
#include "../../undosys/link/aeULinkSetCurve.h"
#include "../../undosys/link/aeULinkAdd.h"
#include "../../undosys/link/aeULinkRemove.h"
#include "../../undosys/link/aeULinkSetRepeat.h"
#include "../../undosys/link/aeULinkSetBone.h"
#include "../../undosys/link/aeULinkSetBoneParameter.h"
#include "../../undosys/link/aeULinkSetBoneMinimum.h"
#include "../../undosys/link/aeULinkSetBoneMaximum.h"
#include "../../undosys/link/aeULinkToggleWrapY.h"
#include "../../undosys/link/aeULinkPaste.h"

#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeSpinTextFieldListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPLink &pPanel;
	
public:
	cBaseAction( aeWPLink &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeLink * const link = pPanel.GetLink();
		if( ! animator || ! link ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator, link ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator, aeLink *link ) = 0;
	
	virtual void Update(){
		aeAnimator * const animator = pPanel.GetAnimator();
		aeLink * const link = pPanel.GetLink();
		if( animator && link ){
			Update( *animator, *link );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const aeAnimator &, const aeLink & ){
		SetEnabled( true );
		SetSelected( false );
	}
};


class cActionCopy : public cBaseAction{
public:
	cActionCopy( aeWPLink &panel ) : cBaseAction( panel, "Copy",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ),
		"Copy link to clipboard" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeLink *link ){
		igdeClipboardDataReference cdata;
		cdata.TakeOver( new aeClipboardDataLink( link ) );
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( cdata );
		return nullptr;
	}
};

class cActionCut : public cBaseAction{
public:
	cActionCut( aeWPLink &panel ) : cBaseAction( panel, "Cut",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ),
		"Cut link into clipboard" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeLink *link ){
		igdeClipboardDataReference cdata;
		cdata.TakeOver( new aeClipboardDataLink( link ) );
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( cdata );
		return new aeULinkRemove( link );
	}
};

class cActionPaste : public igdeAction{
	aeWPLink &pPanel;
	
public:
	cActionPaste( aeWPLink &panel ) : igdeAction( "Paste",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ),
		"Paste link from clipboard" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( ! animator ){
			return;
		}
		
		aeClipboardDataLink * const cdata = ( aeClipboardDataLink* )pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName( aeClipboardDataLink::TYPE_NAME );
		if( ! cdata ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkPaste( animator, cdata->GetLinks() ) );
		animator->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.GetWithTypeName( aeClipboardDataLink::TYPE_NAME ) && pPanel.GetAnimator() );
	}
};

class cListLinks : public igdeListBoxListener{
	aeWPLink &pPanel;
	
public:
	cListLinks( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		if( pPanel.GetAnimator() ){
			pPanel.GetAnimator()->SetActiveLink( listBox->GetSelectedItem()
				? ( aeLink* )listBox->GetSelectedItem()->GetData() : NULL );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		const aeWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, windowMain.GetActionLinkAdd() );
		helper.MenuCommand( menu, windowMain.GetActionLinkDuplicate() );
		helper.MenuCommand( menu, windowMain.GetActionLinkRemove() );
		helper.MenuCommand( menu, windowMain.GetActionLinkRemoveUnused() );
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionCopy( pPanel ), true );
		helper.MenuCommand( menu, new cActionCut( pPanel ), true );
		helper.MenuCommand( menu, new cActionPaste( pPanel ), true );
	}
};

class cTextName : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	cTextName( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const decString value = textField->GetText();
		aeLink * const link = pPanel.GetLink();
		if( ! link || link->GetName() == value ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetName( link, value ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};
 
class cComboBone : public igdeComboBoxListener{
	aeWPLink &pPanel;
	bool &pPreventUpdate;
	
public:
	cComboBone( aeWPLink &panel, bool &preventUpdate ) :
		pPanel( panel ), pPreventUpdate( preventUpdate ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( pPreventUpdate ){
			return;
		}
		
		const decString &bone = comboBox->GetText();
		aeLink * const link = pPanel.GetLink();
		if( ! link || link->GetBone() == bone ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetBone( link, bone ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};

class cComboConnectionController : public igdeComboBoxListener{
	aeWPLink &pPanel;
	bool &pPreventUpdate;
	
public:
	cComboConnectionController( aeWPLink &panel, bool &preventUpdate ) :
		pPanel( panel ), pPreventUpdate( preventUpdate ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		if( pPreventUpdate ){
			return;
		}
		
		aeController * const controller = comboBox->GetSelectedItem()
			? ( aeController* )comboBox->GetSelectedItem()->GetData() : NULL;
		aeLink * const link = pPanel.GetLink();
		if( ! link || link->GetController() == controller ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetController( link, controller ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};

class cSpinRepeat : public igdeSpinTextFieldListener{
	aeWPLink &pPanel;
	
public:
	cSpinRepeat( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnValueChanged( igdeSpinTextField *textField ){
		const int value = textField->GetValue();
		aeLink * const link = pPanel.GetLink();
		if( ! link || link->GetRepeat() == value ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetRepeat( link, value ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};

class cActionCurveInsertAt : public cBaseAction{
public:
	cActionCurveInsertAt( aeWPLink &panel ) : cBaseAction( panel, "Insert Value At Controller",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
		"Insert value at X coordinate matching linked controller value" ){}
	
	virtual igdeUndo *OnAction( aeAnimator*, aeLink *link ){
		if( ! link->GetController() ){
			return NULL;
		}
		
		const aeController &controller = *link->GetController();
		const float x = decMath::linearStep( controller.GetCurrentValue(),
			controller.GetMinimumValue(), controller.GetMaximumValue() );
		float y = 0.0f;
		if( ! igdeCommonDialogs::GetFloat( &pPanel, "Insert Curve Value", "Y Value:", y ) ){
			return NULL;
		}
		
		decCurveBezier curve( link->GetCurve() );
		curve.AddPoint( decVector2( x, y ) );
		return new aeULinkSetCurve( link, curve );
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	aeWPLink &pPanel;
	igdeUndoReference pUndo;
	
public:
	cEditCurve( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnCurveChanged( igdeViewCurveBezier *viewCurveBezier ){
		if( pUndo ){
			( ( aeULinkSetCurve& )( igdeUndo& )pUndo ).SetNewCurve( viewCurveBezier->GetCurve() );
			
		}else if( ! pPanel.GetLink() || pPanel.GetLink()->GetCurve() == viewCurveBezier->GetCurve() ){
			return;
			
		}else{
			pUndo.TakeOver( new aeULinkSetCurve( pPanel.GetLink(), viewCurveBezier->GetCurve() ) );
		}
		
		pPanel.GetAnimator()->GetUndoSystem()->Add( pUndo );
		pUndo = NULL;
	}
	
	virtual void OnCurveChanging( igdeViewCurveBezier *viewCurveBezier ){
		if( pUndo ){
			( ( aeULinkSetCurve& )( igdeUndo& )pUndo ).SetNewCurve( viewCurveBezier->GetCurve() );
			pUndo->Redo();
			
		}else if( pPanel.GetLink() && pPanel.GetLink()->GetCurve() != viewCurveBezier->GetCurve() ){
			pUndo.TakeOver( new aeULinkSetCurve( pPanel.GetLink(), viewCurveBezier->GetCurve() ) );
		}
	}
	
	virtual void AddContextMenuEntries( igdeViewCurveBezier*, igdeMenuCascade &menu ){
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionCurveInsertAt( pPanel ), true );
	}
};

class cTextBone : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	cTextBone( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const decString value( textField->GetText() );
		aeLink * const link = pPanel.GetLink();
		if( ! link || link->GetBone() == value ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetBone( link, value ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};

class cComboBoneParameter : public igdeComboBoxListener{
	aeWPLink &pPanel;
	
public:
	cComboBoneParameter( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		deAnimatorLink::eBoneParameter parameter = comboBox->GetSelectedItem()
			? ( deAnimatorLink::eBoneParameter )( intptr_t )comboBox->GetSelectedItem()->GetData()
			: deAnimatorLink::ebpPositionZ;
		aeLink * const link = pPanel.GetLink();
		if( ! link || link->GetBoneParameter() == parameter ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetBoneParameter( link, parameter ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};

class cTextBoneMinimum : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	cTextBoneMinimum( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		aeLink * const link = pPanel.GetLink();
		if( ! link || fabsf( link->GetBoneMinimum() - value ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetBoneMinimum( link, value ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};

class cTextBoneMaximum : public igdeTextFieldListener{
	aeWPLink &pPanel;
	
public:
	cTextBoneMaximum( aeWPLink &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		const float value = textField->GetFloat();
		aeLink * const link = pPanel.GetLink();
		if( ! link || fabsf( link->GetBoneMaximum() - value ) < FLOAT_SAFE_EPSILON ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeULinkSetBoneMaximum( link, value ) );
		if( undo ){
			pPanel.GetAnimator()->GetUndoSystem()->Add( undo );
		}
	}
};


class cCheckWrapY : public cBaseAction{
public:
	cCheckWrapY( aeWPLink &panel ) : cBaseAction( panel, "Wrap Y", nullptr,
		"Wrap Y value instead of clamping" ){ }
	
	virtual igdeUndo *OnAction( aeAnimator*, aeLink *link ){
		return new aeULinkToggleWrapY( link );
	}
	
	virtual void Update( const aeAnimator &animator, const aeLink &link ){
		cBaseAction::Update( animator, link );
		SetSelected( link.GetWrapY() );
	}
};
}


// Class aeWPLink
///////////////////

// Constructor, destructor
////////////////////////////

aeWPLink::aeWPLink( aeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pAnimator( NULL ),
pPreventUpdate( false )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, formLine;
	
	pListener = new aeWPLinkListener( *this );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// links
	helper.GroupBoxFlow( content, groupBox, "Links:" );
	helper.ListBox( groupBox, 8, "Links", pListLink, new cListLinks( *this ) );
	pListLink->SetDefaultSorter();
	
	
	// link settings
	helper.GroupBox( content, groupBox, "Link Settings:" );
	helper.EditString( groupBox, "Name:", "Name of the link", pEditName, new cTextName( *this ) );
	
	helper.ComboBox( groupBox, "Controller:", "Sets the controller to query values from",
		pCBController, new cComboConnectionController( *this, pPreventUpdate ) );
	pCBController->SetDefaultSorter();
	
	helper.EditSpinInteger( groupBox, "Repeat:", "Repeat curve along X direction", 1, 99,
		pSpinRepeat, new cSpinRepeat( *this ) );
	
	helper.ComboBoxFilter( groupBox, "Bone:", true,
		"Set bone to use parameter of as input or empty string to not use",
		pCBBone, new cComboBone( *this, pPreventUpdate ) );
	pCBBone->SetDefaultSorter();
	
	helper.ComboBox( groupBox, "Bone Parameter:", "Set bone parameter to use as input",
		pCBBoneParameter, new cComboBoneParameter( *this ) );
	pCBBoneParameter->AddItem( "Position X", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpPositionX );
	pCBBoneParameter->AddItem( "Position Y", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpPositionY );
	pCBBoneParameter->AddItem( "Position Z", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpPositionZ );
	pCBBoneParameter->AddItem( "Rotation X", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpRotationX );
	pCBBoneParameter->AddItem( "Rotation Y", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpRotationY );
	pCBBoneParameter->AddItem( "Rotation Z", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpRotationZ );
	pCBBoneParameter->AddItem( "Scale X", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpScaleX );
	pCBBoneParameter->AddItem( "Scale Y", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpScaleY );
	pCBBoneParameter->AddItem( "Scale Z", nullptr, ( void* )( intptr_t )deAnimatorLink::ebpScaleZ );
	
	helper.EditFloat( groupBox, "Bone Minimum Value:", "Minimum bone value",
		pEditBoneMinimum, new cTextBoneMinimum( *this ) );
	helper.EditFloat( groupBox, "Bone Maximum Value:", "Maximum bone value",
		pEditBoneMaximum, new cTextBoneMaximum( *this ) );
	
	helper.CheckBox( groupBox, pChkWrapY, new cCheckWrapY( *this ), true );
	
	
	helper.GroupBoxFlow( content, groupBox, "Link Curve:" );
	
	helper.ViewCurveBezier( groupBox, pEditCurve, new cEditCurve( *this ) );
	pEditCurve->SetDefaultSize( decPoint( 200, 250 ) );
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp( true );
	pEditCurve->SetClampMin( decVector2( 0.0f, 0.0f ) );
	pEditCurve->SetClampMax( decVector2( 1.0f, 1.0f ) );
}

aeWPLink::~aeWPLink(){
	SetAnimator( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void aeWPLink::SetAnimator( aeAnimator *animator ){
	if( animator == pAnimator ){
		return;
	}
	
	if( pAnimator ){
		pAnimator->RemoveNotifier( pListener );
		pAnimator->FreeReference();
	}
	
	pAnimator = animator;
	
	if( animator ){
		animator->AddNotifier( pListener );
		animator->AddReference();
	}
	
	UpdateRigBoneList();
	UpdateControllerList();
	UpdateLinkList();
}

aeLink *aeWPLink::GetLink() const{
	return pAnimator ? pAnimator->GetActiveLink() : NULL;
}

void aeWPLink::SelectActiveLink(){
	pListLink->SetSelectionWithData( GetLink() );
}

void aeWPLink::UpdateLinkList(){
	aeLink * const selection = GetLink();
	
	pListLink->RemoveAllItems();
	
	if( pAnimator ){
		const aeLinkList &list = pAnimator->GetLinks();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			aeLink * const link = list.GetAt( i );
			pListLink->AddItem( link->GetName(), NULL, link );
		}
		pListLink->SortItems();
	}
	
	pListLink->SetSelectionWithData( selection );
	if( ! pListLink->GetSelectedItem() && pListLink->GetItemCount() > 0 ){
		pListLink->SetSelection( 0 );
	}
	
	UpdateLink();
}

void aeWPLink::UpdateLink(){
	const aeLink * const link = GetLink();
	
	if( link ){
		pEditName->SetText( link->GetName() );
		pCBController->SetSelectionWithData( link->GetController() );
		pSpinRepeat->SetValue( link->GetRepeat() );
		pCBBone->SetText( link->GetBone() );
		pCBBoneParameter->SetSelectionWithData( ( void* )( intptr_t )link->GetBoneParameter() );
		pEditBoneMinimum->SetFloat( link->GetBoneMinimum() );
		pEditBoneMaximum->SetFloat( link->GetBoneMaximum() );
		pEditCurve->SetCurve( link->GetCurve() );
		
	}else{
		pEditName->ClearText();
		pCBController->SetSelectionWithData( NULL );
		pSpinRepeat->SetValue( 1 );
		pCBBone->ClearText();
		pCBBoneParameter->SetSelectionWithData( ( void* )( intptr_t )deAnimatorLink::ebpPositionZ );
		pEditBoneMinimum->ClearText();
		pEditBoneMaximum->ClearText();
		pEditCurve->ClearCurve();
	}
	
	const bool enabled = link;
	pEditName->SetEnabled( enabled );
	pCBController->SetEnabled( enabled );
	pSpinRepeat->SetEnabled( enabled );
	pCBBone->SetEnabled( enabled );
	pCBBoneParameter->SetEnabled( enabled );
	pEditBoneMinimum->SetEnabled( enabled );
	pEditBoneMaximum->SetEnabled( enabled );
	pEditCurve->SetEnabled( enabled );
	
	pChkWrapY->GetAction()->Update();
}

void aeWPLink::UpdateRigBoneList(){
	const decString selection( pCBBone->GetText() );
	
	pPreventUpdate = true;
	try{
		pCBBone->RemoveAllItems();
		
		if( pAnimator ){
			const deRig * const rig = pAnimator->GetEngineRig();
			if( rig ){
				const int count = rig->GetBoneCount();
				int i;
				for( i=0; i<count; i++ ){
					pCBBone->AddItem( rig->GetBoneAt( i ).GetName() );
				}
			}
			pCBBone->SortItems();
		}
		
		pCBBone->StoreFilterItems();
		pCBBone->SetText( selection );
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}

void aeWPLink::UpdateControllerList(){
	aeController * const selection = pCBController->GetSelectedItem()
		? ( aeController* )pCBController->GetSelectedItem()->GetData() : NULL;
	
	pPreventUpdate = true;
	try{
		pCBController->RemoveAllItems();
		pCBController->AddItem( "< No Controller >", NULL, NULL );
		
		if( pAnimator ){
			const aeControllerList &list = pAnimator->GetControllers();
			const int count = list.GetCount();
			decString text;
			int i;
			
			for( i=0; i<count; i++ ){
				aeController * const controller = list.GetAt( i );
				text.Format( "%d: %s", i, controller->GetName().GetString() );
				pCBController->AddItem( text, NULL, controller );
			}
		}
		
		pCBController->SetSelectionWithData( selection );
		pPreventUpdate = false;
		
	}catch( const deException & ){
		pPreventUpdate = false;
		throw;
	}
}
