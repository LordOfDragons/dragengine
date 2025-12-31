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
public:
	typedef deTObjectReference<ceCConditionActorCommand> Ref;

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
	ceCConditionActorCommand(const ceCConditionActorCommand &condition);
	/** \brief Cleans up the conversation condition. */
protected:
	~ceCConditionActorCommand() override;
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the actor id. */
	inline const decString &GetActor() const{ return pActor; }
	/** \brief Sets the actor id. */
	void SetActor(const char *id);
	/** \brief Retrieves the command. */
	inline const decString &GetCommand() const{ return pCommand; }
	/** \brief Sets the command. */
	void SetCommand(const char *command);
	/** \brief Determines if the result is true if the command returned false. */
	inline bool GetNegate() const{ return pNegate; }
	/** \brief Sets if the result is true if the command returned false. */
	void SetNegate(bool negate);
	
	/** \brief Create a copy of this condition. */
    ceConversationCondition::Ref CreateCopy() const override;
	/*@}*/
};

#endif
