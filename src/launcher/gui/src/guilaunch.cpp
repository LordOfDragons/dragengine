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

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif

#include "guilaunch.h"
#include "gui/deglWindowMain.h"

#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/exceptions.h>



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
	
	(void)SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
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
			const int size = argument.GetLength();
			foxArgs[ i ] = new char[ size + 1 ];
			#ifdef OS_W32_VS
				strcpy_s( foxArgs[ i ], size + 1, argument.GetString() );
			#else
				strcpy( foxArgs[ i ], argument.GetString() );
			#endif
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


// visual studio does not support main as entry point
#ifdef OS_W32_VS
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow ){
	int nArgs;
	LPWSTR * const szArglist = CommandLineToArgvW( GetCommandLineW(), &nArgs );
	if( ! szArglist ){
		wprintf( L"CommandLineToArgvW failed\n" );
		return 0;
	}
	
	const int result = main( nArgs, ( char** )szArglist );

	LocalFree( szArglist );

	return result;
}
#endif
