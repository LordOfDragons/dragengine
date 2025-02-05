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
#include <ctype.h>

#include "deglLauncher.h"
#include "config/deglConfiguration.h"
#include "config/deglConfigWindow.h"
#include "game/deglGameIcon.h"
#include "gui/deglWindowMain.h"

#include <delauncher/engine/delEngineInstanceThreaded.h>
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

pCmdLineGame( NULL ),
pCmdLineQuitNow( false )
{
	if( ! windowMain ){
		DETHROW( deeInvalidParam );
	}
	
	// also log to "/logs/delauncher-gui"
	AddFileLogger( "delauncher-gui" );
	
	// set default engine instance executable (windows only)
	#ifdef OS_W32
	delEngineInstanceThreaded::SetDefaultExecutableName( "delauncher-gui-engine" );
	#endif
	
	// build argument list
	int i;
	for( i=1; i<argc; i++ ){
		pArguments.AddArgument( decUnicodeString::NewFromUTF8( argv[ i ] ) );
	}
	pParseArguments();
	
	// quit if requested. special use case
	if( pCmdLineQuitNow ){
		return;
	}
	
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
	
	// locate the game to run. we can not use MatchesPattern() to figure out if this
	// is a *.delga or *.degame file since this call fails if relative path is used
	// which begins with ../ . instead use string matching which works too
	delGame::Ref game;
	
	//if( pRunGame.MatchesPattern( "*.delga" ) || pRunGame.MatchesPattern( "*.degame" ) ){
	if( pRunGame.EndsWith( ".delga" ) || pRunGame.EndsWith( ".degame" ) ){
		// make the path absolute if relative
		if( ! decPath::IsNativePathAbsolute( pRunGame ) ){
			decPath path( decPath::CreateWorkingDirectory() );
			path.AddNativePath( pRunGame );
			pRunGame = path.GetPathNative();
		}
		
		// the game definition file is not required to be installed if defined by file. It is
		// always used even if a game with the same identifier is already installed. running a
		// game by explicit game file overrides the installed one. otherwise it is difficult
		// for the user to understand why something else happens than he indented
		GetLogger()->LogInfoFormat( GetLogSource(), "Run Game: '%s'", pRunGame.GetString() );
		
		delGameList list;
		
		try{
			const delEngineInstance::Ref instance( delEngineInstance::Ref::New(
				GetEngineInstanceFactory().CreateEngineInstance( *this, GetEngine().GetLogFile() ) ) );
			
			instance->StartEngine();
			instance->LoadModules();
			
			GetGameManager().LoadGameFromDisk( instance, pRunGame, list );
			
		}catch( const deException &e ){
			pWindowMain->DisplayException( e );
			return false;
		}
		
		if( list.GetCount() == 0 ){
			decString message;
			message.Format( "No game definition found: %s", pRunGame.GetString() );
			GetLogger()->LogInfo( GetLogSource(), message );
			FXMessageBox::error( pWindowMain, MBOX_OK, "Run Game", "%s", message.GetString() );
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
			"Game not found: %s", pRunGame.GetString() );
		return false;
	}
	
	if( game->GetCanRun() ){
		delGameProfile *profile = game->GetProfileToUse();
		
		if( ! pRunProfileName.IsEmpty() ){
			profile = GetGameManager().GetProfiles().GetNamed( pRunProfileName );
			if( ! profile ){
				FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Run Game",
					"No profile found named '%s'", pRunProfileName.GetString() );
				return false;
			}
		}
		
		if( profile->GetValid() ){
			delGameRunParams runParams;
			runParams.SetGameProfile( profile );
			
			decString error;
			if( ! runParams.FindPatches( *game, game->GetUseLatestPatch(), game->GetUseCustomPatch(), error ) ){
				FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Can not run game", "%s", error.GetString() );
				return false;
			}
			
			decString arguments( profile->GetRunArguments() );
			
			if( ! profile->GetReplaceRunArguments() ){
				arguments = game->GetRunArguments() + " " + arguments;
			}
			
			const int argCount = pRunGameArgList.GetArgumentCount();
			int i;
			for( i=0; i<argCount; i++ ){
				arguments.Append( " " );
				decString argument( pRunGameArgList.GetArgumentAt( i )->ToUTF8() );
				if( argument.Find( ' ' ) != -1 ){
					argument = decString( "\"" ) + argument + decString( "\"" );
				}
				arguments.Append( argument );
			}
			
			runParams.SetRunArguments( arguments );
			
			runParams.SetFullScreen( profile->GetFullScreen() );
			runParams.SetWidth( profile->GetWidth() );
			runParams.SetHeight( profile->GetHeight() );
			
			const decPoint windowSize(game->GetDisplayScaledWindowSize());
			if(windowSize != decPoint()){
				runParams.SetWidth(windowSize.x);
				runParams.SetHeight(windowSize.y);
				runParams.SetFullScreen(false);
			}
			
			game->StartGame( runParams );
		}
		
	}else if( ! game->GetAllFormatsSupported() ){
		FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Can not run game",
			"One or more File Formats required by the game are not working.\n\n"
			"Try updating Drag[en]gine to the latest version");
		return false;
		
	}else{
		FXMessageBox::error( pWindowMain->getApp(), MBOX_OK, "Can not run game",
			"Game related properties are incorrect.\n\n"
			"Try updating Drag[en]gine to the latest version" );
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
	
	// check some special arguments first which require uncluttered output
	if( argumentCount > 0 ){
		const decString argument( pArguments.GetArgumentAt( argumentIndex )->ToUTF8() );
		
		if( argument == "--version" ){
			printf( "%s", DE_VERSION );
			pCmdLineQuitNow = true;
			return;
		}
	}
	
	// log command line
	GetLogger()->LogInfo( GetLogSource(), "Command line arguments:" );
	for( argumentIndex=0; argumentIndex<argumentCount; argumentIndex++ ){
		GetLogger()->LogInfoFormat( GetLogSource(), "- '%s'",
			pArguments.GetArgumentAt( argumentIndex )->ToUTF8().GetString() );
	}
	
	argumentIndex = 0;
	
	// windows URI scheme support
	#ifdef OS_W32
	if( pParseWindowsURIScheme() ){
		return;
	}
	#endif
	
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
			// deprecated
			/*
			if( argumentCount - argumentIndex > 0 ){
				pCmdLineInstallDelga = pArguments.GetArgumentAt( ++argumentIndex )->ToUTF8();
				
			}else{
				GetLogger()->LogError( GetLogSource(), "Missing filename after --install" );
				DETHROW_INFO( deeInvalidParam, "Missing filename after --install" );
			}
			*/
			
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

bool deglLauncher::pParseWindowsURIScheme(){
	// check if this is a windows URI scheme
	const int argumentCount = pArguments.GetArgumentCount();
	if( argumentCount == 0 ){
		return false;
	}
	
	decString urischeme( pArguments.GetArgumentAt( 0 )->ToUTF8() );
	if( ! urischeme.BeginsWith( "delauncher:" ) ){
		return false;
	}
	
	// convert the arguments into a single string just to be on the safe side.
	// only required if the caller did not encode the URL properly
	int i;
	for( i=1; i<argumentCount; i++ ){
		urischeme.AppendCharacter( ' ' );
		urischeme += pArguments.GetArgumentAt( i )->ToUTF8();
	}
	
	// parse URI scheme
	urischeme = urischeme.GetMiddle( 11 );
	
	if( urischeme.BeginsWith( "run?" ) ){
		const decStringList parameters( urischeme.GetMiddle( 4 ).Split( '&' ) );
		const int parameterCount = parameters.GetCount();
		
		for( i=0; i<parameterCount; i++ ){
			const decString &parameter = parameters.GetAt( i );
			
			if( parameter.BeginsWith( "file=" ) ){
				pRunGame = pUrlDecode( parameter.GetMiddle( 5 ) );
				
			}else if( parameter.BeginsWith( "profile=" ) ){
				pRunProfileName = pUrlDecode( parameter.GetMiddle( 8 ) );
				
			}else if( parameter.BeginsWith( "argument=" ) ){
				pRunGameArgList.AddArgument( decUnicodeString::NewFromUTF8( pUrlDecode( parameter.GetMiddle( 9 ) ) ) );
			}
		}
		
	}else if( urischeme.BeginsWith( "install?" ) ){
		// deprecated
		/*
		const decStringList parameters( urischeme.GetMiddle( 8 ).Split( '&' ) );
		const int parameterCount = parameters.GetCount();
		
		for( i=0; i<parameterCount; i++ ){
			const decString &parameter = parameters.GetAt( i );
			
			if( parameter.BeginsWith( "file=" ) ){
				pCmdLineInstallDelga = pUrlDecode( parameter.GetMiddle( 5 ) );
			}
		}
		*/
		
	}else if( urischeme == "ready" ){
		// special command used only for installing without launching. immediately quit
		pCmdLineQuitNow = true;
	}
	
	return true;
}

decString deglLauncher::pUrlDecode( const char *url ){
	const char *walker = url;
	decString decoded;
	
	while( *walker ){
		if( walker[ 0 ] == '%'
		&&  walker[ 1 ] && isxdigit( walker[ 1 ] )
		&&  walker[ 2 ] && isxdigit( walker[ 2 ] ) ){
			const char hex[ 3 ] = { walker[ 1 ], walker[ 2 ], 0 };
			decoded.AppendCharacter( ( char )strtol( hex, nullptr, 16 ) );
			walker += 3;
			
		}else{
			decoded.AppendCharacter( *( walker++ ) );
		}
	}
	
	return decoded;
}
