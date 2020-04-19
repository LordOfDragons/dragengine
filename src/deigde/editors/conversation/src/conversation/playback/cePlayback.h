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
 * \brief Conversation playback.
 */
class cePlayback{
public:
	/** \brief Camera handling. */
	enum eCameraHandling{
		/** \brief Conversation playback. */
		echConversation,
		
		/** \brief Selected camera shot. */
		echCameraShot,
		
		/** \brief Free moving. */
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
	/** \brief Create playback. */
	cePlayback( ceConversation &conversation );
	
	/** \brief Clean up playback. */
	~cePlayback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation(){ return pConversation; }
	inline const ceConversation &GetConversation() const{ return pConversation; }
	
	/** \brief Playback camera. */
	inline cePlaybackCamera *GetCamera() const{ return pCamera; }
	
	
	
	/** \brief Topic to play back. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief Set topic to play back. */
	void SetTopic( ceConversationTopic *topic );
	
	
	
	/** \brief Conversation is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	/** \brief Set conversation is running. */
	void SetRunning( bool running );
	
	/** \brief Conversation is paused. */
	inline bool GetPaused() const{ return pPaused; }
	
	/** \brief Set conversation is paused. */
	void SetPaused( bool paused );
	
	/** \brief Rewind to beginning of topic. */
	void Rewind();
	
	/** \brief Reset camera. */
	void ResetCamera();
	
	/** \brief Camera handling. */
	inline eCameraHandling GetCameraHandling() const{ return pCameraHandling; }
	
	/** \brief Set camera handling. */
	void SetCameraHandling( eCameraHandling handling );
	
	
	
	/** \brief Action stack. */
	inline cePlaybackActionStack &GetActionStack(){ return pActionStack; }
	inline const cePlaybackActionStack &GetActionStack() const{ return pActionStack; }
	
	/** \brief Command list. */
	inline cePlaybackCommandList &GetCommands(){ return pCommandList; }
	inline const cePlaybackCommandList &GetCommandList() const{ return pCommandList; }
	
	/** \brief Variable list. */
	inline cePlaybackVariableList &GetVariables(){ return pVariableList; }
	inline const cePlaybackVariableList &GetVariableList() const{ return pVariableList; }
	
	/** \brief List of missing words encountered so far. */
	inline decStringSet &GetMissingWords(){ return pMissingWords; }
	inline const decStringSet &GetMissingWords() const{ return pMissingWords; }
	
	/** \brief Trigger table. */
	inline igdeTriggerTargetList &GetTriggerTable(){ return pTriggerTable; }
	inline const igdeTriggerTargetList &GetTriggerTable() const{ return pTriggerTable; }
	
	
	
	/** \brief Play single action. */
	void PlaySingleAction( ceConversationAction *action, float time );
	
	
	
	/** \brief Number of actors. */
	inline int GetActorCount() const{ return pActorCount; }
	
	/** \brief Set number of actors. */
	void SetActorCount( int count );
	
	/** \brief Actor. */
	cePlaybackActor &GetActorAt( int index ) const;
	
	
	
	/** \brief Update playback. */
	void Update( float elapsed );
	
	/** \brief Update camera. */
	void UpdateCamera( float elapsed );
	
	/** \brief Process actions. */
	void ProcessActions( float elapsed );
	
	/** \brief Advance to next action. */
	void AdvanceToNextAction();
	
	/** \brief Fast forward until the actor stopped speaking. */
	void FastForwardSpeaking();
	
	/** \brief Cancel looping action list. */
	void CancelLoopingLayer( int stackDepth );
	
	/** \brief Last action played back or \em NULL if not set. */
	inline ceConversationAction *GetLastPlayedAction() const{ return pLastPlayedAction; }
	
	/** \brief Last action played back parent topic or \em NULL if not set. */
	inline ceConversationTopic *GetLastPlayedActionTopic() const{ return pLastPlayedActionTopic; }
	
	
	
	/** \brief Set text box text. */
	void SetTextBoxText( const decUnicodeString &text );
	
	/** \brief Clear text box text. */
	void ClearTextBoxText();
	/*@}*/
	
	
	
private:
	void SetLastPlayedAction( ceConversationTopic *topic, ceConversationAction *action );
};

#endif
