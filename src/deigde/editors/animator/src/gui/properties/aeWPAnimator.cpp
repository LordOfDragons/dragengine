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
#include <stdint.h>

#include "aeWPAnimator.h"
#include "aeWPAnimatorListener.h"
#include "aeWindowProperties.h"
#include "../aeWindowMain.h"
#include "../../animator/aeAnimator.h"
#include "../../clipboard/aeClipboardDataBones.h"
#include "../../undosys/animator/aeUAnimatorAddBone.h"
#include "../../undosys/animator/aeUAnimatorRemoveBone.h"
#include "../../undosys/animator/aeUAnimatorMirrorBones.h"
#include "../../undosys/animator/aeUAnimatorSetBones.h"
#include "../../undosys/animator/aeUAnimatorSetAnimationPath.h"
#include "../../undosys/animator/aeUAnimatorSetRigPath.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/igdeWindow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeListBoxListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>



// Actions
///////////

namespace{

class cBaseAction : public igdeAction{
protected:
	aeWPAnimator &pPanel;
	
public:
	cBaseAction( aeWPAnimator &panel, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( ! animator ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( animator ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ) = 0;
	
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


class cPathRig : public igdeEditPathListener{
	aeWPAnimator &pPanel;
public:
	cPathRig( aeWPAnimator &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( ! animator || animator->GetRigPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeUAnimatorSetRigPath( animator, editPath->GetPath() ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
};

class cPathAnimation : public igdeEditPathListener{
	aeWPAnimator &pPanel;
public:
	cPathAnimation( aeWPAnimator &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		aeAnimator * const animator = pPanel.GetAnimator();
		if( ! animator || animator->GetAnimationPath() == editPath->GetPath() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new aeUAnimatorSetAnimationPath( animator, editPath->GetPath() ) );
		if( undo ){
			animator->GetUndoSystem()->Add( undo );
		}
	}
};


class cActionRigBoneAdd : public cBaseAction{
public:
	cActionRigBoneAdd( aeWPAnimator &panel ) : cBaseAction( panel, "Add",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add rig bone" ){}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ){
		const decString &name = pPanel.GetCBRigBoneText();
		return ! name.IsEmpty() && ! animator->GetListBones().Has( name )
			? new aeUAnimatorAddBone( animator, name ) : NULL;
	}
	
	virtual void Update(const aeAnimator &animator ){
		const decString &name = pPanel.GetCBRigBoneText();
		SetEnabled( ! name.IsEmpty() && ! animator.GetListBones().Has( name ) );
	}
};

class cActionRigBoneRemove : public cBaseAction{
public:
	cActionRigBoneRemove( aeWPAnimator &panel ) : cBaseAction( panel, "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove rig bone" ){}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ){
		const decString &name = pPanel.GetCBRigBoneText();
		return ! name.IsEmpty() && animator->GetListBones().Has( name )
			? new aeUAnimatorRemoveBone( animator, name ) : NULL;
	}
	
	virtual void Update(const aeAnimator &animator ){
		const decString &name = pPanel.GetCBRigBoneText();
		SetEnabled( ! name.IsEmpty() && animator.GetListBones().Has( name ) );
	}
};

class cActionMirrorRigBones : public cBaseAction{
public:
	cActionMirrorRigBones( aeWPAnimator &panel ) : cBaseAction( panel, "Mirror Bones",
		NULL, "Mirror rig bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ){
		// TODO add a dialog to allow changing the mirror parameter (or add a new menu command)
		return new aeUAnimatorMirrorBones( animator );
	}
	
	virtual void Update(const aeAnimator &animator ){
		SetEnabled( animator.GetListBones().GetCount() > 0 );
	}
};

class cActionCopyRigBones : public cBaseAction{
public:
	cActionCopyRigBones( aeWPAnimator &panel ) : cBaseAction( panel, "Copy",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ), "Copy bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ){
		igdeClipboardDataReference clip;
		clip.TakeOver( new aeClipboardDataBones( animator->GetListBones() ) );
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( clip );
		return nullptr;
	}
};

class cActionPasteRigBones : public cBaseAction{
public:
	cActionPasteRigBones( aeWPAnimator &panel ) : cBaseAction( panel, "Paste",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ), "Copy bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ){
		aeClipboardDataBones * const clip = ( aeClipboardDataBones* )pPanel.GetWindowProperties()
			.GetWindowMain().GetClipboard().GetWithTypeName( aeClipboardDataBones::TYPE_NAME  );
		if( ! clip ){
			return nullptr;
		}
		
		aeUAnimatorSetBones * const undo = new aeUAnimatorSetBones(
			animator, animator->GetListBones() + clip->GetBones() );
		undo->SetShortInfo( "Animator paste bones" );
		return undo;
	}
	
	virtual void Update(const aeAnimator & ){
		SetEnabled( pPanel.GetWindowProperties().GetWindowMain().GetClipboard().
			HasWithTypeName( aeClipboardDataBones::TYPE_NAME ) );
	}
};

class cActionExportRigBones : public cBaseAction{
public:
	cActionExportRigBones( aeWPAnimator &panel ) : cBaseAction( panel, "Export To Text",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSave ), "Export bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ){
		const decStringSet bones = animator->GetListBones();
		const int count = bones.GetCount();
		decString text;
		int i;
		for( i=0; i<count; i++ ){
			if( i > 0 ){
				text.AppendCharacter( '\n' );
			}
			text.Append( bones.GetAt( i ) );
		}
		igdeCommonDialogs::GetMultilineString( pPanel.GetParentWindow(), "Export To Text", "Bones", text );
		return nullptr;
	}
	
	virtual void Update(const aeAnimator &animator ){
		SetEnabled( animator.GetListBones().GetCount() > 0 );
	}
};

class cActionImportRigBones : public cBaseAction{
public:
	cActionImportRigBones( aeWPAnimator &panel ) : cBaseAction( panel, "Import From Text",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ), "Import bones" ){}
	
	virtual igdeUndo *OnAction( aeAnimator *animator ){
		decString text;
		while( true ){
			if( ! igdeCommonDialogs::GetMultilineString( pPanel.GetParentWindow(),
			"Import From Text", "Bones. One bone per line.", text ) ){
				return nullptr;
			}
			break;
		}
		
		const decStringList lines( text.Split( '\n' ) );
		const int count = lines.GetCount();
		decStringSet bones;
		int i;
		
		for( i=0; i<count; i++ ){
			if( ! lines.GetAt( i ).IsEmpty() ){
				bones.Add( lines.GetAt( i ) );
			}
		}
		
		aeUAnimatorSetBones * const undo = new aeUAnimatorSetBones( animator, animator->GetListBones() + bones );
		undo->SetShortInfo( "Animator import bones" );
		return undo;
	}
};

class cListRigBones : public igdeListBoxListener{
protected:
	aeWPAnimator &pPanel;
	
public:
	cListRigBones( aeWPAnimator &panel ) : pPanel( panel ){ }
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		if( pPanel.GetAnimator() && listBox->GetSelectedItem() ){
			pPanel.SetCBRigBoneText( listBox->GetSelectedItem()->GetText() );
		}
	}
	
	virtual void AddContextMenuEntries( igdeListBox*, igdeMenuCascade &menu ){
		if( ! pPanel.GetAnimator() ){
			return;
		}
		
		igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( menu, new cActionRigBoneAdd( pPanel ), true );
		helper.MenuCommand( menu, new cActionRigBoneRemove( pPanel ), true );
		helper.MenuCommand( menu, new cActionMirrorRigBones( pPanel ), true );
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionCopyRigBones( pPanel ), true );
		helper.MenuCommand( menu, new cActionPasteRigBones( pPanel ), true );
		helper.MenuSeparator( menu );
		helper.MenuCommand( menu, new cActionExportRigBones( pPanel ), true );
		helper.MenuCommand( menu, new cActionImportRigBones( pPanel ), true );
	}
};

}



// Class aeWPAnimator
/////////////////////////

// Constructor, destructor
////////////////////////////

aeWPAnimator::aeWPAnimator( aeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pAnimator( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference content, groupBox, formLine;
	
	pListener = new aeWPAnimatorListener( *this );
	
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// animation
	helper.GroupBox( content, groupBox, "Animator File Path:" );
	helper.EditPath( groupBox, "Rig:", "Rig resource used by the animator",
		igdeEnvironment::efpltRig, pEditRigPath, new cPathRig( *this ) );
	helper.EditPath( groupBox, "Animation:", "Aniamtion resource used by the animator",
		igdeEnvironment::efpltAnimation, pEditAnimPath, new cPathAnimation( *this ) );
	
	
	// affected bones
	helper.GroupBoxFlow( content, groupBox, "Affected bones:" );
	
	formLine.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	groupBox->AddChild( formLine );
	helper.ComboBoxFilter( formLine, true, "Bone name", pCBBones, NULL );
	helper.Button( formLine, pBtnBoneAdd, new cActionRigBoneAdd( *this ), true );
	helper.Button( formLine, pBtnBoneDel, new cActionRigBoneRemove( *this ), true );
	
	helper.ListBox( groupBox, 5, "Affectes bones", pListBones, new cListRigBones( *this ) );
	pListBones->SetDefaultSorter();
}

aeWPAnimator::~aeWPAnimator(){
	SetAnimator( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void aeWPAnimator::SetAnimator( aeAnimator *animator ){
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
	UpdateAnimator();
	OnAnimatorPathChanged();
}

void aeWPAnimator::UpdateAnimator(){
	if( pAnimator ){
		pEditRigPath->SetPath( pAnimator->GetRigPath() );
		pEditAnimPath->SetPath( pAnimator->GetAnimationPath() );
		
		const decStringSet &boneList = pAnimator->GetListBones();
		const int boneCount = boneList.GetCount();
		const decString selection( pListBones->GetSelectedItem()
			? pListBones->GetSelectedItem()->GetText() : "" );
		int i;
		
		pListBones->RemoveAllItems();
		for( i=0; i<boneCount; i++ ){
			pListBones->AddItem( boneList.GetAt( i ) );
		}
		pListBones->SortItems();
		pListBones->SetSelection( pListBones->IndexOfItem( selection ) );
		if( ! pListBones->GetSelectedItem() && pListBones->GetItemCount() > 0 ){
			pListBones->SetSelection( 0 );
		}
		
	}else{
		pEditRigPath->ClearPath();
		pEditAnimPath->ClearPath();
		
		pListBones->RemoveAllItems();
	}
	
	const bool enabled = pAnimator;
	pEditRigPath->SetEnabled( enabled );
	pEditAnimPath->SetEnabled( enabled );
	pCBBones->SetEnabled( enabled );
	pListBones->SetEnabled( enabled );
	
	pBtnBoneAdd->GetAction()->Update();
	pBtnBoneDel->GetAction()->Update();
}

void aeWPAnimator::OnAnimatorPathChanged(){
	if( pAnimator ){
		pEditRigPath->SetBasePath( pAnimator->GetDirectoryPath() );
		pEditAnimPath->SetBasePath( pAnimator->GetDirectoryPath() );
		
	}else{
		pEditRigPath->SetBasePath( "" );
		pEditAnimPath->SetBasePath( "" );
	}
}

void aeWPAnimator::UpdateRigBoneList(){
	const decString selection( GetCBRigBoneText() );
	
	pCBBones->RemoveAllItems();
	
	if( pAnimator ){
		const deRig * const rig = pAnimator->GetEngineRig();
		if( rig ){
			const int count = rig->GetBoneCount();
			int i;
			for( i=0; i<count; i++ ){
				pCBBones->AddItem( rig->GetBoneAt( i ).GetName() );
			}
		}
		
		pCBBones->SortItems();
		pCBBones->StoreFilterItems();
	}
	
	pCBBones->SetText( selection );
}

const decString &aeWPAnimator::GetCBRigBoneText() const{
	return pCBBones->GetText();
}

void aeWPAnimator::SetCBRigBoneText( const char *text ){
	pCBBones->SetText( text );
}
