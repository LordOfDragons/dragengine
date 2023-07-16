/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "seWPMapped.h"
#include "seWPMappedListener.h"
#include "seWindowProperties.h"
#include "../seWindowMain.h"
#include "../../skin/mapped/seMapped.h"
#include "../../clipboard/seClipboardDataMapped.h"
#include "../../undosys/mapped/seUMappedSetBone.h"
#include "../../undosys/mapped/seUMappedSetCurve.h"
#include "../../undosys/mapped/seUMappedSetInputLower.h"
#include "../../undosys/mapped/seUMappedSetInputType.h"
#include "../../undosys/mapped/seUMappedSetInputUpper.h"
#include "../../undosys/mapped/seUMappedSetName.h"
#include "../../undosys/mapped/seUMappedToggleInputClamped.h"
#include "../../undosys/mapped/seUMappedPaste.h"
#include "../../undosys/mapped/seUMappedRemove.h"
#include "../../undosys/mapped/seUMappedDuplicate.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/video/deVideo.h>



// Actions
////////////

namespace{

class cBaseActionSkin : public igdeAction{
protected:
	seWPMapped &pPanel;
	
public:
	cBaseActionSkin( seWPMapped &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ), pPanel( panel ){ }
	
	virtual void OnAction() override{
		seSkin * const skin = pPanel.GetSkin();
		if( ! skin ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( skin ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( seSkin *skin ) = 0;
	
	virtual void Update() override{
		seSkin * const skin = pPanel.GetSkin();
		if( skin ){
			Update( *skin );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const seSkin & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseActionMapped : public cBaseActionSkin{
public:
	cBaseActionMapped( seWPMapped &panel, const char *text, igdeIcon *icon, const char *description ) :
	cBaseActionSkin( panel, text, icon, description ){ }
	
	virtual igdeUndo *OnAction( seSkin *skin ) override{
		seMapped * const mapped = pPanel.GetMapped();
		return mapped ? OnActionMapped( skin, mapped ) : nullptr;
	}
	
	virtual igdeUndo *OnActionMapped( seSkin *skin, seMapped *mapped ) = 0;
	
	virtual void Update( const seSkin &skin ) override{
		seMapped * const mapped = pPanel.GetMapped();
		if( mapped ){
			UpdateMapped( skin, *mapped );
			
		}else{
			cBaseActionSkin::Update( skin );
		}
	}
	
	virtual void UpdateMapped( const seSkin &, const seMapped & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	seWPMapped &pPanel;
	
public:
	cBaseTextFieldListener( seWPMapped &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if( ! skin || ! mapped ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( *textField, skin, mapped ) );
		if( undo ){
			skin->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin *skin, seMapped *mapped ) = 0;
};


class cActionCopy : public cBaseActionMapped{
public:
	cActionCopy( seWPMapped &panel ) : cBaseActionMapped ( panel, "Copy Mapped",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ), "Copy mapped" ){}
	
	virtual igdeUndo *OnActionMapped( seSkin*, seMapped *mapped ) override{
		igdeClipboardDataReference data;
		seMappedList list;
		list.Add( mapped );
		data.TakeOver( new seClipboardDataMapped( list ) );
		
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( data );
		return nullptr;
	}
};

class cActionCut : public cBaseActionMapped{
public:
	cActionCut( seWPMapped &panel ) : cBaseActionMapped ( panel, "Cut Mapped",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCut ), "Cut mapped" ){}
	
	virtual igdeUndo *OnActionMapped( seSkin*, seMapped *mapped ) override{
		igdeClipboardDataReference data;
		seMappedList list;
		list.Add( mapped );
		data.TakeOver( new seClipboardDataMapped( list ) );
		
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( data );
		
		seUMappedRemove * const undo = new seUMappedRemove( mapped );
		
		if( undo->GetDependencyCount() > 0 && igdeCommonDialogs::QuestionFormat( &pPanel,
		igdeCommonDialogs::ebsYesNo, "Cut Mapped", "Mapped is used by %d dependencies. "
		"Cutting mapped will also unset it from all dependencies.", undo->GetDependencyCount() )
		== igdeCommonDialogs::ebNo ){
			undo->FreeReference();
			return nullptr;
		}
		
		return undo;
	}
};

class cActionPaste : public cBaseActionSkin{
public:
	cActionPaste( seWPMapped &panel ) : cBaseActionSkin( panel, "Paste Mapped",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ), "Paste mapped" ){}
	
	virtual igdeUndo *OnAction( seSkin *skin ) override{
		const seClipboardDataMapped * const data = ( seClipboardDataMapped* )
			pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			GetWithTypeName( seClipboardDataMapped::TYPE_NAME );
		return data ? new seUMappedPaste( skin, *data ) : nullptr;
	}
	
	virtual void Update( const seSkin & ) override{
		SetEnabled( pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName( seClipboardDataMapped::TYPE_NAME ) );
	}
};

class cActionDuplicate : public cBaseActionMapped{
public:
	cActionDuplicate( seWPMapped &panel ) : cBaseActionMapped( panel, "Duplicate Mapped",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiDuplicate ), "Duplicate mapped" ){}
	
	virtual igdeUndo *OnActionMapped( seSkin *skin, seMapped *mapped ) override{
		return new seUMappedDuplicate( skin, *mapped );
	}
};

class cListMapped : public igdeListBoxListener{
	seWPMapped &pPanel;
public:
	cListMapped( seWPMapped &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ) override{
		seSkin * const skin = pPanel.GetSkin();
		if( skin ){
			skin->SetActiveMapped( listBox->GetSelectedItem()
				? ( seMapped* )listBox->GetSelectedItem()->GetData() : nullptr );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ) override{
		seWindowMain &windowMain = pPanel.GetWindowProperties().GetWindowMain();
		igdeEnvironment &env = menu.GetEnvironment();
		igdeUIHelper &helper = env.GetUIHelper();
		
		helper.MenuCommand( menu, windowMain.GetActionMappedAdd() );
		helper.MenuCommand( menu, windowMain.GetActionMappedRemove() );
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionCopy( pPanel ), true );
		helper.MenuCommand( menu, new cActionCut( pPanel ), true );
		helper.MenuCommand( menu, new cActionPaste( pPanel ), true );
		
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionDuplicate( pPanel ), true );
	}
};

class cTextName : public igdeTextFieldListener{
	seWPMapped &pPanel;
public:
	cTextName( seWPMapped &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ) override{
		seMapped * const mapped = pPanel.GetMapped();
		const decString &value = textField->GetText();
		if( ! mapped || mapped->GetName() == value ){
			return;
		}
		
		if( pPanel.GetSkin()->GetMappedList().HasNamed( value ) ){
			igdeCommonDialogs::Error( &pPanel, "Set Mapped Name", "Mapped name exists already" );
			textField->SetText( mapped->GetName() );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUMappedSetName( mapped, value ) );
		pPanel.GetSkin()->GetUndoSystem()->Add( undo );
	}
};

class cComboInputType : public igdeComboBoxListener{
	seWPMapped &pPanel;
public:
	cComboInputType( seWPMapped &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if( ! skin || ! mapped || ! comboBox->GetSelectedItem() ){
			return;
		}
		
		const deSkinMapped::eInputTypes type = ( deSkinMapped::eInputTypes )
			( intptr_t )comboBox->GetSelectedItem()->GetData();
		if( type == mapped->GetInputType() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new seUMappedSetInputType( mapped, type ) );
		skin->GetUndoSystem()->Add( undo );
	}
};

class cEditCurve : public igdeViewCurveBezierListener{
	seWPMapped &pPanel;
	seUMappedSetCurve::Ref pUndo;
	
public:
	cEditCurve( seWPMapped &panel ) : pPanel( panel ){ }
	
	virtual void OnCurveChanged( igdeViewCurveBezier *viewCurveBezier ) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if( ! skin || ! mapped ){
			return;
		}
		
		if( pUndo ){
			pUndo->SetCurve( viewCurveBezier->GetCurve() );
			
		}else{
			if( mapped->GetCurve() == viewCurveBezier->GetCurve() ){
				return;
			}
			pUndo.TakeOver( new seUMappedSetCurve( mapped, viewCurveBezier->GetCurve() ) );
		}
		
		skin->GetUndoSystem()->Add( pUndo );
		pUndo = nullptr;
	}
	
	virtual void OnCurveChanging( igdeViewCurveBezier *viewCurveBezier ) override{
		seSkin * const skin = pPanel.GetSkin();
		seMapped * const mapped = pPanel.GetMapped();
		if( ! skin || ! mapped ){
			return;
		}
		
		if( pUndo ){
			pUndo->SetCurve( viewCurveBezier->GetCurve() );
			pUndo->Redo();
			
		}else{
			if( mapped->GetCurve() == viewCurveBezier->GetCurve() ){
				return;
			}
			pUndo.TakeOver( new seUMappedSetCurve( mapped, viewCurveBezier->GetCurve() ) );
		}
	}
};

class cTextInputLower : public cBaseTextFieldListener{
public:
	cTextInputLower( seWPMapped &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seMapped *mapped ) override{
		const float value = textField.GetFloat();
		return fabsf( value - mapped->GetInputLower() ) > FLOAT_SAFE_EPSILON
			? new seUMappedSetInputLower( mapped, value ) : nullptr;
	}
};

class cTextInputUpper : public cBaseTextFieldListener{
public:
	cTextInputUpper( seWPMapped &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seMapped *mapped ) override{
		const float value = textField.GetFloat();
		return fabsf( value - mapped->GetInputUpper() ) > FLOAT_SAFE_EPSILON
			? new seUMappedSetInputUpper( mapped, value ) : nullptr;
	}
};

class cActionInputClamped : public cBaseActionMapped {
public:
	cActionInputClamped( seWPMapped &panel ) : cBaseActionMapped ( panel,
		"Input Clamped:", nullptr, "Input value is clamperd to input range instead od wrapping around" ){ }
	
	virtual igdeUndo *OnActionMapped( seSkin*, seMapped *mapped ) override{
		return new seUMappedToggleInputClamped( mapped );
	}
	
	virtual void UpdateMapped( const seSkin &skin, const seMapped &mapped ) override{
		cBaseActionMapped::UpdateMapped( skin, mapped );
		SetSelected( mapped.GetInputClamped() );
	}
};

class cTextBone : public cBaseTextFieldListener{
public:
	cTextBone( seWPMapped &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField &textField, seSkin*, seMapped *mapped ) override{
		const decString &bone = textField.GetText();
		return bone != mapped->GetBone() ? new seUMappedSetBone( mapped, bone ) : nullptr;
	}
};

}



// Class seWPMapped
/////////////////////

// Constructor, destructor
////////////////////////////

seWPMapped::seWPMapped( seWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( seWPMappedListener::Ref::New( new seWPMappedListener( *this ) ) ),
pSkin( nullptr )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, panel, groupBox, form, formLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	helper.GroupBoxFlow( content, groupBox, "Mapped:" );
	helper.ListBox( groupBox, 8, "Mapped", pListMapped, new cListMapped( *this ) );
	pListMapped->SetDefaultSorter();
	
	form.TakeOver( new igdeContainerForm( env ) );
	groupBox->AddChild( form );
	helper.EditString( form, "Name:", "Name of mapped", pEditName, new cTextName( *this ) );
	
	
	helper.ComboBox( form, "Input Type:", "Type of input to use for curve",
		pCBInputType, new cComboInputType( *this ) );
	
	pCBInputType->AddItem( "Time", nullptr, ( void* )( intptr_t )deSkinMapped::eitTime );
	pCBInputType->AddItem( "BonePositionX", nullptr, ( void* )( intptr_t )deSkinMapped::eitBonePositionX );
	pCBInputType->AddItem( "BonePositionY", nullptr, ( void* )( intptr_t )deSkinMapped::eitBonePositionY );
	pCBInputType->AddItem( "BonePositionZ", nullptr, ( void* )( intptr_t )deSkinMapped::eitBonePositionZ );
	pCBInputType->AddItem( "BoneRotationX", nullptr, ( void* )( intptr_t )deSkinMapped::eitBoneRotationX );
	pCBInputType->AddItem( "BoneRotationY", nullptr, ( void* )( intptr_t )deSkinMapped::eitBoneRotationY );
	pCBInputType->AddItem( "BoneRotationZ", nullptr, ( void* )( intptr_t )deSkinMapped::eitBoneRotationZ );
	pCBInputType->AddItem( "BoneScaleX", nullptr, ( void* )( intptr_t )deSkinMapped::eitBoneScaleX );
	pCBInputType->AddItem( "BoneScaleY", nullptr, ( void* )( intptr_t )deSkinMapped::eitBoneScaleY );
	pCBInputType->AddItem( "BoneScaleZ", nullptr, ( void* )( intptr_t )deSkinMapped::eitBoneScaleZ );
	
	helper.EditFloat( form, "Input Lower:", "Lower input range", pEditInputLower, new cTextInputLower( *this ) );
	helper.EditFloat( form, "Input Upper:", "Upper input range", pEditInputUpper, new cTextInputUpper( *this ) );
	helper.CheckBox( form, pChkInputClamped, new cActionInputClamped( *this ), true );
	
	helper.EditString( form, "Bone:", "Name of the bone to use if bone related input type is used",
		pEditBone, new cTextBone( *this ) );
	
	helper.ViewCurveBezier( groupBox, pEditCurve, new cEditCurve( *this ) );
	pEditCurve->SetDefaultSize( decPoint( 200, 250 ) );
	pEditCurve->ClearCurve();
	pEditCurve->SetClamp( true );
	pEditCurve->SetClampMin( decVector2( 0.0f, 0.0f ) );
	pEditCurve->SetClampMax( decVector2( 1.0f, 1.0f ) );
}

seWPMapped::~seWPMapped(){
	SetSkin( nullptr );
}



// Management
///////////////

void seWPMapped::SetSkin( seSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->RemoveListener( pListener );
	}
	
	pSkin = skin;
	
	if( skin ){
		skin->AddListener( pListener );
	}
	
	UpdateMappedList();
}

seMapped *seWPMapped::GetMapped() const{
	return pSkin ? pSkin->GetActiveMapped() : nullptr;
}

void seWPMapped::UpdateMappedList(){
	seMapped * const selection = GetMapped();
	
	pListMapped->RemoveAllItems();
	
	if( pSkin ){
		const seMappedList &list = pSkin->GetMappedList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			seMapped * const mapped = list.GetAt( i );
			pListMapped->AddItem( mapped->GetName(), nullptr, mapped );
		}
		
		pListMapped->SortItems();
	}
	
	pListMapped->SetSelectionWithData( selection );
	UpdateMapped();
}

void seWPMapped::SelectActiveMapped(){
	pListMapped->SetSelectionWithData( GetMapped() );
}

void seWPMapped::UpdateMapped(){
	const seMapped * const mapped = GetMapped();
	
	if( mapped ){
		pEditName->SetText( mapped->GetName() );
		pEditCurve->SetCurve( mapped->GetCurve() );
		pCBInputType->SetSelectionWithData( ( void* )( intptr_t )mapped->GetInputType() );
		pEditInputLower->SetFloat( mapped->GetInputLower() );
		pEditInputUpper->SetFloat( mapped->GetInputUpper() );
		pEditBone->SetText( mapped->GetBone() );
		
	}else{
		pEditName->ClearText();
		pEditCurve->ClearCurve();
		pCBInputType->SetSelectionWithData( ( void* )( intptr_t )deSkinMapped::eitTime );
		pEditInputLower->ClearText();
		pEditInputUpper->ClearText();
		pEditBone->ClearText();
	}
	
	const bool enabled = mapped;
	
	pEditName->SetEnabled( enabled );
	
	pChkInputClamped->GetAction()->Update();
	
	pEditCurve->SetEnabled( enabled );
	pCBInputType->SetEnabled( enabled );
	pEditInputLower->SetEnabled( enabled );
	pEditInputUpper->SetEnabled( enabled );
	pEditBone->SetEnabled( enabled );
}
