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
#include "deHeightTerrainSector.h"
#include "deHeightTerrainTexture.h"
#include "deHeightTerrainNavSpace.h"
#include "../../decal/deDecal.h"
#include "../../image/deImage.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../systems/modules/ai/deBaseAIHeightTerrain.h"
#include "../../../systems/modules/graphic/deBaseGraphicHeightTerrain.h"
#include "../../../systems/modules/physics/deBasePhysicsHeightTerrain.h"



// Class deHeightTerrainSector
////////////////////////////////

// NOTE:
// the current visible faces system uses a simple bit-per-face array
// to store the visibility. a better system would be to use an RLE
// based system with the following encoding:
// X Y X Y ...
// where X indicates the number of 1s and Y the number of 0s. this
// encoding does not require to store the value to repeat since it
// alternates between 1 and 0 starting with 1 for each line. The value
// 255 is special indicating that 254 of 0 or 1 are written and that
// the next value is of the same type. Therefore the data
// 200 times 1, 500 times 0, 200 times 1
// would be encoded like this ( in paranthesis the bit used for this
// run as well as the number of encoded elements ):
// 200(1/200), 255(0/254), 246(0/246), 200(1/200)

// Constructor, destructor
////////////////////////////

deHeightTerrainSector::deHeightTerrainSector( const decPoint &sector ) :
pSector( sector ),
pParentHeightTerrain( NULL ),
pIndex( -1 ),
pVisibleFaces( NULL ),
pVFByteCount( 0 ),
pDecalRoot( NULL ),
pDecalTail( NULL ),
pDecalCount( 0 ){
}

deHeightTerrainSector::~deHeightTerrainSector(){
	SetHeightImage( NULL );
	RemoveAllNavSpaces();
	RemoveAllDecals();
	RemoveAllTextures();
	if( pVisibleFaces ){
		delete [] pVisibleFaces;
	}
}



// Management
///////////////

void deHeightTerrainSector::SetParentHeightTerrain( deHeightTerrain *heightTerrain ){
	pParentHeightTerrain = heightTerrain;
}

void deHeightTerrainSector::SetIndex( int index ){
	pIndex = index;
}



void deHeightTerrainSector::SetHeightImage( deImage *heightImage ){
	if( heightImage && heightImage->GetComponentCount() != 1 ){
		DETHROW( deeInvalidParam );
	}
	if( pHeightImage == heightImage ){
		return;
	}
	
	// note: calculate extends requires image data to be retained. thing is
	//       deprecated anyways and should be done by physics module
	if( pHeightImage ){
		pHeightImage->ReleaseImageData();
	}
	
	pHeightImage = heightImage;
	
	if( heightImage ){
		heightImage->RetainImageData();
	}
	
	if( pParentHeightTerrain ){
		const int last = pParentHeightTerrain->GetSectorResolution() - 1;
		pParentHeightTerrain->NotifyHeightChanged( pSector, pSector, decPoint(), decPoint( last, last ) );
	}
}

void deHeightTerrainSector::CalculateHeightExtends( float &minHeight, float &maxHeight ){
	minHeight = 0.0f;
	maxHeight = 0.0f;
	
	if( ! pHeightImage || ! pParentHeightTerrain ){
		return;
	}
	
	if( ! pHeightImage->GetData() ){
		DETHROW( deeInvalidParam ); // data not retained. user has to use SetForceRetainImageData
		// note: this entire calculate extends thing is deprecated anyways and should be done by physics module
	}
	
	const int width = pHeightImage->GetWidth();
	const int height = pHeightImage->GetHeight();
	const int size = width * height;
	int i;
	
	if( pHeightImage->GetBitCount() == 8 ){
		const sGrayscale8 * const imageData = pHeightImage->GetDataGrayscale8();
		const float scaling = pParentHeightTerrain->GetHeightScaling() * HT_8BIT_PTOH;
		
		for( i=0; i<size; i++ ){
			const float curHeight = ( imageData[ i ].value - HT_8BIT_BASE ) * scaling;
			
			if( curHeight < minHeight ){
				minHeight = curHeight;
				
			}else if( curHeight > maxHeight ){
				maxHeight = curHeight;
			}
		}
		
	}else if( pHeightImage->GetBitCount() == 16 ){
		const sGrayscale16 * const imageData = pHeightImage->GetDataGrayscale16();
		const float scaling = pParentHeightTerrain->GetHeightScaling() * HT_16BIT_PTOH;
		
		for( i=0; i<size; i++ ){
			const float curHeight = ( imageData[ i ].value - HT_16BIT_BASE ) * scaling;
			
			if( curHeight < minHeight ){
				minHeight = curHeight;
				
			}else if( curHeight > maxHeight ){
				maxHeight = curHeight;
			}
		}
		
	}else{
		const sGrayscale32 * const imageData = pHeightImage->GetDataGrayscale32();
		
		for( i=0; i<size; i++ ){
			const float curHeight = imageData[ i ].value * pParentHeightTerrain->GetHeightScaling();
			
			if( curHeight < minHeight ){
				minHeight = curHeight;
				
			}else if( curHeight > maxHeight ){
				maxHeight = curHeight;
			}
		}
	}
	
	minHeight += pParentHeightTerrain->GetBaseHeight();
	maxHeight += pParentHeightTerrain->GetBaseHeight();
}



// Face Visibility
////////////////////

bool deHeightTerrainSector::GetFaceVisibleAt( int x, int y ) const{
	if( ! pParentHeightTerrain ){
		DETHROW( deeInvalidParam );
	}
	const int pointCount = pParentHeightTerrain->GetSectorResolution();
	if( x < 0 || x >= pointCount || y < 0 || y >= pointCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pVisibleFaces ){
		return true;
	}
	
	const int bitOffset = pointCount * y + x;
	return ( pVisibleFaces[ bitOffset >> 3 ] & ( 1 << ( bitOffset & 0x7 ) ) ) != 0;
}

void deHeightTerrainSector::SetFaceVisibleAt( int x, int y, bool visible ){
	if( ! pParentHeightTerrain ){
		DETHROW( deeInvalidParam );
	}
	const int pointCount = pParentHeightTerrain->GetSectorResolution();
	if( x < 0 || x >= pointCount || y < 0 || y >= pointCount ){
		DETHROW( deeInvalidParam );
	}
	
	pCreateVisibleFaces();
	
	const int bitOffset = pointCount * y + x;
	
	if( visible ){
		pVisibleFaces[ bitOffset >> 3 ] |= ( unsigned char )( 1 << ( bitOffset & 0x7 ) );
		
	}else{
		pVisibleFaces[ bitOffset >> 3 ] &= ~( ( unsigned char )( 1 << ( bitOffset & 0x7 ) ) );
	}
}

void deHeightTerrainSector::SetAllFacesVisible( bool visible ){
	if( visible ){
		if( pVisibleFaces ){
			delete [] pVisibleFaces;
			pVisibleFaces = NULL;
		}
		
	}else{
		pCreateVisibleFaces();
		memset( pVisibleFaces, 0, pVFByteCount );
	}
}



// Textures
/////////////

int deHeightTerrainSector::GetTextureCount() const{
	return pTextures.GetCount();
}

deHeightTerrainTexture *deHeightTerrainSector::GetTextureAt( int index ) const{
	return ( deHeightTerrainTexture* )pTextures.GetAt( index );
}

int deHeightTerrainSector::IndexOfTexture( deHeightTerrainTexture *texture ) const{
	return pTextures.IndexOf( texture );
}

bool deHeightTerrainSector::HasTexture( deHeightTerrainTexture *texture ) const{
	return pTextures.Has( texture );
}

void deHeightTerrainSector::AddTexture( deHeightTerrainTexture *texture ){
	if( HasTexture( texture ) ){
		DETHROW( deeInvalidParam );
	}
	pTextures.Add( texture );
	
	if( pParentHeightTerrain ){
		pParentHeightTerrain->NotifySectorChanged( pIndex );
	}
}

void deHeightTerrainSector::RemoveTexture( deHeightTerrainTexture *texture ){
	pTextures.RemoveFrom( pTextures.IndexOf( texture ) );
	delete texture;
	
	if( pParentHeightTerrain ){
		pParentHeightTerrain->NotifySectorChanged( pIndex );
	}
}

void deHeightTerrainSector::RemoveAllTextures(){
	const int count = pTextures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deHeightTerrainTexture* )pTextures.GetAt( i );
	}
	pTextures.RemoveAll();
	
	if( pParentHeightTerrain ){
		pParentHeightTerrain->NotifySectorChanged( pIndex );
	}
}



// Decals
///////////

void deHeightTerrainSector::AddDecal( deDecal *decal ){
	if( ! decal || decal->GetParentComponent() || decal->GetParentHeightTerrainSector() ){
		DETHROW( deeInvalidParam );
	}
	
	if( pDecalTail ){
		pDecalTail->SetLLHeightTerrainSectorNext( decal );
		decal->SetLLHeightTerrainSectorPrev( pDecalTail );
		decal->SetLLHeightTerrainSectorNext( NULL ); // not required by definition, just to make sure...
		
	}else{
		decal->SetLLHeightTerrainSectorPrev( NULL ); // not required by definition, just to make sure...
		decal->SetLLHeightTerrainSectorNext( NULL ); // not required by definition, just to make sure...
		pDecalRoot = decal;
	}
	
	pDecalTail = decal;
	pDecalCount++;
	decal->SetParentHeightTerrainSector( this );
	decal->AddReference();
	
	if( pParentHeightTerrain ){
		if( pParentHeightTerrain->GetPeerGraphic() ){
			pParentHeightTerrain->GetPeerGraphic()->DecalAdded( pIndex, decal );
		}
		if( pParentHeightTerrain->GetPeerPhysics() ){
			pParentHeightTerrain->GetPeerPhysics()->DecalAdded( pIndex, decal );
		}
	}
}

void deHeightTerrainSector::RemoveDecal( deDecal *decal ){
	if( ! decal || decal->GetParentHeightTerrainSector() != this ){
		DETHROW( deeInvalidParam );
	}
	
	if( decal->GetLLHeightTerrainSectorPrev() ){
		decal->GetLLHeightTerrainSectorPrev()->SetLLHeightTerrainSectorNext( decal->GetLLHeightTerrainSectorNext() );
	}
	if( decal->GetLLHeightTerrainSectorNext() ){
		decal->GetLLHeightTerrainSectorNext()->SetLLHeightTerrainSectorPrev( decal->GetLLHeightTerrainSectorPrev() );
	}
	if( decal == pDecalRoot ){
		pDecalRoot = decal->GetLLHeightTerrainSectorNext();
	}
	if( decal == pDecalTail ){
		pDecalTail = decal->GetLLHeightTerrainSectorPrev();
	}
	pDecalCount--;
	
	decal->SetParentHeightTerrainSector( NULL );
	decal->SetLLHeightTerrainSectorPrev( NULL );
	decal->SetLLHeightTerrainSectorNext( NULL );
	
	if( pParentHeightTerrain ){
		if( pParentHeightTerrain->GetPeerGraphic() ){
			pParentHeightTerrain->GetPeerGraphic()->DecalRemoved( pIndex, decal );
		}
		if( pParentHeightTerrain->GetPeerPhysics() ){
			pParentHeightTerrain->GetPeerPhysics()->DecalRemoved( pIndex, decal );
		}
	}
	
	decal->FreeReference();
}

void deHeightTerrainSector::RemoveAllDecals(){
	while( pDecalTail ){
		deDecal * const next = pDecalTail->GetLLComponentPrev();
		pDecalTail->FreeReference();
		pDecalTail = next;
		pDecalCount--;
	}
	pDecalRoot = NULL;
	
	if( pParentHeightTerrain ){
		if( pParentHeightTerrain->GetPeerGraphic() ){
			pParentHeightTerrain->GetPeerGraphic()->AllDecalsRemoved( pIndex );
		}
		if( pParentHeightTerrain->GetPeerPhysics() ){
			pParentHeightTerrain->GetPeerPhysics()->AllDecalsRemoved( pIndex );
		}
	}
}



// Navigation spaces
//////////////////////

int deHeightTerrainSector::GetNavSpaceCount() const{
	return pNavSpaces.GetCount();
}

deHeightTerrainNavSpace *deHeightTerrainSector::GetNavSpaceAt( int index ) const{
	return ( deHeightTerrainNavSpace* )pNavSpaces.GetAt( index );
}

int deHeightTerrainSector::IndexOfNavSpace( deHeightTerrainNavSpace *navspace ) const{
	return pNavSpaces.IndexOf( navspace );
}

bool deHeightTerrainSector::HasNavSpace( deHeightTerrainNavSpace *navspace ) const{
	return pNavSpaces.Has( navspace );
}

void deHeightTerrainSector::AddNavSpace( deHeightTerrainNavSpace *navspace ){
	if( HasNavSpace( navspace ) ){
		DETHROW( deeInvalidParam );
	}
	pNavSpaces.Add( navspace );
	
	if( pParentHeightTerrain && pParentHeightTerrain->GetPeerAI() ){
		pParentHeightTerrain->GetPeerAI()->NavSpaceAdded( pIndex, navspace );
	}
}

void deHeightTerrainSector::RemoveNavSpace( deHeightTerrainNavSpace *navspace ){
	const int index = pNavSpaces.IndexOf( navspace );
	
	pNavSpaces.RemoveFrom( index );
	
	if( pParentHeightTerrain && pParentHeightTerrain->GetPeerAI() ){
		pParentHeightTerrain->GetPeerAI()->NavSpaceRemoved( pIndex, index );
	}
	
	delete navspace;
}

void deHeightTerrainSector::RemoveAllNavSpaces(){
	const int count = pNavSpaces.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deHeightTerrainNavSpace* )pNavSpaces.GetAt( i );
	}
	pNavSpaces.RemoveAll();
	
	if( pParentHeightTerrain && pParentHeightTerrain->GetPeerAI() ){
		pParentHeightTerrain->GetPeerAI()->AllNavSpacesRemoved( pIndex );
	}
}

void deHeightTerrainSector::NotifyNavSpaceLayerChanged( int index ){
	if( pParentHeightTerrain && pParentHeightTerrain->GetPeerAI() ){
		pParentHeightTerrain->GetPeerAI()->NavSpaceLayerChanged( pIndex, index );
	}
}

void deHeightTerrainSector::NotifyNavSpaceTypeChanged( int index ){
	if( pParentHeightTerrain && pParentHeightTerrain->GetPeerAI() ){
		pParentHeightTerrain->GetPeerAI()->NavSpaceTypeChanged( pIndex, index );
	}
}

void deHeightTerrainSector::NotifyNavSpaceSnappingChanged( int index ){
	if( pParentHeightTerrain && pParentHeightTerrain->GetPeerAI() ){
		pParentHeightTerrain->GetPeerAI()->NavSpaceSnappingChanged( pIndex, index );
	}
}

void deHeightTerrainSector::NotifyNavSpaceLayoutChanged( int index ){
	if( pParentHeightTerrain && pParentHeightTerrain->GetPeerAI() ){
		pParentHeightTerrain->GetPeerAI()->NavSpaceLayoutChanged( pIndex, index );
	}
}



// Private Functions
//////////////////////

void deHeightTerrainSector::pCreateVisibleFaces(){
	if( pVisibleFaces ){
		return;
	}
	
	if( ! pParentHeightTerrain ){
		DETHROW( deeInvalidParam );
	}
	const int pointCount = pParentHeightTerrain->GetSectorResolution();
	const int byteCount = ( ( pointCount * pointCount - 1 ) >> 3 ) + 1;
	
	pVisibleFaces = new unsigned char[ byteCount ];
	memset( pVisibleFaces, 255, byteCount );
	
	pVFByteCount = byteCount;
}
