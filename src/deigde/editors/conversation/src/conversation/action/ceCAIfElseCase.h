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

#ifndef _CECAIFELSECASE_H_
#define _CECAIFELSECASE_H_

#include "ceConversationAction.h"
#include "../condition/ceConversationCondition.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>


/**
 * \brief Player Choice Option.
 */
class ceCAIfElseCase : public deObject{
private:
	ceConversationCondition::Ref pCondition;
	ceConversationAction::List pActions;
	bool pTIMExpanded, pTIMConditionExpanded, pTIMActionsExpanded;
	
public:
	typedef deTObjectReference<ceCAIfElseCase> Ref;
	typedef decTObjectOrderedSet<ceCAIfElseCase> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create if else case. */
	ceCAIfElseCase();
	
	/** \brief Create if else case. */
	ceCAIfElseCase(const ceCAIfElseCase &ifcase);
	
	/** \brief Clean up the if else case. */
protected:
	virtual ~ceCAIfElseCase();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Condition or \em nullptr if there is none. */
	inline const ceConversationCondition::Ref &GetCondition() const{ return pCondition; }
	
	/** \brief Set condition or \em nullptr if there is none. */
	void SetCondition(ceConversationCondition *condition);
	
	/** \brief List of actions. */
	inline ceConversationAction::List &GetActions(){ return pActions; }
	inline const ceConversationAction::List &GetActions() const{ return pActions; }
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded(bool expanded);
	
	/** \brief Condition tree list item expanded state. */
	inline bool GetTIMConditionExpanded() const{ return pTIMConditionExpanded; }
	
	/** \brief Set condition tree list item expanded state. */
	void SetTIMConditionExpanded(bool expanded);
	
	/** \brief Actions tree list item expanded state. */
	inline bool GetTIMActionsExpanded() const{ return pTIMActionsExpanded; }
	
	/** \brief Set actions tree list item expanded state. */
	void SetTIMActionsExpanded(bool expanded);
	/*@}*/
};

#endif
