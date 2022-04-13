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

#include "declActionDelga.h"
#include "declActionDelgaHelper.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatch.h>
#include <delauncher/game/patch/delPatchManager.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declActionDelga
////////////////////////

// Constructor, destructor
////////////////////////////

declActionDelga::declActionDelga( declLauncher &launcher ) :
pLauncher( launcher ){
}

declActionDelga::~declActionDelga(){
}



// Management
///////////////

void declActionDelga::PrintSyntax(){
	printf( "Drag[en]gine Console Launcher.\n" );
	printf( "Written by Plüss Roland ( roland@rptd.ch ).\n" );
	printf( "Released under the GPL ( http://www.gnu.org/licenses/gpl.html ), 2011.\n" );
	printf( "\n" );
	printf( "Manage DELGA.\n" );
	printf( "\n" );
	printf( "Syntax:\n" );
	printf( "delauncher-console delga content <filename>\n" );
	printf( "   List content of DELGA file\n" );
	/*
	printf( "\n" );
	printf( "delauncher-console delga install <filename>\n" );
	printf( "   Install DELGA file. If content is already installed the call fails.\n" );
	printf( "   Return code 0: DELGA installed successfully\n" );
	printf( "   Return code 1: Installing DELGA failed\n" );
	*/
}



int declActionDelga::Run(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if( argumentList.GetArgumentCount() < 2 ){
		PrintSyntax();
		return -2;
	}
	
	const decString action( argumentList.GetArgumentAt( 1 )->ToUTF8() );
	
	if( action == "content" ){
		pLauncher.Prepare();
		return pContent();
		
		/*
	}else if( action == "install" ){
		pLauncher.Prepare();
		return pInstall();
		*/
		
	}else{
		PrintSyntax();
		return -2;
	}
}



// Private Functions
//////////////////////

int declActionDelga::pContent(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if( argumentList.GetArgumentCount() < 3 ){
		PrintSyntax();
		return -2;
	}
	
	declActionDelgaHelper delga( pLauncher, argumentList.GetArgumentAt( 2 )->ToUTF8() );
	delga.Load();
	
	printf( "Games (alias (identifier) => title):\n" );
	const delGameList &games = delga.GetGames();
	const int gameCount = games.GetCount();
	int i;
	for( i=0; i<gameCount; i++ ){
		const delGame &game = *games.GetAt( i );
		printf( "- '%s' (%s) => %s\n",
			game.GetAliasIdentifier().GetString(),
			game.GetIdentifier().ToHexString( false ).GetString(),
			game.GetTitle().ToUTF8().GetString() );
	}
	
	printf( "\nPatches (name (patch alias) => identifier (patch identifier):\n" );
	const delPatchList &patches = delga.GetPatches();
	const int patchCount = patches.GetCount();
	for( i=0; i<patchCount; i++ ){
		const delPatch &patch = *patches.GetAt( i );
		const delGame * const game = pLauncher.GetGameManager().GetGames().GetWithID( patch.GetGameID() );
		printf( "- '%s' (%s) => %s (%s)\n",
			patch.GetName().ToUTF8().GetString(),
			game ? game->GetTitle().ToUTF8().GetString() : "?",
			patch.GetIdentifier().ToHexString( false ).GetString(),
			patch.GetGameID().ToHexString( false ).GetString() );
	}
	
	return 0;
}

int declActionDelga::pInstall(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if( argumentList.GetArgumentCount() < 3 ){
		PrintSyntax();
		return -2;
	}
	
	// load delga file to see what game is inside
	declActionDelgaHelper delga( pLauncher, argumentList.GetArgumentAt( 2 )->ToUTF8() );
	delga.Load();
	if( ! delga.HasContent() ){
		printf( "Empty DELGA.\n" );
		return -1;
	}
	
	// check if any of the games inside are installed
	const delGameList &games = delga.GetGames();
	int i;
	for( i=0; i<games.GetCount(); i++ ){
		if( pLauncher.GetGameManager().GetGames().HasWithID( games.GetAt( i )->GetIdentifier() ) ){
			printf( "Game '%s' is already installed\n", games.GetAt( i )->GetTitle().ToUTF8().GetString() );
			return -1;
		}
	}
	
	const delPatchList &patches = delga.GetPatches();
	for( i=0; i<patches.GetCount(); i++ ){
		if( pLauncher.GetPatchManager().GetPatches().HasWithID( patches.GetAt( i )->GetIdentifier() ) ){
			printf( "Patch '%s' is already installed\n", patches.GetAt( i )->GetName().ToUTF8().GetString() );
			return -1;
		}
	}
	
	// show what would be installed and ask if this is the right thing to do
	printf( "The following content will be installed:\n" );
	for( i=0; i<games.GetCount(); i++ ){
		const delGame &game = *games.GetAt( i );
		printf( "- Game '%s'\n", game.GetTitle().ToUTF8().GetString() );
	}
	for( i=0; i<patches.GetCount(); i++ ){
		const delPatch &patch = *patches.GetAt( i );
		const delGame *game = pLauncher.GetGameManager().GetGames().GetWithID( patch.GetGameID() );
		if( ! game ){
			game = delga.GetGames().GetWithID( patch.GetGameID() );
		}
		printf( "- Patch '%s' for game '%s'\n",
			patch.GetName().ToUTF8().GetString(),
			game ? game->GetTitle().ToUTF8().GetString() : "?" );
	}
	
	printf( "\nDo you want to continue? [y/n] " );
	if( ! pLauncher.ReadInputConfirm() ){
		printf( "Aborting\n" );
		return -1;
	}
	
	// install delga file
	delga.Install();
	return 0;
}
