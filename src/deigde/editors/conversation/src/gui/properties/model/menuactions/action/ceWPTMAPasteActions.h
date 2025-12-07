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

#ifndef _CEWPTMAPASTEACTIONS_H_
#define _CEWPTMAPASTEACTIONS_H_

#include "../ceWPTMenuAction.h"

class ceConversation;
class ceConversationActionList;
class ceUCActionPaste;



/**
 * \brief Menu action paste all topic actions.
 */
class ceWPTMAPasteActions : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAPasteActions();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAPasteActions(ceWindowMain &windowMain,
		ceConversation &conversation);
	
	/** \brief Crete menu action. */
	ceWPTMAPasteActions(ceWindowMain &windowMain,
		ceConversation &conversation, const char *text);
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{return *pConversation;}
	
	
	
	/** \brief Do menu action. */
	virtual void OnAction();
	
	/** \brief Create undo action for pasting actions. */
	virtual ceUCActionPaste *CreateUndo(const ceConversationActionList &actions);
	/*@}*/
};

#endif
