/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deglPGListItemGame.h"
#include "deglPanelGames.h"
#include "../deglSharedIcon.h"
#include "../../game/deglGameIcon.h"

#include <dragengine/common/exceptions.h>



// Class deglPGListItemGame
/////////////////////////////

FXIMPLEMENT(deglPGListItemGame, FXIconItem, nullptr, 0)

// Constructor, destructor
////////////////////////////

deglPGListItemGame::deglPGListItemGame(){}

deglPGListItemGame::deglPGListItemGame(deglPanelGames *panelGames, delGame *game) :
FXIconItem("", nullptr, nullptr, nullptr),
pPanelGames(panelGames),
pGame(game)
{
	if(!panelGames || !game){
		DETHROW(deeInvalidParam);
	}
	
	decString gameStatus;
	FXString text;
	
	const decString gameTitle(game->GetTitle().ToUTF8());
	const decString gameCreator(game->GetCreator().ToUTF8());
	delGameProfile * const profile = game->GetProfileToUse();
	
	if(game->GetCanRun() && profile->GetValid()){
		gameStatus = "Ready";
		
	}else if(!profile->GetValid()){
		gameStatus = "Profile Broken";
		
	}else if(!game->GetAllFormatsSupported()){
		gameStatus = "Missing Modules";
		
	}else{
		gameStatus = "Installation Broken";
	}
	
	text.format("%s\t%s\t%s", gameTitle.GetString(), gameStatus.GetString(), gameCreator.GetString());
	setText(text);
	
	const deglGameIcon *icon = (deglGameIcon*)game->GetIcons().GetSmallest(48);
	if(icon && icon->GetFoxIcon()){
		pIconBig = icon->GetScaledFoxIcon(48);
		setBigIcon(pIconBig->GetIcon());
	}
	
	icon = (deglGameIcon*)game->GetIcons().GetSmallest(32);
	if(icon && icon->GetFoxIcon()){
		pIconMini = icon->GetScaledFoxIcon(32);
		setMiniIcon(pIconMini->GetIcon());
	}
}

deglPGListItemGame::~deglPGListItemGame(){
}
