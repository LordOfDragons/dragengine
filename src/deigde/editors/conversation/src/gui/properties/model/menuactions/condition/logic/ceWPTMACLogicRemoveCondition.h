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

#ifndef _CEWPTMACLOGICREMOVECONDITION_H_
#define _CEWPTMACLOGICREMOVECONDITION_H_

#include "../../ceWPTMenuAction.h"

class ceConversation;
class ceConversationCondition;
class ceConversationTopic;
class ceConversationAction;
class ceCConditionLogic;



/**
 * \brief Menu action remove action to topic.
 */
class ceWPTMACLogicRemoveCondition : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceConversationAction *pAction;
	ceCConditionLogic *pLogic;
	ceConversationCondition *pCondition;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMACLogicRemoveCondition();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMACLogicRemoveCondition( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceConversationAction &action, ceCConditionLogic &logic,
		ceConversationCondition *condition );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	
	/** \brief Action. */
	inline ceConversationAction *GetAction() const{ return pAction; }
	
	/** \brief If-else action. */
	inline ceCConditionLogic *GetLogic() const{ return pLogic; }
	
	/** \brief Action. */
	inline ceConversationCondition *GetCondition() const{ return pCondition; }
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
