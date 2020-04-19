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

#ifndef _CEWPTTIMAPLAYERCHOICEOPTIONCONDITION_H_
#define _CEWPTTIMAPLAYERCHOICEOPTIONCONDITION_H_

#include "../../condition/ceWPTTIMConditionContainer.h"

class ceCAPlayerChoice;
class ceCAPlayerChoiceOption;
class ceWPTTIMAPlayerChoice;
class ceWPTTIMAPlayerChoiceOption;



/**
 * \brief Condition player choice actions.
 */
class ceWPTTIMAPlayerChoiceOptionCondition : public ceWPTTIMConditionContainer{
private:
	ceCAPlayerChoiceOption *pOption;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAPlayerChoiceOptionCondition( ceWindowMain &windowMain, ceConversation &conversation,
		ceCAPlayerChoice &playerChoice, ceCAPlayerChoiceOption *option );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAPlayerChoiceOptionCondition();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Parent if-case model. */
	ceWPTTIMAPlayerChoiceOption *GetModelOption() const;
	
	/** \brief Parent player choice model. */
	ceWPTTIMAPlayerChoice *GetModelPlayerChoice() const;
	
	/** \brief Update action. */
	void Update();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief User requests context menu for selected child condition. */
	virtual void ContextMenuCondition( igdeMenuCascade &contextMenu, ceConversationCondition *condition );
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	/*@}*/
};

#endif
