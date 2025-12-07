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

#ifndef _CEWPTMAPCHOICEACTIONSMOVEACTION_H_
#define _CEWPTMAPCHOICEACTIONSMOVEACTION_H_

#include "../../ceWPTMenuAction.h"

class ceConversation;
class ceConversationAction;
class ceConversationTopic;
class ceCAPlayerChoice;



/**
 * \brief Menu action remove action to topic.
 */
class ceWPTMAPChoiceActionsMoveAction : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceCAPlayerChoice *pPlayerChoice;
	ceConversationAction *pAction;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAPChoiceActionsMoveAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create menu action. */
	ceWPTMAPChoiceActionsMoveAction(ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceCAPlayerChoice &playerChoice, ceConversationAction *action, int index,
		const char *text, igdeIcon *icon);
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{return *pConversation;}
	
	/** \brief Topic. */
	inline ceConversationTopic *GetTopic() const{return pTopic;}
	
	/** \brief Action. */
	inline ceConversationAction *GetAction() const{return pAction;}
	
	/** \brief If-else action. */
	inline ceCAPlayerChoice *GetPlayerChoice() const{return pPlayerChoice;}
	
	/** \brief Index of new position. */
	inline int GetIndex() const{return pIndex;}
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
