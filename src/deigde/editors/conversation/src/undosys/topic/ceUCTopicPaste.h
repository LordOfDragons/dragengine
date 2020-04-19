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

#ifndef _CEUCTOPICPASTE_H_
#define _CEUCTOPICPASTE_H_

#include "../../conversation/topic/ceConversationTopicList.h"

#include <deigde/undo/igdeUndo.h>

class ceConversationFile;
class ceConversationTopic;



/**
 * \brief Undo action paste conversation topic.
 */
class ceUCTopicPaste : public igdeUndo{
private:
	ceConversationFile *pFile;
	ceConversationTopicList pTopics;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	ceUCTopicPaste( ceConversationFile *file, const ceConversationTopicList &topics );
	
	/** \brief Clean up undo action. */
	virtual ~ceUCTopicPaste();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief List of topics. */
	inline const ceConversationTopicList &GetTopics() const{ return pTopics; }
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
