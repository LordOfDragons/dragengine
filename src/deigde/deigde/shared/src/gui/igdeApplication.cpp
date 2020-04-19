/* 
 * Drag[en]gine IGDE
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

#include "igdeApplication.h"
#include "igdeMainWindow.h"
#include "native/toolkit.h"
#include "native/fox/igdeNativeFoxApplication.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeStringList.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>

#ifdef OS_W32
#include <dragengine/app/deOSWindows.h>
#endif



// Static
///////////

igdeApplication *igdeApplication::pApp = NULL;



// Class igdeApplication
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeApplication::igdeApplication() :
pNativeApplication( NULL )
{
	if( pApp ){
		DETHROW( deeInvalidParam );
	}
	
	pApp = this;
	
	pNativeApplication = new igdeNativeFoxApplication( *this );
}

igdeApplication::~igdeApplication(){
	if( pNativeApplication ){
		delete ( igdeNativeFoxApplication* )pNativeApplication;
	}
	
	if( pApp == this ){
		pApp = NULL;
	}
}



// Management
///////////////

igdeMainWindow *igdeApplication::GetMainWindow() const{
	return ( igdeMainWindow* )( igdeWidget* )pMainWindow;
}

#ifdef OS_UNIX
void igdeApplication::Run( int argCount, char **args ){
	// WARNING FOX expects the first parameter to be present. stripping it causes segfaults!
	decUnicodeStringList arguments;
	int i;
	for( i=0; i<argCount; i++ ){
		arguments.Add( decUnicodeString::NewFromUTF8( args[ i ] ) );
	}
	
	pSharedRun( arguments );
}

#elif defined OS_W32
void igdeApplication::Run(){
// 	printf("igdeApplication %d: WidgetCount %d\n", __LINE__, igdeUIFoxHelper::DebugCountWindows(NULL));
	
// 	try{
	// WARNING FOX expects the first parameter to be present. stripping it causes segfaults!
	// 
	// quick note on why we use GetCommandLineW() here and not the value from wWinMain.
	// MinGW does not support wWinMain so it has to be faked by implementing WinMain which
	// calls GetCommandLineW(). This can be unfortunatly not fed directly to wWinMain
	// because GetCommandLineW() includes the executable name as first argument whereas
	// CRT calls wWinMain without the executable name as first argument. This incompatibility
	// makes it hard to do this properly. Instead this class simply ignores the wWinMain
	// provided command line value and fetches GetCommandLineW. ugly but it works
	decUnicodeArgumentList parseArguments;
	parseArguments.ParseCommand( deOSWindows::WideToUnicode( GetCommandLineW() ) );
	
	const int count = parseArguments.GetArgumentCount();
	decUnicodeStringList arguments;
	int i;
	for( i=0; i<count; i++ ){
		arguments.Add( *parseArguments.GetArgumentAt( i ) );
	}
	
	pSharedRun( arguments );
// 	}catch(...){
// 		printf("igdeApplication %d: WidgetCount %d\n", __LINE__, igdeUIFoxHelper::DebugCountWindows(NULL));
// 		throw;
// 	}
// 	printf("igdeApplication %d: WidgetCount %d\n", __LINE__, igdeUIFoxHelper::DebugCountWindows(NULL));
}

#else
#error "Unsupported OS"
#endif

decColor igdeApplication::GetSystemColor( igdeEnvironment::eSystemColors color ) const{
	if( ! pNativeApplication ){
		DETHROW( deeInvalidParam );
	}
	return ( ( igdeNativeFoxApplication* )pNativeApplication )->GetSystemColor( color );
}

void igdeApplication::GetAppFontConfig( igdeFont::sConfiguration &config ){
	if( ! pNativeApplication ){
		DETHROW( deeInvalidParam );
	}
	( ( igdeNativeFoxApplication* )pNativeApplication )->GetAppFontConfig( config );
}

igdeApplication &igdeApplication::app(){
	if( ! pApp ){
		DETHROW( deeInvalidParam );
	}
	return *pApp;
}

void igdeApplication::RunModalWhileShown( igdeWindow &window ){
	if( ! pNativeApplication ){
		DETHROW( deeInvalidParam );
	}
	( ( igdeNativeFoxApplication* )pNativeApplication )->RunModalWhileShown( window );
}



// Protected Functions
////////////////////////

void igdeApplication::SetMainWindow( igdeMainWindow *mainWindow, bool takeOver ){
	if( takeOver ){
		pMainWindow.TakeOver( mainWindow );
		
	}else{
		pMainWindow = mainWindow;
	}
}

void igdeApplication::CleanUp(){
}



// Private Functions
//////////////////////

void igdeApplication::pSharedRun( decUnicodeStringList &arguments ){
	( ( igdeNativeFoxApplication* )pNativeApplication )->Initialize( arguments );
	
	try{
		if( Initialize( arguments ) ){
			( ( igdeNativeFoxApplication* )pNativeApplication )->Run();
		}
		
	}catch( const deException &e ){
		e.PrintError();
		( ( igdeNativeFoxApplication* )pNativeApplication )->ShowError( e );
		
		try{
			CleanUp();
			pMainWindow = NULL;
			
		}catch( const deException &e ){
			e.PrintError();
			( ( igdeNativeFoxApplication* )pNativeApplication )->ShowError( e );
		}
		
		( ( igdeNativeFoxApplication* )pNativeApplication )->Quit();
		throw;
	}
	
	CleanUp();
	pMainWindow = NULL;
	
	( ( igdeNativeFoxApplication* )pNativeApplication )->Quit();
}
