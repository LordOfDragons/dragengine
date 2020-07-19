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

#ifndef _CEUACTIONHELPERS_H_
#define _CEUACTIONHELPERS_H_

#include <dragengine/deObject.h>

class ceConversationAction;
class ceConversationActionList;


/**
 * \brief Undo Conversation Action Helpers.
 */
class ceUActionHelpers{
public:
	/** \name Management */
	/*@{*/
	/** \brief Action to set active after action is removed. */
	static ceConversationAction *ActivateActionAfterRemove(
		const ceConversationActionList &list, ceConversationAction *removedAction );
	/*@}*/
};

#endif
