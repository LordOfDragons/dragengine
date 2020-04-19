/* 
 * Drag[en]gine Python Script Module
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

#include "spTypeWorld.h"
#include "../spBaseModule.h"
#include "../../ScriptingPython.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/exceptions.h>



// Python Binding
///////////////////

struct sObjectData{
	deWorld *world;
};



// Class spTypeWorld
////////////////////////

// Constructor, destructor
////////////////////////////

spTypeWorld::spTypeWorld( ScriptingPython &sp ) : spBaseType( sp, "World" ){
	SetDocumentation( "World resource" );
	SetNativeDataSize( sizeof( sObjectData ) );
	SetParent( sp.GetNamespaceScenery() );
	
	AddMethod( "update", ( PyCFunction )cfUpdate, METH_VARARGS, "Updates the world." );
}

spTypeWorld::~spTypeWorld(){
	PythonCleanUp();
}



// Management
///////////////

deWorld *spTypeWorld::WorldFromObject( PyObject *object ){
	if( object ){
		return ( ( sObjectData* )GetObjectData( object ) )->world;
		
	}else{
		return NULL;
	}
}

PyObject *spTypeWorld::ObjectFromWorld( deWorld *world ){
	if( world ){
		PyObject *pyoWorld = CreatePyObject();
		
		sObjectData &od = *( ( sObjectData* )GetObjectData( pyoWorld ) );
		od.world = world;
		world->AddReference();
		
		return pyoWorld;
		
	}else{
		Py_INCREF( Py_None );
		return Py_None;
	}
}



void spTypeWorld::Constructor( PyObject *myself, PyObject *args, PyObject *kwds ){
	sObjectData &od = *( ( sObjectData* )GetObjectData( myself ) );
	
	od.world = NULL;
	
	od.world = GetSP().GetGameEngine()->GetWorldManager()->CreateWorld();
}

void spTypeWorld::Destructor( PyObject *myself ) {
	sObjectData &od = *( ( sObjectData* )GetObjectData( myself ) );
	
	if( od.world ){
		od.world->FreeReference();
		od.world = NULL;
	}
}



// Functions
//////////////

PyObject *spTypeWorld::cfUpdate( PyObject *myself, PyObject *args ){
	deWorld &world = *( ( ( sObjectData* )GetObjectData( myself ) )->world );
	float elapsed;
	
	if( ! PyArg_ParseTuple( args, "f", &elapsed ) ){
		return NULL;
	}
	
	world.Update( elapsed );
	
	Py_INCREF( Py_None );
	return Py_None;
}
