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

#include "declActionGames.h"
#include "declActionDelgaHelper.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatch.h>
#include <delauncher/game/patch/delPatchManager.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader::Ref.h>
#include <dragengine/common/file/decBaseFileWriter::Ref.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSContainer::Ref.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declActionGames
////////////////////////

// Constructor, destructor
////////////////////////////

declActionGames::declActionGames( declLauncher &launcher ) :
pLauncher( launcher ){
}

declActionGames::~declActionGames(){
}



// Management
///////////////

void declActionGames::PrintSyntax(){
	printf( "Drag[en]gine Console Launcher.\n" );
	printf( "Written by Plüss Roland ( roland@rptd.ch ).\n" );
	printf( "Released under the GPL ( http://www.gnu.org/licenses/gpl.html ), 2011.\n" );
	printf( "\n" );
	printf( "Manage games.\n" );
	printf( "\n" );
	printf( "Syntax:\n" );
	printf( "delauncher-console games\n" );
	printf( "   List all installed games in the form 'alias (identifier) => title'\n" );
	printf( "\n" );
	printf( "delauncher-console games installed (<identifier> | <alias>)\n" );
	printf( "   Queries if game with identifier or alias is installed\n" );
	printf( "   Return code 0: Game is installed\n" );
	printf( "   Return code 1: Game is not installed\n" );
	/*
	printf( "\n" );
	printf( "delauncher-console games uninstall (<identifier> | <alias>)\n" );
	printf( "   Uninstall game. If game is not installed or multiple games match the call fails.\n" );
	printf( "   Game patches and local files (like saves or caches) are not uninstalled.\n" );
	printf( "   Return code 0: Game uninstalled successfully\n" );
	printf( "   Return code 1: Uninstalling game failed\n" );
	*/
}

int declActionGames::Run(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if( argumentList.GetArgumentCount() < 2 ){
		pLauncher.Prepare();
		return pListGames();
	}
	
	const decString action( argumentList.GetArgumentAt( 1 )->ToUTF8() );
	
	if( action == "installed" ){
		pLauncher.Prepare();
		return pIsInstalled();
		
		/*
	}else if( action == "uninstall" ){
		pLauncher.Prepare();
		return pUninstall();
		*/
		
	}else{
		PrintSyntax();
		return -2;
	}
}



// Private Functions
//////////////////////

int declActionGames::pListGames(){
	const delGameList &gameList = pLauncher.GetGameManager().GetGames();
	const int gameCount = gameList.GetCount();
	int i;
	
	printf( "Available Games (alias (identifier) => title):\n" );
	
	for( i=0; i<gameCount; i++ ){
		const delGame &game = *gameList.GetAt( i );
		printf( "- '%s' (%s) => %s\n", game.GetAliasIdentifier().GetString(),
			game.GetIdentifier().ToHexString( false ).GetString(),
			game.GetTitle().ToUTF8().GetString() );
	}
	
	return 0;
}

int declActionGames::pIsInstalled(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if( argumentList.GetArgumentCount() < 3 ){
		PrintSyntax();
		return -2;
	}
	
	const delGameList &gameList = pLauncher.GetGameManager().GetGames();
	const decString identifier( argumentList.GetArgumentAt( 2 )->ToUTF8() );
	
	try{
		if( gameList.HasWithID( decUuid( identifier, false ) ) ){
			printf( "Game '%s' is installed\n", identifier.GetString() );
			return 0;
		}
		
	}catch( const deException & ){
		// not a uuid
	}
	
	if( gameList.GetWithAlias( identifier ).GetCount() > 0 ){
		printf( "Game '%s' is installed\n", identifier.GetString() );
		return 0;
	}
	
	printf( "Game '%s' is not installed\n", identifier.GetString() );
	return 1;
}

int declActionGames::pUninstall(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if( argumentList.GetArgumentCount() < 3 ){
		PrintSyntax();
		return -2;
	}
	
	// find game to uninstall
	const delGameList &gameList = pLauncher.GetGameManager().GetGames();
	const decString identifier( argumentList.GetArgumentAt( 2 )->ToUTF8() );
	delGameList games;
	
	try{
		delGame * const game = gameList.GetWithID( decUuid( identifier, false ) );
		if( game ){
			games.Add( game );
		}
		
	}catch( const deException & ){
		// not a uuid
	}
	
	if( games.GetCount() == 0 ){
		games = gameList.GetWithAlias( identifier );
	}
	
	if( games.GetCount() == 0 ){
		printf( "Game '%s' is not installed\n", identifier.GetString() );
		return 1;
	}
	
	if( games.GetCount() > 1 ){
		printf( "Multiple games matching '%s'. Please use identifier to clarify:\n", identifier.GetString() );
		const int count = games.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			const delGame &game = *games.GetAt( i );
			printf( "- %s => %s\n", game.GetTitle().ToUTF8().GetString(),
				game.GetIdentifier().ToHexString( false ).GetString() );
		}
		return 1;
	}
	
	// check if the game is located in a delga file
	const delGame &game = *games.GetAt( 0 );
	if( game.GetDelgaFile().IsEmpty() ){
		printf( "Game is not located in a *.delga file. Can not uninstall\n" );
		return -1;
	}
	
	// check if another game or patch shares the same delga file
	bool hasSharedGamesPatches = false;
	int i, count = gameList.GetCount();
	for( i=0; i<count; i++ ){
		const delGame &checkGame = *gameList.GetAt( i );
		if( &checkGame == &game || checkGame.GetDelgaFile() != game.GetDelgaFile() ){
			continue;
		}
		
		printf( "Game '%s'(%s) shares the same *.delga file.\n",
			checkGame.GetTitle().ToUTF8().GetString(),
			checkGame.GetIdentifier().ToHexString( false ).GetString() );
		hasSharedGamesPatches = true;
	}
	
	const delPatchList &patchList = pLauncher.GetPatchManager().GetPatches();
	count = patchList.GetCount();
	
	for( i=0; i<count; i++ ){
		const delPatch &checkPatch = *patchList.GetAt( i );
		if( checkPatch.GetDelgaFile() != game.GetDelgaFile() ){
			continue;
		}
		
		const delGame * const pgame = pLauncher.GetGameManager().GetGames().GetWithID( checkPatch.GetGameID() );
		printf( "Patch '%s'(%s) for game '%s' shares the same *.delga file.\n",
			checkPatch.GetName().ToUTF8().GetString(),
			checkPatch.GetIdentifier().ToHexString( false ).GetString(),
			pgame ? pgame->GetTitle().ToUTF8().GetString() : "?" );
		hasSharedGamesPatches = true;
	}
	
	if( hasSharedGamesPatches ){
		printf( "\nUninstalling these games/patches too? [y/n] " );
		if( ! pLauncher.ReadInputConfirm() ){
			printf( "Aborting\n" );
			return -1;
		}
		printf( "\n" );
	}
	
	// ask user if this is the right choice
	printf( "Ready to uninstall game '%s'.\n", game.GetTitle().ToUTF8().GetString() );
	printf( "Do you want to continue? [y/n] " );
	if( ! pLauncher.ReadInputConfirm() ){
		printf( "Aborting\n" );
		return -1;
	}
	
	printf( "\n" );
	
	// uninstall
	declActionDelgaHelper( pLauncher, game.GetDelgaFile() ).Uninstall();
	return 0;
}
