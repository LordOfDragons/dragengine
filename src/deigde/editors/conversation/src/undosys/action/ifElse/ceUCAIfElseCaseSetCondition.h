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

#ifndef _CEUCAIFELSECASESETCONDITION_H_
#define _CEUCAIFELSECASESETCONDITION_H_

#include <deigde/undo/igdeUndo.h>

#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/condition/ceConversationCondition.h"



/**
 * \brief Undo action if-else conversation action set case condition.
 */
class ceUCAIfElseCaseSetCondition : public igdeUndo{
public:
	typedef deTObjectReference<ceUCAIfElseCaseSetCondition> Ref;
	
	
private:
	ceConversationTopic::Ref pTopic;
	ceCAIfElse::Ref pIfElse;
	ceCAIfElseCase::Ref pCase;
	ceConversationCondition::Ref pOldCondition;
	ceConversationCondition::Ref pNewCondition;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo object. */
	ceUCAIfElseCaseSetCondition(ceConversationTopic *topic, ceCAIfElse *ifElse,
	ceCAIfElseCase *ifcase, ceConversationCondition *newCondition);
	
	/** \brief Clean up undo object. */
protected:
	virtual ~ceUCAIfElseCaseSetCondition();
public:
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
