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
