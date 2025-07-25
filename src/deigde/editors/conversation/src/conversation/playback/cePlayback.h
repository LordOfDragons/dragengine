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

#ifndef _CEPLAYBACK_H_
#define _CEPLAYBACK_H_

#include "cePlaybackActionStack.h"
#include "command/cePlaybackCommandList.h"
#include "variable/cePlaybackVariableList.h"
#include "../action/ceConversationActionList.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#include <deigde/triggersystem/igdeTriggerTargetList.h>

class ceConversation;
class ceConversationTopic;
class ceCameraShot;
class cePlaybackActor;
class ceTextBoxText;
class cePlaybackCamera;



/**
 * Conversation playback.
 */
class cePlayback{
public:
	/** Camera handling. */
	enum eCameraHandling{
		/** Conversation playback. */
		echConversation,
		
		/** Selected camera shot. */
		echCameraShot,
		
		/** Free moving. */
		echFree
	};
	
	
	
private:
	ceConversation &pConversation;
	
	ceConversationTopic *pTopic;
	cePlaybackActor *pActors;
	int pActorCount;
	bool pRunning;
	bool pPaused;
	bool pAutoAdvanceCommands;
	eCameraHandling pCameraHandling;
	ceTextBoxText *pTextBoxText;
	cePlaybackCamera *pCamera;
	cePlaybackActionStack::Ref pMainActionStack, pActiveActionStack;
	decObjectOrderedSet pSideActionStacks;
	cePlaybackCommandList pCommandList;
	cePlaybackVariableList pVariableList;
	ceConversationActionList pTestActionList;
	igdeTriggerTargetList pTriggerTable;
	decStringSet pMissingWords;
	
	ceConversationAction *pLastPlayedAction;
	ceConversationTopic *pLastPlayedActionTopic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create playback. */
	cePlayback( ceConversation &conversation );
	
	/** Clean up playback. */
	~cePlayback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Conversation. */
	inline ceConversation &GetConversation(){ return pConversation; }
	inline const ceConversation &GetConversation() const{ return pConversation; }
	
	/** Playback camera. */
	inline cePlaybackCamera *GetCamera() const{ return pCamera; }
	
	
	
	/** Topic to play back. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** Set topic to play back. */
	void SetTopic( ceConversationTopic *topic );
	
	
	
	/** Conversation is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	/** Set conversation is running. */
	void SetRunning( bool running );
	
	/** Conversation is paused. */
	inline bool GetPaused() const{ return pPaused; }
	
	/** Set conversation is paused. */
	void SetPaused( bool paused );
	
	/** Rewind to beginning of topic. */
	void Rewind();
	
	/** Reset camera. */
	void ResetCamera();
	
	/** Camera handling. */
	inline eCameraHandling GetCameraHandling() const{ return pCameraHandling; }
	
	/** Set camera handling. */
	void SetCameraHandling( eCameraHandling handling );
	
	/** Auto advance certain commands or wait for user. */
	inline bool GetAutoAdvanceCommands() const{ return pAutoAdvanceCommands; }
	
	/** Set to auto advance certain commands or wait for user. */
	void SetAutoAdvanceCommands( bool autoAdvance );
	
	
	
	/** Active action stack. */
	inline const cePlaybackActionStack::Ref &GetActiveActionStack() const{ return pActiveActionStack; }
	
	/** Main action stack. */
	inline const cePlaybackActionStack::Ref &GetMainActionStack() const{ return pMainActionStack; }
	
	/** Side action stacks. */
	inline const decObjectOrderedSet &GetSideActionStacks() const{ return pSideActionStacks; }
	
	/** Add side action stack and run it once. */
	void AddSideActionStack(const cePlaybackActionStack::Ref &stack);
	
	/** Command list. */
	inline cePlaybackCommandList &GetCommands(){ return pCommandList; }
	inline const cePlaybackCommandList &GetCommandList() const{ return pCommandList; }
	
	/** Variable list. */
	inline cePlaybackVariableList &GetVariables(){ return pVariableList; }
	inline const cePlaybackVariableList &GetVariableList() const{ return pVariableList; }
	
	/** List of missing words encountered so far. */
	inline decStringSet &GetMissingWords(){ return pMissingWords; }
	inline const decStringSet &GetMissingWords() const{ return pMissingWords; }
	
	/** Trigger table. */
	inline igdeTriggerTargetList &GetTriggerTable(){ return pTriggerTable; }
	inline const igdeTriggerTargetList &GetTriggerTable() const{ return pTriggerTable; }
	
	
	
	/** Play single action. */
	void PlaySingleAction( ceConversationAction *action, float time );
	
	
	
	/** Number of actors. */
	inline int GetActorCount() const{ return pActorCount; }
	
	/** Set number of actors. */
	void SetActorCount( int count );
	
	/** Actor. */
	cePlaybackActor &GetActorAt( int index ) const;
	
	
	
	/** Update playback. */
	void Update( float elapsed );
	
	/** Update camera. */
	void UpdateCamera( float elapsed );
	
	/** Process actions. */
	void ProcessActions( float elapsed );
	
	/** Advance to next action. */
	void AdvanceToNextAction();
	
	/** Fast forward until the actor stopped speaking. */
	void FastForwardSpeaking();
	
	/** Cancel looping action list. */
	void CancelLoopingLayer( int stackDepth );
	
	/** Last action played back or \em NULL if not set. */
	inline ceConversationAction *GetLastPlayedAction() const{ return pLastPlayedAction; }
	
	/** Last action played back parent topic or \em NULL if not set. */
	inline ceConversationTopic *GetLastPlayedActionTopic() const{ return pLastPlayedActionTopic; }
	
	
	
	/** Set text box text. */
	void SetTextBoxText( const decUnicodeString &text );
	
	/** Clear text box text. */
	void ClearTextBoxText();
	/*@}*/
	
	
	
private:
	void pProcessActions(float elapsed);
	void SetLastPlayedAction( ceConversationTopic *topic, ceConversationAction *action );
};

#endif
