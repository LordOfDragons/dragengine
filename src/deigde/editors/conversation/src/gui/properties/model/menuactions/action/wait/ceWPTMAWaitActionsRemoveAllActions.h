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

#ifndef _CEWPTMAWAITACTIONSREMOVEALLACTIONS_H_
#define _CEWPTMAWAITACTIONSREMOVEALLACTIONS_H_

#include "../ceWPTMARemoveAllActions.h"

class ceConversationTopic;
class ceCAWait;



/**
 * \brief Menu action remove all actions.
 */
class ceWPTMAWaitActionsRemoveAllActions : public ceWPTMARemoveAllActions{
public:
	typedef deTObjectReference<ceWPTMAWaitActionsRemoveAllActions> Ref;

private:
	ceConversationTopic *pTopic;
	ceCAWait *pWait;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAWaitActionsRemoveAllActions();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAWaitActionsRemoveAllActions(ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic, ceCAWait &wait);
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief If-else action. */
	inline ceCAWait *GetWait() const{ return pWait; }

	/** \brief Create undo action for adding action. */
	igdeUndo::Ref CreateUndo() override;
	/*@}*/
};

#endif
