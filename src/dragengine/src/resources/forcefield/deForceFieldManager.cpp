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

#include "deForceField.h"
#include "deForceFieldManager.h"

#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/physics/deBasePhysicsForceField.h"



// Class deForceFieldManager
//////////////////////////////

// Constructor, destructor
////////////////////////////

deForceFieldManager::deForceFieldManager( deEngine *engine ) : deResourceManager( engine, ertForceField ){
	SetLoggingName( "force field" );
}

deForceFieldManager::~deForceFieldManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deForceFieldManager::GetForceFieldCount() const{
	return pFields.GetCount();
}

deForceField *deForceFieldManager::GetRootForceField() const{
	return ( deForceField* )pFields.GetRoot();
}

deForceField *deForceFieldManager::CreateForceField(){
	deForceField *field = NULL;
	
	try{
		field = new deForceField( this );
		if( ! field ) DETHROW( deeOutOfMemory );
		
		GetPhysicsSystem()->LoadForceField( field );
		
		pFields.Add( field );
		
	}catch( const deException & ){
		if( field ){
			field->FreeReference();
		}
		throw;
	}
	
	return field;
}



void deForceFieldManager::ReleaseLeakingResources(){
	if( GetForceFieldCount() > 0 ){
		LogWarnFormat( "%i leaking force fields", GetForceFieldCount() );
		pFields.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deForceFieldManager::SystemPhysicsLoad(){
	deForceField *field = ( deForceField* )pFields.GetRoot();
	
	while( field ){
		if( ! field->GetPeerPhysics() ){
			GetPhysicsSystem()->LoadForceField( field );
		}
		
		field = ( deForceField* )field->GetLLManagerNext();
	}
}

void deForceFieldManager::SystemPhysicsUnload(){
	deForceField *field = ( deForceField* )pFields.GetRoot();
	
	while( field ){
		field->SetPeerPhysics( NULL );
		field = ( deForceField* )field->GetLLManagerNext();
	}
}



void deForceFieldManager::RemoveResource( deResource *resource ){
	pFields.RemoveIfPresent( resource );
}
