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

#include <stdlib.h>

#include "delEngineInstance.h"

#include <dragengine/common/exceptions.h>



// Class delEngineInstance::Factory
/////////////////////////////////////

delEngineInstance::Factory::Factory(){}
delEngineInstance::Factory::~Factory(){}



// Class delEngineInstance
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineInstance::delEngineInstance( delLauncher &launcher, const char *logfile ) :
pLauncher( launcher ),
pLogFile( logfile ),
pUseConsole( false ){
}

delEngineInstance::~delEngineInstance(){
}



// Management
///////////////

void delEngineInstance::SetLogFile( const decString &logFile ){
	pLogFile = logFile;
}

void delEngineInstance::SetUseConsole( bool useConsole ){
	pUseConsole = useConsole;
}

#ifdef OS_BEOS
void delEngineInstance::BeosMessageReceived( BMessage *message ){
}
#endif
