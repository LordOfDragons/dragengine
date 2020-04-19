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

#include <stdlib.h>
#include <string.h>

#include "deglWindowLogger.h"
#include "deglWindowLoggerListener.h"
#include "../../logger/deglLoggerHistory.h"
#include "../../logger/deglLoggerHistoryEntry.h"

#include <dragengine/common/exceptions.h>



// Class deglWindowLoggerListener
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deglWindowLoggerListener::deglWindowLoggerListener( deglWindowLogger *window ){
	if( ! window ) DETHROW( deeInvalidParam );
	
	pWindow = window;
}

deglWindowLoggerListener::~deglWindowLoggerListener(){
}



// Management
///////////////

void deglWindowLoggerListener::MessageAdded( deglLoggerHistory *history, deglLoggerHistoryEntry &entry ){
	pWindow->AddLogToTable( entry ); // safe since we are here inside a locked mutex
}

void deglWindowLoggerListener::HistoryCleared( deglLoggerHistory *history ){
	pWindow->ClearLogsTable(); // safe since we are here inside a locked mutex
}
