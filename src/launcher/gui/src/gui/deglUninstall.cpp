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

#include "deglUninstall.h"
#include "deglWindowMain.h"
#include "../deglLauncher.h"
#include "../config/deglConfiguration.h"
#include "../engine/deglEngine.h"
#include "../engine/deglEngineInstance.h"
#include "../game/deglGame.h"
#include "../game/deglGameManager.h"
#include "../game/patch/deglPatch.h"
#include "../game/patch/deglPatchManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainerReference.h>
#include <dragengine/logger/deLogger.h>



// Class deglUninstall
////////////////////////

// Constructor, destructor
////////////////////////////

deglUninstall::deglUninstall( deglWindowMain &window ) :
pWindow( window ){
}

deglUninstall::~deglUninstall(){
}



// Management
///////////////

bool deglUninstall::UninstallGame( deglGame &game ){
	// check if the game is located in a delga file
	if( game.GetDelgaFile().IsEmpty() ){
		FXMessageBox::information( &pWindow, MBOX_OK, "Uninstall Game",
			"Game is not located in a *.delga file. Can not uninstall" );
		return false;
	}
	
	// check if another game or patch shares the same delga file
	const deglGameList &gameList = pWindow.GetLauncher()->GetGameManager()->GetGameList();
	decString text;
	int i, count = gameList.GetCount();
	for( i=0; i<count; i++ ){
		const deglGame &checkGame = *gameList.GetAt( i );
		if( &checkGame == &game || checkGame.GetDelgaFile() != game.GetDelgaFile() ){
			continue;
		}
		
		text.AppendFormat( "Game '%s'(%s) shares the same *.delga file.\n",
			checkGame.GetTitle().ToUTF8().GetString(),
			checkGame.GetIdentifier().ToHexString( false ).GetString() );
	}
	
	const deglPatchList &patchList = pWindow.GetLauncher()->GetPatchManager().GetPatches();
	count = patchList.GetCount();
	
	for( i=0; i<count; i++ ){
		const deglPatch &checkPatch = *patchList.GetAt( i );
		if( checkPatch.GetDelgaFile() != game.GetDelgaFile() ){
			continue;
		}
		
		const deglGame * const game = gameList.GetWithID( checkPatch.GetGameID() );
		text.AppendFormat( "Patch '%s'(%s) for game '%s' shares the same *.delga file.\n",
			checkPatch.GetName().ToUTF8().GetString(),
			checkPatch.GetIdentifier().ToHexString( false ).GetString(),
			game ? game->GetTitle().ToUTF8().GetString() : "?" );
	}
	
	if( ! text.IsEmpty() ){
		text += "\nUninstalling these games/patches too?";
		if( FXMessageBox::question( &pWindow, MBOX_YES_NO, "Uninstall Game",
		"%s", text.GetString() ) != MBOX_CLICKED_YES ){
			return false;
		}
	}
	
	// ask user if this is the right choice
	if( FXMessageBox::question( &pWindow, MBOX_YES_NO, "Uninstall Game",
	"Uninstall game '%s'?", game.GetTitle().ToUTF8().GetString() ) != MBOX_CLICKED_YES ){
		return false;
	}
	
	// uninstall
	pUninstallDelga( game.GetDelgaFile() );
	
	FXMessageBox::information( &pWindow, MBOX_OK, "Uninstall Game", "Uninstalled successfully" );
	return true;
}

bool deglUninstall::UninstallPatch( deglPatch &patch ){
	// check if the patch is located in a delga file
	if( patch.GetDelgaFile().IsEmpty() ){
		FXMessageBox::information( &pWindow, MBOX_OK, "Uninstall Patch",
			"Patch is not located in a *.delga file. Can not uninstall" );
		return false;
	}
	
	// check if another game or patch shares the same delga file
	const deglGameList &gameList = pWindow.GetLauncher()->GetGameManager()->GetGameList();
	decString text;
	int i, count = gameList.GetCount();
	for( i=0; i<count; i++ ){
		const deglGame &checkGame = *gameList.GetAt( i );
		if( checkGame.GetDelgaFile() != patch.GetDelgaFile() ){
			continue;
		}
		
		text.AppendFormat( "Game '%s'(%s) shares the same *.delga file.\n",
			checkGame.GetTitle().ToUTF8().GetString(),
			checkGame.GetIdentifier().ToHexString( false ).GetString() );
	}
	
	const deglPatchList &patchList = pWindow.GetLauncher()->GetPatchManager().GetPatches();
	count = patchList.GetCount();
	
	for( i=0; i<count; i++ ){
		const deglPatch &checkPatch = *patchList.GetAt( i );
		if( &checkPatch == &patch || checkPatch.GetDelgaFile() != patch.GetDelgaFile() ){
			continue;
		}
		
		const deglGame * const game = gameList.GetWithID( checkPatch.GetGameID() );
		text.AppendFormat( "Patch '%s'(%s) for game '%s' shares the same *.delga file.\n",
			checkPatch.GetName().ToUTF8().GetString(),
			checkPatch.GetIdentifier().ToHexString( false ).GetString(),
			game ? game->GetTitle().ToUTF8().GetString() : "?" );
	}
	
	if( ! text.IsEmpty() ){
		text += "\nUninstalling these games/patches too?";
		if( FXMessageBox::question( &pWindow, MBOX_YES_NO, "Uninstall Patch",
		"%s", text.GetString() ) != MBOX_CLICKED_YES ){
			return false;
		}
	}
	
	// ask user if this is the right choice
	if( FXMessageBox::question( &pWindow, MBOX_YES_NO, "Uninstall Patch",
	"Uninstall game '%s'?", patch.GetName().ToUTF8().GetString() ) != MBOX_CLICKED_YES ){
		return false;
	}
	
	// uninstall
	pUninstallDelga( patch.GetDelgaFile() );
	
	FXMessageBox::information( &pWindow, MBOX_OK, "Uninstall Patch", "Uninstalled successfully" );
	return true;
}



// Private Functions
//////////////////////

void deglUninstall::pUninstallDelga( const decString &filename ){
	deVFSContainerReference container;
	container.TakeOver( new deVFSDiskDirectory( decPath::CreatePathNative(
		pWindow.GetLauncher()->GetConfiguration()->GetPathGames() ) ) );
	
	decPath target( decPath::CreatePathUnix( "/" ) );
	target.AddComponent( decPath::CreatePathNative( filename ).GetLastComponent() );
	container->DeleteFile( target );
}
