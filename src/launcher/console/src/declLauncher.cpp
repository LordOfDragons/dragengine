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
#include "logger/declLoggerFiltered.h"

#include <delauncher/engine/delEngineInstanceDirect.h>

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
pConfiguration( NULL ){
	// also log to "/logs/delauncher-console.log"
	AddFileLogger( "delauncher-console.log" );
	
	// set launcher to use direct engine instance
	SetEngineInstanceFactory( new delEngineInstanceDirect::Factory );
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
	pConfiguration = new declConfiguration( *this );
	
	pInitLogger();
	
	pConfiguration->LoadConfiguration();
}

int declLauncher::Run(){
	if( pArgList.GetArgumentCount() < 1 ){
		PrintSyntax();
		return -1;
	}
	
	const decString actionName( pArgList.GetArgumentAt( 0 )->ToUTF8() );
	
	if( actionName == "help" ){
		declActionHelp( *this ).Run();
		
	}else if( actionName == "version" ){
		printf( "%s", DE_VERSION );
		return 0;
		
	}else if( actionName == "run" ){
		Init();
		declRunGame( *this ).Run();
		
	}else if( actionName == "games" ){
		Init();
		return declActionGames( *this ).Run();
		
	}else if( actionName == "delga" ){
		Init();
		return declActionDelga( *this ).Run();
		
	}else if( actionName == "profiles" ){
		Init();
		declListProfiles( *this ).Run();
		
	}else if( actionName == "patches" ){
		Init();
		declActionPatches( *this ).Run();
		
	}else{
		GetLogger()->LogErrorFormat( LOGSOURCE, "Unknown action '%s'", actionName.GetString() );
		PrintSyntax();
	}
	
	return 0;
}

void declLauncher::CleanUp(){
	if( pConfiguration ){
		//pConfiguration->SaveConfiguration();
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
	// clear logger chain set up by the shared launcher. we want a custom one
	GetLogger()->RemoveAllLoggers();
	
	const declLoggerFiltered::Ref loggerLauncher( declLoggerFiltered::Ref::New( new declLoggerFiltered ) );
	GetLogger()->AddLogger( loggerLauncher );
	
	const deLoggerChain::Ref loggerLauncherError( deLoggerChain::Ref::New( new deLoggerChain ) );
	loggerLauncher->SetLoggerError( loggerLauncherError );
	
	const deLoggerChain::Ref loggerLauncherWarn( deLoggerChain::Ref::New( new deLoggerChain ) );
	loggerLauncher->SetLoggerWarning( loggerLauncherWarn );
	
	const deLoggerChain::Ref loggerLauncherInfo( deLoggerChain::Ref::New( new deLoggerChain ) );
	loggerLauncher->SetLoggerInfo( loggerLauncherInfo );
	
	declLoggerFiltered::Ref loggerEngine( declLoggerFiltered::Ref::New( new declLoggerFiltered ) );
	loggerEngine->SetLoggerError( loggerLauncherError );
	loggerEngine->SetLoggerWarning( loggerLauncherWarn );
	pEngineLogger = loggerEngine;
	
	const deLoggerChain::Ref loggerEngineDebug( deLoggerChain::Ref::New( new deLoggerChain ) );
	pEngineLoggerDebug = loggerEngineDebug;
	
	// console
	const deLoggerConsoleColor::Ref loggerConsole( deLoggerConsoleColor::Ref::New( new deLoggerConsoleColor ) );
	loggerLauncherError->AddLogger( loggerConsole );
	loggerEngineDebug->AddLogger( loggerConsole );
	
	// file
	const deLoggerFile::Ref loggerFile( deLoggerFile::Ref::New( new deLoggerFile(
		decBaseFileWriter::Ref::New( GetVFS()->OpenFileForWriting(
			decPath::CreatePathUnix( "/logs/delauncher-console.log" ) ) ) ) ) );
	
	loggerLauncherError->AddLogger( loggerFile );
	loggerLauncherWarn->AddLogger( loggerFile );
	loggerLauncherInfo->AddLogger( loggerFile );
	
	loggerEngine->SetLoggerInfo( loggerFile );
	loggerEngineDebug->AddLogger( loggerFile );
}
