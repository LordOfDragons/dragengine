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

#ifndef _CEWPTTIMAPLAYERCHOICEOPTIONACTIONS_H_
#define _CEWPTTIMAPLAYERCHOICEOPTIONACTIONS_H_

#include "../ceWPTTIMActions.h"

class ceWPTTIMAPlayerChoice;
class ceWPTTIMAPlayerChoiceOption;



/**
 * \brief Action player choice option actions.
 */
class ceWPTTIMAPlayerChoiceOptionActions : public ceWPTTIMActions{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAPlayerChoiceOptionActions(ceWindowMain &windowMain,
		ceConversation &conversation, const ceConversationActionList &actions);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAPlayerChoiceOptionActions();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Parent if-case model. */
	ceWPTTIMAPlayerChoiceOption *GetModelOption() const;
	
	/** \brief Parent player choice model. */
	ceWPTTIMAPlayerChoice *GetModelPlayerChoice() const;
	
	
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** \brief User requests context menu for selected child action. */
	virtual void ContextMenuAction(igdeMenuCascade &contextMenu, ceConversationAction *action);
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	/*@}*/
};

#endif
