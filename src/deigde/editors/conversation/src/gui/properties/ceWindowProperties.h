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

#ifndef _CEWINDOWPROPERTIES_H_
#define _CEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>

class ceWindowMain;
class ceConversation;
class ceWPTopic;
class ceWPConversation;
class ceWPTopic;
class ceWPView;
class ceWPUndoHistory;


/**
 * \brief Properties Panel.
 */
class ceWindowProperties : public igdeTabBook{
private:
	ceWindowMain &pWindowMain;
	
	ceWPConversation *pPanelConversation;
	ceWPTopic *pPanelTopic;
	ceWPView *pPanelView;
	ceWPUndoHistory *pPanelUndoHistory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	ceWindowProperties( ceWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~ceWindowProperties();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline ceWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Topic panel. */
	ceWPTopic &GetPanelTopic() const;
	
	/** \brief Set conversation. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Conversation path changed. */
	void OnConversationPathChanged();
	/*@}*/
};

#endif
