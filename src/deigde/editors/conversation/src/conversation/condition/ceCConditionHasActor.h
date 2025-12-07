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

#ifndef _CECCONDITIONHASACTOR_H_
#define _CECCONDITIONHASACTOR_H_

#include "ceConversationCondition.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Has Actor Conversation Condition.
 * Determines if a named actor does or does not exist in the conversation.
 */
class ceCConditionHasActor : public ceConversationCondition{
private:
	decString pActor;
	bool pNegate;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new conversation condition. */
	ceCConditionHasActor();
	/** Creates a new conversation condition. */
	ceCConditionHasActor(const ceCConditionHasActor &condition);
	/** Cleans up the conversation condition. */
	virtual ~ceCConditionHasActor();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the actor id. */
	inline const decString &GetActor() const{return pActor;}
	/** Sets the actor id. */
	void SetActor(const char *id);
	/** Determines if the test is for non-existence instead of existence. */
	inline bool GetNegate() const{return pNegate;}
	/** Sets if the test is for non-existence instead of existence. */
	void SetNegate(bool negate);
	
	/** Create a copy of this condition. */
    virtual ceConversationCondition *CreateCopy() const;
	/*@}*/
};

#endif
