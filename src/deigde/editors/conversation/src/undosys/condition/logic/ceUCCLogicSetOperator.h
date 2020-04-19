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

#ifndef _CEUCCLOGICSETOPERATOR_H_
#define _CEUCCLOGICSETOPERATOR_H_

#include "../../../conversation/condition/ceCConditionLogic.h"

#include <deigde/undo/igdeUndo.h>

class ceCConditionLogic;
class ceConversationAction;
class ceConversationTopic;



/**
 * \brief Undo Logic Conversation Condition Set Operator.
 */
class ceUCCLogicSetOperator : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceConversationAction *pAction;
	ceCConditionLogic *pLogic;
	ceCConditionLogic::eOperators pOldOperator;
	ceCConditionLogic::eOperators pNewOperator;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCCLogicSetOperator( ceConversationTopic *topic, ceConversationAction *action,
		ceCConditionLogic *logic, ceCConditionLogic::eOperators newOperator );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCCLogicSetOperator();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
