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

#ifndef _CECASTOPTOPIC_H_
#define _CECASTOPTOPIC_H_

#include "ceConversationAction.h"

#include <dragengine/common/string/decString.h>



/**
 * \brief Stop topic conversation action.
 * 
 * Forward to end of current topic. Runs next action if the stack is not empty.
 * 
 */
class ceCAStopTopic : public ceConversationAction{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create stop topic conversation action. */
	ceCAStopTopic();
	
	/** \brief Create copy of stop topic conversation action. */
	ceCAStopTopic( const ceCAStopTopic &action );
	
	/** \brief Clean up stop conversation conversation action. */
	virtual ~ceCAStopTopic();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create copy action. */
    virtual ceConversationAction *CreateCopy() const;
	/*@}*/
};

#endif
