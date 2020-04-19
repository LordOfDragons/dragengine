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

#ifndef _CEUCCVARSETOPERATOR_H_
#define _CEUCCVARSETOPERATOR_H_

#include "../../../conversation/condition/ceCConditionVariable.h"

#include <deigde/undo/igdeUndo.h>

class ceCConditionVariable;
class ceConversationAction;
class ceConversationTopic;



/**
 * \brief Undo Variable Conversation Condition Set Operator.
 */
class ceUCCVarSetOperator : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceConversationAction *pAction;
	ceCConditionVariable *pCondition;
	ceCConditionVariable::eOperators pOldOperator;
	ceCConditionVariable::eOperators pNewOperator;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCCVarSetOperator( ceConversationTopic *topic, ceConversationAction *action,
		ceCConditionVariable *condition, ceCConditionVariable::eOperators newOperator );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCCVarSetOperator();
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
