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
