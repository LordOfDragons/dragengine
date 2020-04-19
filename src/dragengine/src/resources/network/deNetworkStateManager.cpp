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

#include "deNetworkState.h"
#include "deNetworkStateManager.h"
#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deNetworkSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/network/deBaseNetworkState.h"
#include "../../systems/modules/scripting/deBaseScriptingNetworkState.h"
#include "../../common/exceptions.h"



// Class deNetworkStateManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deNetworkStateManager::deNetworkStateManager( deEngine *engine ) :
deResourceManager( engine, ertNetworkState )
{
	SetLoggingName( "network state" );
}

deNetworkStateManager::~deNetworkStateManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNetworkStateManager::GetNetworkStateCount() const{
	return pStates.GetCount();
}

deNetworkState *deNetworkStateManager::GetRootNetworkState() const{
	return ( deNetworkState* )pStates.GetRoot();
}

deNetworkState *deNetworkStateManager::CreateState( bool readOnly ){
	deNetworkState *state = NULL;
	
	try{
		state = new deNetworkState( this, readOnly );
		
		GetNetworkSystem()->LoadState( state );
		GetScriptingSystem()->LoadNetworkState( state );
		
		pStates.Add( state );
		
	}catch( const deException & ){
		if( state ){
			state->FreeReference();
		}
		throw;
	}
	
	return state;
}



void deNetworkStateManager::ReleaseLeakingResources(){
	const int count = GetNetworkStateCount();
	if( count > 0 ){
		LogWarnFormat( "%i leaking network states", count );
		pStates.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNetworkStateManager::SystemNetworkLoad(){
	deNetworkState *state = ( deNetworkState* )pStates.GetRoot();
	deNetworkSystem &netSys = *GetNetworkSystem();
	
	while( state ){
		if( ! state->GetPeerNetwork() ){
			netSys.LoadState( state );
		}
		
		state = ( deNetworkState* )state->GetLLManagerNext();
	}
}

void deNetworkStateManager::SystemNetworkUnload(){
	deNetworkState *state = ( deNetworkState* )pStates.GetRoot();
	
	while( state ){
		state->SetPeerNetwork ( NULL );
		state = ( deNetworkState* )state->GetLLManagerNext();
	}
}

void deNetworkStateManager::SystemScriptingLoad(){
	deNetworkState *state = ( deNetworkState* )pStates.GetRoot();
	deScriptingSystem &scrSys = *GetScriptingSystem();
	
	while( state ){
		if( ! state->GetPeerScripting() ){
			scrSys.LoadNetworkState( state );
		}
		
		state = ( deNetworkState* )state->GetLLManagerNext();
	}
}

void deNetworkStateManager::SystemScriptingUnload(){
	deNetworkState *state = ( deNetworkState* )pStates.GetRoot();
	
	while( state ){
		state->SetPeerScripting( NULL );
		state = ( deNetworkState* )state->GetLLManagerNext();
	}
}

void deNetworkStateManager::RemoveResource( deResource *resource ){
	pStates.RemoveIfPresent( resource );
}
