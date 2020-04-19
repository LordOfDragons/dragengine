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

#ifndef _CECCONDITIONHASACTOR_H_
#define _CECCONDITIONHASACTOR_H_

#include "ceConversationCondition.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Has Actor Conversation Condition.
 * Determines if a named actor does or does not exist in the conversation.
 */
class ceCConditionHasActor : public ceConversationCondition{
private:
	decString pActor;
	bool pNegate;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation condition. */
	ceCConditionHasActor();
	/** Creates a new conversation condition. */
	ceCConditionHasActor( const ceCConditionHasActor &condition );
	/** Cleans up the conversation condition. */
	virtual ~ceCConditionHasActor();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the actor id. */
	inline const decString &GetActor() const{ return pActor; }
	/** Sets the actor id. */
	void SetActor( const char *id );
	/** Determines if the test is for non-existence instead of existence. */
	inline bool GetNegate() const{ return pNegate; }
	/** Sets if the test is for non-existence instead of existence. */
	void SetNegate( bool negate );
	
	/** Create a copy of this condition. */
    virtual ceConversationCondition *CreateCopy() const;
	/*@}*/
};

#endif
