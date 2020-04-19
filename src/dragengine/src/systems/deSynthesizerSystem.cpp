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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deSynthesizerSystem.h"
#include "modules/deLoadableModule.h"
#include "modules/synthesizer/deBaseSynthesizerModule.h"
#include "deModuleSystem.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../parallel/deParallelProcessing.h"
#include "../resources/synthesizer/deSynthesizer.h"
#include "../resources/synthesizer/deSynthesizerManager.h"
#include "../resources/synthesizer/deSynthesizerInstance.h"
#include "../resources/sound/deSound.h"



extern const int *vResourcePeerCreationOrder;



// Class deSynthesizerSystem
///////////////////////////////

// Constructor, Destructor
////////////////////////////

deSynthesizerSystem::deSynthesizerSystem( deEngine *engine ) :
deBaseSystem( engine, "Synthesizer", deModuleSystem::emtSynthesizer ),
pActiveModule( NULL ){
}

deSynthesizerSystem::~deSynthesizerSystem(){
}



// Management
///////////////

void deSynthesizerSystem::LoadSound( deSound *sound ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( sound->GetPeerSynthesizer() ){
		DETHROW( deeInvalidParam );
	}
	sound->SetPeerSynthesizer( pActiveModule->CreateSound( sound ) );
}

void deSynthesizerSystem::LoadSynthesizer( deSynthesizer *synthesizer ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( synthesizer->GetPeerSynthesizer() ){
		DETHROW( deeInvalidParam );
	}
	synthesizer->SetPeerSynthesizer( pActiveModule->CreateSynthesizer( synthesizer ) );
}

void deSynthesizerSystem::LoadSynthesizerInstance( deSynthesizerInstance *instance ){
	if( ! GetIsRunning() ){
		return;
	}
	
	if( instance->GetPeerSynthesizer() ){
		DETHROW( deeInvalidParam );
	}
	instance->SetPeerSynthesizer( pActiveModule->CreateSynthesizerInstance( instance ) );
}



// Overloadables
//////////////////

void deSynthesizerSystem::SetActiveModule( deLoadableModule *module ){
	deBaseSystem::SetActiveModule( module );
	pActiveModule = ( deBaseSynthesizerModule* )module->GetModule();
}

void deSynthesizerSystem::ClearPermanents(){
	deBaseSystem::ClearPermanents();
	pActiveModule = NULL;
}

void deSynthesizerSystem::PostStart(){
	deEngine &engine = *GetEngine();
	int i;
	
	// init module
	if( ! pActiveModule->Init() ){
		DETHROW( deeInvalidAction );
	}
	
	// load synthesizer system related stuff
	for( i=0; i<engine.GetResourceManagerCount(); i++ ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemSynthesizerLoad();
	}
}

void deSynthesizerSystem::PreStop(){
	deEngine &engine = *GetEngine();
	int i;
	
	// remove all parallel tasks if present
	engine.GetParallelProcessing().FinishAndRemoveTasksOwnedBy( pActiveModule );
	
	// unload synthesizer system related stuff
	for( i=engine.GetResourceManagerCount()-1; i>=0; i-- ){
		engine.GetResourceManagerAt( vResourcePeerCreationOrder[ i ] )->SystemSynthesizerUnload();
	}
	
	// shutdown current module
	pActiveModule->CleanUp();
}



// Private functions
//////////////////////

void deSynthesizerSystem::pCleanUp(){
}
