/* 
 * Drag[en]gine GUI Launcher
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifdef OS_W32_HELPER

#include <delauncher/engine/delEngineProcessMain.h>

int main( int argc, char **args ){
	return delEngineProcessMain().RunMain( argc, args );
}

#ifdef OS_W32_VS

#include <windows.h>
#include <stdio.h>

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

#endif
