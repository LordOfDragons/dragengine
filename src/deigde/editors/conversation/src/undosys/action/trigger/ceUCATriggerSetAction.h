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

#ifndef _CEUCATRIGGERSETACTION_H_
#define _CEUCATRIGGERSETACTION_H_

#include "../../../conversation/action/ceCATrigger.h"

#include <deigde/undo/igdeUndo.h>

class ceConversationTopic;



/**
 * \brief Undo action trigger conversation action set action.
 */
class ceUCATriggerSetAction : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceCATrigger *pAction;
	ceCATrigger::eActions pOldAction;
	ceCATrigger::eActions pNewAction;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCATriggerSetAction( ceConversationTopic *topic, ceCATrigger *action, ceCATrigger::eActions newOperator );
	/** \brief Cleans up the undo object. */
	virtual ~ceUCATriggerSetAction();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
