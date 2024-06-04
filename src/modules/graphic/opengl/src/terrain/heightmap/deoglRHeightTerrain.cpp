/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
