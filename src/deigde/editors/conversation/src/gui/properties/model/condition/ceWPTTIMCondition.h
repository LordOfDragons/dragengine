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

#ifndef _CEWPTTIMCONDITION_H_
#define _CEWPTTIMCONDITION_H_

#include "../ceWPTTreeItemModel.h"
#include "../../../../conversation/condition/ceConversationCondition.h"
#include "../../../../conversation/condition/ceConversationConditionReference.h"

class ceConversationAction;


/**
 * \brief Base class for conditions.
 */
class ceWPTTIMCondition : public ceWPTTreeItemModel{
private:
	ceConversationAction *pAction;
	ceConversationConditionReference pCondition;
	
public:
	static const ceConversationCondition::eConditionTypes ListAddMenuConditions[ 8 ];
	static const int ListAddMenuConditionsCount;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMCondition( ceWindowMain &windowMain, eTypes type,
		ceConversation &conversation, ceConversationAction &action,
		ceConversationCondition *condition );
	
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
	virtual ceWPTTIMCondition *DeepFindCondition( ceConversationCondition *condition );
	
	/** \brief Update condition. */
	virtual void Update();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Get condition owning this model if any. */
	virtual ceConversationCondition *GetOwnerCondition() const;
	/*@}*/
};

#endif
