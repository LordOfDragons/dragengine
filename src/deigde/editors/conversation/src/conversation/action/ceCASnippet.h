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

#ifndef _CECASNIPPET_H_
#define _CECASNIPPET_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Snippet Conversation Action.
 * Process a list of actions from another topic.
 */
class ceCASnippet : public ceConversationAction{
private:
	decString pFile;
	decString pTopic;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new snippet conversation action. */
	ceCASnippet();
	/** Creates a new snippet conversation action. */
	ceCASnippet( const ceCASnippet &snippet );
	/** Cleans up the snippet conversation action. */
	virtual ~ceCASnippet();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the file to look for. */
	inline const decString &GetFile() const{ return pFile; }
	/** Sets the file to look for. */
	void SetFile( const char *file );
	/** Retrieves the topic to look for. */
	inline const decString &GetTopic() const{ return pTopic; }
	/** Sets the topic to look for. */
	void SetTopic( const char *topic );
	
	/** Create a copy of this action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
