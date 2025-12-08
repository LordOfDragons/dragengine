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

#ifndef _CEUNDOCACTION_H_
#define _CEUNDOCACTION_H_

#include <dragengine/deObject.h>

class ceConversationAction;
class ceConversationTopic;



/**
 * \brief Undo Conversation Action.
 * Stores an action including the topic it is located under for undo purpose.
 */
class ceUndoCAction : public deObject{
private:
	ceConversationAction::Ref pAction;
	ceConversationTopic::Ref pTopic;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUndoCAction> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUndoCAction(ceConversationAction *action, ceConversationTopic *topic);
protected:
	/** \brief Clean up undo. */
	virtual ~ceUndoCAction();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the action. */
	inline ceConversationAction *GetAction() const{ return pAction; }
	/** \brief Retrieves the topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	/*@}*/
};

#endif
