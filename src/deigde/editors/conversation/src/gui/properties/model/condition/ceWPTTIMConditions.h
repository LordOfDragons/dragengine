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
	ceWPTTIMConditions( ceWindowMain &windowMain, eTypes type, ceConversation &conversation,
		ceConversationAction &action, ceConversationCondition *condition,
		const ceConversationConditionList &conditions );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMConditions();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Model with condition or \em NULL. */
	ceWPTTIMCondition *GetChildWith( ceConversationCondition *condition ) const;
	
	/** \brief Structure changed. */
	void StructureChanged();
	
	
	
	/** \brief Update condition. */
	virtual void Update();
	
	/** \brief Update conditions. */
	virtual void UpdateConditions();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	
	
	/** \brief Create condition model. */
	static ceWPTTIMCondition *CreateConditionModel( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationAction &action,
		ceConversationCondition *condition );
	/*@}*/
};

#endif
