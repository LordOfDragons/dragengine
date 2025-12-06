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

#ifndef _CECAGAMECOMMAND_H_
#define _CECAGAMECOMMAND_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Conversation action sending a command to the game.
 * \details The command is a single text string parsed by the conversation script into an
 *          array of arguments and then send to the conversation playback subclass. The
 *          format of the command is game specific. It is the responsibility of the game
 *          to handle invalid or unknown commands.
 */
class ceCAGameCommand : public ceConversationAction{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceCAGameCommand> Ref;
	
	
private:
	decString pCommand;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceCAGameCommand();
	/** \brief Creates a new conversation action. */
	ceCAGameCommand( const ceCAGameCommand &action );
	/** \brief Cleans up the conversation action. */
	virtual ~ceCAGameCommand();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the command string. */
	inline const decString &GetCommand() const{ return pCommand; }
	/** \brief Sets the command string. */
	void SetCommand( const char *command );
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
