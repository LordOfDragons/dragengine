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

#ifndef _CECAIFELSE_H_
#define _CECAIFELSE_H_

#include "ceConversationAction.h"
#include "ceConversationActionList.h"
#include "ceCAIfElseCaseList.h"

#include <dragengine/common/string/decString.h>

class ceConversationCondition;



/**
 * \brief If-Else Conversation Action.
 * 
 * Process one of two lists of actions depending on a condition. If the condition
 * holds true the if list of actions is processed otherwise the else list of
 * actions.
 * 
 */
class ceCAIfElse : public ceConversationAction{
private:
	ceCAIfElseCaseList pCases;
	ceConversationActionList pElseActions;
	bool pTIMExpanded;
	bool pTIMElseExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create if-else conversation action. */
	ceCAIfElse();
	
	/** \brief Create copy of if-else conversation action. */
	ceCAIfElse( const ceCAIfElse &action );
	
	/** \brief Clean up if-else conversation action. */
	virtual ~ceCAIfElse();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief List of cases. */
	inline ceCAIfElseCaseList &GetCases(){ return pCases; }
	inline const ceCAIfElseCaseList &GetCases() const{ return pCases; }
	
	/** \brief List of else-actions. */
	inline ceConversationActionList &GetElseActions(){ return pElseActions; }
	inline const ceConversationActionList &GetElseActions() const{ return pElseActions; }
	
	/** \brief Create copy of action. */
	virtual ceConversationAction *CreateCopy() const;
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded( bool expanded );
	
	/** \brief Else tree list item expanded state. */
	inline bool GetTIMElseExpanded() const{ return pTIMElseExpanded; }
	
	/** \brief Set else tree list item expanded state. */
	void SetTIMElseExpanded( bool expanded );
	/*@}*/
};

#endif
