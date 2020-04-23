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

#ifdef OS_BEOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include <Application.h>
#include <MessageFilter.h>
#include <MessageQueue.h>
#include <Roster.h>
#include <Screen.h>
#include <Window.h>

#include "deOSBeOS.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../systems/deInputSystem.h"
#include "../systems/modules/input/deBaseInputModule.h"



// Class deOSBeOS
///////////////////

// Constructor, Destructor
////////////////////////////

deOSBeOS::deOSBeOS() :
pScreenWidth( 0 ),
pScreenHeight( 0 ),
pCurWindow( NULL ),
pHostingMainWindow( NULL ),
pHostingRenderWindow( NULL )
{
	const BRect screenSize( BScreen().Frame() );
	pScreenWidth = screenSize.IntegerWidth();
	pScreenHeight = screenSize.IntegerHeight();
}

deOSBeOS::~deOSBeOS(){
	pCleanUp();
}



// Management
///////////////

decString deOSBeOS::GetPathEngine(){
	const char * const envPath = getenv( "DE_ENGINE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( DE_ENGINE_PATH );
}

decString deOSBeOS::GetPathShare(){
	const char * const envPath = getenv( "DE_SHARE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( DE_SHARE_PATH );
}

decString deOSBeOS::GetPathSystemConfig(){
	const char * const envPath = getenv( "DE_CONFIG_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( DE_CONFIG_PATH );
}

decString deOSBeOS::GetPathUserConfig(){
	const char * const envPath = getenv( "DE_CONFIG_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( pGetHomeDirectory() + "/config/settings/dragengine" );
}

decString deOSBeOS::GetPathUserCache(){
	const char * const envPath = getenv( "DE_CACHE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( pGetHomeDirectory() + "/config/cache/dragengine" );
}

decString deOSBeOS::GetPathUserCapture(){
	const char * const envPath = getenv( "DE_CAPTURE_PATH" );
	if( envPath ){
		return decString( envPath );
	}
	return decString( GetPathUserCache() + "/config/capture" );
}



// Display information
////////////////////////

int deOSBeOS::GetDisplayCount(){
	return 1;
}

decPoint deOSBeOS::GetDisplayCurrentResolution( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}

int deOSBeOS::GetDisplayCurrentRefreshRate( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return 30;
}

int deOSBeOS::GetDisplayResolutionCount( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return 1;
}

decPoint deOSBeOS::GetDisplayResolution( int display, int resolution ){
	if( display != 0 || resolution != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}



// Casting
////////////

deOSBeOS *deOSBeOS::CastToOSBeOS(){
	return this;
}



// Unix related
/////////////////

void deOSBeOS::SetWindow( BWindow *window ){
	// check if there is a window at the moment
	if( pCurWindow ){
		// ungrab mouse pointer
	}
	
	// set new window
	pCurWindow = window;
	
	// show new window if not null
	if( pCurWindow ){
		pCurWindow->Show();
	}
}

void deOSBeOS::SetHostingMainWindow( BWindow *window ){
	pHostingMainWindow = window;
}

void deOSBeOS::SetHostingRenderWindow( BWindow *window ){
	pHostingRenderWindow = window;
}

bool deOSBeOS::HasHostingMainWindow() const{
	return pHostingMainWindow != NULL;
}

bool deOSBeOS::HasHostingRenderWindow() const{
	return pHostingRenderWindow != NULL;
}



void deOSBeOS::ProcessEventLoop( bool sendToInputModule ){
	BMessage *message = NULL;
	while( true ){
		// get next message
		pMessageQueue.Lock();
		message = pMessageQueue.NextMessage();
		pMessageQueue.Unlock();
		if( ! message ){
			break; // finished
		}
		
		/*const char whatStr[4] = {(char)((message->what>>24)&0xff), (char)((message->what>>16)&0xff),
			(char)((message->what>>8)&0xff), (char)(message->what&0xff)};
		printf( "OS.ProcessEventLoop: procees %.4s\n", (char*)whatStr );*/
		
		// process message
		try{
			switch( message->what ){
			case B_APP_ACTIVATED:
				SetAppActive( message->GetBool( "active", true ) );
				break;
				
			case B_QUIT_REQUESTED:
				GetEngine()->Quit();
				break;
				
			default:
				break;
			}
			
			if( sendToInputModule ){
				GetEngine()->GetInputSystem()->GetActiveModule()->EventLoop( *message );
			}
			
		}catch( const deException &e ){
			if( message ){
				delete message;
			}
			throw;
		}
		
		delete message;
		message = NULL;
	}
}

void deOSBeOS::MessageReceived( BMessage *message ){
	pMessageQueue.Lock();
	pMessageQueue.AddMessage( new BMessage( *message ) );
	pMessageQueue.Unlock();
}



// Private Functions
//////////////////////

void deOSBeOS::pCleanUp(){
}

decString deOSBeOS::pGetHomeDirectory(){
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
