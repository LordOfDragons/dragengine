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

#ifndef _CEWPTTIMAWAIT_H_
#define _CEWPTTIMAWAIT_H_

#include "../ceWPTTIMAction.h"
#include "../../../../../conversation/action/ceCAWait.h"

class ceWPTTIMAWaitCondition;
class ceWPTTIMAWaitActions;


/**
 * \brief Action if-else.
 */
class ceWPTTIMAWait : public ceWPTTIMAction{
private:
	ceWPTTIMAWaitCondition *pCondition;
	ceWPTTIMAWaitActions *pActions;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAWait( ceWindowMain &windowMain, ceConversation &conversation, ceCAWait *action );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAWait();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceCAWait *GetActionWait() const{ return ( ceCAWait* )GetAction(); }
	
	/** \brief Model with condition. */
	inline ceWPTTIMAWaitCondition *GetChildCondition() const{ return pCondition; }
	
	/** \brief Model with actions. */
	inline ceWPTTIMAWaitActions *GetChildActions() const{ return pActions; }
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update action lists if present. */
	virtual void UpdateActionLists();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	/*@}*/
};

#endif
