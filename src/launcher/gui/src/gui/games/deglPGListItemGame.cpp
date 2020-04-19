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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deglPGListItemGame.h"
#include "deglPanelGames.h"
#include "../deglSharedIcon.h"
#include "../../game/deglGame.h"
#include "../../game/profile/deglGameProfile.h"

#include <dragengine/common/exceptions.h>



// Class deglPGListItemGame
/////////////////////////////

FXIMPLEMENT( deglPGListItemGame, FXIconItem, NULL, 0 )

// Constructor, destructor
////////////////////////////

deglPGListItemGame::deglPGListItemGame(){ }

deglPGListItemGame::deglPGListItemGame( deglPanelGames *panelGames, deglGame *game ) :
FXIconItem( "", NULL, NULL, NULL ),
pPanelGames( panelGames ),
pGame( game )
{
	if( ! panelGames || ! game ){
		DETHROW( deeInvalidParam );
	}
	
	decString gameStatus;
	FXString text;
	
	const decString gameTitle( game->GetTitle().ToUTF8() );
	const decString gameCreator( game->GetCreator().ToUTF8() );
	deglGameProfile * const profile = game->GetProfileToUse();
	
	if( game->GetCanRun() && profile->GetValid() ){
		gameStatus = "Ready";
		
	}else if( ! profile->GetValid() ){
		gameStatus = "Profile Broken";
		
	}else if( ! game->GetAllFormatsSupported() ){
		gameStatus = "Missing Modules";
		
	}else{
		gameStatus = "Installation Broken";
	}
	
	text.format( "%s\t%s\t%s", gameTitle.GetString(), gameStatus.GetString(), gameCreator.GetString() );
	setText( text );
	
	pIconBig = game->GetFoxIconBig();
	pIconMini = game->GetFoxIconMini();
	
	if( game->GetFoxIconBig() ){
		setBigIcon( game->GetFoxIconBig()->GetIcon() );
	}
	if( game->GetFoxIconMini() ){
		setMiniIcon( game->GetFoxIconMini()->GetIcon() );
	}
}

deglPGListItemGame::~deglPGListItemGame(){
}



// Management
///////////////

