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
pHTUpdateTrackerValue( 0 )
{
	if( ! heightTerrain ){
		DETHROW( deeInvalidParam );
	}
	
	pHeightTerrain = heightTerrain;
	heightTerrain->AddReference();
	
	pHTUpdateTrackerValue = heightTerrain->GetUpdateTracker();
	
	try{
		RebuildSectors();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglHTView::~deoglHTView(){
	pCleanUp();
}



// Management
///////////////

deoglHTViewSector *deoglHTView::GetSectorAt( int index ) const{
	if( index < 0 || index >= pSectorCount ) DETHROW( deeInvalidParam );
	
	return pSectors[ index ];
}

void deoglHTView::AddSector( deoglHTViewSector *sector ){
	if( ! sector ) DETHROW( deeInvalidParam );
	
	if( pSectorCount == pSectorSize ){
		int newSize = pSectorSize * 3 / 2 + 1;
		deoglHTViewSector **newArray = new deoglHTViewSector*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
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




void deoglHTView::RebuildSectors(){
	int s, sectorCount = pHeightTerrain->GetSectorCount();
	deoglHTViewSector *sector = NULL;
	
	try{
		RemoveAllSectors();
		
		for( s=0; s<sectorCount; s++ ){
			sector = new deoglHTViewSector( *this, pHeightTerrain->GetSectorAt( s ) );
			AddSector( sector );
			sector = NULL;
		}
		
	}catch( const deException & ){
		if( sector ) delete sector;
		throw;
	}
}

void deoglHTView::ResetClusters(){
	int s;
	
	for( s=0; s<pSectorCount; s++ ){
		pSectors[ s ]->ResetClusters();
	}
}
/*
void deoglHTView::DetermineVisibilityUsing( deoglDCollisionVolume *collisionVolume ){
	int s;
	
	for( s=0; s<pSectorCount; s++ ){
		pSectors[ s ]->DetermineVisibilityUsing( collisionVolume );
	}
}
*/
void deoglHTView::UpdateLODLevels( const decVector &camera ){
	int s;
	
	for( s=0; s<pSectorCount; s++ ){
		pSectors[ s ]->UpdateLODLevels( camera );
	}
}



void deoglHTView::PrepareForRendering(){
	if( pHeightTerrain->GetUpdateTracker() == pHTUpdateTrackerValue ){
		return;
	}
	
	RebuildSectors();
	pHTUpdateTrackerValue = pHeightTerrain->GetUpdateTracker();
}



// Private functions
//////////////////////

void deoglHTView::pCleanUp(){
	RemoveAllSectors();
	if( pSectors ){
		delete [] pSectors;
	}
	
	if( pHeightTerrain ){
		pHeightTerrain->FreeReference();
	}
}
