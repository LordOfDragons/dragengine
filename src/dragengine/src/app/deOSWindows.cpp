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

#ifdef OS_W32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// required before shlobj.h or SHGetKnownFolderPath is not found
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT _WIN32_WINNT_WIN7

// required before shlobj.h or FOLDERID_* constants are not present
#ifndef INITKNOWNFOLDERS
#define INITKNOWNFOLDERS
#endif

#ifndef INITGUID
#define INITGUID
#endif

#include <shlobj.h>
#include <knownfolders.h>

#include "deOSWindows.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../common/file/decPath.h"
#include "../common/string/unicode/decUnicodeString.h"
#include "../logger/deLogger.h"
#include "../systems/deInputSystem.h"
#include "../systems/modules/input/deBaseInputModule.h"



// Class deOSWindows
//////////////////////

// Constructor, Destructor
////////////////////////////

deOSWindows::deOSWindows() :
pInstApp( NULL ),
pCurWindow( NULL )
{
	pScreenWidth = GetSystemMetrics( SM_CXFULLSCREEN );
	pScreenHeight = GetSystemMetrics( SM_CYFULLSCREEN );
	
	const char *value;
	decPath path;
	
	pPathEngineBase = GetRegistryValue( "SOFTWARE\\Drag[en]gine", "PathEngine", DE_ENGINE_BASE_PATH );
	
	pPathEngine = pPathEngineBase + "\\Data";
	value = getenv( "DE_ENGINE_PATH" );
	if( value ){
		pPathEngine = value;
	}
	pPathEngine = ParseNativePath( pPathEngine );
	
	//pPathShare = GetRegistryValue( "SOFTWARE\\Drag[en]gine", "PathEngineShare", DE_SHARE_PATH );
	pPathShare = pPathEngineBase + "\\Share";
	value = getenv( "DE_SHARE_PATH" );
	if( value ){
		pPathShare = value;
	}
	pPathShare = ParseNativePath( pPathShare );
	
	pPathSystemConfig = pPathEngineBase + "\\Config";
	value = getenv( "DE_CONFIG_PATH" );
	if( value ){
		pPathSystemConfig = value;
	}
	pPathSystemConfig = ParseNativePath( pPathSystemConfig );
	
	pPathUserConfig = "@RoamingAppData\\Dragengine\\Config";
	value = getenv( "DE_CONFIG_PATH" );
	if( value ){
		pPathUserConfig = value;
	}
	pPathUserConfig = ParseNativePath( pPathUserConfig );
	
	pPathUserCache = "@LocalAppData\\Dragengine\\Cache";
	value = getenv( "DE_CACHE_PATH" );
	if( value ){
		pPathUserCache = value;
	}
	pPathUserCache = ParseNativePath( pPathUserCache );
	
	pPathUserCapture = "@LocalAppData\\Dragengine\\Capture";
	value = getenv( "DE_CAPTURE_PATH" );
	if( value ){
		pPathUserCapture = value;
	}
	pPathUserCapture = ParseNativePath( pPathUserCapture );
}

deOSWindows::~deOSWindows(){
	pCleanUp();
}



// Management
///////////////

decString deOSWindows::GetPathEngine(){
	return pPathEngine;
}

decString deOSWindows::GetPathShare(){
	return pPathShare;
}

decString deOSWindows::GetPathSystemConfig(){
	return pPathSystemConfig;
}

decString deOSWindows::GetPathUserConfig(){
	return pPathUserConfig;
}

decString deOSWindows::GetPathUserCache(){
	return pPathUserCache;
}

decString deOSWindows::GetPathUserCapture(){
	return pPathUserCapture;
}



void deOSWindows::ProcessEventLoop( bool sendToInputModule ){
	deBaseInputModule &inputModule = *GetEngine()->GetInputSystem()->GetActiveModule();
	MSG message;
	
	while( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) ){
		// if( pCurWindow && message.hwnd == pCurWindow ){
		switch( message.message ){
		case WM_QUIT:
			GetEngine()->Quit();
			break;
			
		case WM_ACTIVATEAPP:
			SetAppActive( message.wParam == TRUE );
			DispatchMessage( &message );
			break;
			
// 		case WM_ACTIVATE:
// 			SetAppActive( LOWORD( message.wParam ) == TRUE );
// 			break;
			
		case WM_SIZE:
			//const int windowWidth = LOWORD( message.lParam );
			//const int windowHeight = HIWORD( message.wParam );
			
			// notify graphic module about the change
			// notify the script module about the change
			
			break;
		
		default:
			//TranslateMessage( &message );
			DispatchMessage( &message );
		}
		
		if( sendToInputModule ){
			inputModule.EventLoop( message );
		}
    }
}



// Display information
////////////////////////

int deOSWindows::GetDisplayCount(){
	return 1;
}

decPoint deOSWindows::GetDisplayCurrentResolution( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}

int deOSWindows::GetDisplayCurrentRefreshRate( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return 30;
}

int deOSWindows::GetDisplayResolutionCount( int display ){
	if( display != 0 ){
		DETHROW( deeInvalidParam );
	}
	return 1;
}

decPoint deOSWindows::GetDisplayResolution( int display, int resolution ){
	if( display != 0 || resolution != 0 ){
		DETHROW( deeInvalidParam );
	}
	return decPoint( pScreenWidth, pScreenHeight );
}



// Casting
////////////

deOSWindows *deOSWindows::CastToOSWindows(){
	return this;
}



// windows accessors
//////////////////////



// Windows specific
/////////////////////

void deOSWindows::SetInstApp( HINSTANCE hInstApp ){
	pInstApp = hInstApp;
}

void deOSWindows::SetWindow( HWND window ){
//	RECT windowRect;
	// check if the is a window at the moment
	//if( pCurWindow ){
		//ReleaseCapture();
	//}
	
	// set new window
	pCurWindow = window;
	// show new window if not null
	if( pCurWindow ){
		ShowWindow( pCurWindow, SW_SHOW );
//		GetClientRect( pCurWindow, &windowRect );
//		pCurWindowWidth = windowRect.right - windowRect.left;
//		pCurWindowHeight = windowRect.bottom - windowRect.top;
	}
}



decString deOSWindows::ParseNativePath( const char *path ){
	const decString spath( path );
	
	if( spath.IsEmpty() ){
		return spath;
	}
	
	if( spath.GetAt( 0 ) != '@' ){
		return spath;
	}
	
	int index = spath.Find( '\\' );
	if( index == -1 ){
		index = spath.GetLength();
	}
	
	const decString special( spath.GetMiddle( 1, index ) );
	GUID nFolder;
	
	if( special == "ProgramFiles" ){
		nFolder = FOLDERID_ProgramFiles;
		
	}else if( special == "System" ){
		nFolder = FOLDERID_System;
		
	}else if( special == "RoamingAppData" ){
		nFolder = FOLDERID_RoamingAppData;
		
	}else if( special == "ProgramData" ){
		nFolder = FOLDERID_ProgramData;
		
	}else if( special == "Public" ){
		nFolder = FOLDERID_Public;
		
	}else if( special == "PublicDocuments" ){
		nFolder = FOLDERID_PublicDocuments;
		
	}else if( special == "PublicGameTasks" ){
		nFolder = FOLDERID_PublicGameTasks;
		
	}else if( special == "LocalAppData" ){
		nFolder = FOLDERID_LocalAppData;
		
	}else if( special == "Documents" ){
		nFolder = FOLDERID_Documents;
		
	}else if( special == "Windows" ){
		nFolder = FOLDERID_Windows;
		
	}else{
		return spath;
	}
	
	PWCHAR folderPath = NULL;
	if( SHGetKnownFolderPath( nFolder, 0, NULL, &folderPath ) != S_OK ){
		DETHROW( deeInvalidParam );
	}
	
	const int folderPathLen = wcslen( folderPath );
	decUnicodeString ustrFolderPath;
	int i;
	
	for( i=0; i<folderPathLen; i++ ){
		ustrFolderPath.AppendCharacter( folderPath[ i ] );
	}
	
	return ustrFolderPath.ToUTF8() + spath.GetMiddle( index );
}

void deOSWindows::Utf8ToWide( const char *utf8, wchar_t *wide, int maxSize ){
	UnicodeToWide( decUnicodeString::NewFromUTF8( utf8 ), wide, maxSize );
}

void deOSWindows::UnicodeToWide( const decUnicodeString &unicode, wchar_t *wide, int maxSize ){
	const int count = unicode.GetLength();
	int i;
	
	if( count >= maxSize ){
		DETHROW( deeInvalidParam );
	}
	
	for( i=0; i<count; i++ ){
		wide[ i ] = unicode[ i ];
	}
	wide[ i ] = 0;
}

decString deOSWindows::WideToUtf8( const wchar_t *wide ){
	return WideToUnicode( wide ).ToUTF8();
}

decUnicodeString deOSWindows::WideToUnicode( const wchar_t *wide ){
	const int count = wcslen( wide );
	decUnicodeString unicode;
	int i;
	
	unicode.Set( 0, count );
	for( i=0; i<count; i++ ){
		unicode.SetAt( i, wide[ i ] );
	}
	
	return unicode;
}

decString deOSWindows::GetRegistryValue( const char *key, const char *entry, const char *defaultValue ){
	HKEY hKey;
	if( RegOpenKeyExA( HKEY_LOCAL_MACHINE, key, 0, KEY_READ, &hKey ) != ERROR_SUCCESS ){
		return defaultValue;
	}
	
	DWORD bufferSize = 0;
	if( RegQueryValueExA( hKey, entry, 0, NULL, NULL, &bufferSize ) != ERROR_SUCCESS ){
		RegCloseKey( hKey );
		return defaultValue;
	}
	
	CHAR * const buffer = new CHAR[ bufferSize ];
	if( RegQueryValueExA( hKey, entry, 0, NULL, ( LPBYTE )buffer, &bufferSize ) != ERROR_SUCCESS ){
		delete [] buffer;
		RegCloseKey( hKey );
		return defaultValue;
	}
	
	const decString returnValue( buffer );
	delete [] buffer;
	RegCloseKey( hKey );
	
	return returnValue;
}

void deOSWindows::SetRegistryValue( const char *key, const char *entry, const char *value ){
	HKEY hKey;
	if( RegCreateKeyExA( HKEY_LOCAL_MACHINE, key, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL )
	!= ERROR_SUCCESS ){
		DETHROW( deeInvalidAction );
	}
	
	if( RegSetValueExA( hKey, entry, 0, REG_SZ, ( BYTE* )value, strlen( value ) ) != ERROR_SUCCESS ){
		RegCloseKey( hKey );
		DETHROW( deeInvalidAction );
	}
	
	RegCloseKey( hKey );
}



// Private Functions
//////////////////////

void deOSWindows::pCleanUp(){
}

#endif
