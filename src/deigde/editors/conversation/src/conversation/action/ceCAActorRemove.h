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

#ifndef _CECAACTORREMOVE_H_
#define _CECAACTORREMOVE_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Remove actor conversation action.
 * \details Removes an actor from the conversation if existing.
 */
class ceCAActorRemove : public ceConversationAction{
private:
	decString pActor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation action. */
	ceCAActorRemove();
	/** \brief Creates a new conversation action. */
	ceCAActorRemove( const ceCAActorRemove &action );
	/** \brief Cleans up the conversation action. */
	virtual ~ceCAActorRemove();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the actor id. */
	inline const decString &GetActor() const{ return pActor; }
	/** \brief Sets the actor id. */
	void SetActor( const char *id );
	
	/** \brief Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
