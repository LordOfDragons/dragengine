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
#include <stdlib.h>
#include <string.h>

#ifdef OS_UNIX
#include <unistd.h>
#endif

#include "delEngineProcess.h"
#include "delEngineProcessRunGame.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class delEngineProcessRunGame
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineProcessRunGame::delEngineProcessRunGame( delEngineProcess &process ) :
pProcess( process ){
}

delEngineProcessRunGame::~delEngineProcessRunGame(){
}



// Management
///////////////

void delEngineProcessRunGame::Run(){
	unsigned char command;
	
	while( true ){
		// read a command from the pipe. blocks the thread until the pipe has a command.
		// in the unholy case of the game crashing an exception is thrown. in this case
		// we just go home an leave the cleaning up to the parent process.
		try{
			command = BlockingReadCommandFromPipe();
			
		}catch( const deException & ){
			break;
		}
		
		if(command == delEngineProcess::eccStopGame
		|| command == delEngineProcess::eccStopProcess){
			// stop process is handled the same way as stop game. this is more of a
			// historical situation than something useful
			
			// although we are in a separate thread we can simply call Quit on the game
			// engine. this is because the Quit call just sets a quit request variable
			// to true. this operation is thread safe no matter in what interleaved way
			// the machine code is executed
			pProcess.GetEngine()->Quit();
			
			// write answer to pipe once game exits. only required for stop game command
			if(command == delEngineProcess::eccStopGame){
				pProcess.SetStopGame();
			}
			
			// after this we do no more read from the pipe. this is since after the stop
			// game command is received the interpretation of upcoming commands has to
			// revert to the default behavior
			break;
			
		}else{
			pProcess.WriteUCharToPipe(delEngineProcess::ercFailed);
		}
	}
}

int delEngineProcessRunGame::BlockingReadCommandFromPipe(){
	unsigned char command;
	
	#ifdef OS_W32
		DWORD bytesRead = 0;
		
		if( ! ReadFile( pProcess.GetPipeIn(), &command, sizeof( command ), &bytesRead, NULL ) ){
			DETHROW_INFO( deeInvalidAction, "read pipe failed" );
		}
		if( bytesRead < sizeof( command ) ){
			DETHROW_INFO( deeInvalidAction, "read pipe too short data received" );
		}
		
	#else
		ssize_t bytes = 0;
		
		while( true ){
			bytes = read( pProcess.GetPipeIn(), &command, sizeof( command ) );
			
			if( bytes == sizeof( command ) ){
				break;
			}
			
			if( bytes == -1 && errno != EINTR ){
				DETHROW_INFO( deeInvalidAction, "read pipe failed" );
			}
			
			//printf( "read call interrupted, restarting!\n" );
		}
	#endif
	
	return command;
}
