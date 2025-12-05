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

#include "ceDialogPasteSnippet.h"
#include "../../conversation/strip/ceStrip.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/action/ceCAPlayerChoice.h"
#include "../../conversation/action/ceCAWait.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceCACameraShot.h"
#include "../../conversation/action/ceCAComment.h"
#include "../../conversation/action/ceCAGameCommand.h"
#include "../../conversation/action/ceCAActorCommand.h"
#include "../../conversation/action/ceCAIfElse.h"
#include "../../conversation/target/ceTarget.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>



// Class ceDialogPasteSnippet
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceDialogPasteSnippet::ceDialogPasteSnippet( igdeEnvironment &environment, ceConversation *conversation ) :
igdeDialog( environment, "Paste Conversation Snippet" ),
pConversation( conversation )
{
	if( ! conversation ){
		DETHROW( deeInvalidParam );
	}
	
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainer::Ref content;
	content.TakeOver( new igdeContainerForm( environment, igdeContainerForm::esLast ) );
	
	helper.ComboBoxFilter( content, "Camera Shot 1:", true,
		"Camera shot to use for the first actor", pCBCameraShot1, NULL );
	pCBCameraShot1->SetDefaultSorter();
	
	helper.ComboBoxFilter( content, "Camera Shot 2:", true,
		"Camera shot to use for the second actor", pCBCameraShot2, NULL );
	pCBCameraShot2->SetDefaultSorter();
	
	helper.ComboBoxFilter( content, "Target 1:", true,
		"Target to use for the camera shot for the first actor", pCBTarget1, NULL );
	pCBTarget1->SetDefaultSorter();
	
	helper.ComboBoxFilter( content, "Target 2:", true,
		"Target to use for the camera shot for the second actor", pCBTarget2, NULL );
	pCBTarget2->SetDefaultSorter();
	
	helper.EditFloat( content, "Wait Camera Shot:",
		"Delay in seconds for added camera shot actions", pEditDelayCameraShot, NULL );
	
	helper.EditFloat( content, "Wait Actor Speak:",
		"Delay in seconds for added actor speak actions", pEditDelayActorSpeak, NULL );
	
	helper.EditFloat( content, "Scale Actor Speak:",
		"Scale duration of spoken words added to actor speak actions", pEditScaleActorSpeak, NULL );
	
	helper.EditString( content, "Snippet:", "Conversation snippet to generate actions from",
		pEditSnippet, 80, 20, NULL );
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar( buttonBar, "Generate Actions", "Cancel" );
	
	AddContent( content, buttonBar );
	
	// update lists
	UpdateCameraShotLists();
	UpdateTargetLists();
	
	// restore last used parameters
	const ceConversation::sPasteSnippetDialogParams &params = conversation->GetPasteSnippetDialogParams();
	pEditSnippet->SetText( params.snippet );
	pCBCameraShot1->SetText( params.cameraShot1 );
	pCBCameraShot2->SetText( params.cameraShot2 );
	pCBTarget1->SetText( params.target1 );
	pCBTarget2->SetText( params.target2 );
	pEditDelayCameraShot->SetFloat( params.delayCameraShot );
	pEditDelayActorSpeak->SetFloat( params.delayActorSpeak );
	pEditScaleActorSpeak->SetFloat( params.scaleActorSpeak );
}

ceDialogPasteSnippet::~ceDialogPasteSnippet(){
}



// Management
///////////////

void ceDialogPasteSnippet::UpdateCameraShotLists(){
	const ceCameraShotList list( pConversation->AllCameraShots() );
	const decString selection1( pCBCameraShot1->GetText() );
	const decString selection2( pCBCameraShot2->GetText() );
	const int count = list.GetCount();
	int i;
	
	pCBCameraShot1->RemoveAllItems();
	pCBCameraShot2->RemoveAllItems();
	
	for( i=0; i<count; i++ ){
		const decString &name = list.GetAt( i )->GetName();
		pCBCameraShot1->AddItem( name );
		pCBCameraShot2->AddItem( name );
	}
	
	pCBCameraShot1->SortItems();
	pCBCameraShot2->SortItems();
	
	pCBCameraShot1->StoreFilterItems();
	pCBCameraShot2->StoreFilterItems();
	
	pCBCameraShot1->SetText( selection1 );
	pCBCameraShot2->SetText( selection2 );
}

void ceDialogPasteSnippet::UpdateTargetLists(){
	const ceTargetList list( pConversation->AllTargets() );
	const decString &selection1( pCBTarget1->GetText() );
	const decString &selection2( pCBTarget2->GetText() );
	const int count = list.GetCount();
	int i;
	
	pCBTarget1->RemoveAllItems();
	pCBTarget2->RemoveAllItems();
	
	for( i=0; i<count; i++ ){
		const decString &name = list.GetAt( i )->GetName();
		pCBTarget1->AddItem( name );
		pCBTarget2->AddItem( name );
	}
	
	pCBTarget1->SortItems();
	pCBTarget2->SortItems();
	
	pCBTarget1->StoreFilterItems();
	pCBTarget2->StoreFilterItems();
	
	pCBTarget1->SetText( selection1 );
	pCBTarget2->SetText( selection2 );
}



void ceDialogPasteSnippet::GenerateActions(){
	const decString &cameraShot1 = pCBCameraShot1->GetText();
	const decString &cameraShot2 = pCBCameraShot2->GetText();
	const decString &target1 = pCBTarget1->GetText();
	const decString &target2 = pCBTarget2->GetText();
	const bool hasCameraShot1 = ! cameraShot1.IsEmpty();
	const bool hasCameraShot2 = ! cameraShot2.IsEmpty();
	const float delayCameraShots = pEditDelayCameraShot->GetFloat();
	const float delayActorSpeak = pEditDelayActorSpeak->GetFloat();
	const float scaleActorSpeak = pEditScaleActorSpeak->GetFloat();
	decString actor, lastActor, text;
	deObjectReference action;
	bool firstActor = true;
	int i;
	
	// parse snippet into actions
	const decStringList snippet( pEditSnippet->GetText().Split( '\n' ) );
	
	const int count = snippet.GetCount();
	for( i=0; i<count; i++ ){
		const decString &line = snippet.GetAt( i );
		const int delimiter = line.Find( ':' );
		
		// if line starts with a dash generate a comment
		if( line.GetLength() > 0 && line.GetAt( 0 ) == '#' ){
			action.TakeOver( new ceCAComment );
			ceCAComment * const actionComment = action;
			actionComment->SetComment( line.GetMiddle( 1 ).GetTrimmed() );
			pActions.Add( actionComment );
			continue;
		}
		
		// if line starts with a bang generate a command action
		if( line.GetLength() > 0 && line.GetAt( 0 ) == '!' ){
			// if we have an actor specified create an actor command
			if( delimiter == -1 ){
				action.TakeOver( new ceCAGameCommand );
				ceCAGameCommand * const actionGameCommand = action;
				actionGameCommand->SetCommand( line.GetMiddle( 1 ).GetTrimmed() );
				pActions.Add( actionGameCommand );
				
			// else create a game comment
			}else{
				action.TakeOver( new ceCAActorCommand );
				ceCAActorCommand * const actionActorCommand = action;
				actionActorCommand->SetActor( line.GetLeft( delimiter ).GetTrimmed() );
				actionActorCommand->SetCommand( line.GetMiddle( delimiter + 1 ).GetTrimmed() );
				pActions.Add( actionActorCommand );
			}
			
			continue;
		}
		
		// get actor speaking
		if( delimiter == -1 ){
			actor = lastActor;
			text = line.GetTrimmed();
			
		}else{
			actor = line.GetLeft( delimiter ).GetTrimmed();
			text = line.GetMiddle( delimiter + 1 ).GetTrimmed();
		}
		
		// skip if empty or unusable
		if( actor.IsEmpty() || text.IsEmpty() ){
			continue;
		}
		
		// if the actor changed add a camera shot
		if( actor != lastActor ){
			if( firstActor ? hasCameraShot1 : hasCameraShot2 ){
				action.TakeOver( new ceCACameraShot );
				ceCACameraShot * const actionCameraShot = action;
				actionCameraShot->SetName( firstActor ? cameraShot1 : cameraShot2 );
				actionCameraShot->SetCameraTarget( firstActor ? target2 : target1 );
				actionCameraShot->SetDuration( 10.0f );
				actionCameraShot->SetLookAtTarget( firstActor ? target1 : target2 );
				actionCameraShot->SetDelay( delayCameraShots );
				pActions.Add( actionCameraShot );
			}
			firstActor = ! firstActor;
		}
		
		// add action speak for the actor
		action.TakeOver( new ceCAActorSpeak( pConversation->GetEngine() ) );
		ceCAActorSpeak * const actionActorSpeak = action;
		actionActorSpeak->SetActor( actor );
		actionActorSpeak->SetTextBoxText( decUnicodeString::NewFromUTF8( text ) );
		actionActorSpeak->SetUseSpeechAnimation( true );
		actionActorSpeak->SetDelay( delayActorSpeak );
		WordsFromText( actionActorSpeak->GetWordList(), actionActorSpeak->GetTextBoxText(), scaleActorSpeak );
		pActions.Add( actionActorSpeak );
		
		// keep track of which actor spoke the last time
		lastActor = actor;
	}
}

void ceDialogPasteSnippet::WordsFromText( ceStripList &wordList, const decUnicodeString &text, float scale ){
	const decUnicodeString padding = decUnicodeString::NewFromUTF8( " \t\n\r,.;:\"?!" );
	const float letterDuration = 0.075f;
// 	const float phrasePause = 0.5f;
	const int length = text.GetLength();
	deObjectReference entry;
	decUnicodeString word;
	int i;
	
	wordList.RemoveAll();
	
	for( i=0; i<=length; i++ ){
		const int character = i < length ? text.GetAt( i ) : ' ';
		
		if( padding.Find( character, 0 ) != -1 ){
			if( word.GetLength() > 0 ){
				entry.TakeOver( new ceStrip( word.GetLower().ToUTF8(),
					letterDuration * ( float )word.GetLength() * scale, 0.0f ) );
				wordList.Add( entry );
				word.SetFromUTF8( "" );
			}
			
		}else{
			word.AppendCharacter( character );
		}
	}
}



bool ceDialogPasteSnippet::Accept(){
	// save last used parameters
	ceConversation::sPasteSnippetDialogParams &params = pConversation->GetPasteSnippetDialogParams();
	params.snippet = pEditSnippet->GetText();
	params.cameraShot1 = pCBCameraShot1->GetText();
	params.cameraShot2 = pCBCameraShot2->GetText();
	params.target1 = pCBTarget1->GetText();
	params.target2 = pCBTarget2->GetText();
	params.delayCameraShot = pEditDelayCameraShot->GetFloat();
	params.delayActorSpeak = pEditDelayActorSpeak->GetFloat();
	params.scaleActorSpeak = pEditScaleActorSpeak->GetFloat();
	
	// generate actions
	GenerateActions();
	
	return igdeDialog::Accept();
}



void ceDialogPasteSnippet::OnDialogShown(){
	pEditSnippet->Focus();
}
