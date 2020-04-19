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

#include "deNavigator.h"
#include "deNavigatorManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAISystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/ai/deBaseAINavigator.h"
#include "../../../common/exceptions.h"



// Class deNavigatorManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigatorManager::deNavigatorManager( deEngine *engine ) : deResourceManager( engine, ertNavigator ){
	SetLoggingName( "navigator" );
}

deNavigatorManager::~deNavigatorManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNavigatorManager::GetNavigatorCount() const{
	return pNavigators.GetCount();
}

deNavigator *deNavigatorManager::GetRootNavigator() const{
	return ( deNavigator* )pNavigators.GetRoot();
}

deNavigator *deNavigatorManager::CreateNavigator(){
	deNavigator *navigator = NULL;
	
	try{
		navigator = new deNavigator( this );
		if( ! navigator ){
			DETHROW( deeOutOfMemory );
		}
		
		GetAISystem()->LoadNavigator( navigator );
		
		pNavigators.Add( navigator );
		
	}catch( const deException & ){
		if( navigator ){
			navigator->FreeReference();
		}
		throw;
	}
	
	return navigator;
}



void deNavigatorManager::ReleaseLeakingResources(){
	const int count = GetNavigatorCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking navigators", count );
		pNavigators.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNavigatorManager::SystemAILoad(){
	deNavigator *navigator = ( deNavigator* )pNavigators.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while( navigator ){
		if( ! navigator->GetPeerAI() ){
			aisys.LoadNavigator( navigator );
		}
		
		navigator = ( deNavigator* )navigator->GetLLManagerNext();
	}
}

void deNavigatorManager::SystemAIUnload(){
	deNavigator *navigator = ( deNavigator* )pNavigators.GetRoot();
	
	while( navigator ){
		navigator->SetPeerAI( NULL );
		navigator = ( deNavigator* )navigator->GetLLManagerNext();
	}
}

void deNavigatorManager::RemoveResource( deResource *resource ){
	pNavigators.RemoveIfPresent( resource );
}
