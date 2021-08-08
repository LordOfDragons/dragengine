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

#include "deglLauncher.h"
#include "config/deglConfiguration.h"
#include "config/deglConfigWindow.h"
#include "game/deglGameIcon.h"
#include "gui/deglWindowMain.h"

#include <delauncher/engine/delEngineInstance.h>
#include <delauncher/game/delGameRunParams.h>

#include <dragengine/common/exceptions.h>


// Class deglLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

deglLauncher::deglLauncher( deglWindowMain *windowMain, int argc, char **argv ) :
delLauncher( "LauncherGui", "delauncher-gui-engine" ),

pSignalHandler( *this ),
pConfiguration( *this ),

pWindowMain( windowMain ),

pCmdLineGame( NULL )
{
	if( ! windowMain ){
		DETHROW( deeInvalidParam );
	}
	
	// also log to "/log/delauncher-gui.log"
	AddFileLogger( "delauncher-gui" );
	
	// build argument list
	int i;
	for( i=1; i<argc; i++ ){
		pArguments.AddArgument( decUnicodeString::NewFromUTF8( argv[ i ] ) );
	}
	pParseArguments();
	
	// load configuration
	pConfiguration.LoadConfiguration();
	
	// prepare launcher
	Prepare();
}

deglLauncher::~deglLauncher(){
	pConfiguration.SaveConfiguration();
}



// Management
///////////////

bool deglLauncher::HasCommandLineRunGame() const{
	return ! pRunGame.IsEmpty();
}

bool deglLauncher::RunCommandLineGame(){
	if( ! pCmdLineInstallDelga.IsEmpty() ){
		return true;
	}
	if( ! HasCommandLineRunGame() || pRunGame.IsEmpty() ){
		return true;
	}
	
	// just in case this somehow is called multiple times, which should never happen
	pCmdLineGame = nullptr;
	
	// locate the game to run
	delGame::Ref game;
	
	if( pRunGame.MatchesPattern( "*.delga" ) or pRunGame.MatchesPattern( "*.degame" ) ){
		// the game definition file is not required to be installed if defined by file. It is
		// always used even if a game with the same identifier is already installed. running a
		// game by explicit game file overrides the installed one. otherwise it is difficult
		// for the user to understand why something else happens than he indented
		delGameList list;
		
		try{
			delEngineInstance instance( *this, GetEngine().GetLogFile() );
			instance.StartEngine();
			instance.LoadModules();
			
			GetGameManager().LoadGameFromDisk( instance, pRunGame, list );
			
		}catch( const deException &e ){
			pWindowMain->DisplayException( e );
			return false;
		}
		
		if( list.GetCount() == 0 ){
			GetLogger()->LogInfo( GetLogSource(), "No valid game definition found." );
			FXMessageBox::error( pWindowMain, MBOX_OK, "Run Game", "No game definition found" );
			return false;
		}
		
		pCmdLineGame = list.GetAt( 0 ); // TODO support multiple games using a choice for for example
		
		// load configuration if the game is not installed. this allows to keep the parameter
		// changes alive done by the player inside the game
		if( ! GetGameManager().GetGames().Has( pCmdLineGame ) ){
			pCmdLineGame->LoadConfig();
		}
		
		pCmdLineGame->VerifyRequirements();
		game = pCmdLineGame;
		
	}else{
		try{
			game = GetGameManager().GetGames().GetWithID( decUuid( pRunGame, false ) );
		}catch( const deException & ){
			// not an UUID
		}
		
		if( ! game ){
			const delGameList matching( GetGameManager().GetGames().GetWithAlias( pRunGame ) );
			
			if( matching.GetCount() == 1 ){
				game = matching.GetAt( 0 );
				
			}else if( matching.GetCount() > 1 ){
				GetLogger()->LogInfoFormat( GetLogSource(),
					"More than one game matching '%s'.", pRunGame.GetString() );
				FXMessageBox::error( pWindowMain, MBOX_OK, "Run Game",
					"More than one game matching '%s'.", pRunGame.GetString() );
				return false;
			}
		}
	}
	
	if( ! game ){
		FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Run Game",
			"No game found with identifier '%s'", pRunGame.GetString() );
		return false;
	}
	
	if( game->GetCanRun() ){
		delGameProfile * const profile = game->GetProfileToUse();
		
		if( profile->GetValid() ){
			delGameRunParams runParams;
			runParams.SetGameProfile( profile );
			
			decString error;
			if( ! runParams.FindPatches( *game, game->GetUseLatestPatch(), game->GetUseCustomPatch(), error ) ){
				FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Can not run game", "%s", error.GetString() );
				return false;
			}
			
			if( profile->GetReplaceRunArguments() ){
				runParams.SetRunArguments( profile->GetRunArguments() );
				
			}else{
				runParams.SetRunArguments( game->GetRunArguments() + " " + profile->GetRunArguments() );
			}
			
			//runParams.SetRunArguments( runParams.GetRunArguments() + " " + pRunGameArgList );
			
			runParams.SetFullScreen( profile->GetFullScreen() );
			runParams.SetWidth( profile->GetWidth() );
			runParams.SetHeight( profile->GetHeight() );
			
			if( game->GetWindowSize() != decPoint() ){
				runParams.SetWidth( game->GetWindowSize().x );
				runParams.SetHeight( game->GetWindowSize().y );
				runParams.SetFullScreen( false );
			}
			
			game->StartGame( runParams );
		}
		
	}else if( ! game->GetAllFormatsSupported() ){
		FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Can not run game",
			"One or more File Formats required by the game are not working." );
		return false;
		
	}else{
		FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Can not run game",
			"Game related properties are incorrect." );
		return false;
	}
	
	return true;
}

void deglLauncher::RunCommandLineGameStopCheck(){
	if( ! HasCommandLineRunGame() ){
		return;
	}
	
	if( ! pCmdLineGame ){
		pWindowMain->close( false );
		return;
	}
	
	if( ! GetGameManager().GetGames().Has( pCmdLineGame ) ){
		pCmdLineGame->PulseChecking();
	}
	
	if( pCmdLineGame->IsRunning() ){
		return;
	}
	
	pWindowMain->close( false );
}



void deglLauncher::PulseChecking(){
	GetGameManager().PulseChecking();
}



delGameIcon *deglLauncher::CreateGameIcon( int size, const char* path ){
	return new deglGameIcon( size, path );
}



// Private Functions
//////////////////////

void deglLauncher::pParseArguments(){
	const int argumentCount = pArguments.GetArgumentCount();
	int argumentIndex = 0;
	
	// check for options. they all start with a dash.
	while( argumentIndex < argumentCount ){
		const decString argument( pArguments.GetArgumentAt( argumentIndex )->ToUTF8() );
		
		if( argument == "--profile" ){
			if( argumentCount - argumentIndex > 0 ){
				pRunProfileName = pArguments.GetArgumentAt( ++argumentIndex )->ToUTF8();
				
			}else{
				GetLogger()->LogError( GetLogSource(), "Missing profile name after --profile" );
				DETHROW_INFO( deeInvalidParam, "Missing profile name after --profile" );
			}
			
		}else if( argument == "--install" ){
			if( argumentCount - argumentIndex > 0 ){
				pCmdLineInstallDelga = pArguments.GetArgumentAt( ++argumentIndex )->ToUTF8();
				
			}else{
				GetLogger()->LogError( GetLogSource(), "Missing filename after --install" );
				DETHROW_INFO( deeInvalidParam, "Missing filename after --install" );
			}
			
		}else if( argument.GetLength() > 0 ){
			if( argument[ 0 ] == '-' ){
				const int optionLen = argument.GetLength();
				int o;
				
				for( o=1; o<optionLen; o++ ){
					const int option = argument[ o ];
					
					if( option == 'p' ){
						if( o == optionLen - 1 ){
							if( argumentCount - argumentIndex > 0 ){
								pRunProfileName = pArguments.GetArgumentAt( ++argumentIndex )->ToUTF8();
								
							}else{
								GetLogger()->LogError( GetLogSource(), "Missing profile name after -p" );
								DETHROW_INFO( deeInvalidParam, "Missing profile name after -p" );
							}
							
						}else{
							decString message;
							message.Format( "Invalid option '%s' (p not last character)", argument.GetString() );
							
							GetLogger()->LogError( GetLogSource(), message );
							DETHROW_INFO( deeInvalidParam, message );
						}
						
					}else{
						decString message;
						message.Format( "Unknown option -%c", ( char )option );
						GetLogger()->LogError( GetLogSource(), message );
						DETHROW_INFO( deeInvalidParam, message );
					}
				}
				
			}else{
				break;
			}
			
		}else{
			break;
		}
		
		argumentIndex++;
	}
	
	// the next argument can be one of this:
	// - game definition file if file name ends with .degame
	// - delga file if file name ends with .delga
	// - game id otherwise
	if( argumentIndex < argumentCount ){
		pRunGame = pArguments.GetArgumentAt( argumentIndex++ )->ToUTF8();
	}
	
	// the rest are arguments for the game
	while( argumentIndex < argumentCount ){
		pRunGameArgList.AddArgument( *pArguments.GetArgumentAt( argumentIndex ) );
		argumentIndex++;
	}
}
