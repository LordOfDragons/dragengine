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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		
		if( command == delEngineProcess::eccStopGame ){
			// although we are in a separate thread we can simply call Quit on the game
			// engine. this is because the Quit call just sets a quit request variable
			// to true. this operation is thread safe no matter in what interleaved way
			// the machine code is executed
			pProcess.GetEngine()->Quit();
			
			// after this we do no more read from the pipe. this is since after the stop
			// game command is received the interpretation of upcoming commands has to
			// revert to the default behavior
			break;
			
		}else{
			pProcess.WriteUCharToPipe( delEngineProcess::ercFailed );
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
