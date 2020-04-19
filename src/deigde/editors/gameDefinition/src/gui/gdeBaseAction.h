/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEBASEACTION_H_
#define _GDEBASEACTION_H_

#include <deigde/gui/event/igdeAction.h>

class gdeWindowMain;
class igdeUndo;
class gdeGameDefinition;



/**
 * \brief Base action with undo support.
 */
class gdeBaseAction : public igdeAction{
protected:
	gdeWindowMain &pWindowMain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create base action. */
	gdeBaseAction( gdeWindowMain &windowMain, const char *text, const char *description );
	
	/** \brief Create base action. */
	gdeBaseAction( gdeWindowMain &windowMain, igdeIcon *icon, const char *description );
	
	/** \brief Create base action. */
	gdeBaseAction( gdeWindowMain &windowMain, const char *text, igdeIcon *icon, const char *description );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Run action.
	 * 
	 * If game definition is not NULL calls OnAction(gdeGameDefinition&). If a non-NULL undo
	 * object is returned the undo is added to the game definition and redone.
	 */
	virtual void OnAction();
	
	/** \brief Request update of action parameters if required. */
	virtual void Update();
	
	
	
	/** \brief Run action if game definition is not NULL. */
	virtual igdeUndo *OnAction( gdeGameDefinition &gameDefinition ) = 0;
	/*@}*/
};

#endif
