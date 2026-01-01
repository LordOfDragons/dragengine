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

#include "deglUninstall.h"
#include "deglWindowMain.h"
#include "../deglLauncher.h"
#include "../config/deglConfiguration.h"

#include <delauncher/game/patch/delPatch.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Class deglUninstall
////////////////////////

// Constructor, destructor
////////////////////////////

deglUninstall::deglUninstall(deglWindowMain &window) :
pWindow(window){
}

deglUninstall::~deglUninstall(){
}



// Management
///////////////

bool deglUninstall::UninstallGame(delGame &game){
	// check if the game is located in a delga file
	if(game.GetDelgaFile().IsEmpty()){
		FXMessageBox::information(&pWindow, MBOX_OK, "Uninstall Game",
			"Game is not located in a *.delga file. Can not uninstall");
		return false;
	}
	
	// check if another game or patch shares the same delga file
	const delGameList &gameList = pWindow.GetLauncher()->GetGameManager().GetGames();
	decString text;
	int i, count = gameList.GetCount();
	for(i=0; i<count; i++){
		const delGame &checkGame = *gameList.GetAt(i);
		if(&checkGame == &game || checkGame.GetDelgaFile() != game.GetDelgaFile()){
			continue;
		}
		
		text.AppendFormat("Game '%s'(%s) shares the same *.delga file.\n",
			checkGame.GetTitle().ToUTF8().GetString(),
			checkGame.GetIdentifier().ToHexString(false).GetString());
	}
	
	const delPatchList &patchList = pWindow.GetLauncher()->GetPatchManager().GetPatches();
	count = patchList.GetCount();
	
	for(i=0; i<count; i++){
		const delPatch &checkPatch = *patchList.GetAt(i);
		if(checkPatch.GetDelgaFile() != game.GetDelgaFile()){
			continue;
		}
		
		const delGame * const pgame = gameList.GetWithID(checkPatch.GetGameID());
		text.AppendFormat("Patch '%s'(%s) for game '%s' shares the same *.delga file.\n",
			checkPatch.GetName().ToUTF8().GetString(),
			checkPatch.GetIdentifier().ToHexString(false).GetString(),
			pgame ? pgame->GetTitle().ToUTF8().GetString() : "?");
	}
	
	if(!text.IsEmpty()){
		text += "\nUninstalling these games/patches too?";
		if(FXMessageBox::question(&pWindow, MBOX_YES_NO, "Uninstall Game",
		"%s", text.GetString()) != MBOX_CLICKED_YES){
			return false;
		}
	}
	
	// ask user if this is the right choice
	if(FXMessageBox::question(&pWindow, MBOX_YES_NO, "Uninstall Game",
	"Uninstall game '%s'?", game.GetTitle().ToUTF8().GetString()) != MBOX_CLICKED_YES){
		return false;
	}
	
	// uninstall
	pUninstallDelga(game.GetDelgaFile());
	
	FXMessageBox::information(&pWindow, MBOX_OK, "Uninstall Game", "Uninstalled successfully");
	return true;
}

bool deglUninstall::UninstallPatch(delPatch &patch){
	// check if the patch is located in a delga file
	if(patch.GetDelgaFile().IsEmpty()){
		FXMessageBox::information(&pWindow, MBOX_OK, "Uninstall Patch",
			"Patch is not located in a *.delga file. Can not uninstall");
		return false;
	}
	
	// check if another game or patch shares the same delga file
	const delGameList &gameList = pWindow.GetLauncher()->GetGameManager().GetGames();
	decString text;
	int i, count = gameList.GetCount();
	for(i=0; i<count; i++){
		const delGame &checkGame = *gameList.GetAt(i);
		if(checkGame.GetDelgaFile() != patch.GetDelgaFile()){
			continue;
		}
		
		text.AppendFormat("Game '%s'(%s) shares the same *.delga file.\n",
			checkGame.GetTitle().ToUTF8().GetString(),
			checkGame.GetIdentifier().ToHexString(false).GetString());
	}
	
	const delPatchList &patchList = pWindow.GetLauncher()->GetPatchManager().GetPatches();
	count = patchList.GetCount();
	
	for(i=0; i<count; i++){
		const delPatch &checkPatch = *patchList.GetAt(i);
		if(&checkPatch == &patch || checkPatch.GetDelgaFile() != patch.GetDelgaFile()){
			continue;
		}
		
		const delGame * const game = gameList.GetWithID(checkPatch.GetGameID());
		text.AppendFormat("Patch '%s'(%s) for game '%s' shares the same *.delga file.\n",
			checkPatch.GetName().ToUTF8().GetString(),
			checkPatch.GetIdentifier().ToHexString(false).GetString(),
			game ? game->GetTitle().ToUTF8().GetString() : "?");
	}
	
	if(!text.IsEmpty()){
		text += "\nUninstalling these games/patches too?";
		if(FXMessageBox::question(&pWindow, MBOX_YES_NO, "Uninstall Patch",
		"%s", text.GetString()) != MBOX_CLICKED_YES){
			return false;
		}
	}
	
	// ask user if this is the right choice
	if(FXMessageBox::question(&pWindow, MBOX_YES_NO, "Uninstall Patch",
	"Uninstall game '%s'?", patch.GetName().ToUTF8().GetString()) != MBOX_CLICKED_YES){
		return false;
	}
	
	// uninstall
	pUninstallDelga(patch.GetDelgaFile());
	
	FXMessageBox::information(&pWindow, MBOX_OK, "Uninstall Patch", "Uninstalled successfully");
	return true;
}



// Private Functions
//////////////////////

void deglUninstall::pUninstallDelga(const decString &filename){
	decPath target(decPath::CreatePathUnix("/"));
	target.AddComponent(decPath::CreatePathNative(filename).GetLastComponent());
	
	deVFSDiskDirectory::Ref::New(decPath::CreatePathNative(
		pWindow.GetLauncher()->GetPathGames()))->DeleteFile(target);
}
