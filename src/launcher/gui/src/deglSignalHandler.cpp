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

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#ifndef OS_W32
#include <execinfo.h>
#endif

#include "deglLauncher.h"
#include "deglSignalHandler.h"

#include <dragengine/common/exceptions.h>



// Signal Handlers
////////////////////

#ifndef OS_W32
static deglSignalHandler *pSignalHandler = nullptr;

static void signalSegV( int number, siginfo_t *infos, void *ptrContext ){
	const char *logSource = "?";
	deLogger *logger = nullptr;
	
	if( pSignalHandler ){
		logger = pSignalHandler->GetLauncher().GetLogger();
		logSource = pSignalHandler->GetLauncher().GetLogSource();
	}
	
	// some infos
	if( infos->si_code == SEGV_MAPERR ){
		if( logger ){
			logger->LogErrorFormat( logSource,
				"Segmentation Fault! Tried to access not allocated memory at %p.",
				infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Tried to access not allocated memory at %p.\n",
				infos->si_addr );
		}
		
	}else if( infos->si_code == SEGV_ACCERR ){
		if( logger ){
			logger->LogErrorFormat( logSource,
				"Segmentation Fault! Permission denied accessing memory at %p.",
				infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Permission denied accessing memory at %p.\n",
				infos->si_addr );
		}
		
	}else{
		if( logger ){
			logger->LogErrorFormat( logSource,
				"Segmentation Fault! Unknown memory error at %p.", infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Unknown memory error at %p.\n", infos->si_addr );
		}
	}
	
	// stack trace
	void *btentries[ 50 ]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace( btentries, 50 );
	
	if( logger ){
		logger->LogError( logSource, "Backtrace:" );
		
		char ** const btStrings = backtrace_symbols( btentries, btentryCount );
		int i;
		
		for( i=0; i<(int)btentryCount; i++ ){
			logger->LogError( logSource, btStrings[ i ] );
		}
		
		free( btStrings );
		
	}else{
		printf( "Backtrace:" );
		backtrace_symbols_fd( btentries, btentryCount, fileno( stdout ) );
	}
	
	/*
	if( pSignalHandler ){
		if( infos->si_code == SEGV_MAPERR ){
			coreFault->HandleSegFault( decrbCoreFault::eecMemoryNotAllocated, infos->si_addr, ptrContext );
			
		}else if( infos->si_code == SEGV_ACCERR ){
			coreFault->HandleSegFault( decrbCoreFault::eecMemoryNoPermision, infos->si_addr, ptrContext );
			
		}else{
			coreFault->HandleSegFault( decrbCoreFault::eecMemoryUnknown, infos->si_addr, ptrContext );
		}
		
	}else{
		printf( "No launcher object found. Can not gather crash information!\n" );
	}
	*/
	
	if( logger ){
		logger->LogError( logSource, "Done, exiting." );
		
	}else{
		printf( "Done, exiting.\n" );
	}
	
	exit( -1 );
}

static void signalAbort( int number, siginfo_t *infos, void *ptrContext ){
	const char *logSource = "?";
	deLogger *logger = nullptr;
	
	if( pSignalHandler ){
		logger = pSignalHandler->GetLauncher().GetLogger();
		logSource = pSignalHandler->GetLauncher().GetLogSource();
	}
	
	if( logger ){
		logger->LogError( logSource, "Unhandled Exception!" );
		
	}else{
		printf( "Unhandled Exception\n" );
	}
	
	// stack trace
	void *btentries[ 50 ]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace( btentries, 50 );
	
	if( logger ){
		logger->LogError( logSource, "Backtrace:" );
		
		char **btStrings = backtrace_symbols( btentries, btentryCount );
		int i;
		
		for( i=0; i<(int)btentryCount; i++ ){
			logger->LogError( logSource, btStrings[ i ] );
		}
		
		free( btStrings );
		
	}else{
		printf( "Backtrace:" );
		backtrace_symbols_fd( btentries, btentryCount, fileno( stdout ) );
	}
	
	if( logger ){
		logger->LogError( logSource, "Done, exiting." );
		
	}else{
		printf( "Done, exiting.\n" );
	}
	
	exit( -1 );
}
#endif



// Class deglSignalHandler
////////////////////////////

// Constructor, destructor
////////////////////////////

deglSignalHandler::deglSignalHandler( deglLauncher &launcher ) :
pLauncher( launcher ){
	pRegisterSignals();
}

deglSignalHandler::~deglSignalHandler(){
#ifndef OS_W32
	pSignalHandler = nullptr;
#endif
}



// Management
///////////////



// Private Functions
//////////////////////

void deglSignalHandler::pRegisterSignals(){
#ifndef OS_W32
	struct sigaction action;
	
	// set aside the launcher for the signal handling
	pSignalHandler = this;
	
	// add handler for SEGV signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_sigaction = signalSegV;
	action.sa_flags = SA_SIGINFO;
	
	if( sigaction( SIGSEGV, &action, nullptr ) ){
		DETHROW( deeInvalidAction );
	}
	
	// add handler for ABORT signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_sigaction = signalAbort;
	action.sa_flags = SA_SIGINFO;
	
	if( sigaction( SIGABRT, &action, nullptr ) ){
		DETHROW( deeInvalidAction );
	}
	
	// ignore SIGPIPE signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_handler = SIG_IGN;
	action.sa_flags = 0;
	
	if( sigaction( SIGPIPE, &action, nullptr ) ){
		DETHROW( deeInvalidAction );
	}
#endif
}
