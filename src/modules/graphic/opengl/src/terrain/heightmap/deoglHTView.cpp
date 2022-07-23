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

#include "deoglHTView.h"
#include "deoglHTViewSector.h"
#include "deoglHTSector.h"
#include "deoglHTSCluster.h"
#include "deoglRHeightTerrain.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>



// Class deoglHTView
//////////////////////

// Constructor, destructor
////////////////////////////

deoglHTView::deoglHTView( deoglRHeightTerrain *heightTerrain ) :
pHeightTerrain( NULL ),
pSectors( NULL ),
pSectorCount( 0 ),
pSectorSize( 0 ),
pDirtySectors( true )
{
	if( ! heightTerrain ){
		DETHROW( deeInvalidParam );
	}
	
	pHeightTerrain = heightTerrain;
	heightTerrain->AddReference();
	
	heightTerrain->AddListener( this );
}

deoglHTView::~deoglHTView(){
	pCleanUp();
}



// Management
///////////////

deoglHTViewSector *deoglHTView::GetSectorAt( int index ) const{
	if( index < 0 || index >= pSectorCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pSectors[ index ];
}

void deoglHTView::AddSector( deoglHTViewSector *sector ){
	if( ! sector ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSectorCount == pSectorSize ){
		const int newSize = pSectorSize * 3 / 2 + 1;
		deoglHTViewSector **newArray = new deoglHTViewSector*[ newSize ];
		if( pSectors ){
			memcpy( newArray, pSectors, sizeof( deoglHTViewSector* ) * pSectorSize );
			delete [] pSectors;
		}
		pSectors = newArray;
		pSectorSize = newSize;
	}
	
	pSectors[ pSectorCount++ ] = sector;
}

void deoglHTView::RemoveAllSectors(){
	while( pSectorCount > 0 ){
		pSectorCount--;
		delete pSectors[ pSectorCount ];
	}
}



void deoglHTView::ResetClusters(){
	int i;
	for( i=0; i<pSectorCount; i++ ){
		pSectors[ i ]->ResetClusters();
	}
}

void deoglHTView::UpdateLODLevels( const decVector &camera ){
	int i;
	for( i=0; i<pSectorCount; i++ ){
		pSectors[ i ]->UpdateLODLevels( camera );
	}
}



void deoglHTView::PrepareForRendering(){
	if( pDirtySectors ){
		pDirtySectors = false;
		pRebuildSectors();
	}
}

void deoglHTView::UpdateAllRTSInstances(){
	int i;
	for( i=0; i<pSectorCount; i++ ){
		pSectors[ i ]->UpdateAllRTSInstances();
	}
}



// Private functions
//////////////////////

void deoglHTView::pCleanUp(){
	RemoveAllSectors();
	if( pSectors ){
		delete [] pSectors;
	}
	
	if( pHeightTerrain ){
		pHeightTerrain->RemoveListener( this );
		pHeightTerrain->FreeReference();
	}
}

void deoglHTView::pRebuildSectors(){
	const int count = pHeightTerrain->GetSectorCount();
	deoglHTViewSector *sector = NULL;
	int i;
	
	try{
		RemoveAllSectors();
		
		for( i=0; i<count; i++ ){
			sector = new deoglHTViewSector( *this, pHeightTerrain->GetSectorAt( i ) );
			AddSector( sector );
			sector = NULL;
		}
		
	}catch( const deException & ){
		if( sector ){
			delete sector;
		}
		throw;
	}
}



void deoglHTView::HeightTerrainDestroyed( deoglRHeightTerrain& ){
}

void deoglHTView::SectorsChanged( deoglRHeightTerrain& ){
	pDirtySectors = true;
}
