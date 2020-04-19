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
#include "deEnvMapProbe.h"
#include "deEnvMapProbeManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deEnvMapProbeManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deEnvMapProbeManager::deEnvMapProbeManager( deEngine *engine ) : deResourceManager( engine, ertEnvMapProbe ){
	SetLoggingName( "environment map probe" );
}

deEnvMapProbeManager::~deEnvMapProbeManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deEnvMapProbeManager::GetEnvMapProbeCount() const{
	return pEnvMapProbes.GetCount();
}

deEnvMapProbe *deEnvMapProbeManager::GetRootEnvMapProbe() const{
	return ( deEnvMapProbe* )pEnvMapProbes.GetRoot();
}

deEnvMapProbe *deEnvMapProbeManager::CreateEnvMapProbe(){
	deEnvMapProbe *envMapProbe = NULL;
	
	try{
		envMapProbe = new deEnvMapProbe( this );
		
		GetGraphicSystem()->LoadEnvMapProbe( envMapProbe );
		
		pEnvMapProbes.Add( envMapProbe );
		
	}catch( const deException & ){
		if( envMapProbe ){
			envMapProbe->FreeReference();
		}
		throw;
	}
	
	return envMapProbe;
}



void deEnvMapProbeManager::ReleaseLeakingResources(){
	int count = GetEnvMapProbeCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking environment map probes", count );
		pEnvMapProbes.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deEnvMapProbeManager::SystemGraphicLoad(){
	deEnvMapProbe *envMapProbe = ( deEnvMapProbe* )pEnvMapProbes.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while( envMapProbe ){
		if( ! envMapProbe->GetPeerGraphic() ){
			grasys.LoadEnvMapProbe( envMapProbe );
		}
		
		envMapProbe = ( deEnvMapProbe* )envMapProbe->GetLLManagerNext();
	}
}

void deEnvMapProbeManager::SystemGraphicUnload(){
	deEnvMapProbe *envMapProbe = ( deEnvMapProbe* )pEnvMapProbes.GetRoot();
	
	while( envMapProbe ){
		envMapProbe->SetPeerGraphic( NULL );
		envMapProbe = ( deEnvMapProbe* )envMapProbe->GetLLManagerNext();
	}
}



void deEnvMapProbeManager::RemoveResource( deResource *resource ){
	pEnvMapProbes.RemoveIfPresent( resource );
}
