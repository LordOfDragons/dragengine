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

#ifndef _CEWPTTIMAPLAYERCHOICEOPTIONACTIONS_H_
#define _CEWPTTIMAPLAYERCHOICEOPTIONACTIONS_H_

#include "../ceWPTTIMActions.h"

class ceWPTTIMAPlayerChoice;
class ceWPTTIMAPlayerChoiceOption;



/**
 * \brief Action player choice option actions.
 */
class ceWPTTIMAPlayerChoiceOptionActions : public ceWPTTIMActions{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAPlayerChoiceOptionActions( ceWindowMain &windowMain,
		ceConversation &conversation, const ceConversationActionList &actions );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAPlayerChoiceOptionActions();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Parent if-case model. */
	ceWPTTIMAPlayerChoiceOption *GetModelOption() const;
	
	/** \brief Parent player choice model. */
	ceWPTTIMAPlayerChoice *GetModelPlayerChoice() const;
	
	
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief User requests context menu for selected child action. */
	virtual void ContextMenuAction( igdeMenuCascade &contextMenu, ceConversationAction *action );
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	/*@}*/
};

#endif
