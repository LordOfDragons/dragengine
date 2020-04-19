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

#ifndef _CECCONDITIONLOGIC_H_
#define _CECCONDITIONLOGIC_H_

#include "ceConversationCondition.h"
#include "ceConversationConditionList.h"



/**
 * \brief Logic Conversation Condition Class.
 */
class ceCConditionLogic : public ceConversationCondition{
public:
	/** Logic operators. */
	enum eOperators{
		/** None of the conditions are true. */
		eopNone,
		/** One or more conditions are true. */
		eopAny,
		/** All conditions are true. */
		eopAll
	};
	
private:
	eOperators pOperator;
	ceConversationConditionList pConditions;
	bool pTIMExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation condition. */
	ceCConditionLogic();
	/** Creates a new conversation condition. */
	ceCConditionLogic( const ceCConditionLogic &condition );
	/** Cleans up the conversation condition. */
	virtual ~ceCConditionLogic();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	/** Sets the operator. */
	void SetOperator( eOperators aOperator );
	/** Retrieves the list of conditions. */
	inline ceConversationConditionList &GetConditions(){ return pConditions; }
	inline const ceConversationConditionList &GetConditions() const { return pConditions; }
	
	/** Create a copy of this condition. */
    virtual ceConversationCondition *CreateCopy() const;
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded( bool expanded );
	/*@}*/
};

#endif
