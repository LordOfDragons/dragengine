/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRHeightTerrain.h"
#include "deoglRHTSector.h"
#include "deoglHeightTerrainListener.h"

#include "../../renderthread/deoglRenderThread.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>



// Class deoglRHeightTerrain
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRHeightTerrain::deoglRHeightTerrain( deoglRenderThread &renderThread, const deHeightTerrain &heightTerrain ) :
pRenderThread( renderThread ),
pParentWorld( NULL ),
pSectorResolution( heightTerrain.GetSectorResolution() ),
pSectorSize( heightTerrain.GetSectorSize() ),
pSectorsRequirePrepareForRender( true ),
pListenerIndex( 0 )
{
	LEAK_CHECK_CREATE( renderThread, HeightTerrain );
}

deoglRHeightTerrain::~deoglRHeightTerrain(){
	LEAK_CHECK_FREE( pRenderThread, HeightTerrain );
	NotifyHeightTerrainDestroyed();
	pListeners.RemoveAll();
	SetParentWorld( NULL );
}



// Management
///////////////

void deoglRHeightTerrain::SetParentWorld( deoglRWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	const int count = pSectors.GetCount();
	int i;
	
	if( pParentWorld ){
		for( i=0; i<count; i++ ){
			( ( deoglRHTSector* )pSectors.GetAt( i ) )->RemoveFromWorldCompute();
		}
	}
	
	pParentWorld = world;
	
	if( world ){
		deoglWorldCompute &worldCompute = world->GetCompute();
		for( i=0; i<count; i++ ){
			( ( deoglRHTSector* )pSectors.GetAt( i ) )->AddToWorldCompute( worldCompute );
		}
	}
}

void deoglRHeightTerrain::PrepareForRender(){
	if( pSectorsRequirePrepareForRender ){
		pSectorsRequirePrepareForRender = false;
		
		const int count = pSectors.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			( ( deoglRHTSector* )pSectors.GetAt( i ) )->PrepareForRender();
		}
	}
}



int deoglRHeightTerrain::GetSectorCount() const{
	return pSectors.GetCount();
}

deoglRHTSector &deoglRHeightTerrain::GetSectorAt( int index ) const{
	return *( ( deoglRHTSector* )pSectors.GetAt( index ) );
}

void deoglRHeightTerrain::AddSector( deoglRHTSector *htsector ){
	htsector->SetIndex( pSectors.GetCount() );
	pSectors.Add( htsector );
	if( pParentWorld ){
		htsector->AddToWorldCompute( pParentWorld->GetCompute() );
	}
	
	SectorRequirePrepareForRender();
	NotifySectorsChanged();
}

void deoglRHeightTerrain::RemoveAllSectors(){
	if( pParentWorld ){
		const int count = pSectors.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			( ( deoglRHTSector* )pSectors.GetAt( i ) )->RemoveFromWorldCompute();
		}
	}
	
	pSectors.RemoveAll();
	SectorRequirePrepareForRender();
	NotifySectorsChanged();
}

void deoglRHeightTerrain::SectorRequirePrepareForRender(){
	pSectorsRequirePrepareForRender = true;
}



// Listeners
//////////////

void deoglRHeightTerrain::AddListener( deoglHeightTerrainListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void deoglRHeightTerrain::RemoveListener( deoglHeightTerrainListener *listener ){
	const int index = pListeners.IndexOf( listener );
	if( index == -1 ){
		return;
	}
	
	pListeners.Remove( listener );
	
	if( pListenerIndex >= index ){
		pListenerIndex--;
	}
}

void deoglRHeightTerrain::NotifyHeightTerrainDestroyed(){
	pListenerIndex = 0;
	while( pListenerIndex < pListeners.GetCount() ){
		( ( deoglHeightTerrainListener* )pListeners.GetAt( pListenerIndex ) )->HeightTerrainDestroyed( *this );
		pListenerIndex++;
	}
}

void deoglRHeightTerrain::NotifySectorsChanged(){
	pListenerIndex = 0;
	while( pListenerIndex < pListeners.GetCount() ){
		( ( deoglHeightTerrainListener* )pListeners.GetAt( pListenerIndex ) )->SectorsChanged( *this );
		pListenerIndex++;
	}
}
