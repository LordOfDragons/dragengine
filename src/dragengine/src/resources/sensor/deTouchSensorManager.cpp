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
#include "deTouchSensor.h"
#include "deTouchSensorManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"



// Class deTouchSensorManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deTouchSensorManager::deTouchSensorManager( deEngine *engine ) : deResourceManager( engine, ertTouchSensor ){
	SetLoggingName( "touch sensor" );
}

deTouchSensorManager::~deTouchSensorManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deTouchSensorManager::GetTouchSensorCount() const{
	return pTouchSensors.GetCount();
}

deTouchSensor *deTouchSensorManager::GetRootTouchSensor() const{
	return ( deTouchSensor* )pTouchSensors.GetRoot();
}

deTouchSensor *deTouchSensorManager::CreateTouchSensor(){
	deTouchSensor *touchSensor = NULL;
	
	try{
		touchSensor = new deTouchSensor( this );
		if( ! touchSensor ) DETHROW( deeOutOfMemory );
		
		GetPhysicsSystem()->LoadTouchSensor( touchSensor );
		GetScriptingSystem()->LoadTouchSensor( touchSensor );
		
		pTouchSensors.Add( touchSensor );
		
	}catch( const deException & ){
		if( touchSensor ){
			touchSensor->FreeReference();
		}
		throw;
	}
	
	return touchSensor;
}



void deTouchSensorManager::ReleaseLeakingResources(){
	int count = GetTouchSensorCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking touch sensor", count );
		pTouchSensors.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems support
////////////////////

void deTouchSensorManager::SystemPhysicsLoad(){
	deTouchSensor *touchSensor = ( deTouchSensor* )pTouchSensors.GetRoot();
	dePhysicsSystem &phySys = *GetPhysicsSystem();
	
	while( touchSensor ){
		if( ! touchSensor->GetPeerPhysics() ){
			phySys.LoadTouchSensor( touchSensor );
		}
		
		touchSensor = ( deTouchSensor* )touchSensor->GetLLManagerNext();
	}
}

void deTouchSensorManager::SystemPhysicsUnload(){
	deTouchSensor *touchSensor = ( deTouchSensor* )pTouchSensors.GetRoot();
	
	while( touchSensor ){
		touchSensor->SetPeerPhysics( NULL );
		touchSensor = ( deTouchSensor* )touchSensor->GetLLManagerNext();
	}
}

void deTouchSensorManager::SystemScriptingLoad(){
	deTouchSensor *touchSensor = ( deTouchSensor* )pTouchSensors.GetRoot();
	deScriptingSystem &scrSys = *GetScriptingSystem();
	
	while( touchSensor ){
		if( ! touchSensor->GetPeerScripting() ){
			scrSys.LoadTouchSensor( touchSensor );
		}
		
		touchSensor = ( deTouchSensor* )touchSensor->GetLLManagerNext();
	}
}

void deTouchSensorManager::SystemScriptingUnload(){
	deTouchSensor *touchSensor = ( deTouchSensor* )pTouchSensors.GetRoot();
	
	while( touchSensor ){
		touchSensor->SetPeerScripting( NULL );
		touchSensor = ( deTouchSensor* )touchSensor->GetLLManagerNext();
	}
}



void deTouchSensorManager::RemoveResource( deResource *resource ){
	pTouchSensors.RemoveIfPresent( resource );
}
