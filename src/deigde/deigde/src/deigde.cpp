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

#include "deigde.h"
#include "gui/igdeRealApplication.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// unix entry point
/////////////////////

#if defined OS_UNIX && ! defined OS_BEOS && ! defined OS_MACOS
int main( int argCount, char **args ){
	try{
		igdeRealApplication().Run( argCount, args );
		
	}catch( const deException &e ){
		e.PrintError();
		return 1;
	}
	return 0;
}
#endif



// windows entry point
////////////////////////

#ifdef OS_W32
static int WINAPI RealWinMain(){
	try{
		igdeRealApplication().Run();
		
	}catch( const deException &e ){
		e.PrintError();
		return 1;
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
