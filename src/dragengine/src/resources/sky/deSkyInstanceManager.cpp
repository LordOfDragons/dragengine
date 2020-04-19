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

#include "deSkyInstance.h"
#include "deSkyInstanceManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicSkyInstance.h"


// Class deSkyInstanceManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deSkyInstanceManager::deSkyInstanceManager( deEngine *engine ) :
deResourceManager( engine, ertSkyInstance ){
	SetLoggingName( "sky instance" );
}

deSkyInstanceManager::~deSkyInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deSkyInstanceManager::GetSkyInstanceCount() const{
	return pInstances.GetCount();
}

deSkyInstance *deSkyInstanceManager::GetRootSkyInstance() const{
	return ( deSkyInstance* )pInstances.GetRoot();
}

deSkyInstance *deSkyInstanceManager::CreateSkyInstance(){
	deSkyInstance *instance = NULL;
	
	try{
		instance = new deSkyInstance( this );
		GetGraphicSystem()->LoadSkyInstance( instance );
		pInstances.Add( instance );
		
	}catch( const deException & ){
		if( instance ){
			instance->FreeReference();
		}
		
		throw;
	}
	
	return instance;
}



void deSkyInstanceManager::ReleaseLeakingResources(){
	const int count = GetSkyInstanceCount();
	if( count > 0 ){
		LogWarnFormat( "%i leaking sky instances", count );
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deSkyInstanceManager::SystemGraphicLoad(){
	deSkyInstance *instance = ( deSkyInstance* )pInstances.GetRoot();
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	while( instance ){
		if( ! instance->GetPeerGraphic() ){
			graSys.LoadSkyInstance( instance );
		}
		instance = ( deSkyInstance* )instance->GetLLManagerNext();
	}
}

void deSkyInstanceManager::SystemGraphicUnload(){
	deSkyInstance *instance = ( deSkyInstance* )pInstances.GetRoot();
	
	while( instance ){
		instance->SetPeerGraphic( NULL );
		instance = ( deSkyInstance* )instance->GetLLManagerNext();
	}
}



void deSkyInstanceManager::RemoveResource( deResource *resource ){
	pInstances.RemoveIfPresent( resource );
}
