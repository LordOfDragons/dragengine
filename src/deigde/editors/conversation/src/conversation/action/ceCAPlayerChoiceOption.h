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

#ifndef _CECAPLAYERCHOICEOPTION_H_
#define _CECAPLAYERCHOICEOPTION_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#include "ceConversationActionList.h"

class ceConversationCondition;


/**
 * \brief Player choice option.
 */
class ceCAPlayerChoiceOption : public deObject{
private:
	decUnicodeString pText;
	ceConversationCondition *pCondition;
	ceConversationActionList pActions;
	bool pTIMExpanded;
	bool pTIMConditionExpanded;
	bool pTIMActionsExpanded;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create player choice option. */
	ceCAPlayerChoiceOption();
	
	/** \brief Create copy of player choice option. */
	ceCAPlayerChoiceOption( const ceCAPlayerChoiceOption &option );
	
protected:
	/** \brief Clean up player choice option. */
	virtual ~ceCAPlayerChoiceOption();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Display text. */
	inline const decUnicodeString &GetText() const{ return pText; }
	
	/** \brief Set display text. */
	void SetText( const decUnicodeString &text );
	
	/** \brief Condition or \em NULL if there is none. */
	inline ceConversationCondition *GetCondition() const{ return pCondition; }
	
	/** \brief Set condition or \em NULL if there is none. */
	void SetCondition( ceConversationCondition *condition );
	
	/** \brief Actions. */
	inline ceConversationActionList &GetActions(){ return pActions; }
	inline const ceConversationActionList &GetActions() const{ return pActions; }
	/*@}*/
	
	
	
	/** \name UI */
	/*@{*/
	/** \brief Tree list item expanded state. */
	inline bool GetTIMExpanded() const{ return pTIMExpanded; }
	
	/** \brief Set tree list item expanded state. */
	void SetTIMExpanded( bool expanded );
	
	/** \brief Condition tree list item expanded state. */
	inline bool GetTIMConditionExpanded() const{ return pTIMConditionExpanded; }
	
	/** \brief Set condition tree list item expanded state. */
	void SetTIMConditionExpanded( bool expanded );
	
	/** \brief Actions tree list item expanded state. */
	inline bool GetTIMActionsExpanded() const{ return pTIMActionsExpanded; }
	
	/** \brief Set actions tree list item expanded state. */
	void SetTIMActionsExpanded( bool expanded );
	/*@}*/
};

#endif
