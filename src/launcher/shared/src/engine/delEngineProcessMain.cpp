/* 
 * Drag[en]gine Launcher Shared
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#include "delEngineProcess.h"
#include "delEngineProcessMain.h"

#if defined OS_BEOS
#	include <dragengine/app/deOSBeOS.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_UNIX
#	include <dragengine/app/deOSUnix.h>
#	include <dragengine/app/deOSConsole.h>
#elif defined OS_W32
#	include <dragengine/app/deOSWindows.h>
#else
#	error OS not supported!
#endif


delEngineProcessMain::delEngineProcessMain(){
}

#ifdef OS_W32
int delEngineProcessMain::RunMain( int argc, char **args ){
	HANDLE pipeIn = INVALID_HANDLE_VALUE;
	HANDLE pipeOut = INVALID_HANDLE_VALUE;
	uint16_t logfileLength;
	decString logfile;
	DWORD bytesWritten = 0;
	DWORD bytesRead = 0;
	char sync = 1;
	
//	FILE *w32ConH4xFile = freopen( "OOPS.log", "w", stdout );
//	if( ! w32ConH4xFile ) return -1;
//	setvbuf( w32ConH4xFile, NULL, _IONBF, 0 );
	
//	printf( "GO!\n" );
//	fflush( stdout );
	pipeIn = GetStdHandle( STD_INPUT_HANDLE );
//	printf( "using in pipe %p\n", pipeIn );
//	fflush( stdout );
	
	if( pipeIn == INVALID_HANDLE_VALUE ){
		return -1;
	}
	
	try{
//		printf( "reading write pipe\n" );
//		fflush( stdout );
		// read out-pipe handle
		if( ! ReadFile( pipeIn, &pipeOut, sizeof( pipeOut ), &bytesRead, NULL ) ){
			DETHROW_INFO( deeInvalidParam, "failed reading out-pipe handle" );
		}
		if( bytesRead < sizeof( pipeOut ) ){
			DETHROW_INFO( deeInvalidParam, "reading out-pipe handle too short" );
		}
		
		// read log filename
//		printf( "reading log file length\n" );
//		fflush( stdout );
		if( ! ReadFile( pipeIn, &logfileLength, sizeof( logfileLength ), &bytesRead, NULL ) ){
			DETHROW_INFO( deeInvalidParam, "failed reading log filename" );
		}
		if( bytesRead < sizeof( logfileLength ) ){
			DETHROW_INFO( deeInvalidParam, "reading log filename too short" );
		}
		
//		printf( "reading log file name\n");
//		fflush( stdout );
		logfile.Set( ' ', logfileLength );
		if( ! ReadFile( pipeIn, ( LPVOID )logfile.GetString(), ( int )logfileLength, &bytesRead, NULL ) ){
			DETHROW_INFO( deeInvalidParam, "failed reading log filename" );
		}
		if( bytesRead < ( DWORD )logfileLength ){
			DETHROW_INFO( deeInvalidParam, "reading log filename too short" );
		}
		
		uint8_t flags = 0;
		if( ! ReadFile( pipeIn, &flags, 1, &bytesRead, NULL ) ){
			DETHROW_INFO( deeInvalidParam, "failed reading flags" );
		}
		if( bytesRead < 1 ){
			DETHROW_INFO( deeInvalidParam, "reading flags too short" );
		}
		const bool useConsole = ( flags & 0x1 ) == 0x1;
		
		// send sync
//		printf( "sending sync\n" );
//		fflush( stdout );
		if( ! WriteFile( pipeOut, &sync, 1, &bytesWritten, NULL ) ){
			DETHROW_INFO( deeInvalidAction, "failed sending sync" );
		}
		if( bytesWritten < 1 ){
			DETHROW_INFO( deeInvalidAction, "sending sync too short" );
		}
		
		// start process
//		printf( "start process\n");
//		fflush( stdout );
		delEngineProcess process( pipeIn, pipeOut, logfile );
		process.SetUseConsole( useConsole );
		
		process.Run();
		
	}catch( const deException &e ){
//		e.PrintError();
//		printf( "GetLastError %x\n", ( unsigned int )GetLastError() );
		if( pipeIn != INVALID_HANDLE_VALUE ){
			CloseHandle( pipeIn );
		}
		if( pipeOut != INVALID_HANDLE_VALUE ){
			CloseHandle( pipeOut );
		}
		return -1;
	}
	
	CloseHandle( pipeIn );
	CloseHandle( pipeOut );
	return 0;
}
#endif
