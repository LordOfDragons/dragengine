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
#include <stdint.h>

#include "deglPanelGames.h"
#include "deglPGListItemGame.h"
#include "properties/deglDialogGameProperties.h"
#include "runwith/deglDialogRunGameWith.h"
#include "../deglWindowMain.h"
#include "../deglUninstall.h"
#include "../../deglLauncher.h"
#include "../../config/deglConfiguration.h"

#include <delauncher/game/delGameRunParams.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>

#ifdef OS_UNIX
#include <errno.h>
#endif

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Events
///////////

FXDEFMAP( deglPanelGames ) deglPanelGamesMap[] = {
	FXMAPFUNC( SEL_COMMAND, deglPanelGames::ID_LIST_GAMES, deglPanelGames::onListGamesChanged ),
	FXMAPFUNC( SEL_CHANGED, deglPanelGames::ID_LIST_GAMES, deglPanelGames::onListGamesChanged ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, deglPanelGames::ID_LIST_GAMES, deglPanelGames::onListGamesRDown ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, deglPanelGames::ID_LIST_GAMES, deglPanelGames::onListGamesRUp ),
	FXMAPFUNC( SEL_DOUBLECLICKED, deglPanelGames::ID_LIST_GAMES, deglPanelGames::onListGamesDblClick ),
	FXMAPFUNC( deglPanelGames::ExtIconList::SEL_HEADER_CLICKED, deglPanelGames::ID_LIST_GAMES,
		deglPanelGames::onListGamesHeaderClicked ),
	
	FXMAPFUNC( SEL_COMMAND, deglPanelGames::ID_PU_GAME_PROPS, deglPanelGames::onPUGameProps ),
	FXMAPFUNC( SEL_UPDATE, deglPanelGames::ID_PU_GAME_PROPS, deglPanelGames::updatePUGameProps ),
	FXMAPFUNC( SEL_COMMAND, deglPanelGames::ID_PU_GAME_RUN, deglPanelGames::onPUGameRun ),
	FXMAPFUNC( SEL_UPDATE, deglPanelGames::ID_PU_GAME_RUN, deglPanelGames::updatePUGameRun ),
	FXMAPFUNC( SEL_COMMAND, deglPanelGames::ID_PU_GAME_RUNWITH, deglPanelGames::onPUGameRunWith ),
	FXMAPFUNC( SEL_UPDATE, deglPanelGames::ID_PU_GAME_RUNWITH, deglPanelGames::updatePUGameRun ),
	FXMAPFUNC( SEL_COMMAND, deglPanelGames::ID_PU_GAME_KILL, deglPanelGames::onPUGameKill ),
	FXMAPFUNC( SEL_UPDATE, deglPanelGames::ID_PU_GAME_KILL, deglPanelGames::updatePUGameKill ),
	FXMAPFUNC( SEL_COMMAND, deglPanelGames::ID_PU_GAME_UNINSTALL, deglPanelGames::onPUGameUninstall ),
	FXMAPFUNC( SEL_UPDATE, deglPanelGames::ID_PU_GAME_UNINSTALL, deglPanelGames::updatePUGameUninstall ),
	FXMAPFUNC( SEL_COMMAND, deglPanelGames::ID_PU_GAME_SHOWLOGS, deglPanelGames::onPUGameShowLogs ),
	FXMAPFUNC( SEL_UPDATE, deglPanelGames::ID_PU_GAME_SHOWLOGS, deglPanelGames::updatePUGameShowLogs ),
};

FXDEFMAP( deglPanelGames::ExtIconList ) deglPanelGamesExtIconListMap[] = {
	FXMAPFUNC( SEL_COMMAND, ICONLIST_HEADER_ID, deglPanelGames::ExtIconList::onHeaderClicked )
};



// Class deglPanelGames::ExtIconList
//////////////////////////////////////

FXIMPLEMENT( deglPanelGames::ExtIconList, FXIconList, deglPanelGamesExtIconListMap,
	ARRAYNUMBER( deglPanelGamesExtIconListMap ) )

deglPanelGames::ExtIconList::ExtIconList(){
}

deglPanelGames::ExtIconList::ExtIconList( FXComposite *p, FXObject *tgt, FXSelector sel,
FXuint opts, FXint x, FXint y, FXint w, FXint h ) :
FXIconList( p, tgt, sel, opts, x, y, w, h ){
}

deglPanelGames::ExtIconList::~ExtIconList(){
}

long deglPanelGames::ExtIconList::onHeaderClicked( FXObject*, FXSelector, void* ){
	if( target ){
		return target->tryHandle( this, FXSEL( SEL_HEADER_CLICKED, message ), nullptr );
	}
	return 0;
}



// Class deglPanelGames
/////////////////////////

FXIMPLEMENT( deglPanelGames, FXVerticalFrame, deglPanelGamesMap, ARRAYNUMBER( deglPanelGamesMap ) )

// Constructor, destructor
////////////////////////////

deglPanelGames::deglPanelGames(){ }

deglPanelGames::deglPanelGames( deglWindowMain *windowMain, FXComposite *container ) :
FXVerticalFrame( container, LAYOUT_FILL_Y | LAYOUT_SIDE_LEFT | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ){
	//FXHorizontalFrame *frameLine;
	//FXVerticalFrame *content;
	//FXHeader *header;
	
	pWindowMain = windowMain;
	
	// create content
	pListGames = new ExtIconList( this, this, ID_LIST_GAMES, FOLDINGLIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	if( ! pListGames ) DETHROW( deeOutOfMemory );
	
	pListGames->appendHeader( "Game", nullptr, 350 );
	pListGames->appendHeader( "Status", nullptr, 150 );
	pListGames->appendHeader( "Creator", nullptr, 200 );
	
	pListGames->setSortFunc( deglPanelGames::SortGamesByTitleAsc );
	pSortListGames = elgsTitleAsc;
}

deglPanelGames::~deglPanelGames(){
}



// Management
///////////////

delGame *deglPanelGames::GetSelectedGame() const{
	const int selection = pListGames->getCurrentItem();
	
	if( selection != -1 ){
		return ( ( deglPGListItemGame* )pListGames->getItem( selection ) )->GetGame();
	}
	
	return nullptr;
}

void deglPanelGames::SetSelectedGame( delGame *game ){
	const int count = pListGames->getNumItems();
	int i, selection = -1;
	
	for( i=0; i<count; i++ ){
		if( ( ( deglPGListItemGame* )pListGames->getItem( i ) )->GetGame() == game ){
			selection = i;
			break;
		}
	}
	
	if( selection == -1 && count > 0 ){
		selection = 0;
	}
	
	pListGames->setCurrentItem( selection );
}



void deglPanelGames::UpdateGameList(){
	const delGameList &gameList = pWindowMain->GetLauncher()->GetGameManager().GetGames();
	delGame *selectedGame = GetSelectedGame();
	const int count = gameList.GetCount();
	deglPGListItemGame *listItem;
	delGame *game;
	int i;
	
	pListGames->clearItems();
	
	for( i=0; i<count; i++ ){
		game = gameList.GetAt( i );
		listItem = nullptr;
		
		try{
			listItem = new deglPGListItemGame( this, game );
			pListGames->appendItem( listItem );
			
		}catch( const deException & ){
			if( listItem ){
				delete listItem;
			}
			throw;
		}
	}
	
	if( count > 0 ){
		pListGames->sortItems();
	}
	
	SetSelectedGame( selectedGame );
}



FXint deglPanelGames::SortGamesByTitleAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delGame &game1 = *( ( ( deglPGListItemGame* )item1 )->GetGame() );
	const delGame &game2 = *( ( ( deglPGListItemGame* )item2 )->GetGame() );
	
	return game1.GetTitle().Compare( game2.GetTitle() );
}

FXint deglPanelGames::SortGamesByTitleDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delGame &game1 = *( ( ( deglPGListItemGame* )item1 )->GetGame() );
	const delGame &game2 = *( ( ( deglPGListItemGame* )item2 )->GetGame() );
	
	return game2.GetTitle().Compare( game1.GetTitle() );
}

FXint deglPanelGames::SortGamesByStatusAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delGame &game1 = *( ( ( deglPGListItemGame* )item1 )->GetGame() );
	const delGame &game2 = *( ( ( deglPGListItemGame* )item2 )->GetGame() );
	
	return game1.GetCanRun() < game2.GetCanRun();
}

FXint deglPanelGames::SortGamesByStatusDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delGame &game1 = *( ( ( deglPGListItemGame* )item1 )->GetGame() );
	const delGame &game2 = *( ( ( deglPGListItemGame* )item2 )->GetGame() );
	
	return game2.GetCanRun() < game1.GetCanRun();
}

FXint deglPanelGames::SortGamesByCreatorAsc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delGame &game1 = *( ( ( deglPGListItemGame* )item1 )->GetGame() );
	const delGame &game2 = *( ( ( deglPGListItemGame* )item2 )->GetGame() );
	
	return game1.GetCreator().Compare( game2.GetCreator() );
}

FXint deglPanelGames::SortGamesByCreatorDesc( const FXIconItem *item1, const FXIconItem *item2 ){
	const delGame &game1 = *( ( ( deglPGListItemGame* )item1 )->GetGame() );
	const delGame &game2 = *( ( ( deglPGListItemGame* )item2 )->GetGame() );
	
	return game2.GetCreator().Compare( game1.GetCreator() );
}



// Events
///////////

long deglPanelGames::onListGamesChanged( FXObject*, FXSelector, void* ){
	return 1;
}

long deglPanelGames::onListGamesRDown( FXObject*, FXSelector, void *data ){
	const FXEvent &event = *( ( const FXEvent * )data );
	delGame *game = nullptr;
	
	pListGames->setCurrentItem( pListGames->getItemAt( event.win_x, event.win_y ) );
	
	game = GetSelectedGame();
	
	if( game ){
		const int x = event.root_x;
		const int y = event.root_y;
		FXMenuCommand *menuCommand;
		FXMenuPane *popup = nullptr;
		bool running;
		
		try{
			running = game->IsRunning();
			
			popup = new FXMenuPane( this );
			
			new FXMenuCommand( popup, "Properties...", nullptr, this, ID_PU_GAME_PROPS );
			
			new FXSeparator( popup );
			menuCommand = new FXMenuCommand( popup, "Run Game", nullptr, this, ID_PU_GAME_RUN );
			if( running ){
				menuCommand->disable();
			}
			
			menuCommand = new FXMenuCommand( popup, "Run Game With Profile...", nullptr, this, ID_PU_GAME_RUNWITH );
			if( running ){
				menuCommand->disable();
			}
			
			menuCommand = new FXMenuCommand( popup, "Kill Game", nullptr, this, ID_PU_GAME_KILL );
			
			menuCommand = new FXMenuCommand( popup, "Show Logs", nullptr, this, ID_PU_GAME_SHOWLOGS );
			
			/*
			new FXSeparator( popup );
			menuCommand = new FXMenuCommand( popup, "Uninstall Game", nullptr, this, ID_PU_GAME_UNINSTALL );
			if( running ){
				menuCommand->disable();
			}
			*/
			
			popup->create();
			
			popup->popup( nullptr, x + 1, y + 1 ); // popup-bug. do not show straight under the cursor
			pWindowMain->getApp()->runModalWhileShown( popup );
			
			delete popup;
			
		}catch( const deException &e ){
			if( popup ) delete popup;
			pWindowMain->DisplayException( e );
		}
	}
	
	return 1;
}

long deglPanelGames::onListGamesRUp( FXObject*, FXSelector, void* ){
	return 1;
}

long deglPanelGames::onListGamesDblClick( FXObject *sender, FXSelector selector, void *data ){
	return onPUGameRun( sender, selector, data );
}

long deglPanelGames::onListGamesHeaderClicked( FXObject*, FXSelector, void *data ){
	const int colon = ( intptr_t )data;
	
	if( colon == 0 ){
		if( pSortListGames == elgsTitleAsc ){
			pSortListGames = elgsTitleDesc;
			pListGames->setSortFunc( deglPanelGames::SortGamesByTitleDesc );
			
		}else{
			pSortListGames = elgsTitleAsc;
			pListGames->setSortFunc( deglPanelGames::SortGamesByTitleAsc );
		}
		
		pListGames->sortItems();
		
	}else if( colon == 1 ){
		if( pSortListGames == elgsStatusAsc ){
			pSortListGames = elgsStatusDesc;
			pListGames->setSortFunc( deglPanelGames::SortGamesByStatusDesc );
			
		}else{
			pSortListGames = elgsStatusAsc;
			pListGames->setSortFunc( deglPanelGames::SortGamesByStatusAsc );
		}
		
		pListGames->sortItems();
		
	}else if( colon == 2 ){
		if( pSortListGames == elgsCreatorAsc ){
			pSortListGames = elgsCreatorDesc;
			pListGames->setSortFunc( deglPanelGames::SortGamesByCreatorDesc );
			
		}else{
			pSortListGames = elgsCreatorAsc;
			pListGames->setSortFunc( deglPanelGames::SortGamesByCreatorAsc );
		}
		
		pListGames->sortItems();
	}
	
	return 1;
}



long deglPanelGames::onPUGameProps( FXObject*, FXSelector, void* ){
	delGame * const game = GetSelectedGame();
	if( ! game ){
		return 1;
	}
	
	deglDialogGameProperties dialog( pWindowMain, game, pWindowMain );
	if( dialog.execute( PLACEMENT_OWNER ) ){
		game->SaveConfig();
	}
	
	pWindowMain->GetLauncher()->GetGameManager().Verify();
	UpdateGameList();
	return 1;
}

long deglPanelGames::updatePUGameProps( FXObject *sender, FXSelector, void* ){
	bool enable = false;
	const delGame * const game = GetSelectedGame();
	if( game ){
		enable = ! game->IsRunning();
	}
	sender->tryHandle( sender, FXSEL( SEL_COMMAND, enable ? ID_ENABLE : ID_DISABLE ), nullptr );
	return 1;
}

long deglPanelGames::onPUGameRun( FXObject*, FXSelector, void* ){
	delGame * const game = GetSelectedGame();
	if( ! game ){
		return 1;
	}
	
	if( game->IsRunning() ){
		FXMessageBox::information( this, MBOX_OK, "Start Game", "This game is already running." );
		return 1;
	}
	
	while( true ){
		delGameProfile * const profile = game->GetProfileToUse();
		
		if( game->GetCanRun() && profile->GetValid() ){
			delGameRunParams runParams;
			
			runParams.SetGameProfile( profile );
			
			decString error;
			if( ! runParams.FindPatches( *game, game->GetUseLatestPatch(), game->GetUseCustomPatch(), error ) ){
				FXMessageBox::error( this, MBOX_OK, "Can not run game", "%s", error.GetString() );
				return false;
			}
			
			if( profile->GetReplaceRunArguments() ){
				runParams.SetRunArguments( profile->GetRunArguments().GetString() );
				
			}else{
				runParams.SetRunArguments( ( game->GetRunArguments() + " " + profile->GetRunArguments().GetString() ).GetString() );
			}
			
			runParams.SetWidth( profile->GetWidth() );
			runParams.SetHeight( profile->GetHeight() );
			runParams.SetFullScreen( profile->GetFullScreen() );
			
			if( game->GetWindowSize() != decPoint() ){
				runParams.SetWidth( game->GetWindowSize().x );
				runParams.SetHeight( game->GetWindowSize().y );
				runParams.SetFullScreen( false );
			}
			
			game->StartGame( runParams );
			
			break;
			
		}else if( ! profile->GetValid() ){
			if( FXMessageBox::question( this, MBOX_YES_NO, "Can not run game",
			"The Game Profile is not working. Open the Game Properties to fix the problem?" ) == MBOX_CLICKED_YES ){
				deglDialogGameProperties dialog( pWindowMain, game, pWindowMain );
				bool result;
				
				dialog.SwitchToProperties();
				result = dialog.execute( PLACEMENT_OWNER );
				if( result ){
					game->SaveConfig();
				}
				
				pWindowMain->GetLauncher()->GetGameManager().Verify();
				UpdateGameList();
				
				if( ! result ){
					break;
				}
				
			}else{
				break;
			}
			
		}else if( ! game->GetAllFormatsSupported() ){
			FXMessageBox::information( this, MBOX_OK, "Can not run game", "One or more File Formats required by the game are not working." );
			break;
			
		}else{
			FXMessageBox::information( this, MBOX_OK, "Can not run game", "Game related properties are incorrect." );
			break;
		}
	}
	
	UpdateGameList();
	
	return 1;
}

long deglPanelGames::onPUGameRunWith( FXObject*, FXSelector, void* ){
	delGame * const game = GetSelectedGame();
	if( ! game ){
		return 1;
	}
	
	if( game->IsRunning() ){
		FXMessageBox::information( this, MBOX_OK, "Start Game", "This game is already running." );
		return 1;
	}
	
	while( true ){
		if( game->GetCanRun() ){
			deglDialogRunGameWith dialog( GetWindowMain(), game, this );
			dialog.SetProfile( game->GetActiveProfile() );
			
			const delGameManager &gameManager = pWindowMain->GetLauncher()->GetGameManager();
			delGameRunParams runParams;
			while( true ){
				if( ! dialog.execute( PLACEMENT_OWNER ) ){
					break;
				}
				
				runParams.SetGameProfile( dialog.GetProfile() );
				if( ! runParams.GetGameProfile() ){
					runParams.SetGameProfile( gameManager.GetActiveProfile() );
					if( ! runParams.GetGameProfile() ){
						runParams.SetGameProfile( gameManager.GetDefaultProfile() );
					}
				}
				
				decString error;
				if( ! runParams.FindPatches( *game, game->GetUseLatestPatch(), game->GetUseCustomPatch(), error ) ){
					FXMessageBox::error( this, MBOX_OK, "Can not run game", "%s", error.GetString() );
					return false;
				}
				
				if( runParams.GetGameProfile()->GetReplaceRunArguments() ){
					runParams.SetRunArguments( runParams.GetGameProfile()->GetRunArguments().GetString() );
					
				}else{
					runParams.SetRunArguments( ( game->GetRunArguments() + " " + runParams.GetGameProfile()->GetRunArguments().GetString() ).GetString() );
				}
				
				runParams.SetWidth( runParams.GetGameProfile()->GetWidth() );
				runParams.SetHeight( runParams.GetGameProfile()->GetHeight() );
				runParams.SetFullScreen( runParams.GetGameProfile()->GetFullScreen() );
				
				if( game->GetWindowSize() != decPoint() ){
					runParams.SetWidth( game->GetWindowSize().x );
					runParams.SetHeight( game->GetWindowSize().y );
					runParams.SetFullScreen( false );
				}
				
				if( runParams.GetGameProfile()->GetValid() ){
					game->StartGame( runParams );
					break;
					
				}else{
					FXMessageBox::information( this, MBOX_OK, "Can not run game",
						"The Game Profile is not working, select a different one or fix the problems" );
				}
			}
			
			break;
			
		}else if( ! game->GetAllFormatsSupported() ){
			FXMessageBox::information( this, MBOX_OK, "Can not run game", "One or more File Formats required by the game are not working." );
			break;
			
		}else{
			FXMessageBox::information( this, MBOX_OK, "Can not run game", "Game related properties are incorrect." );
			break;
		}
	}
	
	UpdateGameList();
	
	return 1;
}

long deglPanelGames::updatePUGameRun( FXObject *sender, FXSelector, void* ){
	bool enable = false;
	const delGame * const game = GetSelectedGame();
	if( game ){
		enable = ! game->IsRunning();
	}
	sender->tryHandle( sender, FXSEL( SEL_COMMAND, enable ? ID_ENABLE : ID_DISABLE ), nullptr );
	return 1;
}

long deglPanelGames::onPUGameKill( FXObject*, FXSelector, void* ){
	delGame * const game = GetSelectedGame();
	
	if( game ){
		if( ! game->IsRunning() ){
			FXMessageBox::information( this, MBOX_OK, "Kill Game", "This game is not running. Nothing to kill there." );
			
		}else{
			try{
				game->KillGame();
				
			}catch( const deException &e ){
				pWindowMain->DisplayException( e );
			}
		}
	}
	
	return 1;
}

long deglPanelGames::updatePUGameKill( FXObject *sender, FXSelector, void* ){
	bool enable = false;
	const delGame * const game = GetSelectedGame();
	if( game ){
		enable = game->IsRunning();
	}
	sender->tryHandle( sender, FXSEL( SEL_COMMAND, enable ? ID_ENABLE : ID_DISABLE ), nullptr );
	return 1;
}

long deglPanelGames::onPUGameUninstall( FXObject*, FXSelector, void* ){
	delGame * const game = GetSelectedGame();
	if( ! game ){
		return 1;
	}
	
	if( game->IsRunning() ){
		FXMessageBox::information( this, MBOX_OK, "Uninstall Game", "This game is running. Cannot uninstall." );
		return 1;
	}
	
	try{
		if( deglUninstall( *pWindowMain ).UninstallGame( *game ) ){
			pWindowMain->ReloadGamesAndPatches();
		}
		
	}catch( const deException &e ){
		pWindowMain->DisplayException( e );
	}
	
	return 1;
}

long deglPanelGames::updatePUGameUninstall( FXObject *sender, FXSelector, void* ){
	bool enable = false;
	const delGame * const game = GetSelectedGame();
	if( game ){
		enable = ! game->IsRunning();
	}
	sender->tryHandle( sender, FXSEL( SEL_COMMAND, enable ? ID_ENABLE : ID_DISABLE ), nullptr );
	return 1;
}

long deglPanelGames::onPUGameShowLogs( FXObject*, FXSelector, void* ){
	delGame * const game = GetSelectedGame();
	if( ! game ){
		return 1;
	}
	
	decPath path;
	path.SetFromNative( pWindowMain->GetLauncher()->GetPathLogs() );
	path.AddComponent( "games" );
	path.AddComponent( game->GetIdentifier().ToHexString( false ) );
	
	const deVFSDiskDirectory::Ref container( deVFSDiskDirectory::Ref::New( new deVFSDiskDirectory( path ) ) );
	if( ! container->ExistsFile( decPath::CreatePathUnix( "/logs" ) ) ){
		FXMessageBox::information( this, MBOX_OK, "Show Logs",
			"There are no logs for this game. Logs will be present after running the game" );
		return 1;
	}
	
	path.AddComponent( "logs" );
	
	#ifdef OS_W32
	wchar_t widePath[ MAX_PATH ];
	deOSWindows::Utf8ToWide( path.GetPathNative(), widePath, MAX_PATH );
	ShellExecute( nullptr, L"open", widePath, nullptr, nullptr, SW_SHOWDEFAULT );
		
	#else
	const char * const appname = "xdg-open";
	
	if( fork() == 0 ){
		// GetString() is required otherwise execlp fails to run correctly
		execlp( appname, appname, path.GetPathNative().GetString(), nullptr );
		printf( "Failed running '%s' (error %d)\n", appname, errno );
		exit( 0 );
	}
	#endif
	return 1;
}

long deglPanelGames::updatePUGameShowLogs( FXObject *sender, FXSelector, void* ){
	return sender->tryHandle( sender, FXSEL( SEL_COMMAND, ID_ENABLE ), nullptr );
}
