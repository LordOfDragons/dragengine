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

#ifndef _CEWPTTIMAWAITCONDITION_H_
#define _CEWPTTIMAWAITCONDITION_H_

#include "../../condition/ceWPTTIMConditionContainer.h"

class ceCAWait;
class ceWPTTIMAWait;


/**
 * \brief Condition wait actions.
 */
class ceWPTTIMAWaitCondition : public ceWPTTIMConditionContainer{
private:
	ceCAWait *pAction;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAWaitCondition(ceWindowMain &windowMain,
		ceConversation &conversation, ceCAWait *action);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAWaitCondition();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceCAWait *GetAction() const{return pAction;}
	
	/** \brief Parent wait model. */
	ceWPTTIMAWait *GetModelWait() const;
	
	/** \brief Update action. */
	void Update();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** \brief User requests context menu for selected child condition. */
	virtual void ContextMenuCondition(igdeMenuCascade &contextMenu, ceConversationCondition *condition);
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	/*@}*/
};

#endif
