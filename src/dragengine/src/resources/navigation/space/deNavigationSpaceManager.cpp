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

#include "deNavigationSpace.h"
#include "deNavigationSpaceManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAISystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/ai/deBaseAINavigationSpace.h"
#include "../../../common/exceptions.h"



// Class deNavigationSpaceManager
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationSpaceManager::deNavigationSpaceManager( deEngine *engine ) : deResourceManager( engine, ertNavigationSpace ){
	SetLoggingName( "navigation space" );
}

deNavigationSpaceManager::~deNavigationSpaceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNavigationSpaceManager::GetNavigationSpaceCount() const{
	return pNavSpaces.GetCount();
}

deNavigationSpace *deNavigationSpaceManager::GetRootNavigationSpace() const{
	return ( deNavigationSpace* )pNavSpaces.GetRoot();
}

deNavigationSpace *deNavigationSpaceManager::CreateNavigationSpace(){
	deNavigationSpace *navspace = NULL;
	
	try{
		navspace = new deNavigationSpace( this );
		if( ! navspace ){
			DETHROW( deeOutOfMemory );
		}
		
		GetAISystem()->LoadNavigationSpace( navspace );
		
		pNavSpaces.Add( navspace );
		
	}catch( const deException & ){
		if( navspace ){
			navspace->FreeReference();
		}
		throw;
	}
	
	return navspace;
}



void deNavigationSpaceManager::ReleaseLeakingResources(){
	const int count = GetNavigationSpaceCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking navigation spaces", count );
		pNavSpaces.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNavigationSpaceManager::SystemAILoad(){
	deNavigationSpace *navspace = ( deNavigationSpace* )pNavSpaces.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while( navspace ){
		if( ! navspace->GetPeerAI() ){
			aisys.LoadNavigationSpace( navspace );
		}
		
		navspace = ( deNavigationSpace* )navspace->GetLLManagerNext();
	}
}

void deNavigationSpaceManager::SystemAIUnload(){
	deNavigationSpace *navspace = ( deNavigationSpace* )pNavSpaces.GetRoot();
	
	while( navspace ){
		navspace->SetPeerAI( NULL );
		navspace = ( deNavigationSpace* )navspace->GetLLManagerNext();
	}
}

void deNavigationSpaceManager::RemoveResource( deResource *resource ){
	pNavSpaces.RemoveIfPresent( resource );
}
