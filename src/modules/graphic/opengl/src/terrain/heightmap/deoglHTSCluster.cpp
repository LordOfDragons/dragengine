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

#include "deoglHTSCluster.h"
#include "deoglHTSTexture.h"
#include "deoglHTViewSectorCluster.h"
#include "deoglRHeightTerrain.h"
#include "deoglRHTSector.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../vao/deoglVAO.h"
#include "../../vbo/deoglVBOLayout.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>



// Definitions
////////////////

#define HTSC_VARIANCE_SCALE		( 250.0f / 0.05f )



// Class deoglHTSCluster::WorldComputeElement
///////////////////////////////////////////////

deoglHTSCluster::WorldComputeElement::WorldComputeElement( deoglHTSCluster &cluster ) :
deoglWorldComputeElement( eetHeightTerrainSectorCluster, &cluster ),
pCluster( cluster ){
}

void deoglHTSCluster::WorldComputeElement::UpdateData( sDataElement &data ) const{
	const decDVector center( decDVector( pCluster.GetCenter() ) - GetReferencePosition() );
	const decDVector halfSize( pCluster.GetHalfExtends() );
	
	data.SetExtends( center - halfSize, center + halfSize );
	data.SetEmptyLayerMask();
	data.flags = ( uint32_t )( deoglWorldCompute::eefHeightTerrainSectorCluster
		| deoglWorldCompute::eefStatic | deoglWorldCompute::eefGIStatic );
	data.geometryCount = 0; //( uint32_t )pCluster.GetHTSector()->GetTextureCount() * 2;
	data.highestLod = 0;
}

void deoglHTSCluster::WorldComputeElement::UpdateDataGeometries( sDataElementGeometry *data ) const{
#if 0
	const deoglRHTSector &sector = *pCluster.GetHTSector();
	const deoglHTSCluster &htcluster = sector.GetClusters()[ pCluster.GetIndex() ];
	const bool valid = sector.GetValid() && sector.GetValidTextures();
	const int count = pCluster.GetHTSector()->GetTextureCount();
	const deoglVAO * const vao = htcluster.GetVAO();
	const deoglHTViewSectorCluster &htvscluster = *clhtscluster.GetCluster();
	int i, j;
	
	for( i=0; i<count; i++ ){
		const deoglHTSTexture &texture = sector.GetTextureAt( i );
		const deoglSkinTexture * const skinTexture = texture.GetUseSkinTexture();
		if( ! valid || ! skinTexture ){
			continue;
		}
		
		sInfoTUC info;
		info.geometry = texture.GetTUCGeometry();
		info.depth = texture.GetTUCDepth();
		info.counter = texture.GetTUCDepth();
		info.shadow = texture.GetTUCShadow();
		info.shadowCube = texture.GetTUCShadow();
		info.envMap = texture.GetTUCEnvMap();
		info.luminance = texture.GetTUCLuminance();
		// info.giMaterial = texture.GetTUCGIMaterial(); // missing
		
		const int filters = skinTexture->GetRenderTaskFilters() & ~RenderFilterOutline;
		
		SetDataGeometry( *data, 0, filters, deoglSkinTexturePipelinesList::eptHeightMap1, 0,
			skinTexture, vao, htvscluster.GetRTSInstanceAt( i, 0 ), -1 );
		SetDataGeometryTUCs( *data, info );
		data++;
		
		SetDataGeometry( *data, 0, filters, deoglSkinTexturePipelinesList::eptHeightMap2, 0,
			skinTexture, vao, htvscluster.GetRTSInstanceAt( i, 0 ), -1 );
		SetDataGeometryTUCs( *data, info );
		data++;
		
		if( htvscluster.GetLodLevel() > 0 ){
			for( j=1; j<5; j++ ){
				SetDataGeometry( *data, 0, filters, deoglSkinTexturePipelinesList::eptHeightMap1,
					0, skinTexture, vao, htvscluster.GetRTSInstanceAt( i, j ), -1 );
				SetDataGeometryTUCs( *data, info );
				data++;
				
				SetDataGeometry( *data, 0, filters, deoglSkinTexturePipelinesList::eptHeightMap2,
					0, skinTexture, vao, htvscluster.GetRTSInstanceAt( i, 0 ), -1 );
				SetDataGeometryTUCs( *data, info );
				data++;
			}
		}
	}
#endif
}



// Class deoglHTSCluster
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglHTSCluster::deoglHTSCluster() :
pIndex( -1 ),

pWorldComputeElement( deoglWorldComputeElement::Ref::New( new WorldComputeElement( *this ) ) )
{
	pHTSector = NULL;
	
	pPointCountX = 0;
	pPointCountZ = 0;
	
	memset( &pLOD, '\0', sizeof( pLOD ) );
	
	pNoLOD = false;
	
	pOffsetVBODataPoints = 0;
	pVBODataPoints1 = 0;
	pVBODataPoints2 = 0;
	
	pOffsetVBODataFaces = 0;
	pVBODataFaces = 0;
	
	pVAO = NULL;
	
	pFacePoints = NULL;
	pFacePointCount = 0;
	pFacePointSize = 0;
	
	pDataPoints1 = NULL;
	pDataPoints2 = NULL;
	pDataPointCount = 0;
}

deoglHTSCluster::~deoglHTSCluster(){
	if( pVAO ){
		delete pVAO;
	}
	
	if( pDataPoints2 ){
		delete [] pDataPoints2;
	}
	if( pDataPoints1 ){
		delete [] pDataPoints1;
	}
	if( pFacePoints ){
		delete [] pFacePoints;
	}
}



// Management
///////////////

void deoglHTSCluster::SetHTSector( deoglRHTSector *htsector ){
	pHTSector = htsector;
}

void deoglHTSCluster::SetCoordinates( const decPoint &coordinates ){
	pCoordinates = coordinates;
}

void deoglHTSCluster::SetIndex( int index ){
	pIndex = index;
}

void deoglHTSCluster::SetSize( int firstPointX, int firstPointZ, int pointCountX, int pointCountZ ){
	if( pointCountX < 3 || pointCountZ < 3 ) DETHROW( deeInvalidParam );
	
	pFirstPointX = firstPointX;
	pFirstPointZ = firstPointZ;
	pPointCountX = pointCountX;
	pPointCountZ = pointCountZ;
	
	pDataPointCount = pPointCountX * pPointCountZ;
}

void deoglHTSCluster::InitFromHeightImage( const deHeightTerrainSector &sector ){
	if( ! pHTSector ){
		DETHROW( deeInvalidParam );
	}
	
	const float sectorDim = pHTSector->GetHeightTerrain().GetSectorSize();
	const int imageDim = pHTSector->GetHeightTerrain().GetSectorResolution();
	const float baseHeight = pHTSector->GetBaseHeight();
	const float scaling = pHTSector->GetScaling();
	int x, z, i, l;
	
	// calculate height extends of this cluster
	pCenter.x = ( ( ( float )pFirstPointX + ( float )pPointCountX / 2.0f ) * sectorDim / ( float )( imageDim + 1 ) ) - sectorDim * 0.5f;
	pCenter.y = baseHeight;
	pCenter.z = sectorDim * 0.5f - ( ( ( float )pFirstPointZ + ( float )pPointCountZ / 2.0f ) * sectorDim / ( float )( imageDim + 1 ) );
	pHalfExtends.x = ( ( float )pPointCountX / 2.0f ) * sectorDim / ( float )( imageDim + 1 );
	pHalfExtends.y = scaling;
	pHalfExtends.z = ( ( float )pPointCountZ / 2.0f ) * sectorDim / ( float )( imageDim + 1 );
	
	// create the base LOD level
	pCreateBaseLOD( sector, imageDim );
	
	// create higher LOD levels if required
	if( pNoLOD ){
		for( l=1; l<=HTSC_MAX_LOD; l++ ){
			pLOD[ l ].firstBasePoint = 0;
			pLOD[ l ].basePointCount = 0;
			for( i=0; i<8; i++ ){
				pLOD[ l ].firstBorderPoint[ i ] = 0;
				pLOD[ l ].borderPointCount[ i ] = 0;
			}
		}
		
	}else{
		for( l=1; l<=HTSC_MAX_LOD; l++ ){
			pCreateCheapLOD( l );
		}
	}
	
	// create position data points
	float sizeStep = sectorDim / ( float )( imageDim - 1 );
	float sizeOffset = sectorDim * 0.5f;
	
	pDataPoints1 = new deoglVBOHeightTerrain1[ pDataPointCount ];
	
	for( i=0, z=0; z<pPointCountZ; z++ ){
		for( x=0; x<pPointCountX; x++, i++ ){
			pDataPoints1[ i ].x = ( float )( pFirstPointX + x ) * sizeStep - sizeOffset;
			pDataPoints1[ i ].z = sizeOffset - ( float )( pFirstPointZ + z ) * sizeStep;
		}
	}
	
	pDataPoints2 = new deoglVBOHeightTerrain2[ pDataPointCount ];
}



deoglHTSClusterLOD &deoglHTSCluster::GetLODAt( int level ){
	if( level < 0 || level > HTSC_MAX_LOD ){
		DETHROW( deeInvalidParam );
	}
	return pLOD[ level ];
}

const deoglHTSClusterLOD & deoglHTSCluster::GetLODAt( int level ) const{
	if( level < 0 || level > HTSC_MAX_LOD ){
		DETHROW( deeInvalidParam );
	}
	return pLOD[ level ];
}

void deoglHTSCluster::UpdateHeightExtends( float minHeight, float maxHeight ){
	pCenter.y = ( minHeight + maxHeight ) * 0.5f;
	pHalfExtends.y = ( maxHeight - minHeight ) * 0.5f;
	
	pWorldComputeElement->ComputeUpdateElement();
}



void deoglHTSCluster::DebugPrint(){
	deoglRTLogger &logger = pHTSector->GetHeightTerrain().GetRenderThread().GetLogger();
	
	//int x, z;
	logger.LogInfoFormat( "HTSCluster: start=(%i,%i) points=(%i,%i) center=(%g,%g,%g) halfExtends=(%g,%g,%g)",
		pFirstPointX, pFirstPointZ, pPointCountX, pPointCountZ, pCenter.x, pCenter.y, pCenter.z,
		pHalfExtends.x, pHalfExtends.y, pHalfExtends.z );
	/*
	logger.LogInfoFormat( "- LOD: 0(%i,%i) 1(%i,%i) 2(%i,%i) 3(%i,%i) 4(%i,%i) noLOD=%s fp=%i", pLOD[0].firstBasePoint, pLOD[0].basePointCount,
		pLOD[1].firstBasePoint, pLOD[1].basePointCount, pLOD[2].firstBasePoint, pLOD[2].basePointCount,
		pLOD[3].firstBasePoint, pLOD[3].basePointCount, pLOD[4].firstBasePoint, pLOD[4].basePointCount,
		pNoLOD ? "t" : "f", pFacePointCount );
	*/
}



void deoglHTSCluster::SetOffsetVBODataPoints( int offset ){
	pOffsetVBODataPoints = offset;
}

void deoglHTSCluster::SetVBODataPoints1( GLuint vbo ){
	pVBODataPoints1 = vbo;
}

void deoglHTSCluster::UpdateVBOData1(){
	if( ! pHTSector || ! pVBODataPoints1 ){
		DETHROW( deeInvalidParam );
	}
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = pHTSector->GetHeightTerrain().GetRenderThread() );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints1 ) );
	OGL_CHECK( renderThread, pglBufferSubData( GL_ARRAY_BUFFER,
		sizeof( deoglVBOHeightTerrain1 ) * pOffsetVBODataPoints,
		sizeof( deoglVBOHeightTerrain1 ) * pDataPointCount, pDataPoints1 ) );
}

void deoglHTSCluster::SetVBODataPoints2( GLuint vbo ){
	pVBODataPoints2 = vbo;
}

void deoglHTSCluster::UpdateVBOData2(){
	if( ! pHTSector || ! pVBODataPoints2 ){
		DETHROW( deeInvalidParam );
	}
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = pHTSector->GetHeightTerrain().GetRenderThread() );
	
	const int lastZ = pFirstPointZ + pPointCountZ;
	const int lastX = pFirstPointX + pPointCountX;
	const int imageDim = pHTSector->GetHeightTerrain().GetSectorResolution();
	const float * const heights = pHTSector->GetHeights();
	const float invCellSize = -( ( float )imageDim / pHTSector->GetHeightTerrain().GetSectorSize() );
	const float invCellSize2 = -( invCellSize * 2.0f );
	const float packNorFac = 1.0f / ( float )( 1 << 24 );
	float nx, nz, nscale;
	int i, p, x, z;
	
	// calculate normals. idea is using the height difference and applying a 90 degree
	// rotation counter clockwise in 2D to obtain the normal. this is done once for
	// the xy and zy plane. for the rotations in each plane holds
	// xy-plane: n'=( -hx, c, nz )
	// zy-plane: n'=( nx, c, -hz )
	// where h{x,z} are the height differences and c the distance between two grid
	// points. now doing this for the xy-plane results in n'=( -hx, c, nz ) with the
	// nice property that n'y is always c no matter what the height difference is.
	// therefore applying the zy-plane rotation afterwards we have that the n'y is
	// again c therefore n'y is kept unchanged in the second rotation. using this
	// property the final rotation for both planes becomes
	// n=( -hx, c, -hz )
	// to move the values close to normal length for typical orientations the height
	// differences are first divided by c which gives then
	// n=( -hx/c, 1, -hz/c )
	// in the calculations below the gradient is inversed to avoid taking the negation
	for( p=0, z=pFirstPointZ; z<lastZ; z++ ){
		for( x=pFirstPointX; x<lastX; x++, p++ ){
			i = z * imageDim + x;
			
			if( x == 0 ){
				nx = ( heights[ i ] - heights[ i + 1 ] ) * invCellSize;
				
			}else if( x == imageDim - 1 ){
				nx = ( heights[ i - 1 ] - heights[ i ] ) * invCellSize;
				
			}else{
				nx = ( heights[ i - 1 ] - heights[ i + 1 ] ) * invCellSize2;
			}
			
			if( z == 0 ){
				nz = ( heights[ i + imageDim ] - heights[ i ] ) * invCellSize;
				
			}else if( z == imageDim - 1 ){
				nz = ( heights[ i ] - heights[ i - imageDim ] ) * invCellSize;
				
			}else{
				nz = ( heights[ i + imageDim ] - heights[ i - imageDim ] ) * invCellSize2;
			}
			
			//nscale = 1.0f / sqrtf( nx * nx + nz * nz + 1.0f );
			nscale = 0.5f / sqrtf( nx * nx + nz * nz + 1.0f );
			
			pDataPoints2[ p ].height = heights[ i ];
			pDataPoints2[ p ].normal = packNorFac * ( float )(
				( ( unsigned char )( ( nx * nscale + 0.5f ) * 255.0f ) << 16 ) | 
				( ( unsigned char )( ( nscale + 0.5f ) * 255.0f ) << 8 ) |
				( ( unsigned char )( ( nz * nscale + 0.5f ) * 255.0f ) ) );
			//pDataPoints2[ p ].nx = nx * nscale;
			//pDataPoints2[ p ].ny = nscale;
			//pDataPoints2[ p ].nz = nz * nscale;
		}
	}
	
	// update vbo
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints2 ) );
	OGL_CHECK( renderThread, pglBufferSubData( GL_ARRAY_BUFFER,
		sizeof( deoglVBOHeightTerrain2 ) * pOffsetVBODataPoints,
		sizeof( deoglVBOHeightTerrain2 ) * pDataPointCount, pDataPoints2 ) );
}

void deoglHTSCluster::SetOffsetVBODataFaces( int offset ){
	pOffsetVBODataFaces = offset;
}

void deoglHTSCluster::SetVBODataFaces( GLuint vbo ){
	pVBODataFaces = vbo;
}

void deoglHTSCluster::UpdateVBODataFaces(){
	if( ! pHTSector || ! pVBODataFaces ){
		DETHROW( deeInvalidParam );
	}
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = pHTSector->GetHeightTerrain().GetRenderThread() );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pVBODataFaces ) );
	OGL_CHECK( renderThread, pglBufferSubData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLushort ) * pOffsetVBODataFaces,
		sizeof( GLushort ) * pFacePointCount, pFacePoints ) );
}

void deoglHTSCluster::UpdateVAO(){
	if( ! pHTSector ){
		DETHROW( deeInvalidParam );
	}
	
	deoglRenderThread &renderThread = pHTSector->GetHeightTerrain().GetRenderThread();
	
	if( pVBODataPoints1 && pVBODataPoints2 && pVBODataFaces ){
		if( ! pVAO ){
			pVAO = new deoglVAO( renderThread );
			
			pVAO->SetIndexType( deoglVBOLayout::eitUnsignedShort );
			OGL_CHECK( renderThread, pglBindVertexArray( pVAO->GetVAO() ) );
			
			OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints1 ) );
			
			OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) ); // position
			OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, VBO_PHT1_STRIDE,
				( const GLvoid * )( sizeof( deoglVBOHeightTerrain1 ) * pOffsetVBODataPoints + VBO_PHT1_POSITION ) ) );
			
			OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBODataPoints2 ) );
			
			OGL_CHECK( renderThread, pglEnableVertexAttribArray( 1 ) ); // height
			OGL_CHECK( renderThread, pglVertexAttribPointer( 1, 1, GL_FLOAT, GL_FALSE, VBO_PHT2_STRIDE,
				( const GLvoid * )( sizeof( deoglVBOHeightTerrain2 ) * pOffsetVBODataPoints + VBO_PHT2_HEIGHT ) ) );
			
			OGL_CHECK( renderThread, pglEnableVertexAttribArray( 2 ) ); // normal ( encoded )
			OGL_CHECK( renderThread, pglVertexAttribPointer( 2, 1, GL_FLOAT, GL_FALSE, VBO_PHT2_STRIDE,
				( const GLvoid * )( sizeof( deoglVBOHeightTerrain2 ) * pOffsetVBODataPoints + VBO_PHT2_NORMAL ) ) );
			
			OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pVBODataFaces ) );
			
			OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
			OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
			
			pVAO->EnsureRTSVAO();
		}
		
	}else{
		if( pVAO ){
			delete pVAO;
			pVAO = NULL;
		}
	}
}



void deoglHTSCluster::AddToWorldCompute( deoglWorldCompute &worldCompute ){
	worldCompute.AddElement( pWorldComputeElement );
}

void deoglHTSCluster::UpdateWorldCompute(){
	pWorldComputeElement->ComputeUpdateElement();
}

void deoglHTSCluster::UpdateWorldComputeTextures(){
	pWorldComputeElement->ComputeUpdateElementGeometries();
}

void deoglHTSCluster::RemoveFromWorldCompute(){
	pWorldComputeElement->RemoveFromCompute();
}



// Face Points
////////////////

void deoglHTSCluster::AddFacePoints( int p1, int p2, int p3 ){
	if( pFacePointCount + 3 >= pFacePointSize ){
		int newSize = pFacePointSize * 3 / 2 + 3;
		GLushort * const newArray = new GLushort[ newSize ];
		if( pFacePoints ){
			memcpy( newArray, pFacePoints, sizeof( GLushort ) * pFacePointSize );
			delete [] pFacePoints;
		}
		pFacePoints = newArray;
		pFacePointSize = newSize;
	}
	
	pFacePoints[ pFacePointCount++ ] = ( GLushort )p3;
	pFacePoints[ pFacePointCount++ ] = ( GLushort )p2;
	pFacePoints[ pFacePointCount++ ] = ( GLushort )p1;
}

void deoglHTSCluster::SetFacePointCount( int count ){
	if( count < 0 || count > pFacePointCount || count % 3 > 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pFacePointCount = count;
}



// Private Functions
//////////////////////

void deoglHTSCluster::pCreateBaseLOD( const deHeightTerrainSector &sector, int width ){
	const int lastZ = pPointCountZ - 1;
	const int lastX = pPointCountX - 1;
	int base, x, z;
	
	pNoLOD = false;
	
	pLOD[ 0 ].firstBasePoint = pFacePointCount;
	
	for( z=0; z<lastZ; z++ ){
		base = pPointCountX * z;
		
		for( x=0; x<lastX; x++ ){
			if( sector.GetFaceVisibleAt( pFirstPointX + x, pFirstPointZ + z ) ){
				AddFacePoints( base, base + 1, base + 1 + pPointCountX );
				AddFacePoints( base, base + 1 + pPointCountX, base + pPointCountX );
				
			}else{
				pNoLOD = true;
			}
			
			base++;
		}
	}
	
	pLOD[ 0 ].basePointCount = pFacePointCount - pLOD[ 0 ].firstBasePoint;
}

void deoglHTSCluster::pCreateCheapLOD( int lodLevel ){
	int imageDim = pPointCountX;//pHTSector->GetHeightTerrain()->GetHeightTerrain()->GetSectorResolution();
	int lastPointX = pPointCountX - 1;
	int lastPointZ = pPointCountZ - 1;
	int reduction2 = 1 << ( lodLevel - 1 );
	int reduction = 1 << lodLevel;
	int newPointCountX, newPointCountZ;
	int zStep2 = imageDim * reduction2;
	int zStep = imageDim * reduction;
	int x, z, offset, offset2, offset3;
	int p1, p2, p3, p4, p5;
	
	// NOTE: this is a cheap lod calculation by dropping rows and colons until the face count roughly dropped by factor 4
	// the reduction parameter indicates which rows and colons to keep. Rows and colons being a multiple of the reduction
	// are kept and the rest discarded. since the face count can be uneven due to the fact that grids are not required to
	// be a multiple of two the last row and colon is always kept even if they would not match. hence uneven face counts
	// are patched up with smaller rows or colons at the right respectively bottom border. the new point count calculation
	// takes this into mind as well as the mesh creation.
	
	// determine the new point count for this lod level
	newPointCountX = ( ( pPointCountX - 2 ) / reduction ) + 2;
	newPointCountZ = ( ( pPointCountZ - 2 ) / reduction ) + 2;
	
	// create lod mesh base area. this includes all faces not touching the border points
	pLOD[ lodLevel ].firstBasePoint = pFacePointCount;
	
	for( z=1; z<newPointCountZ-2; z++ ){
		offset = imageDim * ( z * reduction );
		
		for( x=1; x<newPointCountX-2; x++ ){
			p1 = offset + x * reduction;
			p2 = p1 + reduction;
			p3 = p1 + zStep;
			p4 = p3 + reduction;
			
			AddFacePoints( p1, p2, p4 );
			AddFacePoints( p1, p4, p3 );
		}
	}
	
	pLOD[ lodLevel ].basePointCount = pFacePointCount - pLOD[ lodLevel ].firstBasePoint;
	
	// create left border for the same lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtLeft ] = pFacePointCount;
	
	for( z=0; z<newPointCountZ-1; z++ ){
		p1 = imageDim * ( z * reduction );
		p2 = p1 + reduction;
		
		if( z < newPointCountZ - 2 ){
			p3 = p1 + zStep;
			p4 = p3 + reduction;
			
			if( z > 0 ){
				AddFacePoints( p1, p2, p4 );
			}
			AddFacePoints( p1, p4, p3 );
			
		}else{
			p3 = imageDim * lastPointZ;
			
			AddFacePoints( p1, p2, p3 );
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtLeft ] = pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtLeft ];
	
	// create top border for the same lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtTop ] = pFacePointCount;
	
	offset = 0;
	for( x=0; x<newPointCountX-1; x++ ){
		p1 = offset + x * reduction;
		p3 = p1 + zStep;
		
		if( x < newPointCountX - 2 ){
			p2 = p1 + reduction;
			p4 = p3 + reduction;
			
			AddFacePoints( p1, p2, p4 );
			if( x > 0 ){
				AddFacePoints( p1, p4, p3 );
			}
			
		}else{
			p2 = lastPointX;
			
			AddFacePoints( p1, p2, p3 );
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtTop ] = pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtTop ];
	
	// create right border for the same lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtRight ] = pFacePointCount;
	
	for( z=0; z<newPointCountZ-1; z++ ){
		offset = imageDim * ( z * reduction );
		p1 = offset + ( newPointCountX - 2 ) * reduction;
		p2 = offset + lastPointX;
		
		if( z < newPointCountZ - 2 ){
			p3 = p1 + zStep;
			p4 = p2 + zStep;
			
			if( z > 0 ){
				AddFacePoints( p1, p2, p4 );
				AddFacePoints( p1, p4, p3 );
				
			}else{
				AddFacePoints( p2, p4, p3 );
			}
			
		}else{
			p4 = imageDim * lastPointZ + lastPointX;
			
			AddFacePoints( p1, p2, p4 );
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtRight ] =
		pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtRight ];
	
	// create bottom border for the same lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtBottom ] = pFacePointCount;
	
	offset = imageDim * ( ( newPointCountZ - 2 ) * reduction );
	offset2 = imageDim * lastPointZ;
	for( x=0; x<newPointCountX-1; x++ ){
		offset3 = x * reduction;
		p1 = offset + offset3;
		p3 = offset2 + offset3;
		
		if( x < newPointCountX - 2 ){
			p2 = p1 + reduction;
			p4 = p3 + reduction;
			
			if( x > 0 ){
				AddFacePoints( p1, p2, p4 );
				AddFacePoints( p1, p4, p3 );
				
			}else{
				AddFacePoints( p2, p4, p3 );
			}
			
		}else{
			p4 = imageDim * lastPointZ + lastPointX;
			
			AddFacePoints( p1, p4, p3 );
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtBottom ] = pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtBottom ];
	
	// create left border for the lower lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixLeft ] = pFacePointCount;
	
	for( z=0; z<newPointCountZ-1; z++ ){
		p1 = imageDim * ( z * reduction );
		p2 = p1 + reduction;
		p5 = p1 + zStep2;
		
		if( z < newPointCountZ - 2 ){
			p3 = p1 + zStep;
			p4 = p3 + reduction;
			
			if( z > 0 ){
				AddFacePoints( p1, p2, p4 );
			}
			AddFacePoints( p1, p4, p5 );
			AddFacePoints( p5, p4, p3 );
			
		}else{
			p3 = imageDim * lastPointZ;
			
			if( z * reduction + reduction2 < pPointCountZ - 1 ){
				AddFacePoints( p1, p2, p5 );
				AddFacePoints( p5, p2, p3 );
				
			}else{
				AddFacePoints( p1, p2, p3 );
			}
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtFixLeft ] = pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixLeft ];
	
	// create top border for the lower lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixTop ] = pFacePointCount;
	
	offset = 0;
	for( x=0; x<newPointCountX-1; x++ ){
		p1 = offset + x * reduction;
		p3 = p1 + zStep;
		p5 = p1 + reduction2;
		
		if( x < newPointCountX - 2 ){
			p2 = p1 + reduction;
			p4 = p3 + reduction;
			
			AddFacePoints( p1, p5, p4 );
			AddFacePoints( p5, p2, p4 );
			if( x > 0 ){
				AddFacePoints( p1, p4, p3 );
			}
			
		}else{
			p2 = lastPointX;
			
			if( x * reduction + reduction2 < pPointCountX - 1 ){
				AddFacePoints( p1, p5, p3 );
				AddFacePoints( p5, p2, p3 );
				
			}else{
				AddFacePoints( p1, p2, p3 );
			}
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtFixTop ] = pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixTop ];
	
	// create right border for the lower lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixRight ] = pFacePointCount;
	
	for( z=0; z<newPointCountZ-1; z++ ){
		offset = imageDim * ( z * reduction );
		p1 = offset + ( newPointCountX - 2 ) * reduction;
		p2 = offset + lastPointX;
		p5 = p2 + zStep2;
		
		if( z < newPointCountZ - 2 ){
			p3 = p1 + zStep;
			p4 = p2 + zStep;
			
			if( z > 0 ){
				AddFacePoints( p1, p2, p5 );
				AddFacePoints( p1, p5, p4 );
				AddFacePoints( p1, p4, p3 );
				
			}else{
				AddFacePoints( p2, p5, p3 );
				AddFacePoints( p5, p4, p3 );
			}
			
		}else{
			p4 = imageDim * lastPointZ + lastPointX;
			
			if( z * reduction + reduction2 < pPointCountZ - 1 ){
				AddFacePoints( p1, p2, p5 );
				AddFacePoints( p1, p5, p4 );
				
			}else{
				AddFacePoints( p1, p2, p4 );
			}
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtFixRight ] = pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixRight ];
	
	// create bottom border for the lower lod level of neighbor
	pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixBottom ] = pFacePointCount;
	
	offset = imageDim * ( ( newPointCountZ - 2 ) * reduction );
	offset2 = imageDim * lastPointZ;
	for( x=0; x<newPointCountX-1; x++ ){
		offset3 = x * reduction;
		p1 = offset + offset3;
		p3 = offset2 + offset3;
		p5 = p3 + reduction2;
		
		if( x < newPointCountX - 2 ){
			p2 = p1 + reduction;
			p4 = p3 + reduction;
			
			if( x > 0 ){
				AddFacePoints( p1, p2, p4 );
				AddFacePoints( p1, p4, p5 );
				AddFacePoints( p1, p5, p3 );
				
			}else{
				AddFacePoints( p2, p4, p5 );
				AddFacePoints( p2, p5, p3 );
			}
			
		}else{
			p4 = imageDim * lastPointZ + lastPointX;
			
			if( x * reduction + reduction2 < pPointCountX - 1 ){
				AddFacePoints( p1, p4, p5 );
				AddFacePoints( p1, p5, p3 );
				
			}else{
				AddFacePoints( p1, p4, p3 );
			}
		}
	}
	
	pLOD[ lodLevel ].borderPointCount[ deoglHTViewSectorCluster::ebtFixBottom ] = pFacePointCount - pLOD[ lodLevel ].firstBorderPoint[ deoglHTViewSectorCluster::ebtFixBottom ];
}
