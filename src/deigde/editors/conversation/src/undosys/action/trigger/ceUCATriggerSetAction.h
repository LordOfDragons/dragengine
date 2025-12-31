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

#ifndef _CEUCATRIGGERSETACTION_H_
#define _CEUCATRIGGERSETACTION_H_

#include "../../../conversation/action/ceCATrigger.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <deigde/undo/igdeUndo.h>




/**
 * \brief Undo action trigger conversation action set action.
 */
class ceUCATriggerSetAction : public igdeUndo{
public:
	typedef deTObjectReference<ceUCATriggerSetAction> Ref;
	
	
private:
	ceConversationTopic::Ref pTopic;
	ceCATrigger::Ref pAction;
	ceCATrigger::eActions pOldAction;
	ceCATrigger::eActions pNewAction;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCATriggerSetAction(ceConversationTopic *topic, ceCATrigger *action, ceCATrigger::eActions newOperator);
	/** \brief Cleans up the undo object. */
protected:
	virtual ~ceUCATriggerSetAction();
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
