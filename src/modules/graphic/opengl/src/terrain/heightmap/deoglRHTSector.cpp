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

#include "deoglHTSCluster.h"
#include "deoglHTSTexture.h"
#include "deoglRHeightTerrain.h"
#include "deoglRHTSector.h"
#include "deoglTerrainMaskImage.h"
#include "../../deoglBasics.h"
#include "../../extensions/deoglExtResult.h"
#include "../../skin/deoglSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainTexture.h>



// Class deoglRHTSector
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRHTSector::deoglRHTSector( deoglRHeightTerrain &heightTerrain, const deHeightTerrainSector &sector ) :
pHeightTerrain( heightTerrain ),
pIndex( -1 ),

pBaseHeight( 0.0f ),
pScaling( 1.0f ),

pTextures( NULL ),
pTextureCount( 0 ),
pValidTextures( false ),
pDirtyMaskTextures( true ),
pTexturesRequirePrepareForRender( true ),

pHeights( NULL ),
pMinHeight( 0.0f ),
pMaxHeight( 0.0f ),

pVBODataPoints1( NULL ),
pVBODataPoints1Count( 0 ),
pVBODataPoints2( NULL ),
pVBODataPoints2Count( 0 ),
pVBODataFaces( NULL ),
pVBODataFacesCount( 0 ),

pDirtyPoints( true ),

pClusters( NULL ),
pClusterCount( 0 ),

pValid( false )
{
	int i;
	for( i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++ ){
		pMasks[ i ] = NULL;
	}
	
	try{
		pCreateArrays( sector );
		
	}catch( const deException & ){
// 		heightTerrain->GetOpenGL()->SetErrorTrace( e );
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( heightTerrain.GetRenderThread(), HTSector );
}

deoglRHTSector::~deoglRHTSector(){
	LEAK_CHECK_FREE( pHeightTerrain.GetRenderThread(), HTSector );
	pCleanUp();
}



// Management
///////////////

void deoglRHTSector::SetIndex( int index ){
	pIndex = index;
}

decDMatrix deoglRHTSector::CalcWorldMatrix() const{
	return CalcWorldMatrix( pHeightTerrain.GetParentWorld()->GetReferencePosition() );
}

decDMatrix deoglRHTSector::CalcWorldMatrix( const decDVector &referencePosition ) const{
	return decDMatrix::CreateTranslation( CalcWorldPosition( referencePosition ) );
}

decDVector deoglRHTSector::CalcWorldPosition() const{
	DEASSERT_NOTNULL( pHeightTerrain.GetParentWorld() )
	return CalcWorldPosition( pHeightTerrain.GetParentWorld()->GetReferencePosition() );
}

decDVector deoglRHTSector::CalcWorldPosition( const decDVector &referencePosition ) const{
	return decDVector( pHeightTerrain.GetSectorSize() * ( double )pCoordinates.x, 0.0,
		pHeightTerrain.GetSectorSize() * ( double )pCoordinates.y ) - referencePosition;
}

void deoglRHTSector::AddToWorldCompute( deoglWorldCompute &worldCompute ){
	const int count = pClusterCount * pClusterCount;
	int i;
	for( i=0; i<count; i++ ){
		pClusters[ i ].AddToWorldCompute( worldCompute );
	}
}

void deoglRHTSector::UpdateWorldCompute( deoglWorldCompute &worldCompute ){
	const int count = pClusterCount * pClusterCount;
	int i;
	for( i=0; i<count; i++ ){
		pClusters[ i ].UpdateWorldCompute( worldCompute );
	}
}

void deoglRHTSector::RemoveFromWorldCompute( deoglWorldCompute &worldCompute ){
	const int count = pClusterCount * pClusterCount;
	int i;
	for( i=0; i<count; i++ ){
		pClusters[ i ].RemoveFromWorldCompute( worldCompute );
	}
}

void deoglRHTSector::PrepareForRender(){
	if( ! pValidTextures ){
		return;
	}
	
	pCreateVBODataPoints1();
	pCreateVBODataFaces();
	pUpdateMaskTextures();
	pUpdateHeightMap();
	
	if( pTexturesRequirePrepareForRender ){
		pTexturesRequirePrepareForRender = false;
		
		int i;
		for( i=0; i<pTextureCount; i++ ){
			pTextures[ i ]->PrepareForRender();
		}
	}
}



deoglHTSTexture &deoglRHTSector::GetTextureAt( int index ) const{
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	return *pTextures[ index ];
}

void deoglRHTSector::TextureRequirePrepareForRender(){
	pTexturesRequirePrepareForRender = true;
	pHeightTerrain.SectorRequirePrepareForRender();
}



void deoglRHTSector::HeightChanged( const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to ){
	pDirtyPoints = true;
	pHeightTerrain.SectorRequirePrepareForRender();
	
	pSyncHeightMap( sector, from, to );
}

void deoglRHTSector::SectorChanged( const deHeightTerrainSector &sector ){
	pDirtyPoints = true;
	pDirtyMaskTextures = true;
	pTexturesRequirePrepareForRender = true;
	pHeightTerrain.SectorRequirePrepareForRender();
	
	pSyncSector( sector );
}



// Clusters
/////////////

deoglHTSCluster &deoglRHTSector::GetClusterAt( int x, int z ) const{
	if( x < 0 || x >= pClusterCount || z < 0 || z >= pClusterCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pClusters[ pClusterCount * z + x ];
}

deoglHTSCluster & deoglRHTSector::GetClusterAt( const decPoint &coordinate ) const{
	return GetClusterAt( coordinate.x, coordinate.y );
}



// Private functions
//////////////////////

void deoglRHTSector::pCleanUp(){
	deoglDelayedOperations &dops = pHeightTerrain.GetRenderThread().GetDelayedOperations();
	int i;
	
	if( pHeights ){
		delete [] pHeights;
	}
	
	pDropMaskPixelBuffers();
	
	if( pClusters ){
		delete [] pClusters;
	}
	if( pTextures ){
		for( i=0; i<pTextureCount; i++ ){
			delete pTextures[ i ];
		}
		delete [] pTextures;
	}
	for( i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++ ){
		if( pMasks[ i ] ){
			delete pMasks[ i ];
		}
	}
	
	if( pVBODataFaces ){
		for( i=0; i<pVBODataFacesCount; i++ ){
			dops.DeleteOpenGLBuffer( pVBODataFaces[ i ] );
		}
		delete [] pVBODataFaces;
	}
	if( pVBODataPoints2 ){
		for( i=0; i<pVBODataPoints2Count; i++ ){
			dops.DeleteOpenGLBuffer( pVBODataPoints2[ i ] );
		}
		delete [] pVBODataPoints2;
	}
	if( pVBODataPoints1 ){
		for( i=0; i<pVBODataPoints1Count; i++ ){
			dops.DeleteOpenGLBuffer( pVBODataPoints1[ i ] );
		}
		delete [] pVBODataPoints1;
	}
}



void deoglRHTSector::pCreateArrays( const deHeightTerrainSector &sector ){
	pCreateHeightMap( sector );
}

void deoglRHTSector::pCreateHeightMap( const deHeightTerrainSector &sector ){
	const int imageDim = pHeightTerrain.GetSectorResolution();
//	float sectorDim = pHeightTerrain->GetSectorSize();
//	float sizeStep = sectorDim / ( float )imageDim;
//	float sizeOffset = sectorDim * 0.5f;
	int x, z;
	
	// create clusters
	int maxPointsPerCluster = 65;
	int pcx, pcz, curx, curz, index;
	
	pClusterCount = ( ( imageDim - 2 ) / ( maxPointsPerCluster - 1 ) ) + 1;
	
	pClusters = new deoglHTSCluster[ pClusterCount * pClusterCount ];
	
	index = 0;
	curz = 0;
	for( z=0; z<pClusterCount; z++ ){
		if( z < pClusterCount - 1 ){
			pcz = maxPointsPerCluster;
			
		}else{
			pcz = imageDim - curz;
		}
		
		curx = 0;
		for( x=0; x<pClusterCount; x++ ){
			if( x < pClusterCount - 1 ){
				pcx = maxPointsPerCluster;
				
			}else{
				pcx = imageDim - curx;
			}
			
			deoglHTSCluster &cluster = pClusters[ pClusterCount * z + x ];
			cluster.SetHTSector( this );
			cluster.SetCoordinates( decPoint( x, z ) );
			cluster.SetIndex( index++ );
			cluster.SetSize( curx, curz, pcx, pcz );
			
			curx += maxPointsPerCluster - 1;
		}
		
		curz += maxPointsPerCluster - 1;
	}
	
	// init clusters
	// NOTE currently each cluster calculates the LOD chain. now the majority of
	// clusters shares the same configuration ( in terms of points per cluster ).
	// instead of calculating these identical LOD chains multiple times they
	// could be pushed into a LOD chain cache instead which reduces not only
	// memory consumption but also processing time during loading time.
	int offset;
	
	for( z=0; z<pClusterCount; z++ ){
		offset = pClusterCount * z;
		
		for( x=0; x<pClusterCount; x++ ){
			pClusters[ offset + x ].InitFromHeightImage( sector );
		}
	}
	
	// debug
	/*
	for( z=0; z<pClusterCount; z++ ){
		for( x=0; x<pClusterCount; x++ ){
			pClusters[ pClusterCount * z + x ].DebugPrint();
		}
	}
	*/
}

void deoglRHTSector::pSetTextureCount( int count ){
	if( count == pTextureCount ){
		return;
	}
	
	if( pTextures ){
		int i;
		for( i=0; i<pTextureCount; i++ ){
			delete pTextures[ i ];
		}
		delete [] pTextures;
		pTextures = NULL;
		pTextureCount = 0;
	}
	
	if( count > 0 ){
		pTextures = new deoglHTSTexture*[ count ];
		
		for( pTextureCount=0; pTextureCount<count; pTextureCount++ ){
			pTextures[ pTextureCount ] = new deoglHTSTexture( *this, pTextureCount );
		}
	}
}



//#include "dragengine/common/utils/decTimer.h"
//static decTimer timer;
//#define DEBUG_RESET_TIMERS				timer.Reset();
//#define DEBUG_PRINT_TIMER(what)			printf( "[OGL] Timer: %s = %iys\n", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )

void deoglRHTSector::pDropMaskPixelBuffers(){
	int i;
	for( i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++ ){
		pPixBufMasks[ i ] = nullptr;
	}
}



void deoglRHTSector::pSyncSector( const deHeightTerrainSector &sector ){
	const int imageDim = pHeightTerrain.GetSectorResolution();
	
	// store sector parameters
	pCoordinates = sector.GetSector();
	pBaseHeight = sector.GetParentHeightTerrain()->GetBaseHeight();
	pScaling = sector.GetParentHeightTerrain()->GetHeightScaling();
	
	// determine if the sector is valid
	pValid = true;
	
	deImage * const heightImage = sector.GetHeightImage();
	if( heightImage ){
		if( heightImage->GetWidth() != imageDim ){
			pValid = false;
		}
		if( heightImage->GetHeight() != imageDim ){
			pValid = false;
		}
		
	}else{
		pValid = false;
	}
	
	if( ! pValid ){
		pSetTextureCount( 0 );
		return;
	}
	
	// sync textures
	pSyncTextures( sector );
}

void deoglRHTSector::pSyncTextures( const deHeightTerrainSector &sector ){
	const int count = sector.GetTextureCount();
	int i;
	
	// determine if textures are valid and create them if not existing
	pValidTextures = false;
	
	if( count > 0 ){
		pSetTextureCount( count );
		pValidTextures = true;
	}
	
	// copy texture parameters for render thread part
	for( i=0; i<pTextureCount; i++ ){
		const deHeightTerrainTexture &t = *sector.GetTextureAt( i );
		
		pTextures[ i ]->SetMatrix(
			decTexMatrix::CreateScale( t.GetProjectionScaling().x, t.GetProjectionScaling().y )
			* decTexMatrix::CreateTranslation( t.GetProjectionOffset().x, t.GetProjectionOffset().y )
			* decTexMatrix::CreateRotation( t.GetProjectionRotation() ) );
	}
	
	// update skins if dirty
	if( pValidTextures ){
		for( i=0; i<pTextureCount; i++ ){
			const deSkin * const skin = sector.GetTextureAt( i )->GetSkin();
			
			if( skin ){
				pTextures[ i ]->SetSkin( ( ( deoglSkin* )skin->GetPeerGraphic() )->GetRSkin() );
				
			}else{
				pTextures[ i ]->SetSkin( NULL );
			}
		}
	}
	
	// sync texture masks if dirty
	if( pValidTextures ){
		pSyncMaskTextures( sector );
	}
}

void deoglRHTSector::pSyncMaskTextures( const deHeightTerrainSector &sector ){
	const int textureCount = decMath::min( pTextureCount, 16 );
	const int maskCount = ( ( textureCount - 1 ) >> 2 ) + 1;
	int m, t;
	
	int maskWidth = 0, maskHeight = 0;
	int p, x, y, offset;
	
	pDropMaskPixelBuffers();
	
	deImage *images[ 16 ];
	for( t=0; t<16; t++ ){
		images[ t ] = NULL;
	}
	
	for( t=0; t<textureCount; t++ ){
		deImage * const image = sector.GetTextureAt( t )->GetMaskImage();
		if( ! image ){
			continue;
		}
		
		const int imageWidth = image->GetWidth();
		const int imageHeight = image->GetHeight();
		
		if( maskWidth == 0 ){
			maskWidth = imageWidth;
			maskHeight = imageHeight;
		}
		
		if( imageWidth == maskWidth && imageHeight == maskHeight ){
			images[ t ] = image;
		}
	}
	
	if( maskWidth == 0 ){
		return;
	}
	
	const int pixelCount = maskWidth * maskHeight;
	for( m=0; m<maskCount; m++ ){
		pPixBufMasks[ m ].TakeOver( new deoglPixelBuffer( deoglPixelBuffer::epfFloat4, maskWidth, maskHeight, 1 ) );
		deoglPixelBuffer::sFloat4 * const pbdata = pPixBufMasks[ m ]->GetPointerFloat4();
		
		for( t=m*4; t<textureCount; t++ ){
			const int relT = t - m * 4;
			
			if( t == 0 ){
				for( p=0; p<pixelCount; p++ ){
					pbdata[ p ].r = 1.0f;
				}
				
			}else if( images[ t ] ){
				const deoglTerrainMaskImage tmi( *images[ t ] );
				
				if( relT == 0 ){
					for( p=0, y=0; y<maskHeight; y++ ){
						offset = maskWidth * ( maskHeight - 1 - y );
						for( x=0; x<maskWidth; x++ ){
							pbdata[ p++ ].r = tmi.GetMaskValueAt( offset + x );
						}
					}
					
				}else if( relT == 1 ){
					for( p=0, y=0; y<maskHeight; y++ ){
						offset = maskWidth * ( maskHeight - 1 - y );
						for( x=0; x<maskWidth; x++ ){
							pbdata[ p++ ].g = tmi.GetMaskValueAt( offset + x );
						}
					}
					
				}else if( relT == 2 ){
					for( p=0, y=0; y<maskHeight; y++ ){
						offset = maskWidth * ( maskHeight - 1 - y );
						for( x=0; x<maskWidth; x++ ){
							pbdata[ p++ ].b = tmi.GetMaskValueAt( offset + x );
						}
					}
					
				}else if( relT == 3 ){
					for( p=0, y=0; y<maskHeight; y++ ){
						offset = maskWidth * ( maskHeight - 1 - y );
						for( x=0; x<maskWidth; x++ ){
							pbdata[ p++ ].a = tmi.GetMaskValueAt( offset + x );
						}
					}
				}
				
			}else{
				if( relT == 0 ){
					for( p=0; p<pixelCount; p++ ) pbdata[ p ].r = 1.0f;
					
				}else if( relT == 1 ){
					for( p=0; p<pixelCount; p++ ) pbdata[ p ].g = 1.0f;
					
				}else if( relT == 2 ){
					for( p=0; p<pixelCount; p++ ) pbdata[ p ].b = 1.0f;
					
				}else if( relT == 3 ){
					for( p=0; p<pixelCount; p++ ) pbdata[ p ].a = 1.0f;
				}
			}
		}
	}
}

void deoglRHTSector::pSyncHeightMap( const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to ){
	if( ! pValid ){
		return;
	}
	
	deImage * const heightImage = sector.GetHeightImage();
	const float baseHeight = sector.GetParentHeightTerrain()->GetBaseHeight();
	const int imageDim = pHeightTerrain.GetSectorResolution();
	const int pixelCount = imageDim * imageDim;
	int x, y;
	
	// create height values if not existing already
	if( ! pHeights ){
		pHeights = new float[ pixelCount ];
	}
	
	// update height values
	if( heightImage->GetBitCount() == 8 ){
		const sGrayscale8 * const imageData = heightImage->GetDataGrayscale8();
		const float scaling = sector.GetParentHeightTerrain()->GetHeightScaling() * HT_8BIT_PTOH;
		
		for( y=from.y; y<=to.y; y++ ){
			const sGrayscale8 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights + imageDim * y;
			
			for( x=from.x; x<=to.x; x++ ){
				heightsRow[ x ] = baseHeight + ( float )( imageDataRow[ x ].value - HT_8BIT_BASE ) * scaling;
			}
		}
		
	}else if( heightImage->GetBitCount() == 16 ){
		sGrayscale16 * const imageData = heightImage->GetDataGrayscale16();
		const float scaling = sector.GetParentHeightTerrain()->GetHeightScaling() * HT_16BIT_PTOH;
		
		for( y=from.y; y<=to.y; y++ ){
			const sGrayscale16 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights + imageDim * y;
			
			for( x=from.x; x<=to.x; x++ ){
				heightsRow[ x ] = baseHeight + ( float )( imageDataRow[ x ].value - HT_16BIT_BASE ) * scaling;
			}
		}
		
	}else{
		sGrayscale32 * const imageData = heightImage->GetDataGrayscale32();
		const float scaling = sector.GetParentHeightTerrain()->GetHeightScaling();
		
		for( y=from.y; y<=to.y; y++ ){
			const sGrayscale32 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights + imageDim * y;
			
			for( x=from.x; x<=to.x; x++ ){
				heightsRow[ x ] = baseHeight + imageDataRow[ x ].value * scaling;
			}
		}
	}
	
	if( pHeightTerrain.GetParentWorld() ){
		UpdateWorldCompute( pHeightTerrain.GetParentWorld()->GetCompute() );
	}
}



void deoglRHTSector::pUpdateMaskTextures(){
	if( ! pDirtyMaskTextures ){
		return;
	}
	pDirtyMaskTextures = false;
	
	const int textureCount = decMath::min( pTextureCount, 16 );
	int i;
	
	for( i=0; i<4; i++ ){
		const int relT = decMath::min( textureCount - i * 4, 4 );
		
		if( pPixBufMasks[ i ] && relT > 0 ){
			if( ! pMasks[ i ] ){
				pMasks[ i ] = new deoglTexture( pHeightTerrain.GetRenderThread() );
			}
			
			pMasks[ i ]->SetSize( pPixBufMasks[ i ]->GetWidth(), pPixBufMasks[ i ]->GetHeight() );
			
			if( relT == 1 ){
				pMasks[ i ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR8 );
				
			}else if( relT == 2 ){
				//pMasks[ m ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG4 ); // not supported anymore
				//pMasks[ m ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG8 ); // uses 2 bytes
				pMasks[ i ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR3G3B2 ); // uses only 1 byte but wastes 2 for unused component
				
			}else if( relT == 3 ){
				pMasks[ i ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR3G3B2 ); // GL_RGB4, GL_RGB5, GL_RGB8
				
			}else{
				pMasks[ i ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA2 ); // GL_RGBA4, GL_RGBA8
			}
			
			pMasks[ i ]->SetPixels( pPixBufMasks[ i ] );
			
		}else{
			if( pMasks[ i ] ){
				delete pMasks[ i ];
				pMasks[ i ] = NULL;
			}
		}
	}
}

void deoglRHTSector::pCreateVBODataPoints1(){
	if( pVBODataPoints1 ){
		return;
	}
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = pHeightTerrain.GetRenderThread(); )
	const int clusterCount = pClusterCount * pClusterCount;
	const int vboCount = ( ( clusterCount - 1 ) / 32 ) + 1;
	if( vboCount == 0 ){
		return;
	}
	
	int assignedClusters = 0;
	int vboOffset = 0;
	int v, c;
	
	pVBODataPoints1 = new GLuint[ vboCount ];
	
	for( v=0; v<vboCount; v++ ){
		pVBODataPoints1[ v ] = 0;
	}
	
	for( v=0, c=0; c<clusterCount; c++ ){
		if( ! pVBODataPoints1[ v ] ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBODataPoints1[ v ] ) );
			if( ! pVBODataPoints1[ v ] ){
				DETHROW( deeOutOfMemory );
			}
			
			pVBODataPoints1Count++;
		}
		
		pClusters[ c ].SetOffsetVBODataPoints( vboOffset );
		pClusters[ c ].SetVBODataPoints1( pVBODataPoints1[ v ] );
		vboOffset += pClusters[ c ].GetCountVBODataPoints();
		
		assignedClusters++;
		if( assignedClusters == 32 ){
			OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints1[ v ] ) );
			OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
				sizeof( deoglVBOHeightTerrain1 ) * vboOffset, NULL, GL_STATIC_DRAW ) );
//renderThread.GetLogger().LogInfoFormat( "htsector data1: size=%i vbo=%u", sizeof(deoglVBOHeightTerrain1)*vboOffset, pVBODataPoints1[ v ] );
			
			v++;
			assignedClusters = 0;
			vboOffset = 0;
		}
	}
	
	if( v < vboCount && vboOffset > 0 ){
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints1[ v ] ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
			sizeof( deoglVBOHeightTerrain1 ) * vboOffset, NULL, GL_STATIC_DRAW ) );
//renderThread.GetLogger().LogInfoFormat( "htsector data1: size=%i vbo=%u", sizeof(deoglVBOHeightTerrain1)*vboOffset, pVBODataPoints1[ v ] );
	}
	
	for( c=0; c<clusterCount; c++ ){
		pClusters[ c ].UpdateVBOData1();
	}
}

void deoglRHTSector::pCreateVBODataFaces(){
	if( pVBODataFaces ){
		return;
	}
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = pHeightTerrain.GetRenderThread(); )
	const int clusterCount = pClusterCount * pClusterCount;
	const int vboCount = ( ( clusterCount - 1 ) / 32 ) + 1;
	if( vboCount == 0 ){
		return;
	}
	
	int assignedClusters = 0;
	int vboOffset = 0;
	int v, c;
	
	pVBODataFaces = new GLuint[ vboCount ];
	
	for( v=0; v<vboCount; v++ ){
		pVBODataFaces[ v ] = 0;
	}
	
	for( v=0, c=0; c<clusterCount; c++ ){
		if( ! pVBODataFaces[ v ] ){
			OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBODataFaces[ v ] ) );
			if( ! pVBODataFaces[ v ] ){
				DETHROW( deeOutOfMemory );
			}
			
			pVBODataFacesCount++;
		}
		
		pClusters[ c ].SetOffsetVBODataFaces( vboOffset );
		pClusters[ c ].SetVBODataFaces( pVBODataFaces[ v ] );
		vboOffset += pClusters[ c ].GetCountVBODataFaces();
		
		assignedClusters++;
		if( assignedClusters == 32 ){
			OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pVBODataFaces[ v ] ) );
			OGL_CHECK( renderThread, pglBufferData( GL_ELEMENT_ARRAY_BUFFER,
				sizeof( GLushort ) * vboOffset, NULL, GL_STATIC_DRAW ) );
//renderThread.GetLogger().LogInfoFormat( "htsector index: size=%i vbo=%u", sizeof(GLushort)*vboOffset, pVBODataFaces[ v ] );
			
			v++;
			assignedClusters = 0;
			vboOffset = 0;
		}
	}
	
	if( v < vboCount && vboOffset > 0 ){
		OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pVBODataFaces[ v ] ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ELEMENT_ARRAY_BUFFER,
			sizeof( GLushort ) * vboOffset, NULL, GL_STATIC_DRAW ) );
//renderThread.GetLogger().LogInfoFormat( "htsector index: size=%i vbo=%u", sizeof(GLushort)*vboOffset, pVBODataFaces[ v ] );
	}
	
	for( c=0; c<clusterCount; c++ ){
		pClusters[ c ].UpdateVBODataFaces();
	}
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
}

void deoglRHTSector::pUpdateHeightMap(){
	if( ! pDirtyPoints ){
		return;
	}
	pDirtyPoints = false;
	
	if( ! pValid || ! pHeights ){
		return;
	}
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = pHeightTerrain.GetRenderThread(); )
	int /*b, l, p,*/ c, clusterCount = pClusterCount * pClusterCount;
	const int imageDim = pHeightTerrain.GetSectorResolution();
//	float invCellSize = ( float )imageDim / engHT->GetSectorSize();
//	float invCellSize2 = invCellSize * 2.0f;
//	deoglVBOHeightTerrain2 *baseDataPoint;
//	oglHTFacePoint *baseFacePoint;
	int firstPointX, firstPointZ;
	int pointCountX, pointCountZ;
	//float nx, ny, nz;
	int x, z;
	
	// create vbos if not existing already
	if( ! pVBODataPoints2 ){
		int vboOffset = 0, assignedClusters = 0;
		int v, vboCount = ( ( clusterCount - 1 ) / 32 ) + 1;
		
		if( vboCount > 0 ){
			pVBODataPoints2 = new GLuint[ vboCount ];
			if( ! pVBODataPoints2 ) DETHROW( deeOutOfMemory );
			
			for( v=0; v<vboCount; v++ ){
				pVBODataPoints2[ v ] = 0;
			}
			
			for( v=0, c=0; c<clusterCount; c++ ){
				if( ! pVBODataPoints2[ v ] ){
					OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBODataPoints2[ v ] ) );
					if( ! pVBODataPoints2[ v ] ) DETHROW( deeOutOfMemory );
					
					pVBODataPoints2Count++;
				}
				
				pClusters[ c ].SetVBODataPoints2( pVBODataPoints2[ v ] );
				vboOffset += pClusters[ c ].GetCountVBODataPoints();
				
				assignedClusters++;
				if( assignedClusters == 32 ){
					OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints2[ v ] ) );
					OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
						sizeof( deoglVBOHeightTerrain2 ) * vboOffset, NULL, GL_STREAM_DRAW ) );
//renderThread.GetLogger().LogInfoFormat( "htsector data2: size=%i vbo=%u", sizeof( deoglVBOHeightTerrain2 )*vboOffset, pVBODataPoints2[ v ] );
					
					v++;
					assignedClusters = 0;
					vboOffset = 0;
				}
			}
			
			if( v < vboCount && vboOffset > 0 ){
				OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints2[ v ] ) );
				OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER,
					sizeof( deoglVBOHeightTerrain2 ) * vboOffset, NULL, GL_STREAM_DRAW ) );
//renderThread.GetLogger().LogInfoFormat( "htsector data2: size=%i vbo=%u", sizeof( deoglVBOHeightTerrain2 )*vboOffset, pVBODataPoints2[ v ] );
			}
		}
	}
	
	// update vbos, vaos and rts instances
	for( c=0; c<clusterCount; c++ ){
		pClusters[ c ].UpdateVBOData2();
	}
	for( c=0; c<clusterCount; c++ ){
		pClusters[ c ].UpdateVAO();
	}
	
	// update bounding boxes
	pMinHeight = 0.0f;
	pMaxHeight = 0.0f;
	
	float minHeight, maxHeight;
	int lastZ, firstX, lastX;
	
	for( c=0; c<clusterCount; c++ ){
		deoglHTSCluster &cluster = pClusters[ c ];
		firstPointX = cluster.GetFirstPointX();
		firstPointZ = cluster.GetFirstPointZ();
		pointCountX = cluster.GetPointCountX();
		pointCountZ = cluster.GetPointCountZ();
		
		lastZ = firstPointZ + pointCountZ;
		maxHeight = minHeight = pHeights[ imageDim * firstPointZ + firstPointX ];
		
		for( z=firstPointZ; z<lastZ; z++ ){
			firstX = imageDim * z + firstPointX;
			lastX = firstX + pointCountX;
			
			for( x=firstX; x<lastX; x++ ){
				if( pHeights[ x ] < minHeight ){
					minHeight = pHeights[ x ];
					
				}else if( pHeights[ x ] > maxHeight ){
					maxHeight = pHeights[ x ];
				}
			}
		}
		
		cluster.UpdateHeightExtends( minHeight, maxHeight );
		
		if( c == 0 ){
			pMinHeight = minHeight;
			pMaxHeight = maxHeight;
			
		}else{
			pMinHeight = decMath::min( pMinHeight, minHeight );
			pMaxHeight = decMath::max( pMaxHeight, maxHeight );
		}
	}
//DEBUG_PRINT_TIMER( "HTSector: Extends" );
}
