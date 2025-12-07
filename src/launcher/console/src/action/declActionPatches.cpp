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

#include "declActionPatches.h"
#include "declActionDelgaHelper.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatch.h>
#include <delauncher/game/patch/delPatchManager.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declActionPatches
////////////////////////

// Constructor, destructor
////////////////////////////

declActionPatches::declActionPatches(declLauncher &launcher) :
pLauncher(launcher){
}

declActionPatches::~declActionPatches(){
}



// Management
///////////////

void declActionPatches::PrintSyntax(){
	printf("Drag[en]gine Console Launcher.\n");
	printf("Written by Plüss Roland (roland@rptd.ch).\n");
	printf("Released under the GPL (http://www.gnu.org/licenses/gpl.html), 2011.\n");
	printf("\n");
	printf("Manage patches.\n");
	printf("\n");
	printf("Syntax:\n");
	printf("delauncher-console patches\n");
	printf("   List all installed patches in the form 'alias (identifier) => title'\n");
	printf("\n");
	printf("delauncher-console patches installed (<identifier> | <alias>)\n");
	printf("   Queries if patch with identifier or alias is installed\n");
	printf("   Return code 0: Patch is installed\n");
	printf("   Return code 1: Patch is not installed\n");
	/*
	printf("\n");
	printf("delauncher-console patches uninstall (<identifier> | <alias>)\n");
	printf("   Uninstall patch. If patch is not installed or multiple patches match the call fails.\n");
	printf("   Patch game is not uninstalled.\n");
	printf("   Return code 0: Patch uninstalled successfully\n");
	printf("   Return code 1: Uninstalling patch failed\n");
	*/
}

int declActionPatches::Run(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if(argumentList.GetArgumentCount() < 2){
		pLauncher.Prepare();
		return pListPatches();
	}
	
	const decString action(argumentList.GetArgumentAt(1)->ToUTF8());
	
	if(action == "installed"){
		pLauncher.Prepare();
		return pIsInstalled();
		
		/*
	}else if(action == "uninstall"){
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

int declActionPatches::pListPatches(){
	const delPatchList &patches = pLauncher.GetPatchManager().GetPatches();
	const int count = patches.GetCount();
	int i;
	
	printf("Available Patches (patch-name (game-alias) => patch-identifier (game-identifier)\n");
	
	for(i=0; i<count; i++){
		const delPatch &patch = *patches.GetAt(i);
		const delGame * const game = pLauncher.GetGameManager().GetGames().GetWithID(patch.GetGameID());
		printf("- '%s' (%s) => %s (%s)\n", patch.GetName().ToUTF8().GetString(),
			game ? game->GetAliasIdentifier().GetString() : "?",
			patch.GetIdentifier().ToHexString(false).GetString(),
			patch.GetGameID().ToHexString(false).GetString());
	}
	
	return 0;
}

int declActionPatches::pIsInstalled(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if(argumentList.GetArgumentCount() < 3){
		PrintSyntax();
		return -2;
	}
	
	const delPatchList &patchList = pLauncher.GetPatchManager().GetPatches();
	const decString identifier(argumentList.GetArgumentAt(2)->ToUTF8());
	
	try{
		if(patchList.HasWithID(decUuid(identifier, false))){
			printf("Patch '%s' is installed\n", identifier.GetString());
			return 0;
		}
		
	}catch(const deException &){
		// not a uuid
	}
	
	printf("Patch '%s' is not installed\n", identifier.GetString());
	return 1;
}

int declActionPatches::pUninstall(){
	const decUnicodeArgumentList &argumentList = pLauncher.GetArgumentList();
	if(argumentList.GetArgumentCount() < 3){
		PrintSyntax();
		return -2;
	}
	
	// find patch to uninstall
	const delPatchList &patchList = pLauncher.GetPatchManager().GetPatches();
	const decString identifier(argumentList.GetArgumentAt(2)->ToUTF8());
	delPatchList patches;
	
	try{
		delPatch * const patch = patchList.GetWithID(decUuid(identifier, false));
		if(patch){
			patches.Add(patch);
		}
		
	}catch(const deException &){
		// not a uuid
	}
	
	if(patches.GetCount() == 0){
		printf("Patch '%s' is not installed\n", identifier.GetString());
		return 1;
	}
	
	if(patches.GetCount() > 1){
		printf("Multiple patches matching '%s'. Please use identifier to clarify:\n", identifier.GetString());
		const int count = patches.GetCount();
		int i;
		for(i=0; i<count; i++){
			const delPatch &patch = *patches.GetAt(i);
			const delGame * const game = pLauncher.GetGameManager().GetGames().GetWithID(patch.GetGameID());
			printf("- %s (for game '%s') => %s\n", patch.GetName().ToUTF8().GetString(),
				game ? game->GetTitle().ToUTF8().GetString() : "?",
				patch.GetIdentifier().ToHexString(false).GetString());
		}
		return 1;
	}
	
	// check if the patch is located in a delga file
	const delPatch &patch = *patches.GetAt(0);
	if(patch.GetDelgaFile().IsEmpty()){
		printf("Patch is not located in a *.delga file. Can not uninstall\n");
		return -1;
	}
	
	// check if another patch shares the same delga file
	bool hasSharedGamesPatches = false;
	int i, count = patchList.GetCount();
	for(i=0; i<count; i++){
		const delPatch &checkPatch = *patchList.GetAt(i);
		if(&checkPatch == &patch || checkPatch.GetDelgaFile() != patch.GetDelgaFile()){
			continue;
		}
		
		const delGame * const game = pLauncher.GetGameManager().GetGames().GetWithID(checkPatch.GetGameID());
		printf("Patch '%s'(%s) for game '%s' shares the same *.delga file.\n",
			checkPatch.GetName().ToUTF8().GetString(),
			checkPatch.GetIdentifier().ToHexString(false).GetString(),
			game ? game->GetTitle().ToUTF8().GetString() : "?");
		hasSharedGamesPatches = true;
	}
	
	const delGameList &gameList = pLauncher.GetGameManager().GetGames();
	count = gameList.GetCount();
	for(i=0; i<count; i++){
		const delGame &checkGame = *gameList.GetAt(i);
		if(checkGame.GetDelgaFile() != patch.GetDelgaFile()){
			continue;
		}
		
		printf("Game '%s'(%s) shares the same *.delga file.\n",
			checkGame.GetTitle().ToUTF8().GetString(),
			checkGame.GetIdentifier().ToHexString(false).GetString());
		hasSharedGamesPatches = true;
	}
	
	if(hasSharedGamesPatches){
		printf("\nUninstalling these games/patches too? [y/n] ");
		if(! pLauncher.ReadInputConfirm()){
			printf("Aborting\n");
			return -1;
		}
		printf("\n");
	}
	
	// ask user if this is the right choice
	const delGame * const game = pLauncher.GetGameManager().GetGames().GetWithID(patch.GetGameID());
	printf("Ready to uninstall patch '%s' for game '%s'.\n", patch.GetName().ToUTF8().GetString(),
		game ? game->GetTitle().ToUTF8().GetString() : "?");
	printf("Do you want to continue? [y/n] ");
	if(! pLauncher.ReadInputConfirm()){
		printf("Aborting\n");
		return -1;
	}
	
	printf("\n");
	
	// uninstall
	declActionDelgaHelper(pLauncher, patch.GetDelgaFile()).Uninstall();
	return 0;
}
