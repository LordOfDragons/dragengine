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

#include "ceWPTopic.h"
#include "ceWPTopicListener.h"
#include "ceWindowProperties.h"
#include "actions/ceWPACameraShot.h"
#include "actions/ceWPAMusic.h"
#include "actions/ceWPAActorSpeak.h"
#include "actions/ceWPAIfElse.h"
#include "actions/ceWPAPlayerChoice.h"
#include "actions/ceWPAStopConversation.h"
#include "actions/ceWPAStopTopic.h"
#include "actions/ceWPASnippet.h"
#include "actions/ceWPASetVariable.h"
#include "actions/ceWPASetAParam.h"
#include "actions/ceWPAActorCmd.h"
#include "actions/ceWPAGameCommand.h"
#include "actions/ceWPAWait.h"
#include "actions/ceWPATrigger.h"
#include "actions/ceWPAActorAdd.h"
#include "actions/ceWPAActorRemove.h"
#include "actions/ceWPACoordSystemAdd.h"
#include "actions/ceWPACoordSystemRemove.h"
#include "actions/ceWPAComment.h"
#include "conditions/ceWPCLogic.h"
#include "conditions/ceWPCHasActor.h"
#include "conditions/ceWPCActorInConversation.h"
#include "conditions/ceWPCVariable.h"
#include "conditions/ceWPCAParam.h"
#include "conditions/ceWPCActorCommand.h"
#include "conditions/ceWPCGameCommand.h"
#include "conditions/ceWPCTrigger.h"
#include "model/ceWPTTreeModel.h"
#include "model/ceWPTTreeItem.h"
#include "model/ceWPTTreeItemModel.h"
#include "model/action/ceWPTTIMAction.h"
#include "model/condition/ceWPTTIMCondition.h"
#include "../ceWindowMain.h"
#include "../../clipboard/ceClipboardDataTopic.h"
#include "../../clipboard/ceClipboardDataFile.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/action/ceCAPlayerChoiceOption.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAMusic.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAStopConversation.h"
#include "../../conversation/action/ceCAStopTopic.h"
#include "../../conversation/action/ceCASnippet.h"
#include "../../conversation/action/ceCASetVariable.h"
#include "../../conversation/action/ceCASetActorParameter.h"
#include "../../conversation/action/ceCAActorCommand.h"
#include "../../conversation/action/ceCAGameCommand.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/action/ceCATrigger.h"
#include "../../conversation/action/ceCAActorAdd.h"
#include "../../conversation/action/ceCAActorRemove.h"
#include "../../conversation/action/ceCACoordSystemAdd.h"
#include "../../conversation/action/ceCACoordSystemRemove.h"
#include "../../conversation/action/ceCAComment.h"
#include "../../conversation/action/ceCAIfElseCase.h"
#include "../../conversation/condition/ceCConditionLogic.h"
#include "../../conversation/condition/ceCConditionHasActor.h"
#include "../../conversation/condition/ceCConditionActorInConversation.h"
#include "../../conversation/condition/ceCConditionVariable.h"
#include "../../conversation/condition/ceCConditionActorParameter.h"
#include "../../conversation/condition/ceCConditionActorCommand.h"
#include "../../conversation/condition/ceCConditionGameCommand.h"
#include "../../conversation/condition/ceCConditionTrigger.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../conversation/playback/cePlayback.h"
#include "../../conversation/playback/cePlaybackActionStackEntry.h"
#include "../../conversation/playback/cePlaybackProcessAction.h"
#include "../../conversation/actor/ceConversationActor.h"
#include "../../conversation/actor/speechAnimation/ceSpeechAnimation.h"
#include "../../conversation/strip/ceStrip.h"
#include "../../configuration/ceConfiguration.h"
#include "../../undosys/file/ceUCFileAdd.h"
#include "../../undosys/file/ceUCFileRemove.h"
#include "../../undosys/file/ceUCFileSetID.h"
#include "../../undosys/file/ceUCFilePaste.h"
#include "../../undosys/file/ceUCFileDuplicate.h"
#include "../../undosys/topic/ceUCTopicAdd.h"
#include "../../undosys/topic/ceUCTopicRemove.h"
#include "../../undosys/topic/ceUCTopicSetID.h"
#include "../../undosys/topic/ceUCTopicPaste.h"
#include "../../undosys/topic/ceUCTopicDuplicate.h"

#include <deigde/clipboard/igdeClipboard.h>
#include <deigde/clipboard/igdeClipboardDataReference.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTreeList.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/layout/igdeContainerBorderReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionContextMenu.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTreeListListener.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum ePanels{
	epEmpty,
	
	epACameraShot,
	epAMusic,
	epAActorSpeak,
	epAIfElse,
	epAPlayerChoice,
	epAStopConversation,
	epAStopTopic,
	epASnippet,
	epASetVariable,
	epASetAParam,
	epAActorCmd,
	epAGameCommand,
	epAWait,
	epATrigger,
	epAActorAdd,
	epAActorRemove,
	epACoordSystemAdd,
	epACoordSystemRemove,
	epAComment,
	
	epCLogic,
	epCHasActor,
	epCActorInConversation,
	epCVariable,
	epCAParam,
	epCActorCommand,
	epCGameCommand,
	epCTrigger
};



// Actions
////////////

namespace {

class cComboFile : public igdeComboBoxListener{
	ceWPTopic &pPanel;
	
public:
	cComboFile( ceWPTopic &panel ) : pPanel( panel ){}
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversation * const conversation = pPanel.GetConversation();
		if( conversation ){
			conversation->SetActiveFile( comboBox->GetSelectedItem()
				? ( ceConversationFile* )comboBox->GetSelectedItem()->GetData() : NULL );
		}
	}
};

class cActionGroupAdd : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionGroupAdd( ceWPTopic &panel ) : igdeAction( "Add...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add topic group" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetConversation();
		if( ! conversation ){
			return;
		}
		
		decString name( "Group" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Topic Group", "Name:", name ) ){
			return;
		}
		
		if( conversation->GetFileList().HasWithID( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Topic Group", "Topic group exists" );
			return;
		}
		
		igdeUndoReference undo;
		deObjectReference group;
		group.TakeOver( new ceConversationFile( name ) );
		undo.TakeOver( new ceUCFileAdd( conversation, ( ceConversationFile* )( deObject* )group ) );
		conversation->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetConversation() );
	}
};

class cActionGroupRemove : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionGroupRemove( ceWPTopic &panel ) : igdeAction( "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove topic group" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		if( pPanel.GetFile() ){
			igdeUndoReference undo;
			undo.TakeOver( new ceUCFileRemove( pPanel.GetFile() ) );
			pPanel.GetConversation()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetFile() );
	}
};

class cActionGroupRename : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionGroupRename( ceWPTopic &panel ) : igdeAction( "Rename...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Rename topic group" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetConversation();
		ceConversationFile * const file = pPanel.GetFile();
		if( ! file || ! conversation ){
			return;
		}
		
		decString name( file->GetID() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Topic Group", "Name:", name ) || name == file->GetID() ){
			return;
		}
		
		if( conversation->GetFileList().HasWithID( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Topic Group", "Topic group exists" );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCFileSetID( *conversation, file, name ) );
		conversation->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetFile() );
	}
};

class cActionGroupCopy : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionGroupCopy( ceWPTopic &panel ) : igdeAction( "Copy",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ), "Copy topic group" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversationFile * const file = pPanel.GetFile();
		if( ! file ){
			return;
		}
		
		igdeClipboardDataReference clip;
		ceConversationFileList list;
		list.Add( file );
		clip.TakeOver( new ceClipboardDataFile( list ) );
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( clip );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetFile() );
	}
};

class cActionGroupPaste : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionGroupPaste( ceWPTopic &panel ) : igdeAction( "Paste",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ), "Paste topic group" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetConversation();
		ceClipboardDataFile * const clip = ( ceClipboardDataFile* )pPanel.GetWindowProperties().GetWindowMain()
			.GetClipboard().GetWithTypeName( ceClipboardDataFile::TYPE_NAME );
		if( ! conversation || ! clip ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCFilePaste( conversation, clip->GetFiles() ) );
		conversation->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetConversation() && pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.HasWithTypeName( ceClipboardDataFile::TYPE_NAME ) );
	}
};

class cActionGroupDuplicate : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionGroupDuplicate( ceWPTopic &panel ) : igdeAction( "Duplicate...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Duplicate topic group" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversation * const conversation = pPanel.GetConversation();
		ceConversationFile * const file = pPanel.GetFile();
		if( ! file || ! conversation ){
			return;
		}
		
		decString name( file->GetID() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Duplicate Topic Group", "Name:", name ) ){
			return;
		}
		
		if( conversation->GetFileList().HasWithID( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Duplicate Topic Group", "Topic group exists" );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCFileDuplicate( conversation, *file, name ) );
		conversation->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetFile() );
	}
};

class cActionFileMenu : public igdeActionContextMenu{
	ceWPTopic &pPanel;
	
public:
	cActionFileMenu( ceWPTopic &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Show File Menu" ),
		pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( contextMenu, new cActionGroupAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionGroupRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionGroupRename( pPanel ), true );
		
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionGroupCopy( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionGroupPaste( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionGroupDuplicate( pPanel ), true );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetConversation() );
	}
};



class cComboTopic : public igdeComboBoxListener{
	ceWPTopic &pPanel;
	
public:
	cComboTopic( ceWPTopic &panel ) : pPanel( panel ){}
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversationFile * const file = pPanel.GetFile();
		if( file ){
			file->SetActiveTopic( comboBox->GetSelectedItem()
				? ( ceConversationTopic* )comboBox->GetSelectedItem()->GetData() : NULL );
		}
	}
};

class cActionTopicAdd : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionTopicAdd( ceWPTopic &panel ) : igdeAction( "Add...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Add topic" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversationFile * const file = pPanel.GetFile();
		if( ! file ){
			return;
		}
		
		decString name( "Topic" );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Add Topic", "Name:", name ) ){
			return;
		}
		
		if( file->GetTopicList().HasWithID( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Add Topic", "Topic exists" );
			return;
		}
		
		igdeUndoReference undo;
		deObjectReference topic;
		topic.TakeOver( new ceConversationTopic( name ) );
		undo.TakeOver( new ceUCTopicAdd( file, ( ceConversationTopic* )( deObject* )topic ) );
		pPanel.GetConversation()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetFile() );
	}
};

class cActionTopicRemove : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionTopicRemove( ceWPTopic &panel ) : igdeAction( "Remove",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiMinus ), "Remove topic" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		if( pPanel.GetTopic() ){
			igdeUndoReference undo;
			undo.TakeOver( new ceUCTopicRemove( pPanel.GetTopic() ) );
			pPanel.GetConversation()->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTopic() );
	}
};

class cActionTopicRename : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionTopicRename( ceWPTopic &panel ) : igdeAction( "Rename...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Rename topic" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversationFile * const file = pPanel.GetFile();
		ceConversationTopic * const topic = pPanel.GetTopic();
		ceConversation * const conversation = pPanel.GetConversation();
		if( ! topic || ! file || ! conversation ){
			return;
		}
		
		decString name( topic->GetID() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Rename Topic", "Name:", name ) || name == topic->GetID() ){
			return;
		}
		
		if( file->GetTopicList().HasWithID( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Rename Topic", "Topic exists" );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCTopicSetID( *conversation, topic, name ) );
		conversation->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTopic() );
	}
};

class cActionTopicMissingWords : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionTopicMissingWords( ceWPTopic &panel ) : igdeAction( "Missing Words...",
		NULL, "Show missing words" ), pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetTopic();
		if( ! topic ){
			return;
		}
		
		decStringSet missingWords;
		FindMissingWords( *pPanel.GetConversation(), topic->GetActionList(), missingWords );
		
		const int count = missingWords.GetCount();
		if( count == 0 ){
			igdeCommonDialogs::Information( &pPanel, "Missing Words", "No missing words found in topic" );
			return;
		}
		
		decStringList list;
		int i;
		missingWords.SortAscending();
		for( i=0; i<count; i++ ){
			list.Add( missingWords.GetAt( i ) );
		}
		decString result( list.Join( "\n" ) );
		igdeCommonDialogs::GetMultilineString( &pPanel.GetWindowProperties().GetWindowMain(),
			"Missing Words", "Missing words found in topic", result );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTopic() );
	}
	
	void FindMissingWords( const ceConversation &conversation,
	const ceConversationActionList &actions, decStringSet &missingWords ) const{
		const ceConversationActorList &actorList = conversation.GetActorList();
		const int count = actions.GetCount();
		int i, j;
		
		for( i=0; i<count; i++ ){
			const ceConversationAction &action = *actions.GetAt( i );
			
			switch( action.GetType() ){
			case ceConversationAction::eatActorSpeak:{
				const ceCAActorSpeak &speak = ( ceCAActorSpeak& )action;
				if( ! speak.GetUseSpeechAnimation() ){
					continue;
				}
				
				const ceStripList &words = speak.GetWordList();
				const decString &actorID = speak.GetActor();
				const int wordCount = words.GetCount();
				if( wordCount == 0 ){
					continue;
				}
				
				const ceConversationActor * const conversationActor = actorList.GetWithIDOrAliasID( actorID );
				if( ! conversationActor || ! conversationActor->GetSpeechAnimation() ){
					continue;
				}
				
				const ceSpeechAnimation &speechAnimation = *conversationActor->GetSpeechAnimation();
				const ceSAWordList &saWordList = speechAnimation.GetWordList();
				
				for( j=0; j<wordCount; j++ ){
					const decString &word = words.GetAt( j )->GetID();
					if( ! word.IsEmpty() && ! saWordList.HasNamed( word ) ){
						missingWords.Add( word );
					}
				}
				
				} break;
				
			case ceConversationAction::eatIfElse:{
				const ceCAIfElse &ifelse = ( ceCAIfElse& )action;
				const ceCAIfElseCaseList &cases = ifelse.GetCases();
				const int caseCount = cases.GetCount();
				
				for( j=0; j<caseCount; j++ ){
					FindMissingWords( conversation, cases.GetAt( j )->GetActions(), missingWords );
				}
				
				FindMissingWords( conversation, ifelse.GetElseActions(), missingWords );
				
				} break;
				
			case ceConversationAction::eatPlayerChoice:{
				const ceCAPlayerChoice &playerChoice = ( ceCAPlayerChoice& )action;
				const ceCAPlayerChoiceOptionList &options = playerChoice.GetOptions();
				const int optionCount = options.GetCount();
				
				FindMissingWords( conversation, playerChoice.GetActions(), missingWords );
				
				for( j=0; j<optionCount; j++ ){
					FindMissingWords( conversation, options.GetAt( j )->GetActions(), missingWords );
				}
				
				} break;
				
			case ceConversationAction::eatWait:{
				const ceCAWait &wait = ( ceCAWait& )action;
				FindMissingWords( conversation, wait.GetActions(), missingWords );
				
				} break;
				
			default:
				break;
			}
		}
	}
};

class cActionTopicCopy : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionTopicCopy( ceWPTopic &panel ) : igdeAction( "Copy",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiCopy ), "Copy topic" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversationTopic * const topic = pPanel.GetTopic();
		if( ! topic ){
			return;
		}
		
		igdeClipboardDataReference clip;
		ceConversationTopicList list;
		list.Add( topic );
		clip.TakeOver( new ceClipboardDataTopic( list ) );
		pPanel.GetWindowProperties().GetWindowMain().GetClipboard().Set( clip );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTopic() );
	}
};

class cActionTopicPaste : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionTopicPaste( ceWPTopic &panel ) : igdeAction( "Paste",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPaste ), "Paste topic" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversationFile * const file = pPanel.GetFile();
		ceClipboardDataTopic * const clip = ( ceClipboardDataTopic* )pPanel.GetWindowProperties().GetWindowMain()
			.GetClipboard().GetWithTypeName( ceClipboardDataTopic::TYPE_NAME );
		if( ! file || ! clip ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCTopicPaste( file, clip->GetTopics() ) );
		pPanel.GetConversation()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetFile() && pPanel.GetWindowProperties().GetWindowMain().GetClipboard()
			.HasWithTypeName( ceClipboardDataTopic::TYPE_NAME ) );
	}
};

class cActionTopicDuplicate : public igdeAction{
	ceWPTopic &pPanel;
	
public:
	cActionTopicDuplicate( ceWPTopic &panel ) : igdeAction( "Duplicate...",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiPlus ), "Duplicate topic" ),
		pPanel( panel ){}
	
	virtual void OnAction(){
		ceConversationFile * const file = pPanel.GetFile();
		ceConversationTopic * const topic = pPanel.GetTopic();
		if( ! file || ! topic ){
			return;
		}
		
		decString name( topic->GetID() );
		if( ! igdeCommonDialogs::GetString( &pPanel, "Duplicate Topic", "Name:", name ) ){
			return;
		}
		
		if( file->GetTopicList().HasWithID( name ) ){
			igdeCommonDialogs::Error( &pPanel, "Duplicate Topic", "Topic exists" );
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCTopicDuplicate( file, *topic, name ) );
		pPanel.GetConversation()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTopic() );
	}
};

class cActionTopicMenu : public igdeActionContextMenu{
	ceWPTopic &pPanel;
	
public:
	cActionTopicMenu( ceWPTopic &panel ) : igdeActionContextMenu( "",
		panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), "Show Topic Menu" ),
		pPanel( panel ){}
	
	virtual void AddContextMenuEntries( igdeMenuCascade &contextMenu ){
		igdeUIHelper &helper = contextMenu.GetEnvironment().GetUIHelper();
		
		helper.MenuCommand( contextMenu, new cActionTopicAdd( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionTopicRemove( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionTopicRename( pPanel ), true );
		
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionTopicMissingWords( pPanel ), true );
		
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionTopicCopy( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionTopicPaste( pPanel ), true );
		helper.MenuCommand( contextMenu, new cActionTopicDuplicate( pPanel ), true );
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetFile() );
	}
};



class cTreeActionsListener : public igdeTreeListListener{
	ceWPTopic &pPanel;
	
public:
	cTreeActionsListener( ceWPTopic &panel ) : pPanel( panel ){}
	
	virtual void OnSelectionChanged( igdeTreeList *treeList ){
		ceConversationTopic * const topic = pPanel.GetTopic();
		if( ! topic || pPanel.GetActionTreeModel()->GetPreventUpdate() ){
			return;
		}
		
		ceWPTTreeItem * const item = ( ceWPTTreeItem* )treeList->GetSelection();
		if( item ){
			item->OnSelected();
		}
		pPanel.SyncTopicActive();
	}
	
	virtual void OnItemExpanded( igdeTreeList*, igdeTreeItem *item ){
		( ( ceWPTTreeItem* )item )->OnExpanded();
	}
	
	virtual void OnItemCollapsed( igdeTreeList*, igdeTreeItem *item ){
		( ( ceWPTTreeItem* )item )->OnCollapsed();
	}
	
	virtual void AddContextMenuEntries( igdeTreeList *treeList, igdeMenuCascade &menu ){
		if( treeList->GetSelection() ){
			( ( ceWPTTreeItem* )treeList->GetSelection() )->OnContextMenu( menu );
			
		}else{
			pPanel.GetActionTreeModel()->OnContextMenu( menu );
		}
	}
};

}


// Class ceWPTopic
////////////////////

// Constructor, destructor
////////////////////////////

ceWPTopic::ceWPTopic( ceWindowProperties &windowProperties ) :
igdeContainerFlow( windowProperties.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esLast ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pConversation( NULL ),

pModelTreeActions( NULL ),
pPanelACameraShot( NULL ),
pPanelAMusic( NULL ),
pPanelAActorSpeak( NULL ),
pPanelAIfElse( NULL ),
pPanelAPlayerChoice( NULL ),
pPanelAStopConversation( NULL ),
pPanelAStopTopic( NULL ),
pPanelASnippet( NULL ),
pPanelASetVariable( NULL ),
pPanelASetAParam( NULL ),
pPanelAActorCmd( NULL ),
pPanelAGameCommand( NULL ),
pPanelAWait( NULL ),
pPanelATrigger( NULL ),
pPanelAActorAdd( NULL ),
pPanelAActorRemove( NULL ),
pPanelACoordSystemAdd( NULL ),
pPanelACoordSystemRemove( NULL ),
pPanelAComment( NULL ),

pPanelCLogic( NULL ),
pPanelCHasActor( NULL ),
pPanelCActorInConversation( NULL ),
pPanelCVariable( NULL ),
pPanelCAParam( NULL ),
pPanelCActorCommand( NULL ),
pPanelCGameCommand( NULL ),
pPanelCTrigger( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelperProperties();
	igdeContainerReference groupBox, formLine;
	
	pListener = new ceWPTopicListener( *this );
	
	
	// conversation
	helper.GroupBox( *this, groupBox, "Conversation:" );
	
	helper.FormLineStretchFirst( groupBox, "File:", "File to edit", formLine );
	helper.ComboBoxFilter( formLine, "File to edit", pCBFile, new cComboFile( *this ) );
	pCBFile->SetDefaultSorter();
	cActionFileMenu * const actionFileMenu = new cActionFileMenu( *this );
	helper.Button( formLine, pBtnFile, actionFileMenu, true );
	actionFileMenu->SetWidget( pBtnFile );
	
	helper.FormLineStretchFirst( groupBox, "Topic:", "Topic to edit", formLine );
	helper.ComboBoxFilter( formLine, "Topic to edit", pCBTopic, new cComboTopic( *this ) );
	pCBTopic->SetDefaultSorter();
	cActionTopicMenu * const actionTopicMenu = new cActionTopicMenu( *this );
	helper.Button( formLine, pBtnTopic, actionTopicMenu, true );
	actionTopicMenu->SetWidget( pBtnTopic );
	
	
	// actions
	igdeContainerBorderReference groupActions;
	igdeWidgetReference panel;
	
	helper.GroupBoxStaticBorder( *this, groupActions, "Actions:", true );
	helper.TreeList( 10, "Topic Actions", pTreeActions, new cTreeActionsListener( *this ) );
	groupActions->AddChild( pTreeActions, igdeContainerBorder::eaCenter );
	
	pSwitcher.TakeOver( new igdeSwitcher( env ) );
	groupActions->AddChild( pSwitcher, igdeContainerBorder::eaBottom );
	
	helper.Label( pSwitcher, "" ); // empty panel
	
	panel.TakeOver( pPanelACameraShot = new ceWPACameraShot( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAMusic = new ceWPAMusic( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAActorSpeak = new ceWPAActorSpeak( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAIfElse = new ceWPAIfElse( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAPlayerChoice = new ceWPAPlayerChoice( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAStopConversation = new ceWPAStopConversation( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAStopTopic = new ceWPAStopTopic( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelASnippet = new ceWPASnippet( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelASetVariable = new ceWPASetVariable( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelASetAParam = new ceWPASetAParam( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAActorCmd = new ceWPAActorCmd( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAGameCommand = new ceWPAGameCommand( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAWait = new ceWPAWait( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelATrigger = new ceWPATrigger( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAActorAdd = new ceWPAActorAdd( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAActorRemove = new ceWPAActorRemove( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelACoordSystemAdd = new ceWPACoordSystemAdd( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelACoordSystemRemove = new ceWPACoordSystemRemove( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelAComment = new ceWPAComment( *this ) );
	pSwitcher->AddChild( panel );
	
	panel.TakeOver( pPanelCLogic = new ceWPCLogic( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelCHasActor = new ceWPCHasActor( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelCActorInConversation = new ceWPCActorInConversation( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelCVariable = new ceWPCVariable( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelCAParam = new ceWPCAParam( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelCActorCommand = new ceWPCActorCommand( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelCGameCommand = new ceWPCGameCommand( *this ) );
	pSwitcher->AddChild( panel );
	panel.TakeOver( pPanelCTrigger = new ceWPCTrigger( *this ) );
	pSwitcher->AddChild( panel );
	
	pSwitcher->SetCurrent( epEmpty );
}

ceWPTopic::~ceWPTopic(){
	SetConversation( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void ceWPTopic::SetConversation( ceConversation *conversation ){
	if( conversation == pConversation ){
		return;
	}
	
	if( pModelTreeActions ){
		pModelTreeActions->SetTreeList( NULL );
		delete pModelTreeActions;
		pModelTreeActions = NULL;
	}
	
	if( pConversation ){
		pConversation->RemoveListener( pListener );
		pConversation->FreeReference();
	}
	
	pConversation = conversation;
	
	if( conversation ){
		conversation->AddListener( pListener );
		conversation->AddReference();
		
		pModelTreeActions = new ceWPTTreeModel( pWindowProperties.GetWindowMain(), conversation );
		pModelTreeActions->SetTreeList( pTreeActions );
		
	}else{
		pCBFile->ClearText();
		pCBTopic->ClearText();
	}
	
	const bool enabled = conversation;
	pCBFile->SetEnabled( enabled );
	pCBTopic->SetEnabled( enabled );
	pTreeActions->SetEnabled( enabled );
	
	pBtnFile->GetAction()->Update();
	pBtnTopic->GetAction()->Update();
	
	UpdateFileList();
	UpdateGestureLists();
	UpdateFacePoseLists();
	UpdateCameraShotLists();
	UpdateTargetLists();
}



ceConversationFile *ceWPTopic::GetFile() const{
	return pConversation ? pConversation->GetActiveFile() : NULL;
}

ceWPTTreeItemModel *ceWPTopic::GetActionTreeItem(){
	ceWPTTreeItem * const item = ( ceWPTTreeItem* )pTreeActions->GetSelection();
	return item ? item->GetModel() : NULL;
}

void ceWPTopic::UpdateFileList(){
	ceConversationFile * const file = GetFile();
	
	pCBFile->RemoveAllItems();
	
	if( pConversation ){
		const ceConversationFileList &list = pConversation->GetFileList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceConversationFile * const file = list.GetAt( i );
			pCBFile->AddItem( file->GetID(), NULL, file );
		}
		
		pCBFile->SortItems();
		pCBFile->StoreFilterItems();
	}
	
	SelectActiveFile();
	
	pPanelASnippet->UpdateFileList();
	
	if( file ){
		pConversation->SetActiveFile( file );
	}
}

void ceWPTopic::SelectActiveFile(){
	pCBFile->SetSelectionWithData( GetFile() );
	
	pCBTopic->SetEnabled( pCBFile->GetSelectedItem() );
	pBtnTopic->GetAction()->Update();
	if( ! pCBTopic->GetEnabled() ){
		pCBTopic->ClearText();
	}
	
	UpdateTopicList();
	UpdateFile();
}

void ceWPTopic::UpdateFile(){
}



ceConversationTopic *ceWPTopic::GetTopic() const{
	ceConversationFile * const file = GetFile();
	return file ? file->GetActiveTopic() : NULL;
}

void ceWPTopic::UpdateTopicList(){
	ceConversationTopic * const topic = GetTopic();
	ceConversationFile * const file = GetFile();
	
	pCBTopic->RemoveAllItems();
	
	if( file ){
		const ceConversationTopicList &list = file->GetTopicList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			ceConversationTopic * const topic = list.GetAt( i );
			pCBTopic->AddItem( topic->GetID(), NULL, topic );
		}
		
		pCBTopic->SortItems();
		pCBTopic->StoreFilterItems();
	}
	
	SelectActiveTopic();
	
	pPanelASnippet->UpdateTopicList();
	
	if( file && topic ){
		file->SetActiveTopic( topic );
	}
}

void ceWPTopic::SelectActiveTopic(){
	pCBTopic->SetSelectionWithData( GetTopic() );
	
	UpdateTopic();
}

void ceWPTopic::UpdateTopic(){
}



ceConversationAction *ceWPTopic::GetTreeAction() const{
	ceConversationTopic * const topic = GetTopic();
	if( ! topic ){
		return NULL;
	}
	
	const ceWPTTreeItem * const item = ( const ceWPTTreeItem* )pTreeActions->GetSelection();
	return item && item->GetModel() ? item->GetModel()->GetOwnerAction() : NULL;
}

ceConversationCondition *ceWPTopic::GetTreeCondition() const{
	ceConversationTopic * const topic = GetTopic();
	if( ! topic ){
		return NULL;
	}
	
	const ceWPTTreeItem * const item = ( ceWPTTreeItem* )pTreeActions->GetSelection();
	return item && item->GetModel() ? item->GetModel()->GetOwnerCondition() : NULL;
}

void ceWPTopic::SyncTopicActive(){
	if( pModelTreeActions ){
		ceConversationTopic * const topic = GetTopic();
		if( topic ){
			const ceWPTTreeModel::PreventUpdateGuard preventUpdate( *pModelTreeActions );
			topic->SetActive( GetTreeAction(), GetTreeCondition() );
		}
	}
	
	SelectActivePanel();
}

void ceWPTopic::SelectActivePanel(){
	const ceConversationCondition * const condition = GetTreeCondition();
	const ceConversationAction * const action = GetTreeAction();
	
	if( condition ){
		switch( condition->GetType() ){
		case ceConversationCondition::ectLogic:
			pSwitcher->SetCurrent( epCLogic );
			break;
			
		case ceConversationCondition::ectHasActor:
			pSwitcher->SetCurrent( epCHasActor );
			break;
			
		case ceConversationCondition::ectActorInConversation:
			pSwitcher->SetCurrent( epCActorInConversation );
			break;
			
		case ceConversationCondition::ectVariable:
			pSwitcher->SetCurrent( epCVariable );
			break;
			
		case ceConversationCondition::ectActorParameter:
			pSwitcher->SetCurrent( epCAParam );
			break;
			
		case ceConversationCondition::ectActorCommand:
			pSwitcher->SetCurrent( epCActorCommand );
			break;
			
		case ceConversationCondition::ectGameCommand:
			pSwitcher->SetCurrent( epCGameCommand );
			break;
			
		case ceConversationCondition::ectTrigger:
			pSwitcher->SetCurrent( epCTrigger );
			break;
		}
		
	}else if( action ){
		switch( action->GetType() ){
		case ceConversationAction::eatCameraShot:
			pSwitcher->SetCurrent( epACameraShot );
			break;
			
		case ceConversationAction::eatMusic:
			pSwitcher->SetCurrent( epAMusic );
			break;
			
		case ceConversationAction::eatActorSpeak:
			pSwitcher->SetCurrent( epAActorSpeak );
			break;
			
		case ceConversationAction::eatIfElse:
			pSwitcher->SetCurrent( epAIfElse );
			break;
			
		case ceConversationAction::eatPlayerChoice:
			pSwitcher->SetCurrent( epAPlayerChoice );
			break;
			
		case ceConversationAction::eatStopConversation:
			pSwitcher->SetCurrent( epAStopConversation );
			break;
			
		case ceConversationAction::eatStopTopic:
			pSwitcher->SetCurrent( epAStopTopic );
			break;
			
		case ceConversationAction::eatSnippet:
			pSwitcher->SetCurrent( epASnippet );
			break;
			
		case ceConversationAction::eatSetVariable:
			pSwitcher->SetCurrent( epASetVariable );
			break;
			
		case ceConversationAction::eatSetActorParameter:
			pSwitcher->SetCurrent( epASetAParam );
			break;
			
		case ceConversationAction::eatActorCommand:
			pSwitcher->SetCurrent( epAActorCmd );
			break;
			
		case ceConversationAction::eatGameCommand:
			pSwitcher->SetCurrent( epAGameCommand );
			break;
			
		case ceConversationAction::eatWait:
			pSwitcher->SetCurrent( epAWait );
			break;
			
		case ceConversationAction::eatTrigger:
			pSwitcher->SetCurrent( epATrigger );
			break;
			
		case ceConversationAction::eatActorAdd:
			pSwitcher->SetCurrent( epAActorAdd );
			break;
			
		case ceConversationAction::eatActorRemove:
			pSwitcher->SetCurrent( epAActorRemove );
			break;
			
		case ceConversationAction::eatCoordSystemAdd:
			pSwitcher->SetCurrent( epACoordSystemAdd );
			break;
			
		case ceConversationAction::eatCoordSystemRemove:
			pSwitcher->SetCurrent( epACoordSystemRemove );
			break;
			
		case ceConversationAction::eatComment:
			pSwitcher->SetCurrent( epAComment );
			break;
		}
		
	}else{
		pSwitcher->SetCurrent( epEmpty );
	}
	
	UpdateActive();
}

void ceWPTopic::UpdateActive(){
	switch( ( ePanels )pSwitcher->GetCurrent() ){
	case epACameraShot:
		pPanelACameraShot->UpdateAction();
		break;
		
	case epAMusic:
		pPanelAMusic->UpdateAction();
		break;
		
	case epAActorSpeak:
		pPanelAActorSpeak->UpdateAction();
		break;
		
	case epAIfElse:
		pPanelAIfElse->UpdateAction();
		break;
		
	case epAPlayerChoice:
		pPanelAPlayerChoice->UpdateAction();
		break;
		
	case epAStopConversation:
		pPanelAStopConversation->UpdateAction();
		break;
		
	case epAStopTopic:
		pPanelAStopTopic->UpdateAction();
		break;
		
	case epASnippet:
		pPanelASnippet->UpdateAction();
		break;
		
	case epASetVariable:
		pPanelASetVariable->UpdateAction();
		break;
		
	case epASetAParam:
		pPanelASetAParam->UpdateAction();
		break;
		
	case epAActorCmd:
		pPanelAActorCmd->UpdateAction();
		break;
		
	case epAGameCommand:
		pPanelAGameCommand->UpdateAction();
		break;
		
	case epAWait:
		pPanelAWait->UpdateAction();
		break;
		
	case epATrigger:
		pPanelATrigger->UpdateAction();
		break;
		
	case epAActorAdd:
		pPanelAActorAdd->UpdateAction();
		break;
		
	case epAActorRemove:
		pPanelAActorRemove->UpdateAction();
		break;
		
	case epACoordSystemAdd:
		pPanelACoordSystemAdd->UpdateAction();
		break;
		
	case epACoordSystemRemove:
		pPanelACoordSystemRemove->UpdateAction();
		break;
		
	case epAComment:
		pPanelAComment->UpdateAction();
		break;
		
	case epCLogic:
		pPanelCLogic->UpdateCondition();
		break;
		
	case epCHasActor:
		pPanelCHasActor->UpdateCondition();
		break;
		
	case epCActorInConversation:
		pPanelCActorInConversation->UpdateCondition();
		break;
		
	case epCVariable:
		pPanelCVariable->UpdateCondition();
		break;
		
	case epCAParam:
		pPanelCAParam->UpdateCondition();
		break;
		
	case epCActorCommand:
		pPanelCActorCommand->UpdateCondition();
		break;
		
	case epCGameCommand:
		pPanelCGameCommand->UpdateCondition();
		break;
		
	case epCTrigger:
		pPanelCTrigger->UpdateCondition();
		break;
		
	case epEmpty:
		break;
	}
}



void ceWPTopic::UpdateGestureLists(){
}

void ceWPTopic::UpdateFacePoseLists(){
}

void ceWPTopic::UpdateCameraShotLists(){
	pPanelACameraShot->UpdateCameraShotList();
}

void ceWPTopic::UpdateActorLists(){
	pPanelACameraShot->UpdateActorIDLists();
	pPanelAMusic->UpdateActorIDLists();
	pPanelAActorSpeak->UpdateActorIDLists();
	pPanelAIfElse->UpdateActorIDLists();
	pPanelAPlayerChoice->UpdateActorIDLists();
	pPanelAStopConversation->UpdateActorIDLists();
	pPanelAStopTopic->UpdateActorIDLists();
	pPanelASnippet->UpdateActorIDLists();
	pPanelASetVariable->UpdateActorIDLists();
	pPanelASetAParam->UpdateActorIDLists();
	pPanelAActorCmd->UpdateActorIDLists();
	pPanelAGameCommand->UpdateActorIDLists();
	pPanelAWait->UpdateActorIDLists();
	pPanelATrigger->UpdateActorIDLists();
	pPanelAActorAdd->UpdateActorIDLists();
	pPanelAActorRemove->UpdateActorIDLists();
	pPanelACoordSystemAdd->UpdateActorIDLists();
	pPanelACoordSystemRemove->UpdateActorIDLists();
	pPanelAComment->UpdateActorIDLists();
	
	pPanelCHasActor->UpdateActorList();
	pPanelCActorInConversation->UpdateActorList();
	pPanelCAParam->UpdateActorIDList();
	pPanelCActorCommand->UpdateActorIDList();
}

void ceWPTopic::UpdateTargetLists(){
	pPanelACameraShot->UpdateTargetList();
}

void ceWPTopic::UpdateConvoCoordSysLists(){
	pPanelACoordSystemAdd->UpdateConvoCoordSysIDLists();
	pPanelACoordSystemRemove->UpdateConvoCoordSysIDLists();
}



void ceWPTopic::LocateAction( ceConversationAction *action ){
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pModelTreeActions ){
		return;
	}
	
	ceWPTTIMAction *item = pModelTreeActions->DeepFindAction( action );
	if( ! item ){
		return;
	}
	
	item->SetAsCurrentItem();
	SyncTopicActive();
}

void ceWPTopic::PlayActionFromHere(){
	if( ! pConversation ){
		return;
	}
	
	ceConversationTopic * const topic = GetTopic();
	if( ! topic ){
		return;
	}
	
	ceWPTTreeItem * const item = ( ceWPTTreeItem* )pTreeActions->GetSelection();
	if( ! item || ! item->GetModel() ){
		return;
	}
	
	if( ! pConversation->GetPlayback()->GetActionStack().GetTop().GetParentList() ){
		return; // this should never happen
	}
	
	cePlayback &playback = *pConversation->GetPlayback();
	playback.SetRunning( false );
	playback.SetPaused( false );
	playback.SetTopic( topic );
	playback.Rewind();
	item->GetModel()->BuildPlaybackFromHere();
	
	playback.SetRunning( true );
}
