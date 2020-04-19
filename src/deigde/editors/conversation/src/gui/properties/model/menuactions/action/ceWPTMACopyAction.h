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

#ifndef _CEWPTMACOPYACTION_H_
#define _CEWPTMACOPYACTION_H_

#include "../ceWPTMenuAction.h"

class ceConversationAction;



/**
 * \brief Menu action copy action.
 */
class ceWPTMACopyAction : public ceWPTMenuAction{
private:
	ceConversationAction *pAction;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMACopyAction();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMACopyAction( ceWindowMain &windowMain,
		ceConversationAction *action );
	
	/** \brief Crete menu action. */
	ceWPTMACopyAction( ceWindowMain &windowMain,
		ceConversationAction *action, const char *text, igdeIcon *icon );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceConversationAction *GetAction() const{ return pAction; }
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
