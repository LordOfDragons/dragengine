/* 
 * Drag[en]gine Console Launcher
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

#include "declActionHelp.h"
#include "declActionGames.h"
#include "declActionDelga.h"
#include "declActionPatches.h"
#include "declRunGame.h"
#include "declListProfiles.h"
#include "../declLauncher.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declActionHelp
////////////////////////

// Constructor, destructor
////////////////////////////

declActionHelp::declActionHelp( declLauncher *launcher ){
	if( ! launcher ){
		DETHROW( deeInvalidParam );
	}
	
	pLauncher = launcher;
}

declActionHelp::~declActionHelp(){
}



// Management
///////////////

void declActionHelp::Run(){
	const decUnicodeArgumentList &argumentList = pLauncher->GetArgumentList();
	
	if( argumentList.GetArgumentCount() < 2 ){
		pLauncher->PrintSyntax();
		return;
	}
	
	const decString actionName = argumentList.GetArgumentAt( 1 )->ToUTF8();
	
	if( actionName == "run" ){
		declRunGame( pLauncher ).PrintSyntax();
		
	}else if( actionName == "delga" ){
		declActionDelga( *pLauncher ).PrintSyntax();
		
	}else if( actionName == "games" ){
		declActionGames( *pLauncher ).PrintSyntax();
		
	}else if( actionName == "patches" ){
		declActionPatches( *pLauncher ).PrintSyntax();
		
	}else if( actionName == "profiles" ){
		declListProfiles( pLauncher ).PrintSyntax();
		
	}else{
		pLauncher->PrintSyntax();
		return;
	}
}
