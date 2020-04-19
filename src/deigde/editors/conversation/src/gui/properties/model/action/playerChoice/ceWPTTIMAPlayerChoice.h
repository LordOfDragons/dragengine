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

#ifndef _CEWPTTIMAPLAYERCHOICE_H_
#define _CEWPTTIMAPLAYERCHOICE_H_

#include "../ceWPTTIMAction.h"
#include "../../../../../conversation/action/ceCAPlayerChoice.h"

class ceWPTTIMAPlayerChoiceOption;
class ceWPTTIMAPlayerChoiceActions;


/**
 * \brief Action player choice.
 */
class ceWPTTIMAPlayerChoice : public ceWPTTIMAction{
private:
	ceWPTTIMAPlayerChoiceActions *pActions;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAPlayerChoice( ceWindowMain &windowMain,
		ceConversation &conversation, ceCAPlayerChoice *action );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAPlayerChoice();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceCAPlayerChoice *GetActionPlayerChoice() const{ return ( ceCAPlayerChoice* )GetAction(); }
	
	/** \brief Model with option or \em NULL. */
	ceWPTTIMAPlayerChoiceOption *GetOptionChild( ceCAPlayerChoiceOption *option ) const;
	
	/** \brief Model with actions or \em NULL. */
	inline ceWPTTIMAPlayerChoiceActions *GetActionsChild() const{ return pActions; }
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update action lists if present. */
	virtual void UpdateActionLists();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	/*@}*/
	
	
	
private:
	void pUpdateOptions();
};

#endif
