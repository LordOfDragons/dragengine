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

#ifndef _CECCONDITIONTRIGGER_H_
#define _CECCONDITIONTRIGGER_H_

#include <dragengine/common/string/decString.h>

#include "ceConversationCondition.h"



/**
 * \brief Conversation condition testing trigger state.
 */
class ceCConditionTrigger : public ceConversationCondition{
public:
	/** \brief Test mode. */
	enum eTestModes{
		/** \brief True if the trigger is in fired state. */
		etmFired,
		
		/** \brief True if the trigger is in reset state. */
		etmNotFired,
		
		/** \brief True if the trigger has ever been fired. */
		etmEverFired,
		
		/** \brief True if the trigger has never been fired. */
		etmNeverFired,
	};
	
	
	
private:
	decString pTrigger;
	eTestModes pTestMode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new conversation condition. */
	ceCConditionTrigger();
	
	/** \brief Creates a new conversation condition. */
	ceCConditionTrigger(const ceCConditionTrigger &condition);
	
	/** \brief Cleans up the conversation condition. */
	~ceCConditionTrigger() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Name of the trigger to test. */
	inline const decString &GetTrigger() const{ return pTrigger; }
	
	/** \brief Set name of the trigger to test. */
	void SetTrigger(const char *name);
	
	/** \brief Test mode. */
	inline eTestModes GetTestMode() const{ return pTestMode; }
	
	/** \brief Set test mode. */
	void SetTestMode(eTestModes testMode);
	
	/** \brief Create copy of condition. */
    ceConversationCondition *CreateCopy() const override;
	/*@}*/
};

#endif
