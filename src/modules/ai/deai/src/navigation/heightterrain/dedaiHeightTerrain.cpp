/* 
 * Drag[en]gine AI Module
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
#include <string.h>

#include "dedaiHeightTerrain.h"
#include "dedaiHeightTerrainNavSpace.h"
#include "dedaiHeightTerrainSector.h"
#include "../../deDEAIModule.h"
#include "../../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>



// Class dedaiHeightTerrain
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiHeightTerrain::dedaiHeightTerrain( deDEAIModule &deai, const deHeightTerrain &heightTerrain ) :
pDEAI( deai ),
pHeightTerrain( heightTerrain ),
pParentWorld( NULL )
{
	const int sectorCount = heightTerrain.GetSectorCount();
	int i;
	
	try{
		for( i=0; i<sectorCount; i++ ){
			SectorAdded( heightTerrain.GetSectorAt( i ) );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dedaiHeightTerrain::~dedaiHeightTerrain(){
	pCleanUp();
}



// Management
///////////////

void dedaiHeightTerrain::SetParentWorld( dedaiWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	pParentWorld = world;
	
	const int sectorCount = pSectors.GetCount();
	int i;
	
	for( i=0; i<sectorCount; i++ ){
		( ( dedaiHeightTerrainSector* )pSectors.GetAt( i ) )->ParentWorldChanged();
	}
}



// Sectors
////////////

int dedaiHeightTerrain::GetSectorCount() const{
	return pSectors.GetCount();
}

dedaiHeightTerrainSector *dedaiHeightTerrain::GetSectorAt( int index ) const{
	return ( dedaiHeightTerrainSector* )pSectors.GetAt( index );
}



// Notifications
//////////////////

void dedaiHeightTerrain::ParametersChanged(){
	const int sectorCount = pSectors.GetCount();
	int i;
	
	for( i=0; i<sectorCount; i++){
		( ( dedaiHeightTerrainSector* )pSectors.GetAt( i ) )->SectorChanged();
	}
}

void dedaiHeightTerrain::SectorAdded( deHeightTerrainSector *sector ){
	deObjectReference refSector;
	refSector.TakeOver( new dedaiHeightTerrainSector( *this, *sector ) );
	pSectors.Add( refSector );
}

void dedaiHeightTerrain::SectorRemoved( int index ){
	pSectors.RemoveFrom( index );
}

void dedaiHeightTerrain::AllSectorsRemoved(){
	pSectors.RemoveAll();
}

void dedaiHeightTerrain::SectorChanged( int index ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( index ) )->SectorChanged();
}

void dedaiHeightTerrain::HeightChanged( const decPoint &fromSector,
const decPoint &fromCoordinates, const decPoint &toSector, const decPoint &toCoordinates ){
	const int imageDim = pHeightTerrain.GetSectorResolution();
	const int sectorCount = pSectors.GetCount();
	decPoint localFrom, localTo;
	int i;
	
	for( i=0; i<sectorCount; i++ ){
		dedaiHeightTerrainSector &sector = *( ( dedaiHeightTerrainSector* )pSectors.GetAt( i ) );
		const decPoint &scoord = sector.GetSector().GetSector();
		
		if( scoord.x + 1 >= fromSector.x && scoord.x - 1 <= toSector.x
		&& scoord.y + 1 >= fromSector.y && scoord.y - 1 <= toSector.y ){
			if( fromSector.x == toSector.x ){
				localFrom.x = fromCoordinates.x;
				localTo.x = toCoordinates.x;
				
			}else{
				if( scoord.x == toSector.x ){
					localFrom.x = toCoordinates.x;
					
				}else{
					localFrom.x = 0;
				}
				
				if( scoord.x == fromSector.x ){
					localTo.x = fromCoordinates.x;
					
				}else{
					localTo.x = imageDim;
				}
			}
			
			if( fromSector.y == toSector.y ){
				localFrom.y = fromCoordinates.y;
				localTo.y = toCoordinates.y;
				
			}else{
				if( scoord.y == toSector.y ){
					localFrom.y = toCoordinates.y;
					
				}else{
					localFrom.y = 0;
				}
				
				if( scoord.y == fromSector.y ){
					localTo.y = fromCoordinates.y;
					
				}else{
					localTo.y = imageDim;
				}
			}
			
			if( localFrom.x > 0 ){
				localFrom.x--;
			}
			if( localFrom.y > 0 ){
				localFrom.y--;
			}
			if( localTo.x < imageDim ){
				localTo.x++;
			}
			if( localTo.y < imageDim ){
				localTo.y++;
			}
			
			sector.HeightChanged( localFrom, localTo );
		}
	}
}

void dedaiHeightTerrain::NavSpaceAdded( int sector, deHeightTerrainNavSpace *navspace ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( sector ) )->NavSpaceAdded( navspace );
}

void dedaiHeightTerrain::NavSpaceRemoved( int sector, int index ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( sector ) )->NavSpaceRemoved( index );
}

void dedaiHeightTerrain::AllNavSpacesRemoved( int sector ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( sector ) )->AllNavSpacesRemoved();
}

void dedaiHeightTerrain::NavSpaceLayerChanged( int sector, int index ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( sector ) )->NavSpaceLayerChanged( index );
}

void dedaiHeightTerrain::NavSpaceTypeChanged( int sector, int index ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( sector ) )->NavSpaceTypeChanged( index );
}

void dedaiHeightTerrain::NavSpaceSnappingChanged( int sector, int index ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( sector ) )->NavSpaceSnappingChanged( index );
}

void dedaiHeightTerrain::NavSpaceLayoutChanged( int sector, int index ){
	( ( dedaiHeightTerrainSector* )pSectors.GetAt( sector ) )->NavSpaceLayoutChanged( index );
}



// Private Functions
//////////////////////

void dedaiHeightTerrain::pCleanUp(){
	SetParentWorld( NULL );
	AllSectorsRemoved();
}
