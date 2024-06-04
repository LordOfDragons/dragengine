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

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_UNIX
#include <unistd.h>
#endif

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
			
		}catch( const deException & ){
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
