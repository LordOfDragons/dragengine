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

#ifndef _CEWPTTIMACTIONS_H_
#define _CEWPTTIMACTIONS_H_

#include "../ceWPTTreeItemModel.h"

class ceWPTTIMAction;
class ceConversationAction;
class ceConversationActionList;


/**
 * \brief Actions tree model.
 */
class ceWPTTIMActions : public ceWPTTreeItemModel{
private:
	const ceConversationActionList &pActions;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMActions( ceWindowMain &windowMain, ceConversation &conversation,
		eTypes type, const ceConversationActionList &actions );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMActions();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Model with action or \em NULL. */
	ceWPTTIMAction *GetChildWith( ceConversationAction *action ) const;
	
	/** \brief Structure changed. */
	void StructureChanged();
	
	
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update actions. */
	virtual void UpdateActions();
	
	/** \brief Build playback continuing from here. */
	virtual void BuildPlaybackFromHere() const;
	
	
	
	/** \brief Create action model. */
	static ceWPTTIMAction *CreateActionModel( ceWindowMain &windowMain,
		ceConversation &conversation, ceConversationAction *action );
	/*@}*/
};

#endif
