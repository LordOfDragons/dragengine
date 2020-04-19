/* 
 * Drag[en]gine IGDE World Editor
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

#include "mePropField.h"
#include "mePropFieldType.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>



// Class mePropField
//////////////////////

// Constructor, destructor
////////////////////////////

mePropField::mePropField( deEngine *engine ){
	if( ! engine ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	pWorld = NULL;
	pEngPF = NULL;
	
	pTypes = NULL;
	pTypeCount = 0;
	pTypeSize = 0;
	
	try{
		pEngPF = engine->GetPropFieldManager()->CreatePropField();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	// TEMPORARY
	mePropFieldType *pftype = NULL;
	try{
		pftype = new mePropFieldType( engine );
		pftype->SetPathModel( "/epsylon/models/vegetation/bush1/bush1.demodel" );
		pftype->SetPathSkin( "/epsylon/models/vegetation/bush1/512/bush1.deskin" );
		pftype->RebuildInstances();
		AddType( pftype );
		pftype->FreeReference();
		pftype = NULL;
		
	}catch( const deException & ){
		if( pftype ) pftype->FreeReference();
		pCleanUp();
		throw;
	}
}

mePropField::~mePropField(){
	pCleanUp();
}



// Management
///////////////

void mePropField::SetWorld( meWorld *world ){
	if( world != pWorld ){
		if( pWorld ){
			pWorld->GetEngineWorld()->RemovePropField( pEngPF );
		}
		
		pWorld = world;
		
		if( world ){
			world->GetEngineWorld()->AddPropField( pEngPF );
		}
	}
}



void mePropField::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		pEngPF->SetPosition( position );
		
		SetWorldChanged();
		if( pWorld ) pWorld->NotifyPFChanged( this );
	}
}

void mePropField::SetWorldChanged(){
	if( pWorld ) pWorld->SetChanged( true );
}



// Types
//////////

mePropFieldType *mePropField::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ) DETHROW( deeOutOfBoundary );
	
	return pTypes[ index ];
}

int mePropField::IndexOfType( mePropFieldType *type ) const{
	if( ! type ) DETHROW( deeInvalidParam );
	int t;
	
	for( t=0; t<pTypeCount; t++ ){
		if( pTypes[ t ] == type ){
			return t;
		}
	}
	
	return -1;
}

void mePropField::AddType( mePropFieldType *type ){
	if( ! type ) DETHROW( deeInvalidParam );
	
	if( pTypeCount == pTypeSize ){
		int newSize = pTypeSize * 3 / 2 + 1;
		mePropFieldType **newArray = new mePropFieldType*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pTypes ){
			memcpy( newArray, pTypes, sizeof( mePropFieldType* ) * pTypeSize );
			delete [] pTypes;
		}
		pTypes = newArray;
		pTypeSize = newSize;
	}
	
	pTypes[ pTypeCount ] = type;
	pTypeCount++;
	
	type->SetPropField( this );
	type->AddReference();
	
	SetWorldChanged();
	
	// engine object
	dePropFieldType *engPFType = NULL;
	
	try{
		engPFType = type->CreateEnginePFType();
		if( engPFType ) pEngPF->AddType( engPFType );
		type->SetEnginePFType( engPFType );
		
	}catch( const deException & ){
		if( engPFType ) delete engPFType;
		throw;
	}
}

void mePropField::RemoveType( mePropFieldType *type ){
	int t, index = IndexOfType( type );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( t=index+1; t<pTypeCount; t++ ){
		pTypes[ t - 1 ] = pTypes[ t ];
	}
	pTypeCount--;
	
	if( pEngPF && type->GetEnginePFType() ){
		pEngPF->RemoveType( type->GetEnginePFType() );
		type->SetEnginePFType( NULL );
	}
	type->SetPropField( NULL );
	type->FreeReference();
	
	SetWorldChanged();
}

void mePropField::RemoveAllTypes(){
	while( pTypeCount > 0 ){
		pTypeCount--;
		pTypes[ pTypeCount ]->SetEnginePFType( NULL );
		pTypes[ pTypeCount ]->SetPropField( NULL );
		pTypes[ pTypeCount ]->FreeReference();
	}
	
	pEngPF->RemoveAllTypes();
	
	SetWorldChanged();
}



// Private Functions
//////////////////////

void mePropField::pCleanUp(){
	SetWorld( NULL );
	
	RemoveAllTypes();
	if( pTypes ) delete [] pTypes;
	
	if( pEngPF ) pEngPF->FreeReference();
}

