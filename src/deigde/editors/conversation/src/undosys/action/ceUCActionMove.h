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

#ifndef _CEUCACTIONMOVE_H_
#define _CEUCACTIONMOVE_H_

#include <deigde/undo/igdeUndo.h>

class ceConversationTopic;
class ceConversationAction;



/**
 * \brief Undo Action Move Conversation Action.
 */
class ceUCActionMove : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceConversationAction *pAction;
	int pOldIndex;
	int pNewIndex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCActionMove( ceConversationTopic *topic, ceConversationAction *action, int newIndex );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCActionMove();
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
