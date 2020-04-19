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

#include "meUObjectDropToGround.h"
#include "meUndoDataObject.h"
#include "../../../collisions/meCLClosestElement.h"
#include "../../../filter/meFilterObjectsByClass.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/exceptions.h>



// Class meUObjectDropToGround
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectDropToGround::meUObjectDropToGround( meWorld *world, const meObjectList &objects ){
	if( ! world ) DETHROW( deeInvalidParam );
	
	int count = objects.GetCount();
	meObject *object;
	decString text;
	
	if( count == 0 ) DETHROW( deeInvalidParam );
	
	pWorld = NULL;
	
	pDropOnObjects = true;
	pAlign = false;
	
	pObjects = NULL;
	pObjectCount = 0;
	
	SetShortInfo( "Drop objects to ground" );
	if( count > 1 ){
		text.Format( "%i objects", count );
		
	}else{
		text = "1 object";
	}
	SetLongInfo( text.GetString() );
	
	// TODO: sort the objects using their y coordinate. this way if they
	// are stuck inside each other they stack properly after the dropping
	// since they are dropped in the order they are stored in the list.
	
	try{
		pObjects = new sObject[ count ];
		if( ! pObjects ) DETHROW( deeOutOfMemory );
		
		for( pObjectCount=0; pObjectCount<count; pObjectCount++ ){
			object = objects.GetAt( pObjectCount );
			
			pObjects[ pObjectCount ].oldpos = object->GetPosition();
			pObjects[ pObjectCount ].oldrot = object->GetRotation();
			pObjects[ pObjectCount ].object = object;
			object->AddReference();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
	world->AddReference();
}

meUObjectDropToGround::~meUObjectDropToGround(){
	pCleanUp();
}



// Management
///////////////

void meUObjectDropToGround::SetDropOnObjects( bool dropOnObjects ){
	pDropOnObjects = dropOnObjects;
}

void meUObjectDropToGround::SetAlign( bool align ){
	pAlign = align;
}



void meUObjectDropToGround::Undo(){
	meObject *object;
	int o;
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ].object;
		object->SetPosition( pObjects[ o ].oldpos );
		object->SetRotation( pObjects[ o ].oldrot );
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meUObjectDropToGround::Redo(){
	deBasePhysicsWorld *phyWorld = pWorld->GetEngineWorld()->GetPeerPhysics();
	decVector dropDisplacement( 0.0f, -10.0f, 0.0f );
	meCLClosestElement closestElement( *pWorld );
	decDVector oldPosition, newPosition;
	decVector oldRotation, newRotation;
	meFilterObjectsByClass filter;
	deColliderVolume *collider;
	int testCounter = 0;
	meObject *object;
	float distance;
	int o;
	
	if( ! phyWorld ) return;
	
	closestElement.SetTestHeightTerrain( true );
	closestElement.SetTestObjects( pDropOnObjects );
	closestElement.SetTestDecals( false );
	closestElement.SetFilterObjects( &filter );
	
	filter.SetRejectGhosts( true );
	//filter.SetClassNamesFrom( guiparams.GetAddFilterObjectList() );
	//filter.SetMatchInclusive( guiparams.GetAddFilterObjectInclusive() );
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit( meWorld::eclmEditing );
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit( meWorld::eclmObjects );
	collisionFilter.SetBit( meWorld::eclmHeightTerrains );
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ].object;
		
		collider = object->GetColDetCollider();
		if( ! collider ){
			continue;
		}
		
		collider->SetCollisionFilter( decCollisionFilter( collisionCategory, collisionFilter ) );
		
		oldPosition = pObjects[ o ].oldpos;
		oldRotation = pObjects[ o ].oldrot;
		
		filter.SetRejectObject( object );
		
		while( testCounter < 50 ){
			closestElement.Reset();
			
			collider->SetPosition( oldPosition );
			collider->SetOrientation( decMatrix::CreateRotation( oldRotation * DEG2RAD ).ToQuaternion() );
			
			phyWorld->ColliderMoveHits( collider, dropDisplacement, &closestElement );
			
			if( closestElement.GetHasHit() ){
				distance = closestElement.GetHitDistance();
				
				if( distance > 0.001f ){
					newPosition = oldPosition + decDVector( dropDisplacement * distance );
					
					if( pAlign ){
						// TODO this alignment code is rather simple and not of good quality. one idea would
						// be to use physics simulation to drop the object onto the ground. another solution
						// would be to use multiple collision tests and slightly turning the object similar
						// to an inverse kinematic approach in animation.
						
						const decVector &normal = closestElement.GetHitNormal();
						
						newRotation.Set( acosf( normal.y ) / DEG2RAD, atan2f( normal.x, -normal.z ) / DEG2RAD, oldRotation.z );
						
						closestElement.Reset();
						
						collider->SetPosition( newPosition );
						collider->SetOrientation( decMatrix::CreateRotation( newRotation * DEG2RAD ).ToQuaternion() );
						
						phyWorld->ColliderMoveHits( collider, dropDisplacement, &closestElement );
						
						if( closestElement.GetHasHit() ){
							distance = closestElement.GetHitDistance();
							
							if( distance > 0.001f ){
								newPosition += decDVector( dropDisplacement * distance );
							}
						}
					}
					
					object->SetPosition( newPosition );
					if( pAlign ){
						object->SetRotation( newRotation );
					}
					//pObjects[ o ]->SetNewSector( wsNew->GetCoordinates() );
					
					pWorld->NotifyObjectGeometryChanged( object );
					
					break;
				}
			}
			
			oldPosition.y += 0.1f;
			testCounter++;
		}
	}
}



// Private Functions
//////////////////////

void meUObjectDropToGround::pCleanUp(){
	if( pObjects ){
		while( pObjectCount > 0 ){
			pObjectCount--;
			pObjects[ pObjectCount ].object->FreeReference();
		}
		
		delete [] pObjects;
	}
	
	if( pWorld ) pWorld->FreeReference();
}
