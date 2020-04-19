/* 
 * Drag[en]gine GUI Launcher
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

#if ! defined( OS_W32 ) || defined( OS_W32_HELPER )

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "deglEngineProcess.h"
#include "deglEngineProcessRunGame.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Engine Process"



// Class deglEngineProcessRunGame
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglEngineProcessRunGame::deglEngineProcessRunGame( deglEngineProcess *process ){
	if( ! process ) DETHROW( deeInvalidParam );
	
	pProcess = process;
}

deglEngineProcessRunGame::~deglEngineProcessRunGame(){
}



// Management
///////////////

void deglEngineProcessRunGame::Run(){
	unsigned char command;
	
	while( true ){
		// read a command from the pipe. blocks the thread until the pipe has a command.
		// in the unholy case of the game crashing an exception is thrown. in this case
		// we just go home an leave the cleaning up to the parent process.
		try{
			command = BlockingReadCommandFromPipe();
			
		}catch( const deException &e ){
			break;
		}
		
		if( command == deglEngineProcess::eccStopGame ){
			// although we are in a separate thread we can simply call Quit on the game
			// engine. this is because the Quit call just sets a quit request variable
			// to true. this operation is thread safe no matter in what interleaved way
			// the machine code is executed
			pProcess->GetEngine()->Quit();
			
			// after this we do no more read from the pipe. this is since after the stop
			// game command is received the interpretation of upcoming commands has to
			// revert to the default behavior
			break;
			
		}else{
			pProcess->WriteUCharToPipe( deglEngineProcess::ercFailed );
		}
	}
}

int deglEngineProcessRunGame::BlockingReadCommandFromPipe(){
	unsigned char command;
	
	#ifdef OS_W32
		DWORD bytesRead = 0;
		
		if( ! ReadFile( pProcess->GetPipeIn(), &command, sizeof( command ), &bytesRead, NULL ) ){
			DETHROW( deeInvalidAction );
		}
		if( bytesRead < sizeof( command ) ){
			DETHROW( deeInvalidAction );
		}
		
	#else
		ssize_t bytes = 0;
		
		while( true ){
			bytes = read( pProcess->GetPipeIn(), &command, sizeof( command ) );
			
			if( bytes == sizeof( command ) ){
				break;
			}
			
			if( bytes == -1 && errno != EINTR ){
				DETHROW( deeInvalidAction );
			}
			
			//printf( "read call interrupted, restarting!\n" );
		}
	#endif
	
	return command;
}

#endif // ! OS_W32 || OS_W32_HELPER
