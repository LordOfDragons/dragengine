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

#ifndef _CEPLAYBACKPROCESSACTION_H_
#define _CEPLAYBACKPROCESSACTION_H_

class ceCACoordSystemRemove;
class ceCACoordSystemAdd;
class ceCAActorRemove;
class ceCAActorAdd;
class ceCATrigger;
class decString;
class cePlayback;
class ceConversation;
class ceConversationAction;
class ceCACameraShot;
class ceCAActorSpeak;
class ceCAIfElse;
class ceCAPlayerChoice;
class ceCASnippet;
class ceCAStopConversation;
class ceCAStopTopic;
class ceCASetVariable;
class ceCASetActorParameter;
class ceCAActorCommand;
class ceCAGameCommand;
class ceCAWait;



/**
 * \brief Conversation Playback Process Action.
 * \details Helper class knowing how to process an action of arbitrary type.
 */
class cePlaybackProcessAction{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new process action object. */
	cePlaybackProcessAction();
	/** \brief Cleans up the process action object. */
	~cePlaybackProcessAction();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Process action. */
	void ProcessAction( ceConversation &conversation, ceConversationAction *action );
	
	/** \brief Process camera shot action. */
	void ProcessCameraShot( ceConversation &conversation, const ceCACameraShot &action );
	/** \brief Process actor speak action. */
	void ProcessActorSpeak( ceConversation &conversation, ceCAActorSpeak &action );
	/** \brief Process snippet action. */
	void ProcessSnippet( ceConversation &conversation, ceCASnippet *action );
	/** \brief Process stop conversation action. */
	void ProcessStopConversation( ceConversation &conversation );
	/** \brief Process stop topic action. */
	void ProcessStopTopic( ceConversation &conversation );
	/** \brief Process if-else action. */
	void ProcessIfElse( ceConversation &conversation, ceCAIfElse *action );
	/** \brief Process player choice action. */
	void ProcessPlayerChoice( ceConversation &conversation, ceCAPlayerChoice *action );
	/** \brief Process set variable action. */
	void ProcessSetVariable( ceConversation &conversation, const ceCASetVariable &action );
	/** \brief Process set actor parameter action. */
	void ProcessSetActorParameter( ceConversation &conversation, const ceCASetActorParameter &action );
	/** \brief Process actor command action. */
	void ProcessActorCommand( ceConversation &conversation, const ceCAActorCommand &action );
	/** \brief Process game command action. */
	void ProcessGameCommand( ceConversation &conversation, const ceCAGameCommand &action );
	/** \brief Process wait action. */
	void ProcessWait( ceConversation &conversation, ceCAWait *action );
	/** \brief Process trigger action. */
	void ProcessTrigger( ceConversation &conversation, const ceCATrigger &action );
	/** \brief Process add actor action. */
	void ProcessActorAdd( ceConversation &conversation, const ceCAActorAdd &action );
	/** \brief Process remove actor action. */
	void ProcessActorRemove( ceConversation &conversation, const ceCAActorRemove &action );
	/** \brief Process add coordinate system action. */
	void ProcessCoordSystemAdd( ceConversation &conversation, const ceCACoordSystemAdd &action );
	/** \brief Process remove coordinate system action. */
	void ProcessCoordSystemRemove( ceConversation &conversation, const ceCACoordSystemRemove &action );
	
	/**
	 * \brief Retrieves the index the actor in the given target if existing or -1.
	 * \details Triggers an error if an actor by ID is not found or the actor index
	 *          is outside of boundaries.
	 */
	int GetTargetActor( ceConversation &conversation, const decString &targetName );
	
	
	/** \brief Display error than the actor with the given id does not exist and stop playback. */
	void ErrorNoActorWithID( cePlayback &playback, const char *id );
	/** \brief Display error than the named target does not exist and stop playback. */
	void ErrorTargetNotFound( cePlayback &playback, const char *name );
	/*@}*/
};

#endif
