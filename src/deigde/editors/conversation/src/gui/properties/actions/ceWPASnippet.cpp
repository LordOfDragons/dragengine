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

#include "ceWPASnippet.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCASnippet.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../undosys/action/snippet/ceUCASnippetSetFile.h"
#include "../../../undosys/action/snippet/ceUCASnippetSetTopic.h"
#include "../../../undosys/action/snippet/ceUCASnippetToggleCreateSideLane.h"
#include "../../../configuration/ceConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cComboFile : public igdeComboBoxListener {
	ceWPASnippet &pPanel;
	
public:
	cComboFile( ceWPASnippet &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASnippet * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetFile() ){
			return;
		}
		
		ceUCASnippetSetFile::Ref undo(ceUCASnippetSetFile::Ref::NewWith(
			topic, action, comboBox->GetText()));
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cComboTopic : public igdeComboBoxListener {
	ceWPASnippet &pPanel;
	
public:
	cComboTopic( ceWPASnippet &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASnippet * const action = pPanel.GetAction();
		if( ! topic || ! action  || comboBox->GetText() == action->GetTopic() ){
			return;
		}
		
		ceUCASnippetSetTopic::Ref undo(ceUCASnippetSetTopic::Ref::NewWith(
			topic, action, comboBox->GetText()));
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

class cActionCreateSideLane : public igdeAction{
	ceWPASnippet &pPanel;
	
public:
	cActionCreateSideLane(ceWPASnippet &panel) : igdeAction("Create Side Lane",
		nullptr, "Run snippet in a new side lane"), pPanel(panel){}
	
	void OnAction() override{
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASnippet * const action = pPanel.GetAction();
		if(!topic || !action){
			return;
		}
		
		ceUCASnippetToggleCreateSideLane::Ref undo(ceUCASnippetToggleCreateSideLane::Ref::NewWith(
			topic, action));
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add(undo);
	}
};

class cActionJumpToTopic : public igdeAction {
	ceWPASnippet &pPanel;
	
public:
	cActionJumpToTopic( ceWPASnippet &panel ) : igdeAction( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallRight ),
		"Jump To Topic" ), pPanel( panel ){ }
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCASnippet * const action = pPanel.GetAction();
		if( ! topic || ! action ){
			return;
		}
		
		ceConversationFile * const jumpFile = pPanel.GetParentPanel().GetConversation()
			->GetFileList().GetWithID( action->GetFile() );
		if( ! jumpFile ){
			return;
		}
		
		ceConversationTopic * const jumpTopic = jumpFile->GetTopicList().GetWithID( action->GetTopic() );
		if( ! jumpTopic ){
			return;
		}
		
		jumpFile->SetActiveTopic( jumpTopic );
		pPanel.GetParentPanel().GetConversation()->SetActiveFile( jumpFile );
	}
};

}



// Class ceWPASnippet
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPASnippet::ceWPASnippet( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	igdeContainer::Ref formLine;
	
	CreateGUICommon( *this );
	
	helper.ComboBoxFilter( *this, "File:", true, "File to run topic from", pCBFile, new cComboFile( *this ) );
	pCBFile->SetDefaultSorter();
	
	helper.FormLineStretchFirst( *this, "Topic:", "Topic to run", formLine );
	helper.ComboBoxFilter( formLine, true, "Topic to run", pCBTopic, new cComboTopic( *this ) );
	pCBTopic->SetDefaultSorter();
	helper.Button( formLine, pBtnJumpToTopic, new cActionJumpToTopic( *this ), true );
	
	helper.CheckBox(*this, pChkCreateSideLane, new cActionCreateSideLane(*this), true);
}

ceWPASnippet::~ceWPASnippet(){
}



// Management
///////////////

ceCASnippet *ceWPASnippet::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetTreeAction();
	
	if( action && action->GetType() == ceConversationAction::eatSnippet ){
		return ( ceCASnippet* )action;
		
	}else{
		return NULL;
	}
}

void ceWPASnippet::UpdateAction(){
	const ceCASnippet * const action = GetAction();
	
	UpdateCommonParams();
	
	if( action ){
		pCBFile->SetText( action->GetFile() );
		
		UpdateTopicList();
		pCBTopic->SetText( action->GetTopic() );
		
		pChkCreateSideLane->SetChecked(action->GetCreateSideLane());
		
	}else{
		pCBFile->ClearText();
		
		UpdateTopicList();
		pCBTopic->ClearText();
		
		pChkCreateSideLane->SetChecked(false);
	}
}

void ceWPASnippet::UpdateFileList(){
	ceConversation * const conversation = GetParentPanel().GetConversation();
	const decString selection( pCBFile->GetText() );
	
	pCBFile->RemoveAllItems();
	
	if( conversation ){
		const ceConversationFileList list( conversation->AllFiles() );
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pCBFile->AddItem( list.GetAt( i )->GetID() );
		}
		
		pCBFile->SortItems();
		pCBFile->StoreFilterItems();
	}
	
	pCBFile->SetText( selection );
}

void ceWPASnippet::UpdateTopicList(){
	ceConversation * const conversation = GetParentPanel().GetConversation();
	const decString selection( pCBTopic->GetText() );
	
	pCBTopic->RemoveAllItems();
	
	if( conversation ){
		const ceConversationTopicList list( conversation->AllTopics( pCBFile->GetText() ) );
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			pCBTopic->AddItem( list.GetAt( i )->GetID() );
		}
		
		pCBTopic->SortItems();
		pCBTopic->StoreFilterItems();
	}
	
	pCBTopic->SetText( selection );
}
