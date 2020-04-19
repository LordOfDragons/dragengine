/* 
 * DEIGDE Project
 *
 * Copyright (C) 2018, Plüss Roland ( roland@rptd.ch )
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is projributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "projTestRunConstants.h"
#include "projTestRunCommandThread.h"
#include "projTestRunProcess.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Test-Run Process"



// Class projTestRunCommandThread
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTestRunCommandThread::projTestRunCommandThread( projTestRunProcess &process ) :
pProcess( process ),
pAbort( false ){
}

projTestRunCommandThread::~projTestRunCommandThread(){
}



// Management
///////////////

void projTestRunCommandThread::Run(){
	int command;
	
	while( ! pAbort ){
		// read a command from the pipe. blocks the thread until the pipe has a command.
		// in the unholy case of the game crashing an exception is thrown. in this case
		// we just go home an leave the cleaning up to the parent process.
		try{
			command = ReadCommandFromPipe();
			
		}catch( const deException &e ){
			break;
		}
		
		switch( command ){
		case projTestRunConstants::ecQuit:
			// although we are in a separate thread we can simply call Quit on the game
			// engine. this is because the Quit call just sets a quit request variable
			// to true. this operation is thread safe no matter in what interleaved way
			// the machine code is executed
			pProcess.RequestQuit();
			
			// after this we do no more read from the pipe. this is since after the stop
			// game command is received the interpretation of upcoming commands has to
			// revert to the default behavior
			break;
			
		case 0xff:
			// special value pushed back on the stream by the test-runner process to
			// abort the thread
			break;
			
		default:
			pProcess.WriteUCharToPipe( projTestRunConstants::ercFailed );
		}
	}
}

void projTestRunCommandThread::Abort(){
	pAbort = true;
	Stop();
	
	/*
	// if fungetc returns 0xff the call succeeded. if it returns EOF it failed
	fungetc( 0xff, pProcess.GetPipeIn() );
	Stop();
	*/
}



int projTestRunCommandThread::ReadCommandFromPipe(){
	uint8_t command = 0xff;
	
	#ifdef OS_W32
	DWORD bytesRead = 0;
	
	if( ! ReadFile( pProcess.GetPipeIn(), &command, sizeof( command ), &bytesRead, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	if( bytesRead < sizeof( command ) ){
		DETHROW( deeInvalidAction );
	}
	
	#else
	while( ! pAbort ){
		const ssize_t bytes = read( pProcess.GetPipeIn(), &command, sizeof( command ) );
		if( bytes == sizeof( command ) ){
			break;
		}
		if( bytes == -1 && errno != EINTR ){
			DETHROW( deeInvalidAction );
		}
	}
	#endif
	
	return command;
}
