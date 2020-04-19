/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _CEWPTTIMCONDITIONCONTANIER_H_
#define _CEWPTTIMCONDITIONCONTANIER_H_

#include "../ceWPTTreeItemModel.h"
#include "../../../../conversation/condition/ceConversationConditionReference.h"

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
	ceConversationConditionReference pCondition;
	ceWPTTIMCondition *pChildCondition;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMConditionContainer( ceWindowMain &windowMain, ceConversation &conversation,
		ceConversationAction &action, eTypes type );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMConditionContainer();
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
	void SetCondition( ceConversationCondition *condition );
	
	/**
	 * \brief Set condition in container or updat it.
	 * 
	 * Convenience call for container subclass. If \em condition is the same as the condition
	 * currently set this calls Update() on child condition. Otherwise this calls SetCondition().
	 */
	void SetOrUpdateCondition( ceConversationCondition *condition );
	
	/** \brief Model with child condition or \em NULL. */
	inline ceWPTTIMCondition *GetChildCondition() const{ return pChildCondition; }
	
	
	
	/** \brief Deep find action. */
	virtual ceWPTTIMCondition *DeepFindCondition( ceConversationCondition *condition );
	/*@}*/
};

#endif
