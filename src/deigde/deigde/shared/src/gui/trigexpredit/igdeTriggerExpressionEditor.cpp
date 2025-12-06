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

#include "igdeTriggerExpressionEditor.h"
#include "../igdeApplication.h"
#include "../igdeUIHelper.h"
#include "../igdeButton.h"
#include "../igdeListBox.h"
#include "../igdeTextField.h"
#include "../igdeToggleButton.h"
#include "../igdeTreeList.h"
#include "../igdeContainer.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerBox.h"
#include "../layout/igdeContainerBorder.h"
#include "../layout/igdeContainerSplitted.h"
#include "../event/igdeAction.h"
#include "../event/igdeListBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../event/igdeTreeListListener.h"
#include "../model/igdeListItem.h"
#include "../model/igdeTreeItem.h"
#include "../../triggersystem/igdeTriggerExpression.h"
#include "../../triggersystem/igdeTriggerExpressionParser.h"
#include "../../triggersystem/igdeTriggerExpressionComponent.h"
#include "../../triggersystem/igdeTriggerTarget.h"
#include "../../triggersystem/igdeTriggerTargetList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Actions
////////////

class igdeTriggerExpressionEditor_ActionNegate : public igdeAction {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_ActionNegate( igdeTriggerExpressionEditor &editor ) :
	igdeAction( "Not", NULL, "Negate Result" ), pEditor( editor ){}
	
	virtual void OnAction(){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component ){
			component->SetNegate( ! component->GetNegate() );
			pEditor.UpdateExpressionFromTree();
			pEditor.UpdateTree();
		}
	}
	
	virtual void Update(){
		const igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		SetEnabled( component );
		SetSelected( component && component->GetNegate() );
	}
};

class igdeTriggerExpressionEditor_ActionCurState : public igdeAction {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_ActionCurState( igdeTriggerExpressionEditor &editor ) :
	igdeAction( "Now", NULL, "Check if target is triggered now or ever" ), pEditor( editor ){}
	
	virtual void OnAction(){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component ){
			component->SetCurState( ! component->GetCurState() );
			pEditor.UpdateExpressionFromTree();
			pEditor.UpdateTree();
		}
	}
	
	virtual void Update(){
		const igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		SetEnabled( component && component->GetType() == igdeTriggerExpressionComponent::ectTarget );
		SetSelected( component && component->GetCurState() );
	}
};


class igdeTriggerExpressionEditor_ActionAnd : public igdeAction {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_ActionAnd( igdeTriggerExpressionEditor &editor ) :
	igdeAction( "And", NULL, "All children have to evaluate to true" ), pEditor( editor ){}
	
	virtual void OnAction(){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component ){
			component->SetType( igdeTriggerExpressionComponent::ectAnd );
			pEditor.UpdateExpressionFromTree();
			pEditor.UpdateTree();
		}
	}
	
	virtual void Update(){
		const igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		SetEnabled( component );
		SetSelected( component && component->GetType() == igdeTriggerExpressionComponent::ectAnd );
	}
};

class igdeTriggerExpressionEditor_ActionOr : public igdeAction {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_ActionOr( igdeTriggerExpressionEditor &editor ) :
	igdeAction( "Or", NULL, "One or more children have to evaluate to true" ), pEditor( editor ){}
	
	virtual void OnAction(){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component ){
			component->SetType( igdeTriggerExpressionComponent::ectOr );
			pEditor.UpdateExpressionFromTree();
			pEditor.UpdateTree();
		}
	}
	
	virtual void Update(){
		const igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		SetEnabled( component );
		SetSelected( component && component->GetType() == igdeTriggerExpressionComponent::ectOr );
	}
};

class igdeTriggerExpressionEditor_ActionTarget : public igdeAction {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_ActionTarget( igdeTriggerExpressionEditor &editor ) :
	igdeAction( "Target", NULL, "Target to evaluate" ), pEditor( editor ){}
	
	virtual void OnAction(){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component ){
			component->SetType( igdeTriggerExpressionComponent::ectTarget );
			component->RemoveAllChildren();
			pEditor.UpdateExpressionFromTree();
			pEditor.UpdateTree();
		}
	}
	
	virtual void Update(){
		const igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		SetEnabled( component );
		SetSelected( component && component->GetType() == igdeTriggerExpressionComponent::ectTarget );
	}
};


class igdeTriggerExpressionEditor_ActionAddChild : public igdeAction {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_ActionAddChild( igdeTriggerExpressionEditor &editor ) :
	igdeAction( "", editor.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ),
	"Add child to list" ), pEditor( editor ){}
	
	virtual void OnAction(){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component && component->GetType() != igdeTriggerExpressionComponent::ectTarget ){
			component->AddChild(igdeTriggerExpressionComponent::Ref::NewWith());
			pEditor.UpdateExpressionFromTree();
			pEditor.UpdateTree();
		}
	}
	
	virtual void Update(){
		const igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		SetEnabled( component && component->GetType() != igdeTriggerExpressionComponent::ectTarget );
	}
};

class igdeTriggerExpressionEditor_ActionRemoveChild : public igdeAction {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_ActionRemoveChild( igdeTriggerExpressionEditor &editor ) :
	igdeAction( "", editor.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ),
	"Remove child from list" ), pEditor( editor ){}
	
	virtual void OnAction(){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		const igdeTreeItem * const selection = pEditor.GetSelectedTreeItem();
		if( ! component || ! selection ){
			return;
		}
		
		igdeTriggerExpressionComponent * const parent = selection->GetParent()
			? ( igdeTriggerExpressionComponent* )selection->GetParent()->GetData() : NULL;
		if( parent && parent->GetType() != igdeTriggerExpressionComponent::ectTarget ){
			parent->RemoveChild( component );
			pEditor.UpdateExpressionFromTree();
			pEditor.UpdateTree();
		}
	}
	
	virtual void Update(){
		const igdeTreeItem * const selection = pEditor.GetSelectedTreeItem();
		const igdeTriggerExpressionComponent * const parent = selection && selection->GetParent()
			? ( igdeTriggerExpressionComponent* )selection->GetParent()->GetData() : NULL;
		SetEnabled( parent && parent->GetType() != igdeTriggerExpressionComponent::ectTarget );
	}
};


class igdeTriggerExpressionEditor_TextExpression : public igdeTextFieldListener{
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_TextExpression( igdeTriggerExpressionEditor &editor ) : pEditor( editor ){}
	
	virtual void OnTextChanged( igdeTextField *textField ){
		pEditor.SetExpression( textField->GetText() );
	}
	
	virtual void OnTextChanging( igdeTextField *textField ){
		OnTextChanged( textField );
	}
};

class igdeTriggerExpressionEditor_TextTargetName : public igdeTextFieldListener{
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_TextTargetName( igdeTriggerExpressionEditor &editor ) : pEditor( editor ){}
	
	virtual void OnTextChanged( igdeTextField *textField ){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component ){
			const decString name( textField->GetText() );
			pEditor.SetSelectedItemInTargetList( name );
			if( name != component->GetTargetName() ){
				component->SetTargetName( name );
				pEditor.UpdateExpressionFromTree();
				pEditor.UpdateTree();
			}
		}
	}
	
	virtual void OnTextChanging( igdeTextField *textField ){
		OnTextChanged( textField );
	}
};

class igdeTriggerExpressionEditor_ListTargets : public igdeListBoxListener{
	igdeTriggerExpressionEditor &pEditor;
	igdeTextField &pTargetName;
public:
	igdeTriggerExpressionEditor_ListTargets( igdeTriggerExpressionEditor &editor, igdeTextField &targetName ) :
	pEditor( editor ), pTargetName( targetName ) {}
	
	virtual void OnSelectionChanged( igdeListBox *listBox ){
		igdeTriggerExpressionComponent * const component = pEditor.GetSelectedComponent();
		if( component && component->GetType() == igdeTriggerExpressionComponent::ectTarget
		&& listBox->GetSelectedItem() ){
			pTargetName.SetText( listBox->GetSelectedItem()->GetText() );
		}
		//pEditor.SetSelectedItemInTargetList( pListTargetName->getCurrentItem() ); // to prevent deselection
	}
};

class igdeTriggerExpressionEditor_TextFilterTargetName : public igdeTextFieldListener{
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_TextFilterTargetName( igdeTriggerExpressionEditor &editor ) : pEditor( editor ){}
	
	virtual void OnTextChanged( igdeTextField* ){
		pEditor.UpdateFromTargetList();
	}
	
	virtual void OnTextChanging( igdeTextField *textField ){
		OnTextChanged( textField );
	}
};


class igdeTriggerExpressionEditor_TreeExpression : public igdeTreeListListener {
	igdeTriggerExpressionEditor &pEditor;
public:
	igdeTriggerExpressionEditor_TreeExpression( igdeTriggerExpressionEditor &editor ) : pEditor( editor ){}
	
	virtual void AddContextMenuEntries( igdeTreeList *treeList, igdeMenuCascade &menu ){
	}
	
	virtual void OnSelectionChanged( igdeTreeList* ){
		pEditor.UpdateExpressionComponent();
	}
};



// Class igdeTriggerExpressionEditor
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionEditor::igdeTriggerExpressionEditor( igdeEnvironment &environment ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY, igdeContainerFlow::esLast ),
pParser( NULL ),
pTargetList( NULL ),
pWorkExpression( NULL )
{
	pCreateContent();
	RebuildTree(); // otherwise an initial SetExpression("") has no effect since the string equals
}

igdeTriggerExpressionEditor::igdeTriggerExpressionEditor( igdeEnvironment &environment, igdeAction *action ) :
igdeContainerFlow( environment, igdeContainerFlow::eaY, igdeContainerFlow::esLast ),
pParser( NULL ),
pTargetList( NULL ),
pWorkExpression( NULL )
{
	pCreateContent();
	RebuildTree(); // otherwise an initial SetExpression("") has no effect since the string equals
	SetAction( action );
}

igdeTriggerExpressionEditor::~igdeTriggerExpressionEditor(){
	if( pWorkExpression ){
		pWorkExpression->FreeReference();
	}
}



// Management
///////////////

void igdeTriggerExpressionEditor::SetParser( igdeTriggerExpressionParser *parser ){
	if( parser == pParser ){
		return;
	}
	
	pParser = parser;
	UpdateExpressionFromTree();
}

void igdeTriggerExpressionEditor::SetTargetList( const igdeTriggerTargetList *targetList ){
	if( targetList == pTargetList ){
		return;
	}
	
	pTargetList = targetList;
	UpdateFromTargetList();
}

void igdeTriggerExpressionEditor::SetExpression( const char *expression ){
	if( ! expression ){
		DETHROW( deeInvalidParam );
	}
	
	if( pExpression == expression ){
		return;
	}
	
	pExpression = expression;
	UpdateText();
	RebuildTree();
	OnAction();
}

void igdeTriggerExpressionEditor::SetAction( igdeAction *action ){
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



void igdeTriggerExpressionEditor::UpdateText(){
	pEditExpression->SetText( pExpression );
}

void igdeTriggerExpressionEditor::RebuildTree(){
	pTreeExpression->RemoveAllItems();
	if( pWorkExpression ){
		pWorkExpression->FreeReference();
		pWorkExpression = NULL;
	}
	
	if( pParser ){
		try{
			pWorkExpression = pParser->StringToExpression( pExpression );
			
		}catch( const deException & ){
			pTreeExpression->AppendItem( NULL, "error", GetEnvironment().GetStockIcon( igdeEnvironment::esiWarning ) );
		}
	}
	
	if( ! pWorkExpression || ! pWorkExpression->GetRootComponent() ){
		igdeTriggerExpressionComponent *component = NULL;
		
		if( ! pWorkExpression ){
			pWorkExpression = new igdeTriggerExpression;
		}
		
		try{
			component = new igdeTriggerExpressionComponent;
			component->SetNegate( false );
			component->SetCurState( false );
			component->SetType( igdeTriggerExpressionComponent::ectTarget );
			component->SetTargetName( "" );
			
			pWorkExpression->SetRootComponent( component );
			component->FreeReference();
			
		}catch( const deException & ){
			if( component ){
				component->FreeReference();
			}
			if( pWorkExpression ){
				pWorkExpression->FreeReference();
				pWorkExpression = NULL;
			}
			throw;
		}
	}
	
	if( pWorkExpression ){
		AddComponentToTree( NULL, pWorkExpression->GetRootComponent() );
	}
	
	UpdateExpressionComponent();
}

void igdeTriggerExpressionEditor::UpdateTree(){
	if( ! pWorkExpression ){
		RebuildTree();
		return;
	}
	
	if( pWorkExpression && pTreeExpression->GetFirstChild() ){
		UpdateComponentInTree( pTreeExpression->GetFirstChild(), pWorkExpression->GetRootComponent() );
	}
	
	UpdateExpressionComponent();
}

void igdeTriggerExpressionEditor::AddComponentToTree( igdeTreeItem *parent, igdeTriggerExpressionComponent *component ){
	decString text;
	
	if( component ){
		const decString &targetName = component->GetTargetName();
		const int type = component->GetType();
		
		if( component->GetNegate() ){
			text += "not ";
		}
		
		switch( type ){
		case igdeTriggerExpressionComponent::ectTarget:
			if( component->GetCurState() ){
				text +=  "fired '";
				
			}else{
				text += "ever fired '";
			}
			text += targetName;
			text += "'";
			break;
			
		case igdeTriggerExpressionComponent::ectAnd:
			text += "All";
			break;
			
		case igdeTriggerExpressionComponent::ectOr:
			text += "One or more";
			break;
			
		default:
			text += "??";
		}
		
	}else{
		text += "<empty>";
	}
	
	igdeTreeItem * const item = pTreeExpression->AppendItem( parent, text, NULL, component );
	item->SetExpanded( true );
	pTreeExpression->ItemChanged( item );
	
	if( component && component->GetType() != igdeTriggerExpressionComponent::ectTarget ){
		const int count = component->GetChildCount();
		int i;
		for( i=0; i<count; i++ ){
			AddComponentToTree( item, component->GetChildAt( i ) );
		}
	}
}

void igdeTriggerExpressionEditor::UpdateComponentInTree( igdeTreeItem *item, igdeTriggerExpressionComponent *component ){
	decString text;
	
	if( component ){
		const decString &targetName = component->GetTargetName();
		const int type = component->GetType();
		
		if( component->GetNegate() ){
			text += "not ";
		}
		
		switch( type ){
		case igdeTriggerExpressionComponent::ectTarget:
			if( component->GetCurState() ){
				text +=  "fired '";
				
			}else{
				text += "ever fired '";
			}
			text += targetName;
			text += "'";
			break;
			
		case igdeTriggerExpressionComponent::ectAnd:
			text += "All";
			break;
			
		case igdeTriggerExpressionComponent::ectOr:
			text += "One or more";
			break;
			
		default:
			text += "??";
		}
		
	}else{
		text += "<empty>";
	}
	
	item->SetText( text );
	item->SetData( component );
	pTreeExpression->ItemChanged( item );
	
	if( component && component->GetType() != igdeTriggerExpressionComponent::ectTarget ){
		igdeTreeItem *childItem = item->GetFirstChild();
		const int count = component->GetChildCount();
		int i;
		
		for( i=0; i<count; i++ ){
			if( childItem ){
				UpdateComponentInTree( childItem, component->GetChildAt( i ) );
				
			}else{
				AddComponentToTree( item, component->GetChildAt( i ) );
				childItem = item->GetLastChild();
			}
			childItem = childItem->GetNext();
		}
		
		while( childItem ){
			igdeTreeItem * const removeItem = childItem;
			childItem = childItem->GetNext();
			pTreeExpression->RemoveItem( removeItem );
		}
		
	}else{
		pTreeExpression->RemoveAllItems( item );
	}
}

void igdeTriggerExpressionEditor::UpdateExpressionComponent(){
	igdeTriggerExpressionComponent * const component = GetSelectedComponent();
	
	if( component ){
		pEditTargetName->SetText( component->GetTargetName() );
		SetSelectedItemInTargetList( pListTargetName->IndexOfItem( component->GetTargetName() ) );
		
	}else{
		pEditTargetName->ClearText();
		SetSelectedItemInTargetList( -1 );
	}
	
	pEditTargetName->SetEnabled( component && component->GetType() == igdeTriggerExpressionComponent::ectTarget );
	pListTargetName->SetEnabled(  component && component->GetType() == igdeTriggerExpressionComponent::ectTarget);
	pEditFilterTargetName->SetEnabled( component );
	
	pBtnNegate->GetAction()->Update();
	pBtnCurState->GetAction()->Update();
	pBtnAnd->GetAction()->Update();
	pBtnOr->GetAction()->Update();
	pBtnTarget->GetAction()->Update();
}

void igdeTriggerExpressionEditor::UpdateExpressionFromTree(){
	if( pParser && pWorkExpression ){
		pParser->ExpressionToString( pExpression, *pWorkExpression );
		pEditExpression->SetText( pExpression );
	}
}

igdeTreeItem *igdeTriggerExpressionEditor::GetSelectedTreeItem() const{
	return pTreeExpression->GetSelection();
}

igdeTriggerExpressionComponent *igdeTriggerExpressionEditor::GetSelectedComponent() const{
	return pTreeExpression->GetSelection() ?
		( igdeTriggerExpressionComponent* )pTreeExpression->GetSelection()->GetData() : NULL;
}

void igdeTriggerExpressionEditor::UpdateFromTargetList(){
	const decString filter( pEditFilterTargetName->GetText().GetLower() );
	const bool hasFilter = ! filter.IsEmpty();
	
	pListTargetName->RemoveAllItems();
	
	if( pTargetList ){
		const int count = pTargetList->GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			const decString &targetName = pTargetList->GetAt( i )->GetName();
			if( hasFilter && targetName.GetLower().FindString( filter ) == -1 ){
				continue;
			}
			pListTargetName->AddItem( targetName );
		}
	}
	
	pListTargetName->SortItems();
	SetSelectedItemInTargetList( pListTargetName->IndexOfItem( pEditTargetName->GetText() ) );
}

void igdeTriggerExpressionEditor::SetSelectedItemInTargetList( int selection ){
	pListTargetName->SetSelection( selection );
}

void igdeTriggerExpressionEditor::SetSelectedItemInTargetList( const char *name ){
	SetSelectedItemInTargetList( pListTargetName->IndexOfItem( name ) );
}



void igdeTriggerExpressionEditor::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeTriggerExpressionEditor::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeButton::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}

void igdeTriggerExpressionEditor::OnParameterChanged( igdeAction* ){
}



// Private Functions
//////////////////////

void igdeTriggerExpressionEditor::pCreateContent(){
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainer::Ref form, panel, panel2, groupBox;
	igdeContainerBorder::Ref groupBorder;
	
	// expression string
	form.TakeOver( new igdeContainerForm( env ) );
	helper.EditString( form, "Expression:", "Trigger expression", 50,
		pEditExpression, new igdeTriggerExpressionEditor_TextExpression( *this ) );
	AddChild( form );
	
	// content
	igdeContainerSplitted::Ref panelContent(igdeContainerSplitted::Ref::New(new igdeContainerSplitted(env, igdeContainerSplitted::espRight,
		igdeApplication::app().DisplayScaled(200))));
	AddChild( panelContent );
	
	// expression tree
	panel.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaY ) );
	panelContent->AddChild( panel, igdeContainerSplitted::eaCenter );
	
	helper.GroupBoxStaticFlow( panel, groupBox, "Structure:", true );
	helper.TreeList( groupBox, pTreeExpression, 10, "Trigger expression structure",
		new igdeTriggerExpressionEditor_TreeExpression( *this ) );
	
	// target list
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esLast ) );
	panelContent->AddChild( panel, igdeContainerSplitted::eaSide );
	
	helper.GroupBoxStaticFlow( panel, groupBox, "Component Control:", false );
	panel2.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX ) );
	groupBox->AddChild( panel2 );
	helper.ToggleButton( panel2, pBtnNegate, new igdeTriggerExpressionEditor_ActionNegate( *this ), true );
	helper.ToggleButton( panel2, pBtnCurState, new igdeTriggerExpressionEditor_ActionCurState( *this ), true );
	helper.Separator( panel2 );
	helper.ToggleButton( panel2, pBtnAnd, new igdeTriggerExpressionEditor_ActionAnd( *this ), true );
	helper.ToggleButton( panel2, pBtnOr, new igdeTriggerExpressionEditor_ActionOr( *this ), true );
	helper.ToggleButton( panel2, pBtnTarget, new igdeTriggerExpressionEditor_ActionTarget( *this ), true );
	helper.Separator( panel2 );
	helper.Button( panel2, pBtnAddChild, new igdeTriggerExpressionEditor_ActionAddChild( *this ), true );
	helper.Button( panel2, pBtnRemoveChild, new igdeTriggerExpressionEditor_ActionRemoveChild( *this ), true );
	
	helper.GroupBoxStaticBorder( panel, groupBorder, "Target Name:", true );
	
	helper.EditString( "Target name", pEditTargetName, new igdeTriggerExpressionEditor_TextTargetName( *this ) );
	groupBorder->AddChild( pEditTargetName, igdeContainerBorder::eaTop );
	
	helper.ListBox( 10, "Target names found in trigger table", pListTargetName,
		new igdeTriggerExpressionEditor_ListTargets( *this, pEditTargetName ) );
	pListTargetName->SetDefaultSorter();
	groupBorder->AddChild( pListTargetName, igdeContainerBorder::eaCenter );
	
	form.TakeOver( new igdeContainerForm( env ) );
	helper.EditString( form, "Filter:", "Filter for list of found target names", 20,
		pEditFilterTargetName, new igdeTriggerExpressionEditor_TextFilterTargetName( *this ) );
	groupBorder->AddChild( form, igdeContainerBorder::eaBottom );
}
