/* 
 * Drag[en]gine Game Engine
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
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>

#include "deOSConsole.h"
#include "../common/exceptions.h"



// Class deOSConsole
//////////////////////

// Constructor, destructor
////////////////////////////

deOSConsole::deOSConsole(){
	pScreenWidth = 80; // ncurses?
	pScreenHeight = 60; // ncurses?
	
	// init locale
	setlocale( LC_ALL, "" );
}

deOSConsole::~deOSConsole(){
}



// Management
///////////////

decString deOSConsole::GetPathEngine(){
	const char * const envPath = getenv( "DE_ENGINE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	
	return decString( DE_ENGINE_PATH );
}

decString deOSConsole::GetPathShare(){
	const char * const envPath = getenv( "DE_SHARE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	
	return decString( DE_SHARE_PATH );
}

decString deOSConsole::GetPathSystemConfig(){
	const char * const envPath = getenv( "DE_CONFIG_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	
	return decString( DE_CONFIG_PATH );
}

decString deOSConsole::GetPathUserConfig(){
	const char * const envPath = getenv( "DE_CONFIG_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	
	return decString( pGetHomeDirectory() + "/.config/dragengine" );
}

decString deOSConsole::GetPathUserCache(){
	const char * const envPath = getenv( "DE_CACHE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	
	return decString( pGetHomeDirectory() + "/.cache/dragengine" );
}

decString deOSConsole::GetPathUserCapture(){
	const char * const envPath = getenv( "DE_CAPTURE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	
	return decString( GetPathUserCache() + "/capture" );
}



void deOSConsole::ProcessEventLoop( bool sendToInputModule ){
	// do nothing for the moment
	/*
	see console input module
	*/
}

decString deOSConsole::GetUserLocaleLanguage(){
	const char * const l = setlocale( LC_ALL, nullptr );
	if( l ){
		const decString ls( l );
		const int deli = ls.Find( '_' );
		if( deli != -1 ){
			return ls.GetLeft( deli ).GetLower();
			
		}else{
			return ls.GetLower();
		}
	}
	return "en";
}

decString deOSConsole::GetUserLocaleTerritory(){
	const char * const l = setlocale( LC_ALL, nullptr );
	if( l ){
		const decString ls( l );
		const int deli = ls.Find( '_' );
		if( deli != -1 ){
			const int deli2 = ls.Find( '.', deli + 1 );
			if( deli2 != -1 ){
				return ls.GetMiddle( deli + 1, deli2 ).GetLower();
				
			}else{
				return ls.GetMiddle( deli + 1 ).GetLower();
			}
			
		}else{
			return ls.GetLower();
		}
	}
	return "";
}



// Display information
////////////////////////

int deOSConsole::GetDisplayCount(){
	return 1;
}

decPoint deOSConsole::GetDisplayCurrentResolution( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}

int deOSConsole::GetDisplayCurrentRefreshRate( int display ){
	if( display != 0){
		DETHROW( deeInvalidParam );
	}
	return 30;
}

int deOSConsole::GetDisplayResolutionCount( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return 1;
}

decPoint deOSConsole::GetDisplayResolution( int display, int resolution ){
	if( display != 0 || resolution != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}



// Casting
////////////

deOSConsole *deOSConsole::CastToOSConsole(){
	return this;
}



// Private Functions
//////////////////////

decString deOSConsole::pGetHomeDirectory(){
	// the user configuration directory is located under the user home directory.
	// can be changed at runtime using an environment parameter.
	const char *envPath = getenv( "HOME" );
	if( envPath ){
		return decString( envPath );
	}
	
	envPath = getenv( "USER" );
	if( envPath ){
		return decString( "/home/" ) + envPath;
	}
	
	envPath = getenv( "LOGUSER" );
	if( envPath ){
		return decString( "/home/" ) + envPath;
	}
	
	DETHROW( deeInvalidParam );
}
