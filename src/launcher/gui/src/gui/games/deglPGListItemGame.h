/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLPGLISTITEMGAME_H_
#define _DEGLPGLISTITEMGAME_H_

#include "../foxtoolkit.h"
#include "../deglSharedIcon.h"

#include <delauncher/game/delGame.h>

class deglPanelGames;



/**
 * Games Panel List Item Game.
 */
class deglPGListItemGame : public FXIconItem{
	FXDECLARE( deglPGListItemGame )
protected:
	deglPGListItemGame();
	
private:
	deglPanelGames *pPanelGames;
	const delGame::Ref pGame;
	deglSharedIcon::Ref pIconBig;
	deglSharedIcon::Ref pIconMini;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list item. */
	deglPGListItemGame( deglPanelGames *panelGames, delGame *game );
	
	/** Clean up list item. */
	virtual ~deglPGListItemGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Games panel. */
	inline deglPanelGames *GetPanelGames() const{ return pPanelGames; }
	
	/** Game object. */
	inline delGame *GetGame() const{ return pGame; }
	/*@}*/
};

#endif
