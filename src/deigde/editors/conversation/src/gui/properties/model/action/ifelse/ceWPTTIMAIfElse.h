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

#ifndef _CEWPTTIMAIFELSE_H_
#define _CEWPTTIMAIFELSE_H_

#include "../ceWPTTIMAction.h"
#include "../../../../../conversation/action/ceCAIfElse.h"

class ceWPTTIMAIfElseIfCase;
class ceWPTTIMAIfElseElse;


/**
 * \brief Action if-else.
 */
class ceWPTTIMAIfElse : public ceWPTTIMAction{
private:
	ceWPTTIMAIfElseElse *pElse;
	
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	ceWPTTIMAIfElse( ceWindowMain &windowMain,
		ceConversation &conversation, ceCAIfElse *action );
	
protected:
	/** \brief Clean up tree item model. */
	virtual ~ceWPTTIMAIfElse();
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Action. */
	inline ceCAIfElse *GetActionIfElse() const{ return ( ceCAIfElse* )GetAction(); }
	
	/** \brief Model with if-case or \em NULL. */
	ceWPTTIMAIfElseIfCase *GetIfCaseChild( ceCAIfElseCase *ifCase ) const;
	
	/** \brief Model with else-case or \em NULL. */
	inline ceWPTTIMAIfElseElse *GetElseChild() const{ return pElse; }
	
	/** \brief Update action. */
	virtual void Update();
	
	/** \brief Update action lists if present. */
	virtual void UpdateActionLists();
	
	/** \brief Expanded state changed. */
	virtual void OnExpandedChanged();
	
	/** \brief User requests context menu for selected item. */
	virtual void OnContextMenu( igdeMenuCascade &contextMenu );
	/*@}*/
	
	
	
private:
	void pUpdateIfCases();
	void pUpdateElseCase();
};

#endif
