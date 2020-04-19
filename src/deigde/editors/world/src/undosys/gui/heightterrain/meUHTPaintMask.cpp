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

#include "meUHTPaintMask.h"
#include "../../../world/meWorld.h"
#include "../../../world/meWorldGuiParameters.h"
#include "../../../world/terrain/meHeightTerrain.h"
#include "../../../world/terrain/meHeightTerrainSector.h"
#include "../../../world/terrain/meHeightTerrainTexture.h"
#include "../../../world/terrain/meTerrainMaskImage.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>



// Class meUHTPaintMask
/////////////////////////

// Constructor, destructor
////////////////////////////

meUHTPaintMask::meUHTPaintMask( int drawMode, meWorld *world, meHeightTerrainSector *sector,
meHeightTerrainTexture *texture, const decPoint &grid, const decPoint &size, unsigned char *oldValues ){
	if( ! world || ! sector || ! texture || ! oldValues ) DETHROW( deeInvalidParam );
	
	int pixelCount = size.x * size.y;
	
	if( drawMode == meWorldGuiParameters::empdmDraw ){
		SetShortInfo( "Draw Mask" );
		
	}else{
		SetShortInfo( "Erase Mask" );
	}
	
	pWorld = world;
	pSector = sector;
	pTexture = texture;
	
	pSize = size;
	pOldValues = NULL;
	pNewValues = NULL;
	
	pGrid.x1 = grid.x;
	pGrid.y1 = grid.y;
	pGrid.x2 = grid.x + size.x;
	pGrid.y2 = grid.y + size.y;
	pWorld->GetLogger()->LogInfoFormat( "World Editor", "UHTPaintMask: g=(%i,%i,%i,%i)", pGrid.x1, pGrid.y1, pGrid.x2, pGrid.y2 );
	
	SetMemoryConsumption( sizeof( meUHTPaintMask ) + sizeof( unsigned char ) * pixelCount * 2 );
	
	try{
		pOldValues = new unsigned char[ pixelCount ];
		if( ! pOldValues ) DETHROW( deeOutOfMemory );
		
		pNewValues = new unsigned char[ pixelCount ];
		if( ! pNewValues ) DETHROW( deeOutOfMemory );
		
		pSaveValues();
		
		memcpy( pOldValues, oldValues, sizeof( unsigned char ) * pixelCount );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUHTPaintMask::~meUHTPaintMask(){
	pCleanUp();
}



// Management
///////////////

void meUHTPaintMask::Undo(){
	pRestoreValues( pOldValues );
}

void meUHTPaintMask::Redo(){
	pRestoreValues( pNewValues );
}



// Private Functions
//////////////////////

void meUHTPaintMask::pCleanUp(){
	if( pNewValues ) delete [] pNewValues;
	if( pOldValues ) delete [] pOldValues;
}

void meUHTPaintMask::pSaveValues(){
	deImage *mask = pTexture->GetMaskImage();
	meTerrainMaskImage tmi( mask );
	int sgx, sgy;
	int x, y;
	
	for( y=0; y<pSize.y; y++ ){
		for( x=0; x<pSize.x; x++ ){
			sgx = pGrid.x1 + x;
			sgy = pGrid.y1 + y;
			
			pNewValues[ y * pSize.x + x ] = tmi.GetMaskValueAt( sgx, sgy );
		}
	}
}

void meUHTPaintMask::pRestoreValues( unsigned char *values ){
	deImage *mask = pTexture->GetMaskImage();
	meTerrainMaskImage tmi( mask );
	int sgx, sgy;
	int x, y;
	
	for( y=0; y<pSize.y; y++ ){
		for( x=0; x<pSize.x; x++ ){
			sgx = pGrid.x1 + x;
			sgy = pGrid.y1 + y;
			
			tmi.SetMaskValueAt( sgx, sgy, values[ y * pSize.x + x ] );
		}
	}
	
	pTexture->SetMaskChanged( true );
	pTexture->NotifyTextureMaskChanged();
}
