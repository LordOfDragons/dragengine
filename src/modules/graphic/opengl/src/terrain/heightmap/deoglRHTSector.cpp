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
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../delayedoperation/deoglDelayedDeletion.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../utils/collision/deoglDCollisionBox.h"

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

pBaseHeight( 0.0f ),
pScaling( 1.0f ),

pTextures( NULL ),
pTextureCount( 0 ),
pValidTextures( false ),
pDirtyTextures( true ),

pHeights( NULL ),

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
		pPixBufMasks[ i ] = NULL;
	}
	
	try{
		pCreateArrays( sector );
		
	}catch( const deException &e ){
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

void deoglRHTSector::UpdateVBO(){
	if( ! pValid ){
		return;
	}
	
	if( pDirtyTextures ){
		pUpdateTextures();
		pDirtyTextures = false;
	}
	
	if( pDirtyPoints ){
		pUpdateHeightMap();
		pDirtyPoints = false;
	}
}



deoglHTSTexture &deoglRHTSector::GetTextureAt( int index ){
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	
	return *pTextures[ index ];
}



void deoglRHTSector::HeightChanged( const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to ){
	pDirtyPoints = true;
	
	pSyncHeightMap( sector, from, to );
}

void deoglRHTSector::SectorChanged( const deHeightTerrainSector &sector ){
	pDirtyPoints = true;
	pDirtyTextures = true;
	
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



// Private functions
//////////////////////

class deoglRHTSectorDeletion : public deoglDelayedDeletion{
public:
	deoglHTSTexture **textures;
	int textureCount;
	deoglTexture *masks[ OGLHTS_MAX_MASK_TEXTURES ];
	deoglHTSCluster *clusters;
	GLuint *vboDataPoints1;
	int vboDataPoints1Count;
	GLuint *vboDataPoints2;
	int vboDataPoints2Count;
	GLuint *vboDataFaces;
	int vboDataFacesCount;
	
	deoglRHTSectorDeletion() :
	textures( NULL ),
	textureCount( 0 ),
	clusters( NULL ),
	vboDataPoints1( NULL ),
	vboDataPoints1Count( 0 ),
	vboDataPoints2( NULL ),
	vboDataPoints2Count( 0 ),
	vboDataFaces( NULL ),
	vboDataFacesCount( 0 ){
		int i;
		for( i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++ ){
			masks[ i ] = NULL;
		}
	}
	
	virtual ~deoglRHTSectorDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		int i;
		if( clusters ){
			delete [] clusters;
		}
		if( textures ){
			for( i=0; i<textureCount; i++ ){
				delete textures[ i ];
			}
			delete [] textures;
		}
		for( i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++ ){
			if( masks[ i ] ){
				delete masks[ i ];
			}
		}
		if( vboDataFaces ){
			for( i=0; i<vboDataFacesCount; i++ ){
				pglDeleteBuffers( 1, &vboDataFaces[ i ] );
			}
			delete [] vboDataFaces;
		}
		if( vboDataPoints2 ){
			for( i=0; i<vboDataPoints2Count; i++ ){
				pglDeleteBuffers( 1, &vboDataPoints2[ i ] );
			}
			delete [] vboDataPoints2;
		}
		if( vboDataPoints1 ){
			for( i=0; i<vboDataPoints1Count; i++ ){
				pglDeleteBuffers( 1, &vboDataPoints1[ i ] );
			}
			delete [] vboDataPoints1;
		}
	}
};

void deoglRHTSector::pCleanUp(){
	int i;
	
	if( pHeights ){
		delete [] pHeights;
	}
	
	pDropMaskPixelBuffers();
	
	// drop reference otherwise deletion can cause other deletions to be generated
	// causing a deletion race
	if( pTextures ){
		for( i=0; i<pTextureCount; i++ ){
			pTextures[ i ]->SetSkin( NULL );
		}
	}
	
	// delayed deletion of opengl containing objects
	deoglRHTSectorDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRHTSectorDeletion;
		delayedDeletion->textures = pTextures;
		delayedDeletion->textureCount = pTextureCount;
		for( i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++ ){
			delayedDeletion->masks[ i ] = pMasks[ i ];
		}
		delayedDeletion->clusters = pClusters;
		delayedDeletion->vboDataFaces = pVBODataFaces;
		delayedDeletion->vboDataFacesCount = pVBODataFacesCount;
		delayedDeletion->vboDataPoints1 = pVBODataPoints1;
		delayedDeletion->vboDataPoints1Count = pVBODataPoints1Count;
		delayedDeletion->vboDataPoints2 = pVBODataPoints2;
		delayedDeletion->vboDataPoints2Count = pVBODataPoints2Count;
		pHeightTerrain.GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pHeightTerrain.GetRenderThread().GetLogger().LogException( e );
		throw;
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
	
#if 0
	// create points
	pPoints = new deoglVBOHeightTerrain[ count ];
	if( ! pPoints ) DETHROW( deeOutOfMemory );
	pPointCount = count;
	
	i = 0;
	for( z=0; z<imageDim; z++ ){
		for( x=0; x<imageDim; x++ ){
			pPoints[ i ].x = ( float )( x ) * sizeStep - sizeOffset;
			pPoints[ i ].z = sizeOffset - ( float )( z ) * sizeStep;
			i++;
		}
	}
	
	// create faces
	int visibleFaceCount = 0;
	for( z=0; z<imageDim-1; z++ ){
		for( x=0; x<imageDim-1; x++ ){
			if( pSector->GetFaceVisibleAt( x, z ) ){
				visibleFaceCount += 2;
			}
		}
	}
	
	if( visibleFaceCount > 0 ){
		pFaces = new deoglTerrainFace[ visibleFaceCount ];
		if( ! pFaces ) DETHROW( deeOutOfMemory );
		pFaceCount = visibleFaceCount;
		
		int vertexIndex, faceIndex = 0;
		for( z=0; z<imageDim-1; z++ ){
			for( x=0; x<imageDim-1; x++ ){
				if( pSector->GetFaceVisibleAt( x, z ) ){
					vertexIndex = z * imageDim + x;
					pFaces[ faceIndex ].SetPoint3( vertexIndex );
					pFaces[ faceIndex ].SetPoint2( vertexIndex + 1 );
					pFaces[ faceIndex ].SetPoint1( vertexIndex + imageDim );
					faceIndex++;
					pFaces[ faceIndex ].SetPoint3( vertexIndex + 1 );
					pFaces[ faceIndex ].SetPoint2( vertexIndex + imageDim + 1 );
					pFaces[ faceIndex ].SetPoint1( vertexIndex + imageDim );
					faceIndex++;
				}
			}
		}
	}
#endif
	
	// create clusters
	int maxPointsPerCluster = 65;
	int pcx, pcz, curx, curz;
	
	pClusterCount = ( ( imageDim - 2 ) / ( maxPointsPerCluster - 1 ) ) + 1;
	
	pClusters = new deoglHTSCluster[ pClusterCount * pClusterCount ];
	
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
			
			pClusters[ pClusterCount * z + x ].SetHTSector( this );
			pClusters[ pClusterCount * z + x ].SetSize( curx, curz, pcx, pcz );
			
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
	
	// create the position data points
#if 0
	if( pFacePointCount > 0 ){
		int b, l, c, p, clusterCount = pClusterCount * pClusterCount;
		int firstPointX, firstPointZ, pointCountX;
		deoglVBOHeightTerrain1 *baseDataPoint;
		oglHTFacePoint *baseFacePoint;
		
		pDataPoints1 = new deoglVBOHeightTerrain1[ pFacePointCount ];
		if( ! pDataPoints1 ) DETHROW( deeOutOfMemory );
		
		for( c=0; c<clusterCount; c++ ){
			deoglHTSCluster &cluster = pClusters[ c ];
			firstPointX = cluster.GetFirstPointX();
			firstPointZ = cluster.GetFirstPointZ();
			pointCountX = cluster.GetPointCountX();
			
			for( l=0; l<HTSC_MAX_LOD; l++ ){
				const deoglHTSClusterLOD &clod = cluster.GetLODAt( l );
				
				baseFacePoint = pFacePoints + clod.firstBasePoint;
				baseDataPoint = pDataPoints1 + clod.firstBasePoint;
				for( p=0; p<clod.basePointCount; p++ ){
					x = firstPointX + baseFacePoint[ p ].x;
					z = firstPointZ + baseFacePoint[ p ].z;
					baseDataPoint[ p ].x = ( float )( x ) * sizeStep - sizeOffset;
					baseDataPoint[ p ].z = sizeOffset - ( float )( z ) * sizeStep;
				}
				
				for( b=0; b<8; b++ ){
					baseFacePoint = pFacePoints + clod.firstBorderPoint[ b ];
					baseDataPoint = pDataPoints1 + clod.firstBorderPoint[ b ];
					for( p=0; p<clod.borderPointCount[ b ]; p++ ){
						x = firstPointX + baseFacePoint[ p ].x;
						z = firstPointZ + baseFacePoint[ p ].z;
						baseDataPoint[ p ].x = ( float )( x ) * sizeStep - sizeOffset;
						baseDataPoint[ p ].z = sizeOffset - ( float )( z ) * sizeStep;
					}
				}
			}
		}
	}
#endif
	
	// dirty
	pDirtyPoints = true;
}

void deoglRHTSector::pSetTextureCount( int count ){
	if( count == pTextureCount ){
		return;
	}
	
	int i;
	
	if( pTextures ){
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
		if( pPixBufMasks[ i ] ){
			delete pPixBufMasks[ i ];
			pPixBufMasks[ i ] = NULL;
		}
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
		deHeightTerrainTexture *texture = sector.GetTextureAt( i );
		
		pTextures[ i ]->SetMatrix( decTexMatrix::CreateScale( texture->GetProjectionScaling().x, texture->GetProjectionScaling().y )
			* decTexMatrix::CreateTranslation( texture->GetProjectionOffset().x, texture->GetProjectionOffset().y )
			* decTexMatrix::CreateRotation( texture->GetProjectionRotation() ) );
	}
	
	// update skins if dirty
	if( pValidTextures ){
		for( i=0; i<pTextureCount; i++ ){
			deSkin *skin = sector.GetTextureAt( i )->GetSkin();
			
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
		pPixBufMasks[ m ] = new deoglPixelBuffer( deoglPixelBuffer::epfFloat4, maskWidth, maskHeight, 1 );
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
}



void deoglRHTSector::pUpdateTextures(){
	if( ! pValidTextures ){
		return;
	}
	
	// recalculate texture coordinates
	try{
		pCalculateUVs();
		
	}catch( const deException &e ){
// 				ogl.SetErrorTrace( e );
		throw;
	}
	
	// create vbo if not existing and fill it with data
	if( ! pVBODataPoints1 ){
		pCreateVBODataPoints1();
	}
	if( ! pVBODataFaces ){
		pCreateVBODataFaces();
	}
	
	// update mask textures
	pUpdateMaskTextures();
}

void deoglRHTSector::pUpdateMaskTextures(){
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
			
			pMasks[ i ]->SetPixels( *pPixBufMasks[ i ] );
			
		}else{
			if( pMasks[ i ] ){
				delete pMasks[ i ];
				pMasks[ i ] = NULL;
			}
		}
	}
}

void deoglRHTSector::pCalculateUVs(){
	int i;
	
	for( i=0; i<pTextureCount; i++ ){
		pCalculateUVsPlanar( i );
	}
}

void deoglRHTSector::pCalculateUVsPlanar( int textureIndex ){
#if 0
//	deHeightTerrainTexture *texture = pSector->GetTextureAt( textureIndex );
	const decTexMatrix &matrix = pTextures[ textureIndex ].matrix;
	decVector projU, projV;
	int f;
	
	float dotU, dotV, len;
	decVector tangent;
	
	// determine projection vector
	// TODO remove the true projection calculation and use a fast one as the axes
	// are known in this situation beforehand.
	projU.Set(  1.0f,  0.0f,  0.0f );
	projV.Set(  0.0f,  0.0f, -1.0f );
	
	// calculate the texture coordinates for all faces having this texture somewhere.
	// the texture coordinates are projected along the chosen projection axis using
	// the uv-matrix. the tangents are calculate by crossing the v-projection vector
	// with the face normal.
	for( f=0; f<pFaceCount; f++ ){
		const deoglTerrainFace &oglTFace = pFaces[ f ];
		if( oglTFace.GetTextureIndex() == textureIndex ){
			// calculate the tangent for the face. all points of this face share the same tangent
			const decVector &normal = oglTFace.GetNormal();
			tangent = projV % normal;
			len = tangent.Length();
			if( len != 0.0f ){
				tangent /= len;
			}else{
				tangent.Set( 1.0f, 0.0f, 0.0f );
			}
			
			// first corner
			deoglVBOHeightTerrain &vboP1 = pPoints[ oglTFace.GetPoint1() ];
			
			dotU = vboP1.x * projU.x + vboP1.y * projU.y + vboP1.z * projU.z; // dotU = vboP1.x;
			dotV = vboP1.x * projV.x + vboP1.y * projV.y + vboP1.z * projV.z; // dotV = -vboP1.z;
			vboP1.u = matrix.a11 * dotU + matrix.a12 * dotV + matrix.a13; // vboP1.u = matrix.a11 * vboP1.x - matrix.a12 * vboP1.z + matrix.a13;
			vboP1.v = 1.0f - ( matrix.a21 * dotU + matrix.a22 * dotV + matrix.a23 ); // vboP1.v = 1.0f - matrix.a21 * vboP1.x + matrix.a22 * vboP1.z - matrix.a23;
			
			vboP1.tx = tangent.x;
			vboP1.ty = tangent.y,
			vboP1.tz = tangent.z;
			
			// second corner
			deoglVBOHeightTerrain &vboP2 = pPoints[ oglTFace.GetPoint2() ];
			
			dotU = vboP2.x * projU.x + vboP2.y * projU.y + vboP2.z * projU.z;
			dotV = vboP2.x * projV.x + vboP2.y * projV.y + vboP2.z * projV.z;
			vboP2.u = matrix.a11 * dotU + matrix.a12 * dotV + matrix.a13;
			vboP2.v = 1.0f - ( matrix.a21 * dotU + matrix.a22 * dotV + matrix.a23 );
			
			vboP2.tx = tangent.x;
			vboP2.ty = tangent.y;
			vboP2.tz = tangent.z;
			
			// third corner
			deoglVBOHeightTerrain &vboP3 = pPoints[ oglTFace.GetPoint3() ];
			
			dotU = vboP3.x * projU.x + vboP3.y * projU.y + vboP3.z * projU.z;
			dotV = vboP3.x * projV.x + vboP3.y * projV.y + vboP3.z * projV.z;
			vboP3.u = matrix.a11 * dotU + matrix.a12 * dotV + matrix.a13;
			vboP3.v = 1.0f - ( matrix.a21 * dotU + matrix.a22 * dotV + matrix.a23 );
			
			vboP3.tx = tangent.x;
			vboP3.ty = tangent.y;
			vboP3.tz = tangent.z;
		}
	}
#endif
}

void deoglRHTSector::pCreateVBODataPoints1(){
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
	if( ! pHeights ){
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
	
#if 0
	// create height data points if not existing already
	if( ! pDataPoints2 ){
		pDataPoints2 = new deoglVBOHeightTerrain2[ pFacePointCount ];
		if( ! pDataPoints2 ) DETHROW( deeOutOfMemory );
	}
	
	// update the height data points height values
	// TODO only update the values which are inside the dirty area
	for( c=0; c<clusterCount; c++ ){
		deoglHTSCluster &cluster = pClusters[ c ];
		firstPointX = cluster.GetFirstPointX();
		firstPointZ = cluster.GetFirstPointZ();
		pointCountX = cluster.GetPointCountX();
		
		for( l=0; l<HTSC_MAX_LOD; l++ ){
			const deoglHTSClusterLOD &clod = cluster.GetLODAt( l );
			
			baseFacePoint = pFacePoints + clod.firstBasePoint;
			baseDataPoint = pDataPoints2 + clod.firstBasePoint;
			for( p=0; p<clod.basePointCount; p++ ){
				i = ( firstPointZ + baseFacePoint[ p ].z ) * imageDim + firstPointX + baseFacePoint[ p ].x;
				baseDataPoint[ p ].height = pHeights[ i ];
			}
			
			for( b=0; b<8; b++ ){
				baseFacePoint = pFacePoints + clod.firstBorderPoint[ b ];
				baseDataPoint = pDataPoints2 + clod.firstBorderPoint[ b ];
				for( p=0; p<clod.borderPointCount[ b ]; p++ ){
					i = ( firstPointZ + baseFacePoint[ p ].z ) * imageDim + firstPointX + baseFacePoint[ p ].x;
					baseDataPoint[ p ].height = pHeights[ i ];
				}
			}
		}
	}
//DEBUG_PRINT_TIMER( "HTSector: Update Height Data Points" );
	
	// calculate normals
	// NOTE the normal calculation could be sped up using a LUT. after all only a certain
	//      range of gradients yields different normals
	for( c=0; c<clusterCount; c++ ){
		deoglHTSCluster &cluster = pClusters[ c ];
		firstPointX = cluster.GetFirstPointX();
		firstPointZ = cluster.GetFirstPointZ();
		pointCountX = cluster.GetPointCountX();
		
		for( l=0; l<HTSC_MAX_LOD; l++ ){
			const deoglHTSClusterLOD &clod = cluster.GetLODAt( l );
			
			baseFacePoint = pFacePoints + clod.firstBasePoint;
			baseDataPoint = pDataPoints2 + clod.firstBasePoint;
			for( p=0; p<clod.basePointCount; p++ ){
				x = firstPointX + baseFacePoint[ p ].x;
				z = firstPointZ + baseFacePoint[ p ].z;
				i = z * imageDim + x;
				
				if( x == 0 ){
					nx = 1.0f / ( 1.0f + ( pHeights[ i + 1 ] - pHeights[ i ] ) * invCellSize ) - 1.0f;
					
				}else if( x == imageDim - 1 ){
					nx = 1.0f / ( 1.0f + ( pHeights[ i ] - pHeights[ i - 1 ] ) * invCellSize ) - 1.0f;
					
				}else{
					nx = 1.0f / ( 1.0f + ( pHeights[ i + 1 ] - pHeights[ i - 1 ] ) * invCellSize2 ) - 1.0f;
				}
				
				if( z == 0 ){
					nz = 1.0f / ( 1.0f + ( pHeights[ i ] - pHeights[ i + imageDim ] ) * invCellSize ) - 1.0f;
					
				}else if( z == imageDim - 1 ){
					nz = 1.0f / ( 1.0f + ( pHeights[ i - imageDim ] - pHeights[ i ] ) * invCellSize ) - 1.0f;
					
				}else{
					nz = 1.0f / ( 1.0f + ( pHeights[ i - imageDim ] - pHeights[ i + imageDim ] ) * invCellSize2 ) - 1.0f;
				}
				
				ny = sqrtf( 1.0f - nx * nx + nz * nz );
				
				baseDataPoint[ p ].nx = ( GLbyte )( ( int )( ( nx * 0.5f + 0.5f ) * 255.0f ) - 128 );
				baseDataPoint[ p ].ny = ( GLbyte )( ( int )( ( ny * 0.5f + 0.5f ) * 255.0f ) - 128 );
				baseDataPoint[ p ].nz = ( GLbyte )( ( int )( ( nz * 0.5f + 0.5f ) * 255.0f ) - 128 );
			}
			
			for( b=0; b<8; b++ ){
				baseFacePoint = pFacePoints + clod.firstBorderPoint[ b ];
				baseDataPoint = pDataPoints2 + clod.firstBorderPoint[ b ];
				for( p=0; p<clod.borderPointCount[ b ]; p++ ){
					x = firstPointX + baseFacePoint[ p ].x;
					z = firstPointZ + baseFacePoint[ p ].z;
					i = z * imageDim + x;
					
					if( x == 0 ){
						nx = 1.0f / ( 1.0f + ( pHeights[ i + 1 ] - pHeights[ i ] ) * invCellSize ) - 1.0f;
						
					}else if( x == imageDim - 1 ){
						nx = 1.0f / ( 1.0f + ( pHeights[ i ] - pHeights[ i - 1 ] ) * invCellSize ) - 1.0f;
						
					}else{
						nx = 1.0f / ( 1.0f + ( pHeights[ i + 1 ] - pHeights[ i - 1 ] ) * invCellSize2 ) - 1.0f;
					}
					
					if( z == 0 ){
						nz = 1.0f / ( 1.0f + ( pHeights[ i ] - pHeights[ i + imageDim ] ) * invCellSize ) - 1.0f;
						
					}else if( z == imageDim - 1 ){
						nz = 1.0f / ( 1.0f + ( pHeights[ i - imageDim ] - pHeights[ i ] ) * invCellSize ) - 1.0f;
						
					}else{
						nz = 1.0f / ( 1.0f + ( pHeights[ i - imageDim ] - pHeights[ i + imageDim ] ) * invCellSize2 ) - 1.0f;
					}
					
					ny = sqrtf( 1.0f - nx * nx + nz * nz );
					
					baseDataPoint[ p ].nx = ( GLbyte )( ( int )( ( nx * 0.5f + 0.5f ) * 255.0f ) - 128 );
					baseDataPoint[ p ].ny = ( GLbyte )( ( int )( ( ny * 0.5f + 0.5f ) * 255.0f ) - 128 );
					baseDataPoint[ p ].nz = ( GLbyte )( ( int )( ( nz * 0.5f + 0.5f ) * 255.0f ) - 128 );
				}
			}
		}
	}
//DEBUG_PRINT_TIMER( "HTSector: Calculate Normals" );
	
	// update the vbo
	if( ! pVBODataPoints2 ){
		OGL_CHECK( renderThread, oglGenBuffers( 1, &pVBODataPoints2 ) );
		if( ! pVBODataPoints2 ) DETHROW( deeOutOfMemory );
	}
	
	OGL_CHECK( renderThread, oglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints2 ) );
	OGL_CHECK( renderThread, oglBufferData( GL_ARRAY_BUFFER,
		sizeof( deoglVBOHeightTerrain2 ) * pFacePointCount, pDataPoints2, GL_STREAM_DRAW ) );
//DEBUG_PRINT_TIMER( "HTSector: Update VBO" );
#endif
	
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
	
	// update vbos
	for( c=0; c<clusterCount; c++ ){
		pClusters[ c ].UpdateVBOData2();
	}
	
	// update vaos
	for( c=0; c<clusterCount; c++ ){
		pClusters[ c ].UpdateVAO();
	}
	
	// update bounding boxes
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
	}
//DEBUG_PRINT_TIMER( "HTSector: Extends" );
}
