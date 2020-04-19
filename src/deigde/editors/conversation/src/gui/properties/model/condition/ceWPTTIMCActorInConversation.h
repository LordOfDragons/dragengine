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

#ifndef _CEWPTTIMCACTORINCONVERSATION_H_
#define _CEWPTTIMCACTORINCONVERSATION_H_

#include "ceWPTTIMCondition.h"
#include "../../../../conversation/condition/ceCConditionActorInConversation.h"


/**
 * \brief Condition actor in conversation.
 */
class ceWPTTIMCActorInConversation : public ceWPTTIMCondition{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMCActorInConversation( ceWindowMain &windowMain, ceConversation &conversation,
		ceConversationAction &action, ceCConditionActorInConversation *condition );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMCActorInConversation();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Condition. */
	inline ceCConditionActorInConversation *GetConditionActorInConversation() const{
		return ( ceCConditionActorInConversation* )GetCondition();
	}
	
	/** \brief Update condition. */
	virtual void Update();
	/*@}*/
};

#endif
