/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpHTSector.h"
#include "debpHeightTerrain.h"

#include "../../world/debpWorld.h"

#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/common/exceptions.h>



// Class debpHeightTerrain
////////////////////////////

// Constructor, destructor
////////////////////////////

debpHeightTerrain::debpHeightTerrain( dePhysicsBullet *bullet, deHeightTerrain *heightTerrain ){
	if( ! bullet || ! heightTerrain ) DETHROW( deeInvalidParam );
	
	int s, sectorCount = heightTerrain->GetSectorCount();
	
	pBullet = bullet;
	pHeightTerrain = heightTerrain;
	
	pParentWorld = NULL;
	
	pSectors = NULL;
	pSectorCount = 0;
	pSectorSize = 0;
	
	try{
		for( s=0; s<sectorCount; s++ ){
			SectorAdded( heightTerrain->GetSectorAt( s ) );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

debpHeightTerrain::~debpHeightTerrain(){
	pCleanUp();
}



// Management
///////////////

void debpHeightTerrain::SetParentWorld( debpWorld *parentWorld ){
	int s;
	
	pParentWorld = parentWorld;
	
	for( s=0; s<pSectorCount; s++ ){
		pSectors[ s ]->ParentWorldChanged();
	}
}



debpHTSector *debpHeightTerrain::GetSectorAt( int index ) const{
	if( index < 0 || index >= pSectorCount ) DETHROW( deeInvalidParam );
	
	return pSectors[ index ];
}

debpHTSector *debpHeightTerrain::GetSectorWith( int x, int z ) const{
	int s;
	
	for( s=0; s<pSectorCount; s++ ){
		const decPoint &scoord = pSectors[ s ]->GetSector()->GetSector();
		
		if( x == scoord.x && z == scoord.y ){
			return pSectors[ s ];
		}
	}
	
	return NULL;
}



void debpHeightTerrain::Update(){
	int s;
	
	for( s=0; s<pSectorCount; s++ ){
		pSectors[ s ]->Update();
	}
}



// Notifications
//////////////////

void debpHeightTerrain::ParametersChanged(){
}

void debpHeightTerrain::CollisionFilterChanged(){
	/*
	int s;
	
	for( s=0; s<pTouchSensorCount; s++ ){
		pTouchSensors[ s ]->TestTerrain( this );
	}
	*/
}

void debpHeightTerrain::HeightChanged( const decPoint &fromSector, const decPoint &fromCoordinates,
const decPoint &toSector, const decPoint &toCoordinates ){
	int imageDim = pHeightTerrain->GetSectorResolution();
	int s;
	
	for( s=0; s<pSectorCount; s++ ){
//		const decPoint &scoord = pSectors[ s ]->GetSector()->GetSector();
		
//		if( scoord.x >= fromSector.x && scoord.y >= fromSector.y && scoord.x <= toSector.x && scoord.y <= toSector.y ){
			// TODO determine the area to update
			pSectors[ s ]->UpdateHeights( 0, 0, imageDim, imageDim );
//		}
	}
}



void debpHeightTerrain::SectorAdded( deHeightTerrainSector *sector )
{
	debpHTSector *bpsector = NULL;
	
	try{
		if( pSectorCount == pSectorSize ){
			int newSize = pSectorSize * 3 / 2 + 1;
			debpHTSector **newArray = new debpHTSector*[ newSize ];
			if( ! newArray ) DETHROW( deeOutOfMemory );
			if( pSectors ){
				memcpy( newArray, pSectors, sizeof( debpHTSector* ) * pSectorSize );
				delete [] pSectors;
			}
			pSectors = newArray;
			pSectorSize = newSize;
		}
		
		bpsector = new debpHTSector( this, sector );
		if( ! bpsector ) DETHROW( deeOutOfMemory );
		
		pSectors[ pSectorCount++ ] = bpsector;
		
	}catch( const deException & ){
		if( bpsector ) delete bpsector;
		throw;
	}
}

void debpHeightTerrain::SectorRemoved( int index ){
	if( index < 0 || index >= pSectorCount ) DETHROW( deeInvalidParam );
	
	debpHTSector *bpsector = pSectors[ index ];
	int i;
	
	for( i=index+1; i<pSectorCount; i++ ){
		pSectors[ i - 1 ] = pSectors[ i ];
	}
	pSectorCount--;
	
	delete bpsector;
}

void debpHeightTerrain::AllSectorsRemoved(){
	while( pSectorCount > 0 ){
		pSectorCount--;
		delete pSectors[ pSectorCount ];
	}
}

void debpHeightTerrain::SectorChanged( int index ){
	if( index < 0 || index >= pSectorCount ) DETHROW( deeInvalidParam );
	
	pSectors[ index ]->SectorChanged();
}



void debpHeightTerrain::DecalAdded( int sector, deDecal *decal ){
}

void debpHeightTerrain::DecalRemoved( int sector, deDecal *decal ){
}

void debpHeightTerrain::AllDecalsRemoved( int sector ){
}



// Collision Detection
////////////////////////

void debpHeightTerrain::FindDecalsAt( const decDVector &point, deDecalList &list ){
}

void debpHeightTerrain::FindDecalsTouching( const decShape &shape, deDecalList &list ){
}



// Private functions
//////////////////////

void debpHeightTerrain::pCleanUp(){
	AllSectorsRemoved();
	if( pSectors ) delete [] pSectors;
}
