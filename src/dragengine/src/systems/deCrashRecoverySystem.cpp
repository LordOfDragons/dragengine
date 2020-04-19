/* 
 * Drag[en]gine Game Engine
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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deCrashRecoverySystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/crashrecovery/deBaseCrashRecoveryModule.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"



// class deCrashRecoverySystem
////////////////////////////////

// constructor, destructor
deCrashRecoverySystem::deCrashRecoverySystem( deEngine *engine ) : deBaseSystem( engine, "Crash-Recovery", deModuleSystem::emtCrashRecovery ){
	pActiveModule = NULL;
}
deCrashRecoverySystem::~deCrashRecoverySystem(){
}

// extend those
void deCrashRecoverySystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseCrashRecoveryModule* )module->GetModule();
}
void deCrashRecoverySystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

// overload those
void deCrashRecoverySystem::PostStart(){
	if( ! pActiveModule->Init() ) DETHROW( deeInvalidAction );
}
void deCrashRecoverySystem::PreStop(){
	// remove all parallel tasks if present
	GetEngine()->GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	pActiveModule->CleanUp();
}

// management
bool deCrashRecoverySystem::RecoverFromError(){
	if( ! GetIsRunning() ) return false;
	return pActiveModule->RecoverFromError();
}
