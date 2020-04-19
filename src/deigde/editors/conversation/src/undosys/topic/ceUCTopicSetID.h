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

#ifndef _CEUCTOPICSETID_H_
#define _CEUCTOPICSETID_H_

#include "../action/ceUndoCActionList.h"

#include <deigde/undo/igdeUndo.h>

class ceConversationTopic;
class ceConversation;
class ceConversationActionList;



/**
 * \brief Undo action topic set id.
 */
class ceUCTopicSetID : public igdeUndo{
private:
	ceConversationTopic *pTopic;
	
	decString pOldID;
	decString pNewID;
	
	ceUndoCActionList pSnippets;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	ceUCTopicSetID( const ceConversation &conversation,
		ceConversationTopic *topic, const char *newID );
	
	/** \brief Clean up undo action. */
	virtual ~ceUCTopicSetID();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pSetID( const char *id );
	
	void pAddSnippets( ceConversationTopic *topic, const char *matchGroupID,
		const char *matchTopicID, const ceConversationActionList &actions );
};

#endif
