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

#include "../dragengine_configuration.h"

#ifdef OS_W32

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef OS_W32_VS
#include <unistd.h>
#endif

#ifndef OS_W32_VS
	// required before shlobj.h or SHGetKnownFolderPath is not found
	#ifdef _WIN32_WINNT
	#undef _WIN32_WINNT
	#endif
	#define _WIN32_WINNT _WIN32_WINNT_WIN7
#endif


// required before shlobj.h or FOLDERID_* constants are not present
#ifndef INITKNOWNFOLDERS
#define INITKNOWNFOLDERS
#endif

#ifndef INITGUID
#define INITGUID
#endif

#include <shlobj.h>
#include <knownfolders.h>
#include <ShellScalingApi.h>

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
pInstApp( nullptr ),
pCurWindow( nullptr ),
pResolutionCount( 0 ),
pResolutions( nullptr ),
pRefreshRate( 60 ),
pScaleFactor( 100 )
{
	// this is unfortunately returning too small values
	//pScreenWidth = GetSystemMetrics( SM_CXFULLSCREEN );
	//pScreenHeight = GetSystemMetrics( SM_CYFULLSCREEN );

	RECT rect;
	DEASSERT_TRUE( SystemParametersInfoA( SPI_GETWORKAREA, 0, &rect, 0 ) )
	pScreenWidth = rect.right - rect.left;
	pScreenHeight = rect.bottom - rect.top;
	
	pFindResolutions();
	pFindRefreshRate();
	pFindScaleFactor();

#ifndef OS_W32_APPSTORE
#ifdef OS_W32
	const int valueSize = 256;
	TCHAR value[valueSize] = {};
#else
	const char *value;
#endif
#endif
	decPath path;
	
#ifdef OS_W32_APPSTORE
	pPathEngineBase = GetRegistryValue( "SOFTWARE\\Drag[en]gine", "PathEngine", "" );
	if( pPathEngineBase.IsEmpty() ){
		DETHROW_INFO( deeInvalidParam, "PathEngine registry value is not set" );
	}
#else
	pPathEngineBase = GetRegistryValue( "SOFTWARE\\Drag[en]gine", "PathEngine", DE_ENGINE_BASE_PATH );
#endif
	
	pPathEngine = pPathEngineBase + "\\Data";
#ifndef OS_W32_APPSTORE
#ifdef OS_W32
	if( GetEnvironmentVariable( L"DE_ENGINE_PATH", &value[ 0 ], valueSize ) > 0 ){
		pPathEngine = WideToUtf8( value );
	}
#else
	value = getenv( "DE_ENGINE_PATH" );
	if( value ){
		pPathEngine = value;
	}
#endif
#endif
	pPathEngine = ParseNativePath( pPathEngine );
	
	//pPathShare = GetRegistryValue( "SOFTWARE\\Drag[en]gine", "PathEngineShare", DE_SHARE_PATH );
	pPathShare = pPathEngineBase + "\\Share";
#ifndef OS_W32_APPSTORE
#ifdef OS_W32
	if( GetEnvironmentVariable( L"DE_SHARE_PATH", &value[ 0 ], valueSize ) > 0 ){
		pPathShare = WideToUtf8( value );
	}
#else
	value = getenv( "DE_SHARE_PATH" );
	if( value ){
		pPathShare = value;
	}
#endif
#endif
	pPathShare = ParseNativePath( pPathShare );
	
	pPathSystemConfig = pPathEngineBase + "\\Config";
#ifndef OS_W32_APPSTORE
#ifdef OS_W32
	if( GetEnvironmentVariable( L"DE_CONFIG_PATH", &value[ 0 ], valueSize ) > 0 ){
		pPathSystemConfig = WideToUtf8( value );
	}
#else
	value = getenv( "DE_CONFIG_PATH" );
	if( value ){
		pPathSystemConfig = value;
	}
#endif
#endif
	pPathSystemConfig = ParseNativePath( pPathSystemConfig );
	
	pPathUserConfig = "@RoamingAppData\\Dragengine\\Config";
#ifndef OS_W32_APPSTORE
#ifdef OS_W32
	if( GetEnvironmentVariable( L"DE_CONFIG_PATH", &value[ 0 ], valueSize ) > 0 ){
		pPathUserConfig = WideToUtf8( value );
	}
#else
	value = getenv( "DE_CONFIG_PATH" );
	if( value ){
		pPathUserConfig = value;
	}
#endif
#endif
	pPathUserConfig = ParseNativePath( pPathUserConfig );
	
	pPathUserCache = "@LocalAppData\\Dragengine\\Cache";
#ifndef OS_W32_APPSTORE
#ifdef OS_W32
	if( GetEnvironmentVariable( L"DE_CACHE_PATH", &value[ 0 ], valueSize ) > 0 ){
		pPathUserCache = WideToUtf8( value );
	}
#else
	value = getenv( "DE_CACHE_PATH" );
	if( value ){
		pPathUserCache = value;
	}
#endif
#endif
	pPathUserCache = ParseNativePath( pPathUserCache );
	
	pPathUserCapture = "@LocalAppData\\Dragengine\\Capture";
#ifndef OS_W32_APPSTORE
#ifdef OS_W32
	if( GetEnvironmentVariable( L"DE_CAPTURE_PATH", &value[ 0 ], valueSize ) > 0 ){
		pPathUserCapture = WideToUtf8( value );
	}
#else
	value = getenv( "DE_CAPTURE_PATH" );
	if( value ){
		pPathUserCapture = value;
	}
#endif
#endif
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

decString deOSWindows::GetUserLocaleLanguage(){
	const decString language( pGetUserLanguage() );
	const int deli = language.Find( '-' );
	if( deli != -1 ){
		return language.GetLeft( deli ).GetLower();
		
	}else{
		return language.GetLower();
	}
	return "en";
}

decString deOSWindows::GetUserLocaleTerritory(){
	const decString language( pGetUserLanguage() );
	const int deli = language.Find( '-' );
	if( deli != -1 ){
		const int deli2 = language.Find( '-', deli + 1 );
		if( deli2 != -1 ){
			return language.GetMiddle( deli + 1, deli2 ).GetLower();
			
		}else{
			return language.GetMiddle( deli + 1 ).GetLower();
		}
		
	}else{
		return language.GetLower();
	}
	return "";
}



// Display information
////////////////////////

int deOSWindows::GetDisplayCount(){
	return 1;
}

decPoint deOSWindows::GetDisplayCurrentResolution( int display ){
	DEASSERT_TRUE( display == 0 )
	
	return decPoint( pScreenWidth, pScreenHeight );
}

int deOSWindows::GetDisplayCurrentRefreshRate( int display ){
	DEASSERT_TRUE( display == 0 )
	
	return pRefreshRate;
}

int deOSWindows::GetDisplayResolutionCount( int display ){
	DEASSERT_TRUE( display == 0 )
	
	return pResolutionCount;
}

decPoint deOSWindows::GetDisplayResolution( int display, int resolution ){
	DEASSERT_TRUE( display == 0 )
	DEASSERT_TRUE( resolution >= 0 )
	DEASSERT_TRUE( resolution < pResolutionCount )
	
	return pResolutions[ resolution ];
}

int deOSWindows::GetDisplayCurrentScaleFactor( int display ){
	DEASSERT_TRUE( display == 0 )
	
	return pScaleFactor;
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
	
	// windows causes troubles with path resolving if used inside packaged application
	// which means store application. certain path are redirected and writing to them
	// does not always seem to yield the same path as reading them. this can cause
	// accessing directories to not work since the path is suddenly somewhere else.
	// using flags it is possible to disable redirecting
	// 
	// furthermore windows deletes certain redirected (packaged) directories if the
	// game engine is uninstalled. this would potentially delete all saves, configs
	// and data of all games which would be very bad. by avoiding redirection we can
	// assure the data is not deleted.
	// 
	// unfortunately according to documentation packaged applications can not escape
	// being redirected unless writing directly to the documents directory which is
	// not a good thing to do. we are thus stuck with the redirection and have to
	// put the blame on lost files on microsoft.
	// 
	// that said we still have the problem that accessing path for example for
	// opening a directory is not going to show the packaged location but the non
	// redirected directory. using a flag we can at least fix this problem.
	// the redirection problem has to be solved somehow else but no idea yet how
	// 
	// these flags exist but are not defined in the windows headers for some reason
	// 
	// KF_FLAG_NO_PACKAGE_REDIRECTION = 0x00010000
	//   do not redirect path
	// 
	// KF_FLAG_RETURN_FILTER_REDIRECTION_TARGET = 0x00040000
	//   returned redirected (real) path
	DWORD dwFlags = 0x00040000;
	
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
	if( SHGetKnownFolderPath( nFolder, dwFlags, NULL, &folderPath ) != S_OK ){
		DETHROW( deeInvalidParam );
	}
	
	const int folderPathLen = ( int )wcslen( folderPath );
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
		wide[ i ] = ( wchar_t )unicode[ i ];
	}
	wide[ i ] = 0;
}

decString deOSWindows::WideToUtf8( const wchar_t *wide ){
	return WideToUnicode( wide ).ToUTF8();
}

decUnicodeString deOSWindows::WideToUnicode( const wchar_t *wide ){
	const int count = ( int )wcslen( wide );
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

decString deOSWindows::GetRegistryValueCurrentUser( const char *key, const char *entry, const char *defaultValue ){
	HKEY hKey;
	if( RegOpenKeyExA( HKEY_CURRENT_USER, key, 0, KEY_READ, &hKey ) != ERROR_SUCCESS ){
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
	
	if( RegSetValueExA( hKey, entry, 0, REG_SZ, ( BYTE* )value, ( DWORD )strlen( value ) ) != ERROR_SUCCESS ){
		RegCloseKey( hKey );
		DETHROW( deeInvalidAction );
	}
	
	RegCloseKey( hKey );
}



// Private Functions
//////////////////////

void deOSWindows::pCleanUp(){
	if( pResolutions ){
		delete [] pResolutions;
	}
}

decString deOSWindows::pGetUserLanguage() const{
	ULONG numLanguages = 0;
	ULONG langBufSize = 0;
	
	if( ! GetUserPreferredUILanguages( MUI_LANGUAGE_NAME, &numLanguages, NULL, &langBufSize ) ){
		return "en";
	}
	
	wchar_t * const buffer = new wchar_t[ langBufSize ];
	if( ! GetUserPreferredUILanguages( MUI_LANGUAGE_NAME, &numLanguages, buffer, &langBufSize ) ){
		delete [] buffer;
		return "en";
	}
	
	const decString language( WideToUtf8( buffer ) );
	delete [] buffer;
	return language;
}

void deOSWindows::pFindResolutions(){
	int count = 0;
	DEVMODE mode = {};
	mode.dmSize = sizeof( mode );
	mode.dmDriverExtra = 0;

	while( EnumDisplaySettingsW( NULL, count, &mode ) ){
		count++;
	}

	if( count == 0 ){
		pResolutions = new decPoint[ 1 ];
		pResolutions[ 0 ].x = pScreenWidth;
		pResolutions[ 0 ].y = pScreenHeight;
		pResolutionCount = 1;
		return;
	}

	pResolutions = new decPoint[ count ];
	int i, j;

	for( i=0; i<count; i++ ){
		DEASSERT_TRUE( EnumDisplaySettingsW( NULL, i, &mode ) );

		const decPoint resolution( ( int )mode.dmPelsWidth, ( int )mode.dmPelsHeight );
		
		for( j=0; j<pResolutionCount; j++ ){
			if( pResolutions[ j ] == resolution ){
				break;
			}
		}

		if( j < pResolutionCount ){
			continue;
		}

		pResolutions[ pResolutionCount++ ] = resolution;
	}

	for( i=1; i<pResolutionCount; i++ ){
		const decPoint &a = pResolutions[ i - 1 ];
		const decPoint &b = pResolutions[ i ];
		if( b.x > a.x || ( b.x == a.x && b.y > a.y ) ){
			const decPoint t( a );
			pResolutions[ i - 1 ] = b;
			pResolutions[ i ] = t;
			if( i > 1 ){
				i -= 2;
			}
		}
	}
}

void deOSWindows::pFindScaleFactor(){
	const POINT ptZero = { 0, 0 };
	const HMONITOR monitor = MonitorFromPoint( ptZero, MONITOR_DEFAULTTOPRIMARY );
	DEVICE_SCALE_FACTOR dsf;
	if( GetScaleFactorForMonitor( monitor, &dsf ) == S_OK && dsf != 0 ){
		pScaleFactor = ( int )dsf;
	}
}

void deOSWindows::pFindRefreshRate(){
	DEVMODE mode = {};
	mode.dmSize = sizeof( mode );
	mode.dmDriverExtra = 0;
	if( EnumDisplaySettingsW( NULL, ENUM_CURRENT_SETTINGS, &mode ) && mode.dmDisplayFrequency > 1 ){
		pRefreshRate = ( int )mode.dmDisplayFrequency;
	}

	// NOTE fullscreen: https://stackoverflow.com/questions/2382464/win32-full-screen-and-hiding-taskbar
}

#endif
