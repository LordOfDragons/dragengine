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

#ifndef _CECONVERSATIONFILE_H_
#define _CECONVERSATIONFILE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

#include "../topic/ceConversationTopicList.h"

class ceConversation;



/**
 * \brief Conversation File.
 */
class ceConversationFile : public deObject{
private:
	ceConversation *pConversation;
	
	decString pID;
	
	ceConversationTopicList pTopics;
	ceConversationTopic *pActiveTopic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation file. */
	ceConversationFile( const char *id = "Group" );
	
	/** \brief Create copy of conversation file. */
	ceConversationFile( const ceConversationFile &file );
	
	/** \brief Clean up conversation file. */
	virtual ~ceConversationFile();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent conversation or \em NULL if not set. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** \brief Set parent conversation or \em NULL if not set. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Identifier. */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID( const char *id );
	/*@}*/
	
	
	
	/** \name Topics */
	/*@{*/
	/** \brief Topics. */
	inline const ceConversationTopicList &GetTopicList() const{ return pTopics; }
	
	/** \brief Add topic. */
	void AddTopic( ceConversationTopic *topic );
	
	/** \brief Remove topic. */
	void RemoveTopic( ceConversationTopic *topic );
	
	/** \brief Remove all topics. */
	void RemoveAllTopics();
	
	/** \brief Active topic or \em NULL if none is active. */
	inline ceConversationTopic *GetActiveTopic() const{ return pActiveTopic; }
	
	/** \brief Set active topic or \em NULL if none is active. */
	void SetActiveTopic( ceConversationTopic *topic );
	/*@}*/
};

#endif
