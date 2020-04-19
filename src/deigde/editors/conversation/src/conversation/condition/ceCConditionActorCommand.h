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

#ifndef _CECCONDITIONACTORCOMMAND_H_
#define _CECCONDITIONACTORCOMMAND_H_

#include <dragengine/common/string/decString.h>

#include "ceConversationCondition.h"



/**
 * \brief Conversation condition using the value returned by a command send to an actor.
 * \details The command is a single text string parsed by the conversation script into an array
 *          of arguments and then send to the player action. The format of the command depends
 *          on the type of the player action the actor is using at the time of the command being
 *          send. The command returns a boolean value use directly as condition result.
 */
class ceCConditionActorCommand : public ceConversationCondition{
private:
	decString pActor;
	decString pCommand;
	bool pNegate;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation condition. */
	ceCConditionActorCommand();
	/** \brief Creates a new conversation condition. */
	ceCConditionActorCommand( const ceCConditionActorCommand &condition );
	/** \brief Cleans up the conversation condition. */
	virtual ~ceCConditionActorCommand();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the actor id. */
	inline const decString &GetActor() const{ return pActor; }
	/** \brief Sets the actor id. */
	void SetActor( const char *id );
	/** \brief Retrieves the command. */
	inline const decString &GetCommand() const{ return pCommand; }
	/** \brief Sets the command. */
	void SetCommand( const char *command );
	/** \brief Determines if the result is true if the command returned false. */
	inline bool GetNegate() const{ return pNegate; }
	/** \brief Sets if the result is true if the command returned false. */
	void SetNegate( bool negate );
	
	/** \brief Create a copy of this condition. */
    virtual ceConversationCondition *CreateCopy() const;
	/*@}*/
};

#endif
