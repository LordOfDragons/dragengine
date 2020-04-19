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

#ifndef _CEWPTMACOPYACTIONS_H_
#define _CEWPTMACOPYACTIONS_H_

#include "../ceWPTMenuAction.h"

class ceConversationActionList;



/**
 * \brief Menu action copy all topic actions.
 */
class ceWPTMACopyActions : public ceWPTMenuAction{
private:
	const ceConversationActionList *pActions;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMACopyActions();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMACopyActions( ceWindowMain &windowMain,
		const ceConversationActionList &actions );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Actions. */
	inline const ceConversationActionList &GetActions() const{ return *pActions; }
	
	/** \brief Do menu action. */
	virtual void OnAction();
	/*@}*/
};

#endif
