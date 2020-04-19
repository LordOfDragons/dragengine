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

#ifndef _CEUNDOCACTION_H_
#define _CEUNDOCACTION_H_

#include <dragengine/deObject.h>

class ceConversationAction;
class ceConversationTopic;



/**
 * \brief Undo Conversation Action.
 * Stores an action including the topic it is located under for undo purpose.
 */
class ceUndoCAction : public deObject{
private:
	ceConversationAction *pAction;
	ceConversationTopic *pTopic;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUndoCAction( ceConversationAction *action, ceConversationTopic *topic );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUndoCAction();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the action. */
	inline ceConversationAction *GetAction() const{ return pAction; }
	/** \brief Retrieves the topic. */
	inline ceConversationTopic *GetTopic() const{ return pTopic; }
	/*@}*/
};

#endif
