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

#ifndef _CEWPTTIMCONDITIONS_H_
#define _CEWPTTIMCONDITIONS_H_

#include "ceWPTTIMCondition.h"

class ceConversationConditionList;


/**
 * \brief Conditions tree model.
 */
class ceWPTTIMConditions : public ceWPTTIMCondition{
private:
	const ceConversationConditionList &pConditions;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMConditions(ceWindowMain &windowMain, eTypes type, ceConversation &conversation,
		ceConversationAction &action, ceConversationCondition *condition,
		const ceConversationConditionList &conditions);
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMConditions();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Model with condition or \em NULL. */
	ceWPTTIMCondition *GetChildWith(ceConversationCondition *condition) const;
	
	/** \brief Structure changed. */
	void StructureChanged();
	
	
	
	/** \brief Update condition. */
	virtual void Update();
	
	/** \brief Update conditions. */
	virtual void UpdateConditions();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu(igdeMenuCascade &contextMenu);
	
	
	
	/** \brief Create condition model. */
	static ceWPTTIMCondition *CreateConditionModel(ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationAction &action,
		ceConversationCondition *condition);
	/*@}*/
};

#endif
