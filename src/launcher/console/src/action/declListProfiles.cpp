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

#include "declListProfiles.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"
#include "../engine/declEngine.h"
#include "../game/declGame.h"
#include "../game/declGameManager.h"
#include "../game/patch/declPatchManager.h"

#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include "../game/profile/declGameProfile.h"



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declListProfiles
////////////////////////

// Constructor, destructor
////////////////////////////

declListProfiles::declListProfiles( declLauncher *launcher ){
	if( ! launcher ){
		DETHROW( deeInvalidParam );
	}
	
	pLauncher = launcher;
}

declListProfiles::~declListProfiles(){
}



// Management
///////////////

void declListProfiles::PrintSyntax(){
	printf( "Drag[en]gine Console Launcher.\n" );
	printf( "Written by Plüss Roland ( roland@rptd.ch ).\n" );
	printf( "Released under the GPL ( http://www.gnu.org/licenses/gpl.html ), 2011.\n" );
	printf( "\n" );
	printf( "Lists game profiles.\n" );
	printf( "\n" );
	printf( "Syntax:\n" );
	printf( "delauncherconsole profiles\n" );
	printf( "\n" );
}



void declListProfiles::ParseArguments(){
}

void declListProfiles::InitLauncher(){
	pLauncher->GetEngine()->LoadModuleList();
	pLauncher->GetEngine()->LoadConfig();
	
	pLauncher->GetEngine()->Start( pLauncher->GetEngineLogger(), "" );
	try{
		pLauncher->GetGameManager()->LoadGameList();
		pLauncher->GetPatchManager().LoadPatchList();
		pLauncher->GetGameManager()->LoadGameConfigs();
		
	}catch( const deException & ){
		pLauncher->GetEngine()->Stop();
		throw;
	}
	pLauncher->GetEngine()->Stop();
}

void declListProfiles::Run(){
	ParseArguments();
	
	InitLauncher();
	
	const declGameManager &gameManager = *pLauncher->GetGameManager();
	const declGameProfileList &profiles = gameManager.GetProfileList();
	int i, count;
	
	printf( "Available Game Profiles:\n" );
	count = profiles.GetCount();
	for( i=0; i<count; i++ ){
		const declGameProfile &profile = *profiles.GetAt( i );
		printf( "- '%s'\n", profile.GetName().GetString() );
	}
}
