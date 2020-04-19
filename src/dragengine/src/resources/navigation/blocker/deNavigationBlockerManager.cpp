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

#include "deNavigationBlocker.h"
#include "deNavigationBlockerManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deAISystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/ai/deBaseAINavigationBlocker.h"
#include "../../../common/exceptions.h"



// Class deNavigationBlockerManager
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationBlockerManager::deNavigationBlockerManager( deEngine *engine ) : deResourceManager( engine, ertNavigationBlocker ){
	SetLoggingName( "navigation blocker" );
}

deNavigationBlockerManager::~deNavigationBlockerManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deNavigationBlockerManager::GetNavigationBlockerCount() const{
	return pBlockers.GetCount();
}

deNavigationBlocker *deNavigationBlockerManager::GetRootNavigationBlocker() const{
	return ( deNavigationBlocker* )pBlockers.GetRoot();
}

deNavigationBlocker *deNavigationBlockerManager::CreateNavigationBlocker(){
	deNavigationBlocker *blocker = NULL;
	
	try{
		blocker = new deNavigationBlocker( this );
		GetAISystem()->LoadNavigationBlocker( blocker );
		
		pBlockers.Add( blocker );
		
	}catch( const deException & ){
		if( blocker ){
			blocker->FreeReference();
		}
		throw;
	}
	
	return blocker;
}



void deNavigationBlockerManager::ReleaseLeakingResources(){
	const int count = GetNavigationBlockerCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking navigation blockers", count );
		pBlockers.RemoveAll(); // we do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deNavigationBlockerManager::SystemAILoad(){
	deNavigationBlocker *blocker = ( deNavigationBlocker* )pBlockers.GetRoot();
	deAISystem &aisys = *GetAISystem();
	
	while( blocker ){
		if( ! blocker->GetPeerAI() ){
			aisys.LoadNavigationBlocker( blocker );
		}
		blocker = ( deNavigationBlocker* )blocker->GetLLManagerNext();
	}
}

void deNavigationBlockerManager::SystemAIUnload(){
	deNavigationBlocker *blocker = ( deNavigationBlocker* )pBlockers.GetRoot();
	
	while( blocker ){
		blocker->SetPeerAI( NULL );
		blocker = ( deNavigationBlocker* )blocker->GetLLManagerNext();
	}
}

void deNavigationBlockerManager::RemoveResource( deResource *resource ){
	pBlockers.RemoveIfPresent( resource );
}
