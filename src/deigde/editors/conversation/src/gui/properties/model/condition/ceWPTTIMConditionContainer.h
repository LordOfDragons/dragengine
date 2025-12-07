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

#ifndef _CEWPTTIMCONDITIONCONTANIER_H_
#define _CEWPTTIMCONDITIONCONTANIER_H_

#include "../ceWPTTreeItemModel.h"
#include "../../../../conversation/condition/ceConversationCondition.h"

class ceWPTTIMCondition;
class ceConversationAction;


/**
 * \brief Condition container tree model.
 * 
 * If container is empty no child node is shown. If container contains a condition one
 * child node is shown with the condition inside. The subclass provides the container
 * content while this class handles the rest.
 */
class ceWPTTIMConditionContainer : public ceWPTTreeItemModel{
private:
	ceConversationAction *pAction;
	ceConversationCondition::Ref pCondition;
	ceWPTTIMCondition *pChildCondition;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMConditionContainer(ceWindowMain &windowMain, ceConversation &conversation,
		ceConversationAction &action, eTypes type);
	
protected:
	/** \brief Clean up tree item model. */
	~ceWPTTIMConditionContainer() override;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceConversationAction &GetAction() const{ return *pAction; }
	
	/** \brief Condition in container or \em NULL. */
	inline ceConversationCondition *GetCondition() const{ return pCondition; }
	
	/**
	 * \brief Set condition in container or \em NULL.
	 * 
	 * Updates the child not if condition changed.
	 */
	void SetCondition(ceConversationCondition *condition);
	
	/**
	 * \brief Set condition in container or updat it.
	 * 
	 * Convenience call for container subclass. If \em condition is the same as the condition
	 * currently set this calls Update() on child condition. Otherwise this calls SetCondition().
	 */
	void SetOrUpdateCondition(ceConversationCondition *condition);
	
	/** \brief Model with child condition or \em NULL. */
	inline ceWPTTIMCondition *GetChildCondition() const{ return pChildCondition; }
	
	
	
	/** \brief Deep find action. */
	ceWPTTIMCondition *DeepFindCondition(ceConversationCondition *condition) override;
	/*@}*/
};

#endif
