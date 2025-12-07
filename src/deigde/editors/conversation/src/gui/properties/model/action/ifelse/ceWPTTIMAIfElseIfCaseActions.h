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

#ifndef _CEWPTTIMAIFELSEIFCASEACTIONS_H_
#define _CEWPTTIMAIFELSEIFCASEACTIONS_H_

#include "../ceWPTTIMActions.h"

class ceWPTTIMAIfElse;
class ceWPTTIMAIfElseIfCase;



/**
 * \brief Action if-else actions.
 */
class ceWPTTIMAIfElseIfCaseActions : public ceWPTTIMActions{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAIfElseIfCaseActions(ceWindowMain &windowMain,
		ceConversation &conversation, const ceConversationActionList &actions);
	
protected:
	/** \brief Clean up tree item model. */
	~ceWPTTIMAIfElseIfCaseActions() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Parent if-case model. */
	ceWPTTIMAIfElseIfCase *GetModelIfCase() const;
	
	/** \brief Parent if-else model. */
	ceWPTTIMAIfElse *GetModelIfElse() const;
	
	
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	
	/** \brief User requests context menu for selected child action. */
	void ContextMenuAction(igdeMenuCascade &contextMenu, ceConversationAction *action) override;
	
	/** \brief Update action. */
	void Update() override;
	
	/** \brief Expanded state changed. */
	void OnExpandedChanged() override;
	/*@}*/
};

#endif
