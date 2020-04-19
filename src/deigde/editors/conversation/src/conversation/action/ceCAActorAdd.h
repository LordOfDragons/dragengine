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

#ifndef _CECAACTORADD_H_
#define _CECAACTORADD_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Add actor conversation action.
 * \details Adds an actor to the conversation if not existing already.
 */
class ceCAActorAdd : public ceConversationAction{
private:
	decString pID;
	decString pAliasID;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceCAActorAdd();
	/** \brief Creates a new conversation action. */
	ceCAActorAdd( const ceCAActorAdd &action );
	/** \brief Cleans up the conversation action. */
	virtual ~ceCAActorAdd();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the actor id. */
	inline const decString &GetID() const{ return pID; }
	/** \brief Sets the actor id. */
	void SetID( const char *id );
	/** \brief Retrieves the alias id or an empty string if not used. */
	inline const decString &GetAliasID() const{ return pAliasID; }
	/** \brief Sets the alias id or an empty string if not used. */
	void SetAliasID( const char *id );
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
