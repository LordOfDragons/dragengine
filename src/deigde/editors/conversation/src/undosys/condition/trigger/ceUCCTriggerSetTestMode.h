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

#ifndef _CEUCCTRIGGERSETTESTMODE_H_
#define _CEUCCTRIGGERSETTESTMODE_H_

#include "../../../conversation/condition/ceCConditionTrigger.h"

#include <deigde/undo/igdeUndo.h>

class ceConversationAction;
class ceConversationTopic;



/**
 * \brief Undo trigger conversation condition set test mode.
 */
class ceUCCTriggerSetTestMode : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceConversationAction *pAction;
	ceCConditionTrigger *pCondition;
	ceCConditionTrigger::eTestModes pOldTestMode;
	ceCConditionTrigger::eTestModes pNewTestMode;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCCTriggerSetTestMode> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCCTriggerSetTestMode( ceConversationTopic *topic, ceConversationAction *action,
		ceCConditionTrigger *condition, ceCConditionTrigger::eTestModes newTestMode );
	/** \brief Cleans up the undo object. */
	virtual ~ceUCCTriggerSetTestMode();
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
