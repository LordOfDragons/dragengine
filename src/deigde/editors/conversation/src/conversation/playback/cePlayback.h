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

#ifndef _CEPLAYBACK_H_
#define _CEPLAYBACK_H_

#include "cePlaybackActionStack.h"
#include "command/cePlaybackCommandList.h"
#include "variable/cePlaybackVariableList.h"
#include "../action/ceConversationActionList.h"

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
	bool pActionWaiting;
	bool pAutoAdvanceCommands;
	eCameraHandling pCameraHandling;
	float pActionTime;
	ceTextBoxText *pTextBoxText;
	cePlaybackCamera *pCamera;
	cePlaybackActionStack pActionStack;
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
	
	
	
	/** Action stack. */
	inline cePlaybackActionStack &GetActionStack(){ return pActionStack; }
	inline const cePlaybackActionStack &GetActionStack() const{ return pActionStack; }
	
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
	void SetLastPlayedAction( ceConversationTopic *topic, ceConversationAction *action );
};

#endif
