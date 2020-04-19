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

#ifndef _CEWPTTIMCLOGIC_H_
#define _CEWPTTIMCLOGIC_H_

#include "ceWPTTIMConditions.h"
#include "../../../../conversation/condition/ceCConditionLogic.h"


/**
 * \brief Condition logic.
 */
class ceWPTTIMCLogic : public ceWPTTIMConditions{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMCLogic( ceWindowMain &windowMain, ceConversation &conversation,
		ceConversationAction &action, ceCConditionLogic *condition );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMCLogic();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Condition. */
	inline ceCConditionLogic *GetConditionLogic() const{
		return ( ceCConditionLogic* )GetCondition();
	}
	
	/** \brief Text representation for operator. */
	static const char *GetOperatorText( ceCConditionLogic::eOperators anOperator );
	
	/** \brief Update condition. */
	virtual void Update();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief User requests context menu for selected child condition. */
	virtual void ContextMenuCondition( igdeMenuCascade &contextMenu, ceConversationCondition *condition );
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	/*@}*/
};

#endif
