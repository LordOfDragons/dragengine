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

#include "dePropField.h"
#include "dePropFieldManager.h"

#include "../../deEngine.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../systems/deScriptingSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/graphic/deBaseGraphicPropField.h"
#include "../../systems/modules/physics/deBasePhysicsPropField.h"
#include "../../systems/modules/scripting/deBaseScriptingPropField.h"

#include "../../common/exceptions.h"



// Class dePropFieldManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

dePropFieldManager::dePropFieldManager( deEngine *engine ) : deResourceManager( engine, ertPropField ){
	SetLoggingName( "prop field" );
}

dePropFieldManager::~dePropFieldManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int dePropFieldManager::GetPropFieldCount() const{
	return pFields.GetCount();
}

dePropField *dePropFieldManager::GetRootPropField() const{
	return ( dePropField* )pFields.GetRoot();
}

dePropField *dePropFieldManager::CreatePropField(){
	dePropField *field = NULL;
	
	try{
		field = new dePropField( this );
		
		GetGraphicSystem()->LoadPropField( field );
		GetPhysicsSystem()->LoadPropField( field );
		GetScriptingSystem()->LoadPropField( field );
		
		pFields.Add( field );
		
	}catch( const deException & ){
		if( field ){
			field->FreeReference();
		}
		throw;
	}
	
	return field;
}



void dePropFieldManager::ReleaseLeakingResources(){
	if( GetPropFieldCount() > 0 ){
		LogWarnFormat( "%i leaking prop fields", GetPropFieldCount() );
		pFields.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void dePropFieldManager::SystemGraphicLoad(){
	dePropField *field = ( dePropField* )pFields.GetRoot();
	
	while( field ){
		if( ! field->GetPeerGraphic() ){
			GetGraphicSystem()->LoadPropField( field );
		}
		
		field = ( dePropField* )field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemGraphicUnload(){
	dePropField *field = ( dePropField* )pFields.GetRoot();
	
	while( field ){
		field->SetPeerGraphic( NULL );
		field = ( dePropField* )field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemPhysicsLoad(){
	dePropField *field = ( dePropField* )pFields.GetRoot();
	
	while( field ){
		if( ! field->GetPeerPhysics() ){
			GetPhysicsSystem()->LoadPropField( field );
		}
		
		field = ( dePropField* )field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemPhysicsUnload(){
	dePropField *field = ( dePropField* )pFields.GetRoot();
	
	while( field ){
		field->SetPeerPhysics( NULL );
		field = ( dePropField* )field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemScriptingLoad(){
	dePropField *field = ( dePropField* )pFields.GetRoot();
	
	while( field ){
		if( ! field->GetPeerPhysics() ){
			GetScriptingSystem()->LoadPropField( field );
		}
		
		field = ( dePropField* )field->GetLLManagerNext();
	}
}

void dePropFieldManager::SystemScriptingUnload(){
	dePropField *field = ( dePropField* )pFields.GetRoot();
	
	while( field ){
		field->SetPeerScripting( NULL );
		field = ( dePropField* )field->GetLLManagerNext();
	}
}



void dePropFieldManager::RemoveResource( deResource *resource ){
	pFields.RemoveIfPresent( resource );
}
