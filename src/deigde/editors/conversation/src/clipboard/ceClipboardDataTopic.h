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

#ifndef _CECLIPBOARDDATATOPIC_H_
#define _CECLIPBOARDDATATOPIC_H_

#include "../conversation/topic/ceConversationTopicList.h"

#include <deigde/clipboard/igdeClipboardData.h>



/**
 * \brief Topic clipboard data.
 */
class ceClipboardDataTopic : public igdeClipboardData{
public:
	/** \brief Type name. */
	static const char * const TYPE_NAME;
	
	
	
private:
	ceConversationTopicList pTopics;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create clipboard data. */
	ceClipboardDataTopic( const ceConversationTopicList &topics );
	
protected:
	/**
	 * \brief Clean up object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~ceClipboardDataTopic();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Topics. */
	inline const ceConversationTopicList &GetTopics() const{ return pTopics; }
	/*@}*/
};

#endif
