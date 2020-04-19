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

#include "dePropField.h"
#include "dePropFieldManager.h"
#include "dePropFieldType.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicPropField.h"
#include "../../systems/modules/physics/deBasePhysicsPropField.h"
#include "../../systems/modules/scripting/deBaseScriptingPropField.h"
#include "../../common/exceptions.h"



// Class dePropField
//////////////////////

// Constructor, destructor
////////////////////////////

dePropField::dePropField( dePropFieldManager *manager ) : deResource( manager ){
	pTypes = NULL;
	pTypeCount = 0;
	pTypeSize = 0;
	
	pPeerGraphic = NULL;
	pPeerPhysics = NULL;
	pPeerScripting = NULL;
	
	pParentWorld = NULL;
	pLLWorldNext = NULL;
	pLLWorldPrev = NULL;
}

dePropField::~dePropField(){
	if( pPeerScripting ){
		delete pPeerScripting;
		pPeerScripting = NULL;
	}
	if( pPeerPhysics ){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	RemoveAllTypes();
	if( pTypes ) delete [] pTypes;
}



// Types
//////////

void dePropField::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		if( pPeerGraphic ) pPeerGraphic->PositionChanged();
		if( pPeerPhysics ) pPeerPhysics->PositionChanged();
	}
}



dePropFieldType *dePropField::GetTypeAt( int index ) const{
	if( index < 0 || index >= pTypeCount ) DETHROW( deeOutOfBoundary );
	
	return pTypes[ index ];
}

int dePropField::IndexOfType( dePropFieldType *type ) const{
	if( ! type ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pTypeCount; i++ ){
		if( type == pTypes[ i ] ){
			return i;
		}
	}
	
	return -1;
}

void dePropField::AddType( dePropFieldType *type ){
	if( ! type ) DETHROW( deeInvalidParam );
	
	if( pTypeCount == pTypeSize ){
		int newSize = pTypeSize * 3 / 2 + 1;
		dePropFieldType **newArray = new dePropFieldType*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pTypes ){
			memcpy( newArray, pTypes, sizeof( dePropFieldType* ) * pTypeSize );
			delete [] pTypes;
		}
		pTypes = newArray;
		pTypeSize = newSize;
	}
	
	pTypes[ pTypeCount ] = type;
	pTypeCount++;
	
	if( pPeerGraphic ) pPeerGraphic->TypeAdded( pTypeCount - 1, type );
	if( pPeerPhysics ) pPeerPhysics->TypeAdded( pTypeCount - 1, type );
}

void dePropField::RemoveType( dePropFieldType *type ){
	int i, index = IndexOfType( type );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pTypeCount; i++ ){
		pTypes[ i - 1 ] = pTypes[ i ];
	}
	pTypeCount--;
	
	if( pPeerGraphic ) pPeerGraphic->TypeRemoved( index, type );
	if( pPeerPhysics ) pPeerPhysics->TypeRemoved( index, type );
	
	delete type;
}

void dePropField::RemoveAllTypes(){
	if( pPeerGraphic ) pPeerGraphic->AllTypesRemoved();
	if( pPeerPhysics ) pPeerPhysics->AllTypesRemoved();
	
	while( pTypeCount > 0 ){
		pTypeCount--;
		delete pTypes[ pTypeCount ];
	}
}



void dePropField::NotifyTypeChanged( int index ){
	if( index < 0 || index >= pTypeCount ) DETHROW( deeOutOfBoundary );
	
	if( pPeerGraphic ) pPeerGraphic->TypeChanged( index, pTypes[ index ] );
	if( pPeerPhysics ) pPeerPhysics->TypeChanged( index, pTypes[ index ] );
}

void dePropField::NotifyInstancesChanged( int type ){
	if( type < 0 || type >= pTypeCount ) DETHROW( deeOutOfBoundary );
	
	if( pPeerGraphic ) pPeerGraphic->InstancesChanged( type, pTypes[ type ] );
	if( pPeerPhysics ) pPeerPhysics->InstancesChanged( type, pTypes[ type ] );
}

void dePropField::NotifyAssignmentsChanged( int type ){
	if( type < 0 || type >= pTypeCount ) DETHROW( deeOutOfBoundary );
	
	if( pPeerGraphic ) pPeerGraphic->AssignmentsChanged( type, pTypes[ type ] );
}

void dePropField::NotifyBendStatesChanged( int type ){
	if( type < 0 || type >= pTypeCount ) DETHROW( deeOutOfBoundary );
	
	if( pPeerGraphic ) pPeerGraphic->BendStatesChanged( type, pTypes[ type ] );
}

void dePropField::NotifyGroundChanged(){
	if( pPeerGraphic ) pPeerGraphic->GroundChanged();
}

void dePropField::NotifyCreateInstances( float density ){
	if( pPeerScripting ){
		pPeerScripting->CreateInstances( density );
		
	}else{
		// usually we do nothing if peers do not exist. in this case though the performance of
		// the modules depends on the result of this call. to avoid any problems in the case
		// of a missing peer we simply remove all instances from all types without touching the
		// types at all. this way no performance hit is taken as the worst that can happen is
		// that no props are rendered at all which is not as bad as cringing to a crawl
		int t;
		
		for( t=0; t<pTypeCount; t++ ){
			pTypes[ t ]->SetInstanceCount( 0 );
		}
	}
}

void dePropField::NotifyProjectInstances( const dePropFieldGround &ground, const decVector &direction ){
	if( pPeerPhysics ) pPeerPhysics->ProjectInstances( ground, direction );
}



// System Peers
/////////////////

void dePropField::SetPeerGraphic( deBaseGraphicPropField *peer ){
	if( pPeerGraphic) delete pPeerGraphic;
	pPeerGraphic = peer;
}

void dePropField::SetPeerPhysics( deBasePhysicsPropField *peer ){
	if( pPeerPhysics ) delete pPeerPhysics;
	pPeerPhysics = peer;
}

void dePropField::SetPeerScripting( deBaseScriptingPropField *peer ){
	if( pPeerScripting ) delete pPeerScripting;
	pPeerScripting = peer;
}



// Linked List
////////////////

void dePropField::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void dePropField::SetLLWorldPrev( dePropField *propField ){
	pLLWorldPrev = propField;
}

void dePropField::SetLLWorldNext( dePropField *propField ){
	pLLWorldNext = propField;
}
