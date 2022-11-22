/* 
 * Drag[en]gine Library -- Game Engine
 *
 * Copyright (C) 2015, Plüss Roland ( roland@rptd.ch )
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

#ifdef OS_MACOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <locale.h>
#include <sys/time.h>

#include "deOSMacOS.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../systems/deInputSystem.h"
#include "../systems/modules/input/deBaseInputModule.h"

#import <Cocoa/Cocoa.h>



// Class deOSMacOS
////////////////////

// Constructor, Destructor
////////////////////////////

deOSMacOS::deOSMacOS() :
pScreenWidth( 0 ),
pScreenHeight( 0 ),
pRefreshRate( 0 ),
pCurWindow( NULL ),
pHostingMainWindow( NULL ),
pHostingRenderWindow( NULL )
{
	pScreenWidth = 1024;
	pScreenHeight = 768;
	pRefreshRate = 30;
	
	// init locale
	setlocale( LC_ALL, "" );
}

deOSMacOS::~deOSMacOS(){
	pCleanUp();
}



// Management
///////////////

decString deOSMacOS::GetPathEngine(){
	const char * const envPath = getenv( "DE_ENGINE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( DE_ENGINE_PATH );
}

decString deOSMacOS::GetPathShare(){
	const char * const envPath = getenv( "DE_SHARE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( DE_SHARE_PATH );
}

decString deOSMacOS::GetPathSystemConfig(){
	const char * const envPath = getenv( "DE_CONFIG_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( DE_CONFIG_PATH );
}

decString deOSMacOS::GetPathUserConfig(){
	const char * const envPath = getenv( "DE_CONFIG_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( pGetHomeDirectory() + "/config/settings/dragengine" );
}

decString deOSMacOS::GetPathUserCache(){
	const char * const envPath = getenv( "DE_CACHE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( pGetHomeDirectory() + "/config/cache/dragengine" );
}

decString deOSMacOS::GetPathUserCapture(){
	const char * const envPath = getenv( "DE_CAPTURE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( GetPathUserCache() + "/config/capture" );
}



// Display information
////////////////////////

int deOSMacOS::GetDisplayCount(){
	return 1;
}

decPoint deOSMacOS::GetDisplayCurrentResolution( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}

int deOSMacOS::GetDisplayCurrentRefreshRate( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return pRefreshRate;
}

int deOSMacOS::GetDisplayResolutionCount( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return 1;
}

decPoint deOSMacOS::GetDisplayResolution( int display, int resolution ){
	if( display != 0 || resolution != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}



// Casting
////////////

deOSMacOS *deOSMacOS::CastToOSMacOS(){
	return this;
}



// MacOS related
/////////////////

void deOSMacOS::SetWindow( NSWindow *window ){
	// check if there is a window at the moment
	if( pCurWindow ){
		// ungrab mouse pointer
	}
	
	// set new window
	pCurWindow = window;
	
	// show new window if not null
	if( pCurWindow ){
		//pCurWindow->Show();
	}
}

void deOSMacOS::SetHostingMainWindow( NSWindow *window ){
	pHostingMainWindow = window;
}

void deOSMacOS::SetHostingRenderWindow( NSWindow *window ){
	pHostingRenderWindow = window;
}

bool deOSMacOS::HasHostingMainWindow() const{
	return pHostingMainWindow != NULL;
}

bool deOSMacOS::HasHostingRenderWindow() const{
	return pHostingRenderWindow != NULL;
}



void deOSMacOS::ProcessEventLoop( bool sendToInputModule ){
	// uses Obj-C++ code to interface with Cocoa
	deBaseInputModule &inputModule = *GetEngine()->GetInputSystem()->GetActiveModule();
	
	while( true ){
		NSAutoreleasePool * const pool = [[NSAutoreleasePool alloc] init];
		NSEvent * const event = [NSApp nextEventMatchingMask:NSUIntegerMax untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES];
		
		if( event ){
			if( sendToInputModule ){
				inputModule.EventLoop( *event );
			}
			[NSApp sendEvent:event];
			[NSApp updateWindows];
			[pool release];
			
		}else{
			[pool release];
			break;
		}
	}
}

decString deOSMacOS::GetUserLocaleLanguage(){
	const char * const l = setlocale( LC_ALL, nullptr );
	if( l ){
		const decString ls( l );
		const int deli = ls.Find( '_' );
		if( deli != -1 ){
			return ls.GetLeft( deli ).GetLower();
		}
	}
	return "en";
}

decString deOSMacOS::GetUserLocaleTerritory(){
	const char * const l = setlocale( LC_ALL, nullptr );
	if( l ){
		const decString ls( l );
		const int deli = ls.Find( '_' );
		if( deli != -1 ){
			const int deli2 = ls.Find( '.', deli + 1 );
			if( deli2 != -1 ){
				return ls.GetMiddle( deli + 1, deli2 ).GetLower();
			}
		}
	}
	return "";
}



// Private Functions
//////////////////////

void deOSMacOS::pCleanUp(){
}

decString deOSMacOS::pGetHomeDirectory(){
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

#endif
