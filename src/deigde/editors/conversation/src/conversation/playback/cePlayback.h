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

#include "cePlaybackActor.h"
#include "cePlaybackActionStack.h"
#include "cePlaybackCamera.h"
#include "command/cePlaybackCommand.h"
#include "../action/ceConversationAction.h"
#include "../topic/ceConversationTopic.h"
#include "../textbox/ceTextBoxText.h"

#include <dragengine/common/collection/decTDictionary.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#include <deigde/triggersystem/igdeTriggerTargetList.h>

class ceConversation;
class ceCameraShot;



/**
 * Conversation playback.
 */
class cePlayback : public deObject{
public:
	typedef deTObjectReference<cePlayback> Ref;
	typedef decTDictionary<decString, int> VariableMap;
	
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
	
	ceConversationTopic::Ref pTopic;
	decTObjectOrderedSet<cePlaybackActor> pActors;
	bool pRunning;
	bool pPaused;
	bool pAutoAdvanceCommands;
	eCameraHandling pCameraHandling;
	ceTextBoxText::Ref pTextBoxText;
	cePlaybackCamera::Ref pCamera;
	cePlaybackActionStack::Ref pMainActionStack, pActiveActionStack;
	cePlaybackActionStack::List pSideActionStacks;
	cePlaybackCommand::List pCommands;
	VariableMap pVariables;
	ceConversationAction::List pTestActionList;
	igdeTriggerTargetList pTriggerTable;
	decStringSet pMissingWords;
	
	ceConversationAction::Ref pLastPlayedAction;
	ceConversationTopic::Ref pLastPlayedActionTopic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create playback. */
	cePlayback(ceConversation &conversation);
	
	/** Clean up playback. */
protected:
	~cePlayback();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Conversation. */
	inline ceConversation &GetConversation(){ return pConversation; }
	inline const ceConversation &GetConversation() const{ return pConversation; }
	
	/** Playback camera. */
	inline const cePlaybackCamera::Ref &GetCamera() const{ return pCamera; }
	
	
	
	/** Topic to play back. */
	inline const ceConversationTopic::Ref &GetTopic() const{ return pTopic; }
	
	/** Set topic to play back. */
	void SetTopic(ceConversationTopic *topic);
	
	
	
	/** Conversation is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	/** Set conversation is running. */
	void SetRunning(bool running);
	
	/** Conversation is paused. */
	inline bool GetPaused() const{ return pPaused; }
	
	/** Set conversation is paused. */
	void SetPaused(bool paused);
	
	/** Rewind to beginning of topic. */
	void Rewind();
	
	/** Reset camera. */
	void ResetCamera();
	
	/** Camera handling. */
	inline eCameraHandling GetCameraHandling() const{ return pCameraHandling; }
	
	/** Set camera handling. */
	void SetCameraHandling(eCameraHandling handling);
	
	/** Auto advance certain commands or wait for user. */
	inline bool GetAutoAdvanceCommands() const{ return pAutoAdvanceCommands; }
	
	/** Set to auto advance certain commands or wait for user. */
	void SetAutoAdvanceCommands(bool autoAdvance);
	
	
	
	/** Active action stack. */
	inline const cePlaybackActionStack::Ref &GetActiveActionStack() const{ return pActiveActionStack; }
	
	/** Main action stack. */
	inline const cePlaybackActionStack::Ref &GetMainActionStack() const{ return pMainActionStack; }
	
	/** Side action stacks. */
	inline const cePlaybackActionStack::List &GetSideActionStacks() const{ return pSideActionStacks; }
	
	/** Add side action stack and run it once. */
	void AddSideActionStack(const cePlaybackActionStack::Ref &stack);
	
	/** Command list. */
	inline cePlaybackCommand::List &GetCommands(){ return pCommands; }
	inline const cePlaybackCommand::List &GetCommands() const{ return pCommands; }
	
	/** Variable list. */
	inline VariableMap &GetVariables(){ return pVariables; }
	inline const VariableMap &GetVariables() const{ return pVariables; }
	
	/** List of missing words encountered so far. */
	inline decStringSet &GetMissingWords(){ return pMissingWords; }
	inline const decStringSet &GetMissingWords() const{ return pMissingWords; }
	
	/** Trigger table. */
	inline igdeTriggerTargetList &GetTriggerTable(){ return pTriggerTable; }
	inline const igdeTriggerTargetList &GetTriggerTable() const{ return pTriggerTable; }
	
	
	
	/** Play single action. */
	void PlaySingleAction(ceConversationAction *action, float time);
	
	
	
	/** Number of actors. */
	int GetActorCount() const;
	
	/** Set number of actors. */
	void SetActorCount(int count);
	
	/** Actor. */
	cePlaybackActor &GetActorAt(int index) const;
	
	
	
	/** Update playback. */
	void Update(float elapsed);
	
	/** Update camera. */
	void UpdateCamera(float elapsed);
	
	/** Process actions. */
	void ProcessActions(float elapsed);
	
	/** Advance to next action. */
	void AdvanceToNextAction();
	
	/** Fast forward until the actor stopped speaking. */
	void FastForwardSpeaking();
	
	/** Cancel looping action list. */
	void CancelLoopingLayer(int stackDepth);
	
	/** Last action played back or \em nullptr if not set. */
	inline const ceConversationAction::Ref &GetLastPlayedAction() const{ return pLastPlayedAction; }
	
	/** Last action played back parent topic or \em nullptr if not set. */
	inline const ceConversationTopic::Ref &GetLastPlayedActionTopic() const{ return pLastPlayedActionTopic; }
	
	
	
	/** Set text box text. */
	void SetTextBoxText(const decUnicodeString &text);
	
	/** Clear text box text. */
	void ClearTextBoxText();
	/*@}*/
	
	
	
private:
	void pProcessActions(float elapsed);
	void SetLastPlayedAction(ceConversationTopic *topic, ceConversationAction *action);
};

#endif
