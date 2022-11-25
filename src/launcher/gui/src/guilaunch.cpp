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

#include "guilaunch.h"
#include "gui/deglWindowMain.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/exceptions.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Entry point
////////////////

int main( int argc, char **argv ){
#ifdef OS_UNIX
	struct sigaction act;
	memset( &act, '\0', sizeof( act ) );
	act.sa_handler = SIG_IGN;
	sigaction( SIGPIPE, &act, NULL );
#endif
	
#ifdef OS_W32
	char **foxArgs = NULL;
	int foxArgCount = 0;
	
	// silence certification tool
	(void)SetProcessDpiAwarenessContext( DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 );
#endif
	
	int returnValue = 0;
	
	try{
#ifdef OS_W32
		// WARNING FOX expects the first parameter to be present. stripping it causes segfaults!
		// 
		// quick note on why we use GetCommandLineW() here and not the value from wWinMain.
		// MinGW does not support wWinMain so it has to be faked by implementing WinMain which
		// calls GetCommandLineW(). This can be unfortunatly not fed directly to wWinMain
		// because GetCommandLineW() includes the executable name as first argument whereas
		// CRT calls wWinMain without the executable name as first argument. This incompatibility
		// makes it hard to do this properly. Instead this class simply ignores the wWinMain
		// provided command line value and fetches GetCommandLineW. ugly but it works
		// 
		// WARNING FOX expects arguments to live for the entire lifetime of the application!
		//         Using temporary strings causes segfaults!
		
		decUnicodeArgumentList parseArguments;
		parseArguments.ParseCommand( deOSWindows::WideToUnicode( GetCommandLineW() ) );
		
		foxArgCount = parseArguments.GetArgumentCount();
		foxArgs = new char*[ foxArgCount + 1 ]; // workaround: fox seems to write past the buffer
		
		int i;
		for( i=0; i<foxArgCount; i++ ){
			const decString argument( parseArguments.GetArgumentAt( i )->ToUTF8() );
			foxArgs[ i ] = new char[ argument.GetLength() + 1 ];
			strcpy( foxArgs[ i ], argument.GetString() );
		}
		
		argc = foxArgCount;
		argv = foxArgs;
#endif
		
		FXApp app( "Drag[en]gine Launcher", "Drag[en]gine" );
		app.init( argc, argv );
		
		deglWindowMain * const windowMain = new deglWindowMain( &app, argc, argv );
		
		app.create();
		if( windowMain->RunCommandLineActions() ){
			app.run();
		}
		
	}catch( const deException &e ){
		e.PrintError();
		returnValue = -1;
	}
	
#ifdef OS_W32
	if( foxArgs ){
		int i;
		for( i=0; i<foxArgCount; i++ ){
			delete [] foxArgs[ i ];
		}
		delete [] foxArgs;
	}
#endif
	
	return returnValue;
}
