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

#ifndef _CECAACTORCOMMAND_H_
#define _CECAACTORCOMMAND_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Actor Command Conversation Action.
 * Sends a command to an actor, more precisely to the player action object assigned to it.
 * The command is a single text string parsed by the conversation script into an array of
 * arguments and then send to the player action. The format of the command depends on the
 * type of the player action the actor is using at the time of the command being send.
 * If the command is not understood or improperly formatted the player action does nothing.
 * Thus it is important to ensure make sure this action is used in the proper situation.
 */
class ceCAActorCommand : public ceConversationAction{
private:
	decString pActor;
	decString pCommand;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation action. */
	ceCAActorCommand();
	/** Creates a new conversation action. */
	ceCAActorCommand( const ceCAActorCommand &action );
	/** Cleans up the conversation action. */
	virtual ~ceCAActorCommand();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the actor id. */
	inline const decString &GetActor() const{ return pActor; }
	/** Sets the actor id. */
	void SetActor( const char *id );
	/** Retrieves the command string. */
	inline const decString &GetCommand() const{ return pCommand; }
	/** Sets the command string. */
	void SetCommand( const char *command );
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
