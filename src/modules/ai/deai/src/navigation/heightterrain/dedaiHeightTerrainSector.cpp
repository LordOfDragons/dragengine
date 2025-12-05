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
#include <string.h>

#include "dedaiHeightTerrain.h"
#include "dedaiHeightTerrainSector.h"
#include "dedaiHeightTerrainNavSpace.h"
#include "../spaces/dedaiSpace.h"
#include "../../deDEAIModule.h"
#include "../../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>



// Class dedaiHeightTerrainSector
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiHeightTerrainSector::dedaiHeightTerrainSector( dedaiHeightTerrain &heightTerrain,
	const deHeightTerrainSector &sector ) :
pHeightTerrain( heightTerrain ),
pSector( sector ),
pHeights( NULL )
{
	const int navSpaceCount = sector.GetNavSpaceCount();
	int i;
	
	try{
		pPosition.x = heightTerrain.GetHeightTerrain().GetSectorSize() * sector.GetSector().x;
		pPosition.z = heightTerrain.GetHeightTerrain().GetSectorSize() * sector.GetSector().y;
		
		pUpdateHeights();
		
		for( i=0; i<navSpaceCount; i++ ){
			NavSpaceAdded( sector.GetNavSpaceAt( i ) );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dedaiHeightTerrainSector::~dedaiHeightTerrainSector(){
	pCleanUp();
}



// Management
///////////////



// Navigation spaces
//////////////////////

int dedaiHeightTerrainSector::GetNavSpaceCount() const{
	return pNavSpaces.GetCount();
}

dedaiHeightTerrainNavSpace *dedaiHeightTerrainSector::GetNavSpaceAt( int index ) const{
	return ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( index );
}



// Notifications
//////////////////

void dedaiHeightTerrainSector::ParentWorldChanged(){
	const int navSpaceCount = pNavSpaces.GetCount();
	int i;
	
	for( i=0; i<navSpaceCount; i++ ){
		( ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( i ) )->ParentWorldChanged();
	}
}

void dedaiHeightTerrainSector::SectorChanged(){
	const int navSpaceCount = pNavSpaces.GetCount();
	int i;
	
	if( pHeights ){
		delete [] pHeights;
		pHeights = NULL;
	}
	
	pUpdateHeights();
	
	const int imageSize = pHeightTerrain.GetHeightTerrain().GetSectorResolution();
	const decPoint to( imageSize - 1, imageSize - 1 );
	for( i=0; i<navSpaceCount; i++ ){
		( ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( i ) )->HeightChanged( decPoint(), to );
	}
}

void dedaiHeightTerrainSector::HeightChanged( const decPoint &from, const decPoint &to ){
	const int navSpaceCount = pNavSpaces.GetCount();
	int i;
	
	pUpdateHeights();
	
	for( i=0; i<navSpaceCount; i++ ){
		( ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( i ) )->HeightChanged( from, to );
	}
}



void dedaiHeightTerrainSector::NavSpaceAdded( deHeightTerrainNavSpace *navspace ){
	pNavSpaces.Add(dedaiHeightTerrainNavSpace::Ref::NewWith(*this, *navspace));
}

void dedaiHeightTerrainSector::NavSpaceRemoved( int index ){
	pNavSpaces.RemoveFrom( index );
}

void dedaiHeightTerrainSector::AllNavSpacesRemoved(){
	pNavSpaces.RemoveAll();
}

void dedaiHeightTerrainSector::NavSpaceLayerChanged( int index ){
	( ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( index ) )->LayerChanged();
}

void dedaiHeightTerrainSector::NavSpaceTypeChanged( int index ){
	( ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( index ) )->TypeChanged();
}

void dedaiHeightTerrainSector::NavSpaceSnappingChanged( int index ){
	( ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( index ) )->SnappingChanged();
}

void dedaiHeightTerrainSector::NavSpaceLayoutChanged( int index ){
	( ( dedaiHeightTerrainNavSpace* )pNavSpaces.GetAt( index ) )->LayoutChanged();
}



// Private Functions
//////////////////////

void dedaiHeightTerrainSector::pCleanUp(){
	AllNavSpacesRemoved();
	
	if( pHeights ){
		delete [] pHeights;
	}
}



void dedaiHeightTerrainSector::pUpdateHeights(){
	const int imageDim = pHeightTerrain.GetHeightTerrain().GetSectorResolution() - 1;
	pUpdateHeights( decPoint(), decPoint( imageDim, imageDim ) );
}

void dedaiHeightTerrainSector::pUpdateHeights( const decPoint &from, const decPoint &to ){
	const int imageDim = pHeightTerrain.GetHeightTerrain().GetSectorResolution();
	const float baseHeight = pHeightTerrain.GetHeightTerrain().GetBaseHeight();
	deImage * const heightImage = pSector.GetHeightImage();
	const int pixelCount = imageDim * imageDim;
	
	// create height values if not existing already
	if( ! pHeights ){
		pHeights = new float[ pixelCount ];
	}
	
	// update height values unless image is not set
	int x, y;
	
	if( ! heightImage ){
		for( y=from.y; y<=to.y; y++ ){
			float * const heightsRow = pHeights + imageDim * y;
			for( x=from.x; x<=to.x; x++ ){
				heightsRow[ x ] = 0.0f;
			}
		}
		return;
	}
	
	if( heightImage->GetBitCount() == 8 ){
		const float scaling = pHeightTerrain.GetHeightTerrain().GetHeightScaling() * HT_8BIT_PTOH;
		const sGrayscale8 * const imageData = heightImage->GetDataGrayscale8();
		
		for( y=from.y; y<=to.y; y++ ){
			const sGrayscale8 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights + imageDim * y;
			
			for( x=from.x; x<=to.x; x++ ){
				heightsRow[ x ] = baseHeight + ( float )( imageDataRow[ x ].value - HT_8BIT_BASE ) * scaling;
			}
		}
		
	}else if( heightImage->GetBitCount() == 16 ){
		const float scaling = pHeightTerrain.GetHeightTerrain().GetHeightScaling() * HT_16BIT_PTOH;
		sGrayscale16 * const imageData = heightImage->GetDataGrayscale16();
		
		for( y=from.y; y<=to.y; y++ ){
			const sGrayscale16 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights + imageDim * y;
			
			for( x=from.x; x<=to.x; x++ ){
				heightsRow[ x ] = baseHeight + ( float )( imageDataRow[ x ].value - HT_16BIT_BASE ) * scaling;
			}
		}
		
	}else{
		const float scaling = pHeightTerrain.GetHeightTerrain().GetHeightScaling();
		sGrayscale32 * const imageData = heightImage->GetDataGrayscale32();
		
		for( y=from.y; y<=to.y; y++ ){
			const sGrayscale32 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights + imageDim * y;
			
			for( x=from.x; x<=to.x; x++ ){
				heightsRow[ x ] = baseHeight + imageDataRow[ x ].value * scaling;
			}
		}
	}
}
