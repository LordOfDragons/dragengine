/* 
 * Drag[en]gine IGDE Conversation Editor
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
#include "../../../configuration/ceConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/undo/igdeUndoReference.h>
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
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASnippetSetFile( topic, action, comboBox->GetText() ) );
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
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCASnippetSetTopic( topic, action, comboBox->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
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
	igdeContainerReference formLine;
	
	CreateGUICommon( *this );
	
	helper.ComboBoxFilter( *this, "File:", true, "File to run topic from", pCBFile, new cComboFile( *this ) );
	pCBFile->SetDefaultSorter();
	
	helper.FormLineStretchFirst( *this, "Topic:", "Topic to run", formLine );
	helper.ComboBoxFilter( formLine, true, "Topic to run", pCBTopic, new cComboTopic( *this ) );
	pCBTopic->SetDefaultSorter();
	helper.Button( formLine, pBtnJumpToTopic, new cActionJumpToTopic( *this ), true );
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
		
	}else{
		pCBFile->ClearText();
		
		UpdateTopicList();
		pCBTopic->ClearText();
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
	}
	
	pCBFile->StoreFilterItems();
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
	}
	
	pCBTopic->StoreFilterItems();
	pCBTopic->SetText( selection );
}
