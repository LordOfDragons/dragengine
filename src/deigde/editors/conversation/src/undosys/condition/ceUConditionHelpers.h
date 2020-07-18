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

#ifndef _CEUCONDITIONHELPERS_H_
#define _CEUCONDITIONHELPERS_H_

#include <dragengine/deObject.h>

class ceConversationCondition;
class ceConversationConditionList;


/**
 * \brief Undo Conversation Condition Helpers.
 */
class ceUConditionHelpers{
public:
	/** \name Management */
	/*@{*/
	/** \brief Condition to set active after condition is removed. */
	static ceConversationCondition *ActivateConditionAfterRemove(
		const ceConversationConditionList &list, ceConversationCondition *removedCondition );
	/*@}*/
};

#endif
