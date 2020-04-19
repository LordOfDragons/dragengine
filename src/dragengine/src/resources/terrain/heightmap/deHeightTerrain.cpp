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

#include "deHeightTerrain.h"
#include "deHeightTerrainManager.h"
#include "deHeightTerrainSector.h"
#include "../../decal/deDecalList.h"
#include "../../../deEngine.h"
#include "../../../deObjectReference.h"
#include "../../../common/exceptions.h"
#include "../../../systems/modules/ai/deBaseAIHeightTerrain.h"
#include "../../../systems/modules/audio/deBaseAudioHeightTerrain.h"
#include "../../../systems/modules/graphic/deBaseGraphicHeightTerrain.h"
#include "../../../systems/modules/physics/deBasePhysicsHeightTerrain.h"



// Class deHeightTerrain
//////////////////////////

// Constructor, destructor
////////////////////////////

deHeightTerrain::deHeightTerrain( deHeightTerrainManager *manager,
	float sectorSize, int sectorResolution ) :
deResource( manager ),
pSectorSize( sectorSize ),
pSectorResolution( sectorResolution ),
pBaseHeight( 0.0f ),
pHeightScaling( 1.0f ),
pPeerGraphic( NULL ),
pPeerPhysics( NULL ),
pPeerAudio( NULL ),
pPeerAI( NULL )
{
	if( sectorSize < 0.001f || sectorResolution < 2 ){
		DETHROW( deeInvalidParam );
	}
}

deHeightTerrain::~deHeightTerrain(){
	pCleanUp();
}



// Management
///////////////

void deHeightTerrain::SetBaseHeight( float height ){
	if( fabsf( height - pBaseHeight ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBaseHeight = height;
}

void deHeightTerrain::SetHeightScaling( float scaling ){
	scaling = decMath::max( scaling, 0.0f );
	if( fabsf( scaling - pHeightScaling ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pHeightScaling = scaling;
}

void deHeightTerrain::SetCollisionFilter( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = collisionFilter;
	
	if( pPeerPhysics ){
		pPeerPhysics->CollisionFilterChanged();
	}
}



void deHeightTerrain::NotifyHeightChanged( const decPoint &fromSector,
const decPoint &fromCoordinates, const decPoint &toSector, const decPoint &toCoordinates ){
	if( pPeerGraphic ){
		pPeerGraphic->HeightChanged( fromSector, fromCoordinates, toSector, toCoordinates );
	}
	if( pPeerPhysics ){
		pPeerPhysics->HeightChanged( fromSector, fromCoordinates, toSector, toCoordinates );
	}
	if( pPeerAudio ){
		pPeerAudio->HeightChanged( fromSector, fromCoordinates, toSector, toCoordinates );
	}
	if( pPeerAI ){
		pPeerAI->HeightChanged( fromSector, fromCoordinates, toSector, toCoordinates );
	}
}



// Sectors
////////////

int deHeightTerrain::GetSectorCount() const{
	return pSectors.GetCount();
}

deHeightTerrainSector *deHeightTerrain::GetSectorAt( int index ) const{
	return ( deHeightTerrainSector* )pSectors.GetAt( index );
}

deHeightTerrainSector *deHeightTerrain::GetSectorWith( const decPoint &coordinates ) const{
	const int count = pSectors.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deHeightTerrainSector * const sector = ( deHeightTerrainSector* )pSectors.GetAt( i );
		if( sector->GetSector() == coordinates ){
			return sector;
		}
	}
	
	return NULL;
}

void deHeightTerrain::AddSector( deHeightTerrainSector *sector ){
	if( ! sector || sector->GetParentHeightTerrain() ){
		DETHROW( deeInvalidParam );
	}
	
	const int index = pSectors.GetCount();
	pSectors.Add( sector );
	sector->SetParentHeightTerrain( this );
	sector->SetIndex( index );
	
	if( pPeerGraphic ){
		pPeerGraphic->SectorAdded( sector );
	}
	if( pPeerPhysics ){
		pPeerPhysics->SectorAdded( sector );
	}
	if( pPeerAudio ){
		pPeerAudio->SectorAdded( sector );
	}
	if( pPeerAI ){
		pPeerAI->SectorAdded( sector );
	}
}

void deHeightTerrain::RemoveSector( deHeightTerrainSector *sector ){
	if( ! sector || sector->GetParentHeightTerrain() != this ){
		DETHROW( deeInvalidParam );
	}
	
	const int index = sector->GetIndex();
	sector->SetIndex( -1 );
	sector->SetParentHeightTerrain( NULL );
	pSectors.RemoveFrom( index );
	
	const int count = pSectors.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deHeightTerrainSector* )pSectors.GetAt( i ) )->SetIndex( i );
	}
	
	try{
		if( pPeerGraphic ){
			pPeerGraphic->SectorRemoved( index );
		}
		if( pPeerPhysics ){
			pPeerPhysics->SectorRemoved( index );
		}
		if( pPeerAudio ){
			pPeerAudio->SectorRemoved( index );
		}
		if( pPeerAI ){
			pPeerAI->SectorRemoved( index );
		}
		
	}catch( const deException & ){
		delete sector;
		throw;
	}
	
	delete sector;
}

void deHeightTerrain::RemoveAllSectors(){
	if( pPeerGraphic ){
		pPeerGraphic->AllSectorsRemoved();
	}
	if( pPeerPhysics ){
		pPeerPhysics->AllSectorsRemoved();
	}
	if( pPeerAudio ){
		pPeerAudio->AllSectorsRemoved();
	}
	if( pPeerAI ){
		pPeerAI->AllSectorsRemoved();
	}
	
	const int count = pSectors.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deHeightTerrainSector * const sector = ( deHeightTerrainSector* )pSectors.GetAt( i );
		sector->SetIndex( -1 );
		sector->SetParentHeightTerrain( NULL );
		delete sector;
	}
	pSectors.RemoveAll();
}

void deHeightTerrain::NotifySectorChanged( int sector ){
	if( pPeerGraphic ){
		pPeerGraphic->SectorChanged( sector );
	}
	if( pPeerPhysics ){
		pPeerPhysics->SectorChanged( sector );
	}
	if( pPeerAudio ){
		pPeerAudio->SectorChanged( sector );
	}
	if( pPeerAI ){
		pPeerAI->SectorChanged( sector );
	}
}



// Collision Detection
////////////////////////

void deHeightTerrain::FindDecalsAt( const decDVector &point, deDecalList &list ){
	if( pPeerPhysics ){
		pPeerPhysics->FindDecalsAt( point, list );
	}
}

void deHeightTerrain::FindDecalsTouching( const decShape &shape, deDecalList &list ){
	if( pPeerPhysics ){
		pPeerPhysics->FindDecalsTouching( shape, list );
	}
}



// System Peers
/////////////////

void deHeightTerrain::SetPeerGraphic( deBaseGraphicHeightTerrain *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}

void deHeightTerrain::SetPeerPhysics( deBasePhysicsHeightTerrain *peer ){
	if( peer == pPeerPhysics ){
		return;
	}
	
	if( pPeerPhysics ){
		delete pPeerPhysics;
	}
	pPeerPhysics = peer;
}

void deHeightTerrain::SetPeerAudio( deBaseAudioHeightTerrain *peer ){
	if( peer == pPeerAudio ){
		return;
	}
	
	if( pPeerAudio ){
		delete pPeerAudio;
	}
	pPeerAudio = peer;
}

void deHeightTerrain::SetPeerAI( deBaseAIHeightTerrain *peer ){
	if( peer == pPeerAI ){
		return;
	}
	
	if( pPeerAI ){
		delete pPeerAI;
	}
	pPeerAI = peer;
}



// Private functions
//////////////////////

void deHeightTerrain::pCleanUp(){
	if( pPeerAI ){
		delete pPeerAI;
		pPeerAI = NULL;
	}
	if( pPeerAudio ){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
	if( pPeerPhysics ){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	RemoveAllSectors();
}
