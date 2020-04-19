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

#ifndef _CECONVERSATIONCONDITION_H_
#define _CECONVERSATIONCONDITION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decIntSet.h>



/**
 * \brief Conversation condition.
 */
class ceConversationCondition : public deObject{
public:
	/** \brief Condition types. */
	enum eConditionTypes{
		/** \brief Logic operator. */
		ectLogic,
		
		/** \brief Actor is present in the conversation. */
		ectHasActor,
		
		/** \brief Actor is in a conversation. */
		ectActorInConversation,
		
		/** \brief Compare variable against value. */
		ectVariable,
		
		/** \brief Compare actor parameter against value. */
		ectActorParameter,
		
		/** \brief Send condition command an actor. */
		ectActorCommand,
		
		/** \brief Send condition command to game. */
		ectGameCommand,
		
		/** \brief Trigger state. */
		ectTrigger
	};
	
	
	
private:
	eConditionTypes pType;
	decIntSet pTreeExpandedLists;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation condition. */
	ceConversationCondition( eConditionTypes type );
	
	/** \brief Create copy of conversation condition. */
	ceConversationCondition( const ceConversationCondition &condition );
	
	/** \brief Clean up conversation action. */
	virtual ~ceConversationCondition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eConditionTypes GetType() const{ return pType; }
	
	/** \brief List of expanded lists in a tree view. */
	inline decIntSet &GetTreeExpandedLists(){ return pTreeExpandedLists; }
	inline const decIntSet &GetTreeExpandedLists() const{ return pTreeExpandedLists; }
	
	/** \brief Create copy of condition. */
    virtual ceConversationCondition *CreateCopy() const = 0;
	/*@}*/
};

#endif
