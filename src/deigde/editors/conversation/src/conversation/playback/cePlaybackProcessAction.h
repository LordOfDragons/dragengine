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
