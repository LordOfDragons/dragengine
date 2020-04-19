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

#include "declLauncher.h"
#include "action/declRunGame.h"
#include "action/declActionGames.h"
#include "action/declActionDelga.h"
#include "action/declActionHelp.h"
#include "action/declActionPatches.h"
#include "action/declListProfiles.h"
#include "config/declConfiguration.h"
#include "engine/declEngine.h"
#include "game/declGameManager.h"
#include "logger/declLoggerFiltered.h"

#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/logger/deLoggerChain.h>
#include <dragengine/logger/deLoggerFile.h>
#include <dragengine/logger/deLoggerReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decBaseFileWriterReference.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declLauncher
///////////////////////

// Constructor, destructor
////////////////////////////

declLauncher::declLauncher() :
pConfiguration( NULL ),
pEngine( NULL ),
pGameManager( NULL ),
pPatchManager( *this )
{
	pLogger.TakeOver( new deLoggerConsoleColor );
	pEngineLogger.TakeOver( new deLoggerConsoleColor );
}

declLauncher::~declLauncher(){
	CleanUp();
}



// Management
///////////////

void declLauncher::AddArgument( const decUnicodeString &argument ){
	pArgList.AddArgument( argument );
}

void declLauncher::PrintSyntax(){
	printf( "Drag[en]gine Console Launcher.\n" );
	printf( "Written by Plüss Roland (roland@rptd.ch).\n" );
	printf( "Released under the GPL (http://www.gnu.org/licenses/gpl.html), 2020.\n" );
	printf( "\n" );
	printf( "Syntax:\n" );
	printf( "delauncher-console [ <action> [ <action-options> ] ]\n" );
	printf( "   <action> can be one or more of the following:\n" );
	printf( "      help       Print syntax of an action.\n" );
	printf( "      run        Run games.\n" );
	printf( "      delga      Manage DELGA files (view content, install).\n" );
	printf( "      games      Manage games (list, uninstall).\n" );
	printf( "      profiles   Manage profiles (list).\n" );
	printf( "      patches    Manage patches (list, uninstall).\n" );
	printf( "\n" );
}



void declLauncher::Init(){
	// create configuration and locate path
	pConfiguration = new declConfiguration( this );
	pConfiguration->LocatePath();
	
	// create virtual file system
	pFileSystem.TakeOver( new deVirtualFileSystem );
	pConfiguration->InitVirtualFileSystem();
	
	// init the logger. this has to wait until the virtual file system is
	// created as the log file is obtained from there
	pInitLogger();
	
	// now we can log the informations we found so far
	pConfiguration->LogImportantValues();
	
	// load configuration
	pConfiguration->LoadConfiguration();
	
	// create engine
	pEngine = new declEngine( this );
	
	// create game manager
	pGameManager = new declGameManager( this );
}

int declLauncher::Run(){
	if( pArgList.GetArgumentCount() < 1 ){
		PrintSyntax();
		return -1;
	}
	
	const decString actionName( pArgList.GetArgumentAt( 0 )->ToUTF8() );
	
	if( actionName == "help" ){
		declActionHelp( this ).Run();
		
	}else if( actionName == "run" ){
		Init();
		declRunGame( this ).Run();
		
	}else if( actionName == "games" ){
		Init();
		return declActionGames( *this ).Run();
		
	}else if( actionName == "delga" ){
		Init();
		return declActionDelga( *this ).Run();
		
	}else if( actionName == "profiles" ){
		Init();
		declListProfiles( this ).Run();
		
	}else if( actionName == "patches" ){
		Init();
		declActionPatches( *this ).Run();
		
	}else{
		pLogger->LogErrorFormat( LOGSOURCE, "Unknown action '%s'", actionName.GetString() );
		PrintSyntax();
	}
	
	return 0;
}

void declLauncher::CleanUp(){
	if( pConfiguration ){
		//pConfiguration->SaveConfiguration();
	}
	
	if( pGameManager ){
		delete pGameManager;
		pGameManager = NULL;
	}
	
	if( pEngine ){
		delete pEngine;
		pEngine = NULL;
	}
	if( pConfiguration ){
		delete pConfiguration;
		pConfiguration = NULL;
	}
}

decString declLauncher::ReadInput() const{
	decString input;
	while( true ){
		const int character = fgetc( stdin );
		if( character == '\n' || character == EOF ){
			break;
		}
		input.AppendCharacter( character );
	}
	return input;
}

bool declLauncher::ReadInputConfirm() const{
	const decString input( ReadInput() );
	return input == "y" || input == "Y";
}

int declLauncher::ReadInputSelection() const{
	const decString input( ReadInput() );
	if( input.IsEmpty() ){
		return -1;
		
	}else{
		return input.ToInt();
	}
}



// Private Functions
//////////////////////

void declLauncher::pInitLogger(){
	pLogger.TakeOver( new declLoggerFiltered );
	declLoggerFiltered &loggerLauncher = ( declLoggerFiltered& )( deLogger& )pLogger;
	
	deLoggerReference refLoggerLauncherError;
	refLoggerLauncherError.TakeOver( new deLoggerChain );
	loggerLauncher.SetLoggerError( refLoggerLauncherError );
	deLoggerChain &loggerLauncherError = ( deLoggerChain& )( deLogger& )refLoggerLauncherError;
	
	deLoggerReference refLoggerLauncherWarn;
	refLoggerLauncherWarn.TakeOver( new deLoggerChain );
	loggerLauncher.SetLoggerWarning( refLoggerLauncherWarn );
	deLoggerChain &loggerLauncherWarn = ( deLoggerChain& )( deLogger& )refLoggerLauncherWarn;
	
	deLoggerReference refLoggerLauncherInfo;
	refLoggerLauncherInfo.TakeOver( new deLoggerChain );
	loggerLauncher.SetLoggerInfo( refLoggerLauncherInfo );
	deLoggerChain &loggerLauncherInfo = ( deLoggerChain& )( deLogger& )refLoggerLauncherInfo;
	
	pEngineLogger.TakeOver( new declLoggerFiltered );
	declLoggerFiltered &loggerEngine = ( declLoggerFiltered& )( deLogger& )pEngineLogger;
	loggerEngine.SetLoggerError( refLoggerLauncherError );
	loggerEngine.SetLoggerWarning( refLoggerLauncherWarn );
	
	pEngineLoggerDebug.TakeOver( new deLoggerChain );
	deLoggerChain &loggerEngineDebug = ( deLoggerChain& )( deLogger& )pEngineLoggerDebug;
	
	// console
	deLoggerReference loggerConsole;
	loggerConsole.TakeOver( new deLoggerConsoleColor );
	loggerLauncherError.AddLogger( loggerConsole );
	loggerEngineDebug.AddLogger( loggerConsole );
	
	// file
	decBaseFileWriterReference writer;
	writer.TakeOver( pFileSystem->OpenFileForWriting(
		decPath::CreatePathUnix( "/logs/delauncher-console.log" ) ) );
	
	deLoggerReference loggerFile;
	loggerFile.TakeOver( new deLoggerFile( writer ) );
	
	loggerLauncherError.AddLogger( loggerFile );
	loggerLauncherWarn.AddLogger( loggerFile );
	loggerLauncherInfo.AddLogger( loggerFile );
	
	loggerEngine.SetLoggerInfo( loggerFile );
	loggerEngineDebug.AddLogger( loggerFile );
}
