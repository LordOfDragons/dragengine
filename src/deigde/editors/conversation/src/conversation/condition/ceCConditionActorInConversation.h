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

#ifndef _CECCONDITIONACTORINCONVERSATION_H_
#define _CECCONDITIONACTORINCONVERSATION_H_

#include "ceConversationCondition.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Actor in conversation conversation Condition.
 */
class ceCConditionActorInConversation : public ceConversationCondition{
private:
	decString pActor;
	bool pNegate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation condition. */
	ceCConditionActorInConversation();
	
	/** \brief Create copy of conversation condition. */
	ceCConditionActorInConversation( const ceCConditionActorInConversation &condition );
	
	/** \brief Clean up conversation condition. */
	virtual ~ceCConditionActorInConversation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Actor id. */
	inline const decString &GetActor() const{ return pActor; }
	
	/** \brief Set actor id. */
	void SetActor( const char *id );
	
	/** \brief Negate condition. */
	inline bool GetNegate() const{ return pNegate; }
	
	/** \brief Set if condition is negated. */
	void SetNegate( bool negate );
	
	/** \brief Create copy of condition. */
    virtual ceConversationCondition *CreateCopy() const;
	/*@}*/
};

#endif
