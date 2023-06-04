/* 
 * Drag[en]gine Live Launcher
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

#include "dellLauncher.h"

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#endif

#ifdef OS_UNIX
#include <unistd.h>
#endif

#ifdef OS_BEOS
#include <app/Application.h>
#endif

#include <dragengine/deEngine.h>
#include <dragengine/app/deCmdLineArgs.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// beos entry point
/////////////////////

#ifdef OS_BEOS

#include <dragengine/app/deOSBeOS.h>

class deBApp : public BApplication{
private:
	dellLauncher pLauncher;
	thread_id pThread;
	
public:
	deBApp() :
	BApplication( "application/x-vnd.dragengine-launcher-console" ),
	pThread( -1 ){
	}
	
	virtual void ArgvReceived( int32 argc, char** argv ){
		int i;
		for( i=1; i<argc; i++ ){
			printf( "[LAUNCHER] ArgvReceived: %d = '%s'\n", i, argv[ i ] );
			pLauncher.AddArgument( decUnicodeString::NewFromUTF8( argv[ i ] ) );
		}
	}
	
	virtual void AboutRequested(){
		printf( "[LAUNCHER] AboutRequested()\n" );
	}
	
	virtual bool QuitRequested(){
		printf( "[LAUNCHER] QuitRequested()\n" );
		return true;
	}
	
	virtual void ReadyToRun(){
		printf( "[LAUNCHER] ReadyToRun()\n" );
		if( pThread != -1 ){
			return;
		}
		pThread = spawn_thread( ThreadHandler, "Runner", B_NORMAL_PRIORITY, this );
		resume_thread( pThread );
	}
	
	virtual void MessageReceived( BMessage *message ){
		if( pLauncher.runningGame && pLauncher.runningGame->GetEngineInstance() ){
			pLauncher.runningGame->GetEngineInstance()->BeosMessageReceived( message );
		}
	}
	
	virtual thread_id Run(){
		const thread_id tid = BApplication::Run();
		
		if( pThread != -1 ){
			kill_thread( pThread );
			status_t status;
			wait_for_thread( pThread, &status );
			pThread = -1;
		}
		
		return tid;
	}
	
	static int32 ThreadHandler( void *userData ){
		deBApp &app = *( ( deBApp* )userData );
		
		try{
			app.pLauncher.Run();
			
		}catch( const deException &e ){
			e.PrintError();
		}
		
		app.PostMessage( B_QUIT_REQUESTED );
		return 0;
	}
};

int main( int argcount, char **args ){
	deBApp app;
	app.Run();
	printf( "Quit finished\n" );
	return 0;
}

#endif



// unix entry point
/////////////////////

#if defined OS_UNIX && ! defined OS_BEOS && ! defined OS_MACOS
int main( int argcount, char **args ){
	try{
		dellLauncher launcher;
		
		int i;
		for( i=1; i<argcount; i++ ){
			launcher.AddArgument( decUnicodeString::NewFromUTF8( args[ i ] ) );
		}
		
		launcher.Run();
		
	}catch( const deException &e ){
		e.PrintError();
		return -1;
	}
	
	return 0;
}
#endif



// windows entry point
////////////////////////

#ifdef OS_W32
static int WINAPI RealWinMain(){
	try{
		decUnicodeArgumentList argsList;
		argsList.ParseCommand( deOSWindows::WideToUnicode( GetCommandLineW() ) );
		
		dellLauncher launcher;
		int i;
		for( i=1; i<argsList.GetArgumentCount(); i++ ){
			launcher.AddArgument( *argsList.GetArgumentAt( i ) );
		}
		
		launcher.Run();
		
	}catch( const deException &e ){
		e.PrintError();
		return -1;
	}
	
	return 0;
}

// MinGW does not support wWinMain
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){
	return RealWinMain();
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow ){
	return RealWinMain();
}
#endif



// MacOS entry point
//////////////////////

#ifdef OS_MACOS
// main() entry point is located in consolelauncher_macos.mm and forwards to macosMain
int macosMain( int argcount, char **args ){
	try{
		dellLauncher launcher;
		
		int i;
		for( i=1; i<argcount; i++ ){
			launcher.AddArgument( decUnicodeString::NewFromUTF8( args[ i ] ) );
		}
		
		launcher.Run();
		
	}catch( const deException &e ){
		e.PrintError();
		return -1;
	}
	
	return 0;
}
#endif
