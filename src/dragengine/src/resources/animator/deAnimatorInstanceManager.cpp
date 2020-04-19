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

#include "deAnimatorInstance.h"
#include "deAnimatorInstanceManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/animator/deBaseAnimatorModule.h"
#include "../../systems/deAnimatorSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// Class deAnimatorInstanceManager
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorInstanceManager::deAnimatorInstanceManager( deEngine *engine ) :
deResourceManager( engine, ertAnimatorInstance ){
	SetLoggingName( "animator instance" );
}

deAnimatorInstanceManager::~deAnimatorInstanceManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deAnimatorInstanceManager::GetAnimatorInstanceCount() const{
	return pInstances.GetCount();
}

deAnimatorInstance *deAnimatorInstanceManager::GetRootAnimatorInstance() const{
	return ( deAnimatorInstance* )pInstances.GetRoot();
}

deAnimatorInstance *deAnimatorInstanceManager::CreateAnimatorInstance(){
	deAnimatorInstance *instance = NULL;
	
	try{
		instance = new deAnimatorInstance( this );
		if( ! instance ) DETHROW( deeOutOfMemory );
		
		GetAnimatorSystem()->LoadAnimatorInstance( instance );
		
		pInstances.Add( instance );
		
	}catch( const deException & ){
		if( instance ){
			instance->FreeReference();
		}
		
		throw;
	}
	
	return instance;
}



void deAnimatorInstanceManager::ReleaseLeakingResources(){
	const int count = GetAnimatorInstanceCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking animator instances", count );
		pInstances.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deAnimatorInstanceManager::SystemAnimatorLoad(){
	deAnimatorInstance *instance = ( deAnimatorInstance* )pInstances.GetRoot();
	deAnimatorSystem &aniSys = *GetAnimatorSystem();
	
	while( instance ){
		if( ! instance->GetPeerAnimator() ){
			aniSys.LoadAnimatorInstance( instance );
		}
		
		instance = ( deAnimatorInstance* )instance->GetLLManagerNext();
	}
}

void deAnimatorInstanceManager::SystemAnimatorUnload(){
	deAnimatorInstance *instance = ( deAnimatorInstance* )pInstances.GetRoot();
	
	while( instance ){
		instance->SetPeerAnimator( NULL );
		instance = ( deAnimatorInstance* )instance->GetLLManagerNext();
	}
}



void deAnimatorInstanceManager::RemoveResource( deResource *resource ){
	pInstances.RemoveIfPresent( resource );
}
