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
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceCAActorCommand> Ref;
	
	
private:
	decString pActor;
	decString pCommand;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation action. */
	ceCAActorCommand();
	/** Creates a new conversation action. */
	ceCAActorCommand(const ceCAActorCommand &action);
	/** Cleans up the conversation action. */
	virtual ~ceCAActorCommand();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the actor id. */
	inline const decString &GetActor() const{return pActor;}
	/** Sets the actor id. */
	void SetActor(const char *id);
	/** Retrieves the command string. */
	inline const decString &GetCommand() const{return pCommand;}
	/** Sets the command string. */
	void SetCommand(const char *command);
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
