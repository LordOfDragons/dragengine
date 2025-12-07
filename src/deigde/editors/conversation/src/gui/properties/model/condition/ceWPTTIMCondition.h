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

#ifndef _CEWPTTIMCONDITION_H_
#define _CEWPTTIMCONDITION_H_

#include "../ceWPTTreeItemModel.h"
#include "../../../../conversation/condition/ceConversationCondition.h"

class ceConversationAction;


/**
 * \brief Base class for conditions.
 */
class ceWPTTIMCondition : public ceWPTTreeItemModel{
private:
	ceConversationAction *pAction;
	ceConversationCondition::Ref pCondition;
	
public:
	static const ceConversationCondition::eConditionTypes ListAddMenuConditions[8];
	static const int ListAddMenuConditionsCount;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMCondition(ceWindowMain &windowMain, eTypes type,
		ceConversation &conversation, ceConversationAction &action,
		ceConversationCondition *condition);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMCondition();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceConversationAction &GetAction() const{ return *pAction; }
	
	/** \brief Condition. */
	inline ceConversationCondition *GetCondition() const{ return pCondition; }
	
	/** \brief Deep find condition. */
	virtual ceWPTTIMCondition *DeepFindCondition(ceConversationCondition *condition);
	
	/** \brief Update condition. */
	virtual void Update();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	
	/** \brief Get condition owning this model if any. */
	virtual ceConversationCondition *GetOwnerCondition() const;
	/*@}*/
};

#endif
