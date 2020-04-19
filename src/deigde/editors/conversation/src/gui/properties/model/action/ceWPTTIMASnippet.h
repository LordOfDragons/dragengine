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

#ifndef _CEWPTTIMASNIPPET_H_
#define _CEWPTTIMASNIPPET_H_

#include "ceWPTTIMAction.h"
#include "../../../../conversation/action/ceCASnippet.h"


/**
 * \brief Action snippet.
 */
class ceWPTTIMASnippet : public ceWPTTIMAction{
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMASnippet( ceWindowMain &windowMain, ceConversation &conversation,
		ceCASnippet *action );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMASnippet();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceCASnippet *GetActionSnippet() const{ return ( ceCASnippet* )GetAction(); }
	
	/** \brief Update action. */
	virtual void Update();
	/*@}*/
};

#endif
