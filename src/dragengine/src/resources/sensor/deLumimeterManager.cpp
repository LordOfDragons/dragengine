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
#include "deLumimeter.h"
#include "deLumimeterManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deLumimeterManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deLumimeterManager::deLumimeterManager( deEngine *engine ) : deResourceManager( engine, ertLumimeter ){
	SetLoggingName( "lumimeter" );
}

deLumimeterManager::~deLumimeterManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deLumimeterManager::GetLumimeterCount() const{
	return pLumimeters.GetCount();
}

deLumimeter *deLumimeterManager::GetRootLumimeter() const{
	return ( deLumimeter* )pLumimeters.GetRoot();
}

deLumimeter *deLumimeterManager::CreateLumimeter(){
	deLumimeter *lumimeter = NULL;
	
	try{
		lumimeter = new deLumimeter( this );
		if( ! lumimeter ) DETHROW( deeOutOfMemory );
		
		GetGraphicSystem()->LoadLumimeter( lumimeter );
		
		pLumimeters.Add( lumimeter );
		
	}catch( const deException & ){
		if( lumimeter ){
			lumimeter->FreeReference();
		}
		throw;
	}
	
	return lumimeter;
}



void deLumimeterManager::ReleaseLeakingResources(){
	int count = GetLumimeterCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking lumimeter", count );
		pLumimeters.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deLumimeterManager::SystemGraphicLoad(){
	deLumimeter *lumimeter = ( deLumimeter* )pLumimeters.GetRoot();
	deGraphicSystem &graSys = *GetGraphicSystem();
	
	while( lumimeter ){
		if( ! lumimeter->GetPeerGraphic() ){
			graSys.LoadLumimeter( lumimeter );
		}
		
		lumimeter = ( deLumimeter* )lumimeter->GetLLManagerNext();
	}
}

void deLumimeterManager::SystemGraphicUnload(){
	deLumimeter *lumimeter = ( deLumimeter* )pLumimeters.GetRoot();
	
	while( lumimeter ){
		lumimeter->SetPeerGraphic( NULL );
		lumimeter = ( deLumimeter* )lumimeter->GetLLManagerNext();
	}
}



void deLumimeterManager::RemoveResource( deResource *resource ){
	pLumimeters.RemoveIfPresent( resource );
}
