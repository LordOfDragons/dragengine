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

#ifndef _CEWPTMAPASTESNIPPET_H_
#define _CEWPTMAPASTESNIPPET_H_

#include "../ceWPTMenuAction.h"

class ceConversation;
class ceConversationActionList;
class ceUCActionPaste;



/**
 * \brief Menu action paste conversation snippet.
 */
class ceWPTMAPasteSnippet : public ceWPTMenuAction{
private:
	ceConversation *pConversation;
	
	
	
protected:
	/** \brief FOX constructor. */
	ceWPTMAPasteSnippet();
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Crete menu action. */
	ceWPTMAPasteSnippet( ceWindowMain &windowMain,
		ceConversation &conversation );
	/*@}*/
	
	
	
	/** \brief Management */
	/*@{*/
	/** \brief Conversation. */
	inline ceConversation &GetConversation() const{ return *pConversation; }
	
	/** \brief Do menu action. */
	virtual void OnAction();
	
	/** \brief Create undo action for pasting actions. */
	virtual ceUCActionPaste *CreateUndo( const ceConversationActionList &actions ) = 0;
	/*@}*/
};

#endif
