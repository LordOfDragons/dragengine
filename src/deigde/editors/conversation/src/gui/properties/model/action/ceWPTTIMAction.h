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

#ifndef _CEWPTTIMACTION_H_
#define _CEWPTTIMACTION_H_

#include "../ceWPTTreeItemModel.h"
#include "../../../../conversation/action/ceConversationAction.h"
#include "../../../../conversation/action/ceConversationActionReference.h"


/**
 * \brief Base class for actions.
 */
class ceWPTTIMAction : public ceWPTTreeItemModel{
private:
	ceConversationActionReference pAction;
	
public:
	static const ceConversationAction::eActionTypes ListAddMenuActions[ 19 ];
	static const int ListAddMenuActionsCount;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAction( ceWindowMain &windowMain, eTypes type,
		ceConversation &conversation, ceConversationAction *action );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAction();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceConversationAction *GetAction() const{ return pAction; }
	
	/** \brief Deep find action. */
	virtual ceWPTTIMAction *DeepFindAction( ceConversationAction *action );
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update action lists if present. */
	virtual void UpdateActionLists();
	
	/** \brief Get action owning this model if any. */
	virtual ceConversationAction *GetOwnerAction() const;
	
	/** \brief Build playback continuing from here. */
	virtual void BuildPlaybackFromHere() const;
	/*@}*/
};

#endif
