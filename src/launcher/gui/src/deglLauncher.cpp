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
#include <signal.h>
#ifndef OS_W32
#include <execinfo.h>
#endif

#include "deglLauncher.h"
#include "config/deglConfiguration.h"
#include "config/deglConfigWindow.h"
#include "engine/deglEngine.h"
#include "engine/deglEngineInstance.h"
#include "game/deglGameManager.h"
#include "game/deglGame.h"
#include "game/deglGameRunParams.h"
#include "game/profile/deglGameProfile.h"
#include "gui/deglWindowMain.h"
#include "logger/deglLoggerHistory.h"

#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Signal Handlers
////////////////////

#ifndef OS_W32
static deglLauncher *pLauncherForSegV = NULL;

static void signalSegV( int number, siginfo_t *infos, void *ptrContext ){
	deLogger *logger = NULL;
	
	if( pLauncherForSegV ){
		logger = pLauncherForSegV->GetLogger();
	}
	
	// some infos
	if( infos->si_code == SEGV_MAPERR ){
		if( logger ){
			logger->LogErrorFormat( LOGSOURCE, "Segmentation Fault! Tried to access not allocated memory at %p.", infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Tried to access not allocated memory at %p.\n", infos->si_addr );
		}
		
	}else if( infos->si_code == SEGV_ACCERR ){
		if( logger ){
			logger->LogErrorFormat( LOGSOURCE, "Segmentation Fault! Permission denied accessing memory at %p.", infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Permission denied accessing memory at %p.\n", infos->si_addr );
		}
		
	}else{
		if( logger ){
			logger->LogErrorFormat( LOGSOURCE, "Segmentation Fault! Unknown memory error at %p.", infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Unknown memory error at %p.\n", infos->si_addr );
		}
	}
	
	// stack trace
	void *btentries[ 50 ]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace( btentries, 50 );
	
	if( logger ){
		logger->LogError( LOGSOURCE, "Backtrace:" );
		
		char **btStrings = backtrace_symbols( btentries, btentryCount );
		int i;
		
		for( i=0; i<(int)btentryCount; i++ ){
			logger->LogError( LOGSOURCE, btStrings[ i ] );
		}
		
		free( btStrings );
		
	}else{
		printf( "Backtrace:" );
		backtrace_symbols_fd( btentries, btentryCount, fileno( stdout ) );
	}
	
	/*
	if( pLauncherForSegV ){
		if( infos->si_code == SEGV_MAPERR ){
			coreFault->HandleSegFault( decrbCoreFault::eecMemoryNotAllocated, infos->si_addr, ptrContext );
			
		}else if( infos->si_code == SEGV_ACCERR ){
			coreFault->HandleSegFault( decrbCoreFault::eecMemoryNoPermision, infos->si_addr, ptrContext );
			
		}else{
			coreFault->HandleSegFault( decrbCoreFault::eecMemoryUnknown, infos->si_addr, ptrContext );
		}
		
	}else{
		printf( "No launcher object found. Can not gather crash information!\n" );
	}
	*/
	
	if( logger ){
		logger->LogError( LOGSOURCE, "Done, exiting." );
		
	}else{
		printf( "Done, exiting.\n" );
	}
	
	exit( -1 );
}

static void signalAbort( int number, siginfo_t *infos, void *ptrContext ){
	deLogger *logger = NULL;
	
	if( pLauncherForSegV ){
		logger = pLauncherForSegV->GetLogger();
	}
	
	if( logger ){
		logger->LogError( LOGSOURCE, "Unhandled Exception!" );
		
	}else{
		printf( "Unhandled Exception\n" );
	}
	
	// stack trace
	void *btentries[ 50 ]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace( btentries, 50 );
	
	if( logger ){
		logger->LogError( LOGSOURCE, "Backtrace:" );
		
		char **btStrings = backtrace_symbols( btentries, btentryCount );
		int i;
		
		for( i=0; i<(int)btentryCount; i++ ){
			logger->LogError( LOGSOURCE, btStrings[ i ] );
		}
		
		free( btStrings );
		
	}else{
		printf( "Backtrace:" );
		backtrace_symbols_fd( btentries, btentryCount, fileno( stdout ) );
	}
	
	if( logger ){
		logger->LogError( LOGSOURCE, "Done, exiting." );
		
	}else{
		printf( "Done, exiting.\n" );
	}
	
	exit( -1 );
}
#endif



// Class deglLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

deglLauncher::deglLauncher( deglWindowMain *windowMain, int argc, char **argv ) :
pWindowMain( windowMain ),

pFileSystem( NULL ),
pConfiguration( NULL ),
pEngine( NULL ),
pGameManager( NULL ),
pPatchManager( *this ),
pCmdLineGame( NULL ),
pLogger( NULL ),
pLoggerHistory( NULL )
{
	if( ! windowMain ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	// register signals if on linux
	pRegisterSignals();
	
	// set default color console logger for the case something goes wrong
	// until the proper logger has been set up
	pLogger = new deLoggerConsoleColor;
	
	// build argument list
	for( i=1; i<argc; i++ ){
		pArgList.AddArgument( decUnicodeString::NewFromUTF8( argv[ i ] ) );
	}
	pParseArguments();
	
	// create configuration and locate path
	pConfiguration = new deglConfiguration( this );
	pConfiguration->LocatePath();
	
	// create virtual file system
	pFileSystem = new deVirtualFileSystem;
	pConfiguration->InitVirtualFileSystem();
	
	// init the logger. this has to wait until the virtual file system is
	// created as the log file is obtained from there
	pInitLogger();
	
	// now we can log the information we found so far
	pConfiguration->LogImportantValues();
	
	// load configuration
	pConfiguration->LoadConfiguration();
	
	// create engine
	pEngine = new deglEngine( *this );
	
	// create game manager
	pGameManager = new deglGameManager( this );
	
	// load configuration, modules, games and patches
	{
	deglEngineInstance instance( *this, pEngine->GetLogFile() );
	instance.StartEngine();
	instance.LoadModules();
	
	pEngine->PutEngineIntoVFS( instance );
	
	pLogger->LogInfoFormat( LOGSOURCE, "Engine config path = '%s'", pEngine->GetPathConfig().GetString() );
	pLogger->LogInfoFormat( LOGSOURCE, "Engine share path = '%s'", pEngine->GetPathShare().GetString() );
	pLogger->LogInfoFormat( LOGSOURCE, "Engine lib path = '%s'", pEngine->GetPathLib().GetString() );
	
	pEngine->UpdateResolutions( instance );
	pEngine->ReloadModuleList();
	pEngine->CheckModules( instance );
	pEngine->LoadConfig();
	
	pGameManager->CreateDefaultProfile();
	
	pGameManager->LoadGameList( instance );
	pPatchManager.LoadPatchList( instance );
	}
	
	pGameManager->LoadGameConfigs();
	pGameManager->Verify();
}

deglLauncher::~deglLauncher(){
	// save configurations
	if( pConfiguration ){
		pConfiguration->SaveConfiguration();
	}
	
	// release instances
	if( pCmdLineGame ){
		pCmdLineGame->FreeReference();
	}
	if( pGameManager ){
		delete pGameManager;
	}
	
	if( pEngine ){
		delete pEngine;
	}
	if( pConfiguration ){
		delete pConfiguration;
	}
	
	// this has to be released last
	if( pFileSystem ){
		pFileSystem->FreeReference();
	}
	
	if( pLogger ){
		pLogger->FreeReference();
	}
	if( pLoggerHistory ){
		pLoggerHistory->FreeReference();
	}
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
	if( pCmdLineGame ){
		pCmdLineGame->FreeReference();
		pCmdLineGame = NULL;
	}
	
	// locate the game to run
	deglGame *game = NULL;
	
	if( pRunGame.MatchesPattern( "*.delga" ) or pRunGame.MatchesPattern( "*.degame" ) ){
		// the game definition file is not required to be installed if defined by file. It is
		// always used even if a game with the same identifier is already installed. running a
		// game by explicit game file overrides the installed one. otherwise it is difficult
		// for the user to understand why something else happens than he indented
		deglGameList list;
		
		try{
			deglEngineInstance instance( *this, pEngine->GetLogFile() );
			instance.StartEngine();
			instance.LoadModules();
			
			pGameManager->LoadGameFromDisk( instance, pRunGame, list );
			
		}catch( const deException &e ){
			pWindowMain->DisplayException( e );
			return false;
		}
		
		if( list.GetCount() == 0 ){
			pLogger->LogInfo( LOGSOURCE, "No valid game definition found." );
			FXMessageBox::error( pWindowMain, MBOX_OK, "Run Game", "No game definition found" );
			return false;
		}
		
		pCmdLineGame = list.GetAt( 0 ); // TODO support multiple games using a choice for for example
		pCmdLineGame->AddReference();
		
		// load configuration if the game is not installed. this allows to keep the parameter
		// changes alive done by the player inside the game
		if( ! pGameManager->GetGameList().Has( pCmdLineGame ) ){
			pCmdLineGame->LoadConfig();
		}
		
		pCmdLineGame->VerifyRequirements();
		game = pCmdLineGame;
		
	}else{
		try{
			game = pGameManager->GetGameList().GetWithID( decUuid( pRunGame, false ) );
		}catch( const deException & ){
			// not an UUID
		}
		
		if( ! game ){
			deglGameList matching( pGameManager->GetGameList().GetWithAlias( pRunGame ) );
			if( matching.GetCount() == 1 ){
				game = matching.GetAt( 0 );
				
			}else if( matching.GetCount() > 1 ){
				pLogger->LogInfoFormat( LOGSOURCE, "More than one game matching '%s'.", pRunGame.GetString() );
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
		deglGameProfile *profile = game->GetProfileToUse();
		
		if( profile->GetValid() ){
			deglGameRunParams runParams;
			
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
	
	if( ! pGameManager->GetGameList().Has( pCmdLineGame ) ){
		pCmdLineGame->PulseChecking();
	}
	
	if( pCmdLineGame->IsRunning() ){
		return;
	}
	
	pWindowMain->close( false );
}



void deglLauncher::PulseChecking(){
	pGameManager->PulseChecking();
}



// Private Functions
//////////////////////

void deglLauncher::pParseArguments(){
	const int argumentCount = pArgList.GetArgumentCount();
	int argumentIndex = 0;
	
	// check for options. they all start with a dash.
	while( argumentIndex < argumentCount ){
		const decString argument( pArgList.GetArgumentAt( argumentIndex )->ToUTF8() );
		
		if( argument == "--profile" ){
			if( argumentCount - argumentIndex > 0 ){
				pRunProfileName = pArgList.GetArgumentAt( ++argumentIndex )->ToUTF8();
				
			}else{
				pLogger->LogError( LOGSOURCE, "Missing profile name after --profile" );
				DETHROW( deeInvalidParam );
			}
			
		}else if( argument == "--install" ){
			if( argumentCount - argumentIndex > 0 ){
				            pCmdLineInstallDelga = pArgList.GetArgumentAt( ++argumentIndex )->ToUTF8();
				
			}else{
				pLogger->LogError( LOGSOURCE, "Missing filename after --install" );
				DETHROW( deeInvalidParam );
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
								pRunProfileName = pArgList.GetArgumentAt( ++argumentIndex )->ToUTF8();
								
							}else{
								pLogger->LogError( LOGSOURCE, "Missing profile name after -p" );
								DETHROW( deeInvalidParam );
							}
							
						}else{
							pLogger->LogErrorFormat( LOGSOURCE,
								"Invalid option '%s' (p not last character)", argument.GetString() );
							DETHROW( deeInvalidParam );
						}
						
					}else{
						pLogger->LogErrorFormat( LOGSOURCE, "Unknown option -%c", ( char )option );
						DETHROW( deeInvalidParam );
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
		pRunGame = pArgList.GetArgumentAt( argumentIndex++ )->ToUTF8();
	}
	
	// the rest are arguments for the game
	while( argumentIndex < argumentCount ){
		pRunGameArgList.AddArgument( *pArgList.GetArgumentAt( argumentIndex ) );
		argumentIndex++;
	}
}

void deglLauncher::pInitLogger(){
	deLoggerConsoleColor *loggerConsole = NULL;
	decBaseFileWriter *fileWriter = NULL;
	deLoggerChain *loggerChain = NULL;
	deLoggerFile *loggerFile = NULL;
	
	bool useConsole = false; //true;
	bool useFile = true;
	bool useHistory = true;
	decPath pathLogFile;
	
	if( ! pLoggerHistory ){
		pLoggerHistory = new deglLoggerHistory;
		if( ! pLoggerHistory ) DETHROW( deeOutOfMemory );
		
		pLoggerHistory->SetHistorySize( 250 );
	}
	
	// build the logger combining the requested loggers
	try{
		// create the chain logger
		loggerChain = new deLoggerChain;
		if( ! loggerChain ) DETHROW( deeOutOfMemory );
		
		// add history logger if required
		if( useHistory ){
			loggerChain->AddLogger( pLoggerHistory );
		}
		
		// add console logger if required
		if( useConsole ){
			loggerConsole = new deLoggerConsoleColor;
			if( ! loggerConsole ) DETHROW( deeOutOfMemory );
			
			loggerChain->AddLogger( loggerConsole );
			
			loggerConsole->FreeReference();
			loggerConsole = NULL;
		}
		
		// add file logger if required
		if( useFile ){
			pathLogFile.SetFromUnix( "/logs/delauncher-gui.log" );
			fileWriter = pFileSystem->OpenFileForWriting( pathLogFile );
			
			loggerFile = new deLoggerFile( fileWriter );
			fileWriter->FreeReference();
			fileWriter = NULL;
			
			loggerChain->AddLogger( loggerFile );
			
			loggerFile->FreeReference();
			loggerFile = NULL;
		}
		
		// set the logger
		if( pLogger ){
			pLogger->FreeReference();
		}
		pLogger = loggerChain;
		
	}catch( const deException & ){
		if( fileWriter ){
			fileWriter->FreeReference();
		}
		if( loggerChain ){
			loggerChain->FreeReference();
		}
		if( loggerFile ){
			loggerFile->FreeReference();
		}
		if( loggerConsole ){
			loggerConsole->FreeReference();
		}
		throw;
	}
}

void deglLauncher::pRegisterSignals(){
#ifndef OS_W32
	struct sigaction action;
	
	// set aside the launcher for the signal handling
	pLauncherForSegV = this;
	
	// add handler for SEGV signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_sigaction = signalSegV;
	action.sa_flags = SA_SIGINFO;
	
	if( sigaction( SIGSEGV, &action, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	
	// add handler for ABORT signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_sigaction = signalAbort;
	action.sa_flags = SA_SIGINFO;
	
	if( sigaction( SIGABRT, &action, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	
	// ignore SIGPIPE signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_handler = SIG_IGN;
	action.sa_flags = 0;
	
	if( sigaction( SIGPIPE, &action, NULL ) ){
		DETHROW( deeInvalidAction );
	}
#endif
}
