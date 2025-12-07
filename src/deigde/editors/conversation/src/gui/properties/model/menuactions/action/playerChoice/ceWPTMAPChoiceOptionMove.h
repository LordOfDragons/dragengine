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

#ifndef _CEWPTMAPCHOICEOPTIONMOVE_H_
#define _CEWPTMAPCHOICEOPTIONMOVE_H_

#include "../../ceWPTMenuAction.h"

class ceConversation;
class ceConversationTopic;
class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;


/**
 * \brief Menu action move option in player-choice action.
 */
class ceWPTMAPChoiceOptionMove : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	ceConversationTopic *pTopic;
	ceCAPlayerChoice *pPlayerChoice;
	ceCAPlayerChoiceOption *pOption;
	int pIndex;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAPChoiceOptionMove();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAPChoiceOptionMove(ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationTopic &topic,
		ceCAPlayerChoice &playerChoice, ceCAPlayerChoiceOption *option, int index,
		const char *text, igdeIcon *icon);
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{return *pConversation;}
	
	/** \brief Topic. */
	inline ceConversationTopic &GetTopic() const{return *pTopic;}
	
	/** \brief Player choice action. */
	inline ceCAPlayerChoice &GetPlayerChoice() const{return *pPlayerChoice;}
	
	/** \brief Option. */
	inline ceCAPlayerChoiceOption *GetOption() const{return pOption;}
	
	/** \brief Index of new position. */
	inline int GetIndex() const{return pIndex;}
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
