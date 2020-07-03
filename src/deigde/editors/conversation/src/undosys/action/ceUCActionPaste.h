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

#ifndef _CEUCACTIONPASTE_H_
#define _CEUCACTIONPASTE_H_

#include "../../conversation/action/ceConversationActionList.h"

#include <deigde/undo/igdeUndo.h>

class ceConversationTopic;



/**
 * \brief Undo Action Add Conversation Action.
 */
class ceUCActionPaste : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	ceConversationActionList pActions;
	int pIndex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCActionPaste( ceConversationTopic *topic, const ceConversationActionList &actions, int index );
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCActionPaste();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic &GetTopic() const{ return *pTopic; }
	
	/** \brief Retrieves the list of actions. */
	inline const ceConversationActionList &GetActions() const{ return pActions; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
protected:
	void pInsertActions( ceConversationActionList &list );
	void pRemoveActions( ceConversationActionList &list );
	void pSelectInserted();
	ceConversationAction *ActivateActionAfterRemove( const ceConversationActionList &list ) const;
};

#endif
