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
#include <unistd.h>
#include <string.h>

#include "deoglRModel.h"
#include "deoglModelLOD.h"
#include "deoglModelLODTexCoordSet.h"
#include "deoglModelLODVisitor.h"
#include "face/deoglModelFace.h"
#include "octree/deoglModelOctree.h"
#include "texture/deoglModelTexture.h"
#include "../deoglPreloader.h"
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shaders/paramblock/deoglSPBlockMemory.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../utils/vcoptimizer/deoglVCOptimizer.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/writer/deoglVBOWriterModel.h"
#include "../gi/deoglGIBVHLocal.h"
#include "../utils/bvh/deoglBVH.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglCollisionDetection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>



// Class deoglModelLOD
////////////////////////

// #define DO_TIMING 1

// Constructor, destructor
////////////////////////////

deoglModelLOD::deoglModelLOD( deoglRModel &model, int lodIndex, const deModel &engModel ) :
pModel( model ),
pLODIndex( lodIndex ),
pIBO( 0 ),
pIBOType( deoglVBOLayout::eitUnsignedShort ),
pGIBVHLocal( NULL )
{
	pVBOBlock = NULL;
	pVBOBlockPositionWeight = NULL;
	pVBOBlockCalcNormalTangent = NULL;
	pVBOBlockWriteSkinnedVBO = NULL;
	pVBOBlockWithWeight = NULL;
	
	pPositions = NULL;
	pPositionCount = 0;
	
	pTexCoords = NULL;
	pTexCoordCount = 0;
	
	pTextures = NULL;
	pTextureCount = engModel.GetTextureCount();
	
	pNormals = NULL;
	pNormalCount = 0;
	
	pTangents = NULL;
	pNegateTangents = NULL;
	pTangentCount = 0;
	
	pDoubleSided = false;
	pDecal = false;
	
	pWeightsEntries = NULL;
	pWeightsEntryCount = 0;
	pWeightsCounts = NULL;
	pWeightsCount = 0;
	
	pVertices = NULL;
	pVertexCount = 0;
	
	pFaces = NULL;
	pFaceCount = 0;
	
	pTexCoordSets = NULL;
	pTexCoordSetCount = 0;
	
	pOctree = NULL;
	
	pMaxError = 0.0f;
	pAvgError = 0.0f;
	
	try{
		#ifdef DO_TIMING
		deoglRTLogger &logger = model.GetRenderThread().GetLogger();
		logger.LogInfoFormat( "loading %s lod %d\n", pModel.GetFilename().GetString(), pLODIndex );
		decTimer timer;
		#endif
		
		pBuildArrays( engModel );
		#ifdef DO_TIMING
		logger.LogInfoFormat( "- build array: %f\n", timer.GetElapsedTime() );
		#endif
		
		pCalcTangents();
		#ifdef DO_TIMING
		logger.LogInfoFormat( "- build tangents: %f\n", timer.GetElapsedTime() );
		#endif
		
		pCalcErrorMetrics( engModel );
		#ifdef DO_TIMING
		logger.LogInfoFormat( "- build calc-error-metrics: %f\n", timer.GetElapsedTime() );
		#endif
		
		pOptimizeVertexCache();
		#ifdef DO_TIMING
		logger.LogInfoFormat( "- build optimize-vertex-cache: %f\n", timer.GetElapsedTime() );
		logger.LogInfoFormat( "=> max-error: %f\n", pMaxError );
		#endif
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglModelLOD::deoglModelLOD( deoglRModel &model, int lodIndex, decBaseFileReader &cacheReader ) :
pModel( model ),
pLODIndex( lodIndex ),
pIBO( 0 ),
pIBOType( deoglVBOLayout::eitUnsignedShort ),
pGIBVHLocal( NULL )
{
	pVBOBlock = NULL;
	pVBOBlockPositionWeight = NULL;
	pVBOBlockCalcNormalTangent = NULL;
	pVBOBlockWriteSkinnedVBO = NULL;
	pVBOBlockWithWeight = NULL;
	
	pPositions = NULL;
	pPositionCount = 0;
	
	pTexCoords = NULL;
	pTexCoordCount = 0;
	
	pTextures = NULL;
	pTextureCount = 0;
	
	pNormals = NULL;
	pNormalCount = 0;
	
	pTangents = NULL;
	pNegateTangents = NULL;
	pTangentCount = 0;
	
	pDoubleSided = false;
	pDecal = false;
	
	pWeightsEntries = NULL;
	pWeightsEntryCount = 0;
	pWeightsCounts = NULL;
	pWeightsCount = 0;
	
	pVertices = NULL;
	pVertexCount = 0;
	
	pFaces = NULL;
	pFaceCount = 0;
	
	pTexCoordSets = NULL;
	pTexCoordSetCount = 0;
	
	pOctree = NULL;
	
	pMaxError = 0.0f;
	pAvgError = 0.0f;
	
	try{
		LoadFromCache( cacheReader );
		
	}catch( ... ){
		pCleanUp();
		throw;
	}
}

deoglModelLOD::~deoglModelLOD(){
	pCleanUp();
}



// Management
///////////////

deoglSharedVBOBlock *deoglModelLOD::GetVBOBlock(){
	if( pVBOBlock ){
		return pVBOBlock;
	}
	
	deoglRTBufferObject::eSharedVBOLists listType = deoglRTBufferObject::esvbolStaticModel;
	
	if( pTexCoordSetCount == 1 ){
		listType = deoglRTBufferObject::esvbolStaticModelTCS1;
		
	}else if( pTexCoordSetCount >= 2 ){ // more than 2 not supported yet
		listType = deoglRTBufferObject::esvbolStaticModelTCS2;
	}
	
	deoglRenderThread &renderThread = pModel.GetRenderThread();
	deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType( listType );
	
	if( pVertexCount > svbolist.GetMaxPointCount() ){
		renderThread.GetLogger().LogInfoFormat(
			"Model(%s,%i): Too many points (%i) to fit into shared VBOs."
			" Using over-sized VBO (performance not optimal).",
			pModel.GetFilename().GetString(), pLODIndex, pVertexCount );
	}
	
	pVBOBlock = svbolist.AddData( pVertexCount, pFaceCount * 3 );
	
	pWriteVBOData();
	
	return pVBOBlock;
}

deoglSharedVBOBlock *deoglModelLOD::GetVBOBlockPositionWeight(){
	if( ! pVBOBlockPositionWeight ){
		deoglRenderThread &renderThread = pModel.GetRenderThread();
		deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
			deoglRTBufferObject::esvbolModelPositionWeightIndices );
		
		if( pPositionCount > svbolist.GetMaxPointCount() ){
			renderThread.GetLogger().LogInfoFormat(
				"Model(%s,%i): Too many points (%i) to fit into shared position weight VBO."
				" Using over-sized VBO (performance not optimal).",
				pModel.GetFilename().GetString(), pLODIndex, pPositionCount );
		}
		
		pVBOBlockPositionWeight = svbolist.AddData( pPositionCount );
		
		pWriteVBODataPositionWeight();
	}
	
	return pVBOBlockPositionWeight;
}

deoglSharedVBOBlock *deoglModelLOD::GetVBOBlockCalcNormalTangent(){
	if( ! pVBOBlockCalcNormalTangent ){
		deoglRenderThread &renderThread = pModel.GetRenderThread();
		deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
			deoglRTBufferObject::esvbolModelCalcNormalTangent );
		
		if( pFaceCount > svbolist.GetMaxPointCount() ){
			renderThread.GetLogger().LogInfoFormat(
				"Model(%s,%i): Too many points (%i) to fit into shared calc normal tangent VBO."
				" Using over-sized VBO (performance not optimal).",
				pModel.GetFilename().GetString(), pLODIndex, pFaceCount );
		}
		
		pVBOBlockCalcNormalTangent = svbolist.AddData( pFaceCount );
		
		pWriteVBODataCalcNormalTangent();
	}
	
	return pVBOBlockCalcNormalTangent;
}

deoglSharedVBOBlock *deoglModelLOD::GetVBOBlockWriteSkinnedVBO(){
	if( ! pVBOBlockWriteSkinnedVBO ){
		deoglRenderThread &renderThread = pModel.GetRenderThread();
		deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
			deoglRTBufferObject::esvbolModelWriteSkinnedVBO );
		
		if( pVertexCount > svbolist.GetMaxPointCount() ){
			renderThread.GetLogger().LogInfoFormat(
				"Model(%s,%i): Too many points (%i) to fit into shared write skinned vbo VBO."
				" Using over-sized VBO (performance not optimal).",
				pModel.GetFilename().GetString(), pLODIndex, pVertexCount );
		}
		
		pVBOBlockWriteSkinnedVBO = svbolist.AddData( pVertexCount );
		
		pWriteVBODataWriteSkinnedVBO();
	}
	
	return pVBOBlockWriteSkinnedVBO;
}

deoglSharedVBOBlock *deoglModelLOD::GetVBOBlockWithWeight(){
	if( ! pVBOBlockWithWeight ){
		deoglRenderThread &renderThread = pModel.GetRenderThread();
		deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
			deoglRTBufferObject::esvbolStaticModelWeight );
		
		if( pVertexCount > svbolist.GetMaxPointCount() ){
			renderThread.GetLogger().LogInfoFormat(
				"Model(%s,%i): Too many points (%i) to fit into shared model with weight VBO."
				" Using over-sized VBO (performance not optimal).",
				pModel.GetFilename().GetString(), pLODIndex, pVertexCount );
		}
		
		pVBOBlockWithWeight = svbolist.AddData( pVertexCount );
		
		pWriteVBODataWithWeight();
	}
	
	return pVBOBlockWithWeight;
}

GLuint deoglModelLOD::GetIBO(){
	if( pIBO ){
		return pIBO;
	}
	
	pIBOType = deoglVBOLayout::eitUnsignedInt;
	
	OGL_IF_CHECK( deoglRenderThread &renderThread = pModel.GetRenderThread(); )
	GLuint *data = NULL;
	
	try{
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pIBO ) );
		if( ! pIBO ){
			DETHROW( deeOutOfMemory );
		}
		
		const int bufferSize = ( pFaceCount * 3 ) * sizeof( GLuint );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIBO ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ELEMENT_ARRAY_BUFFER,
			bufferSize, NULL, GL_STATIC_DRAW ) );
		
		OGL_CHECK( renderThread, data = ( GLuint* )pglMapBufferRange( GL_ELEMENT_ARRAY_BUFFER,
			0, bufferSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT) );
		if( ! data ){
			DETHROW( deeInvalidParam );
		}
		
		int i;
		for( i=0; i<pFaceCount; i++ ){
			*(data++) = ( GLuint )pFaces[ i ].GetVertex3();
			*(data++) = ( GLuint )pFaces[ i ].GetVertex2();
			*(data++) = ( GLuint )pFaces[ i ].GetVertex1();
		}
		
	}catch( const deException & ){
		if( data ){
			OGL_CHECK( renderThread, pglUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER ) );
		}
		if( pIBO ){
			OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
			pglDeleteBuffers( 1, &pIBO );
			pIBO = 0;
		}
		throw;
	}
	
	OGL_CHECK( renderThread, pglUnmapBuffer( GL_ELEMENT_ARRAY_BUFFER ) );
	OGL_CHECK( renderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) );
	
	return pIBO;
}



const deoglModelTexture &deoglModelLOD::GetTextureAt( int index ) const{
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	return pTextures[ index ];
}

deoglSharedSPBRTIGroupList &deoglModelLOD::GetSharedSPBRTIGroupListAt( int texture ) const{
	if( texture < 0 || texture >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	return pTextures[ texture ].GetRTIGroups();
}



const deoglModelLODTexCoordSet &deoglModelLOD::GetTextureCoordSetAt( int index ) const{
	if( index < 0 || index >= pTexCoordSetCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pTexCoordSets[ index ];
}



void deoglModelLOD::PrepareOctree(){
	if( pOctree ){
		return;
	}
	
	const deoglRModel::sExtends &extends = pModel.GetExtends();
	const float threshold = 0.001f;
	const int maxDepth = 8;
	int i;
	
	pOctree = new deoglModelOctree( ( extends.minimum + extends.maximum ) * 0.5f,
		( extends.maximum - extends.minimum ) * 0.5f );
	
	for( i=0; i<pFaceCount; i++ ){
		const decVector &mv1 = pPositions[ pVertices[ pFaces[ i ].GetVertex1() ].position ].position;
		const decVector &mv2 = pPositions[ pVertices[ pFaces[ i ].GetVertex2() ].position ].position;
		const decVector &mv3 = pPositions[ pVertices[ pFaces[ i ].GetVertex3() ].position ].position;
		
		decVector faceMinExtend = mv1.Smallest( mv2 ).Smallest( mv3 );
		decVector faceMaxExtend = mv1.Largest( mv2 ).Largest( mv3 );
		
		// it is possible that the extends get very small. make sure the extends
		// are at least of a small size to avoid missing faces
		const decVector diff( faceMaxExtend - faceMinExtend );
		
		if( diff.x < threshold ){
			const float adjust = ( threshold - diff.x ) * 0.5f;
			faceMinExtend.x -= adjust;
			faceMaxExtend.x += adjust;
		}
		
		if( diff.y < threshold ){
			const float adjust = ( threshold - diff.y ) * 0.5f;
			faceMinExtend.y -= adjust;
			faceMaxExtend.y += adjust;
		}
		
		if( diff.z < threshold ){
			const float adjust = ( threshold - diff.z ) * 0.5f;
			faceMinExtend.z -= adjust;
			faceMaxExtend.z += adjust;
		}
		
		// store extends
		pFaces[ i ].SetExtends( faceMinExtend, faceMaxExtend );
		pFaces[ i ].SetCenter( ( faceMinExtend + faceMaxExtend ) * 0.5f );
		
		// insert into tree
		pOctree->InsertFaceIntoTree( pFaces + i, maxDepth );
	}
}



void deoglModelLOD::pCalcErrorMetrics( const deModel &engModel ){
	// TODO this is horribly slow for large models with many vertices due to octree visiting.
	//      needs a better solution
	
	pMaxError = pAvgError = 0.0f;
	if( pLODIndex == 0 || pFaceCount == 0 ){
		return;
	}
	
	const deModelLOD &engLod = *engModel.GetLODAt( pLODIndex );
	if( engLod.GetHasLodError() ){
		pAvgError = pMaxError = engLod.GetLodError();
		return;
	}
	
	const deoglModelLOD &lod0Mesh = pModel.GetLODAt( 0 );
	const int lod0PositionCount = lod0Mesh.GetPositionCount();
	if( lod0PositionCount >= 5000 ){
		// TODO temporary hack. if the number of vertices is high calculation time sky-rockets.
		// for a large model with over 67k vertices (130k faces) this consumes over 30s.
		// the good thing on this calculation though is that it is not required to be run.
		// if skipped the result is sub-optimal but not wrong. if not calculated we need
		// to do this later on using gradual optimization. once the result is present the
		// cache file can be re-written with the optimized value.
		pAvgError = pMaxError = 0.025f * pLODIndex;
		return;
	}
	
// 		deoglRTLogger &logger = pModel.GetRenderThread().GetLogger();
// 		decTimer timer;
	PrepareOctree();
// 		logger.LogInfoFormat( "  - prepare octree: %f\n", timer.GetElapsedTime() );
	
	const oglModelPosition * const lod0Positions = lod0Mesh.GetPositions();
	//const decVector boxExtends = decVector( 0.1f, 0.1f, 0.1f );
	const decVector boxExtends = decVector( 0.2f, 0.2f, 0.2f );
	const float initLODDistance = boxExtends.Length(); //1.0f;
	deoglModelLODVisitor visitor( *this );
	int i;
	
	for( i=0; i<lod0PositionCount; i++ ){
		const decVector &lodPoint = lod0Positions[ i ].position;
		
		visitor.SetBoxExtends( lodPoint - boxExtends, lodPoint + boxExtends );
		visitor.SetLODPoint( lodPoint );
		visitor.SetMinLODDistance( initLODDistance );
		
		pOctree->VisitNodesColliding( &visitor, visitor.GetBoxMinExtend(), visitor.GetBoxMaxExtend() );
		
		if( visitor.GetMinLODDistance() > pMaxError ){
			pMaxError = visitor.GetMinLODDistance();
		}
		pAvgError += visitor.GetMinLODDistance();
	}
	
	if( lod0PositionCount > 0 ){
		pAvgError /= ( float )lod0PositionCount;
	}
	
	// drop octree. saves CPU memory. will be created and kept if somebody really needs it
	delete pOctree;
	pOctree = NULL;
}



struct sCacheTexture{
	int firstFace;
	int faceCount;
	bool doubleSided;
	bool decal;
	int decalOffset;
};

void deoglModelLOD::LoadFromCache( decBaseFileReader &reader ){
	int i, count;
	
	count = reader.ReadInt();
	if( count > 0 ){
		pTextures = new deoglModelTexture[ count ];
		pTextureCount = count;
		
		sCacheTexture cacheTextures;
		for( i=0; i<count; i++ ){
			reader.Read( &cacheTextures, sizeof( sCacheTexture ) );
			pTextures[ i ].SetFirstFace( cacheTextures.firstFace );
			pTextures[ i ].SetFaceCount( cacheTextures.faceCount );
			pTextures[ i ].SetDoubleSided( cacheTextures.doubleSided );
			pTextures[ i ].SetDecal( cacheTextures.decal );
			pTextures[ i ].SetDecalOffset( cacheTextures.decalOffset );
		}
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pPositions = new oglModelPosition[ count ];
		pPositionCount = count;
		reader.Read( pPositions, sizeof( oglModelPosition ) * count );
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pTexCoords = new decVector2[ count ];
		pTexCoordCount = count;
		reader.Read( pTexCoords, sizeof( decVector2 ) * count );
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pNormals = new decVector[ count ];
		pNormalCount = count;
		reader.Read( pNormals, sizeof( decVector ) * count );
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pTangents = new decVector[ count ];
		pNegateTangents = new bool[ count ];
		pTangentCount = count;
		reader.Read( pTangents, sizeof( decVector ) * count);
		reader.Read( pNegateTangents, sizeof( bool ) * count );
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pWeightsEntries = new oglModelWeight[ count ];
		pWeightsEntryCount = count;
		reader.Read( pWeightsEntries, sizeof( oglModelWeight ) * count );
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pWeightsCounts = new int[ count ];
		pWeightsCount = count;
		reader.Read( pWeightsCounts, sizeof( int ) * count );
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pVertices = new oglModelVertex[ count ];
		pVertexCount = count;
		reader.Read( pVertices, sizeof( oglModelVertex ) * count );
	}
	
	count = reader.ReadInt();
	if( count > 0 ){
		pFaces = new deoglModelFace[ count ];
		pFaceCount = count;
		reader.Read( pFaces, sizeof( deoglModelFace ) * count );
	}
	
	count = reader.ReadInt();
	if( count ){
		pTexCoordSets = new deoglModelLODTexCoordSet[ count ];
		pTexCoordSetCount = count;
		
		for( i=0; i<count; i++ ){
			deoglModelLODTexCoordSet &tcs = pTexCoordSets[ i ];
			
			const int textureCoordinateCount = reader.ReadInt();
			tcs.SetTextureCoordinateCount( textureCoordinateCount, false );
			if( textureCoordinateCount > 0 ){
				reader.Read( tcs.GetTextureCoordinates(), sizeof( decVector2 ) * textureCoordinateCount );
			}
			
			const int tangentCount = reader.ReadInt();
			tcs.SetTangentCount( tangentCount );
			if( tangentCount > 0 ){
				reader.Read( tcs.GetTangents(), sizeof( decVector ) * tangentCount );
				reader.Read( tcs.GetNegateTangents(), sizeof( bool ) * tangentCount );
			}
		}
	}
	
	pDoubleSided = ( reader.ReadByte() == 1 );
	pDecal = ( reader.ReadByte() == 1 );
	pMaxError = reader.ReadFloat();
	pAvgError = reader.ReadFloat();
}

void deoglModelLOD::SaveToCache( decBaseFileWriter &writer ){
	int i;
	
	writer.WriteInt( pTextureCount );
	
	sCacheTexture cacheTextures;
	for( i=0; i<pTextureCount; i++ ){
		cacheTextures.firstFace = pTextures[ i ].GetFirstFace();
		cacheTextures.faceCount = pTextures[ i ].GetFaceCount();
		cacheTextures.doubleSided = pTextures[ i ].GetDoubleSided();
		cacheTextures.decal = pTextures[ i ].GetDecal();
		cacheTextures.decalOffset = pTextures[ i ].GetDecalOffset();
		writer.Write( &cacheTextures, sizeof( sCacheTexture ) );
	}
	
	writer.WriteInt( pPositionCount );
	if( pPositionCount > 0 ){
		writer.Write( pPositions, sizeof( oglModelPosition ) * pPositionCount );
	}
	
	writer.WriteInt( pTexCoordCount );
	if( pTexCoordCount > 0 ){
		writer.Write( pTexCoords, sizeof( decVector2 ) * pTexCoordCount );
	}
	
	writer.WriteInt( pNormalCount );
	if( pNormalCount > 0 ){
		writer.Write( pNormals, sizeof( decVector ) * pNormalCount );
	}
	
	writer.WriteInt( pTangentCount );
	if( pTangentCount > 0 ){
		writer.Write( pTangents, sizeof( decVector ) * pTangentCount );
		writer.Write( pNegateTangents, sizeof( bool ) * pTangentCount );
	}
	
	writer.WriteInt( pWeightsEntryCount );
	if( pWeightsEntryCount > 0 ){
		writer.Write( pWeightsEntries, sizeof( oglModelWeight ) * pWeightsEntryCount );
	}
	
	writer.WriteInt( pWeightsCount );
	if( pWeightsCount > 0 ){
		writer.Write( pWeightsCounts, sizeof( int ) * pWeightsCount );
	}
	
	writer.WriteInt( pVertexCount );
	if( pVertexCount > 0 ){
		writer.Write( pVertices, sizeof( oglModelVertex ) * pVertexCount );
	}
	
	writer.WriteInt( pFaceCount );
	if( pFaceCount > 0 ){
		writer.Write( pFaces, sizeof( deoglModelFace ) * pFaceCount );
	}
	
	writer.WriteInt( pTexCoordSetCount );
	for( i=0; i<pTexCoordSetCount; i++ ){
		const deoglModelLODTexCoordSet &tcs = pTexCoordSets[ i ];
		
		const int textureCoordinateCount = tcs.GetTextureCoordinateCount();
		writer.WriteInt( textureCoordinateCount );
		if( textureCoordinateCount > 0 ){
			writer.Write( tcs.GetTextureCoordinates(), sizeof( decVector2 ) * textureCoordinateCount );
		}
		
		const int tangentCount = tcs.GetTangentCount();
		writer.WriteInt( tangentCount );
		if( tangentCount > 0 ){
			writer.Write( tcs.GetTangents(), sizeof( decVector ) * tangentCount );
			writer.Write( tcs.GetNegateTangents(), sizeof( bool ) * tangentCount );
		}
	}
	
	writer.WriteByte( pDoubleSided ? 1 : 0 );
	writer.WriteByte( pDecal ? 1 : 0 );
	writer.WriteFloat( pMaxError );
	writer.WriteFloat( pAvgError );
}



void deoglModelLOD::PrepareGILocalBVH(){
	if( pGIBVHLocal ){
		return;
	}
	
	deoglBVH::sBuildPrimitive *primitives = NULL;
	
	if( pFaceCount > 0 ){
		primitives = new deoglBVH::sBuildPrimitive[ pFaceCount ];
		int i;
		
		for( i=0; i<pFaceCount; i++ ){
			deoglBVH::sBuildPrimitive &primitive = primitives[ i ];
			
			/*
			// this works only if octree has been prepare yet. but the octree does
			// apply some enlaring we do not want to use here
			primitive.minExtend = pFaces[ i ].GetMinExtend();
			primitive.maxExtend = pFaces[ i ].GetMaxExtend();
			primitive.center = pFaces[ i ].GetCenter();
			*/
			
			const deoglModelFace &face = pFaces[ i ];
			const oglModelPosition &v1 = pPositions[ pVertices[ face.GetVertex1() ].position ];
			const oglModelPosition &v2 = pPositions[ pVertices[ face.GetVertex2() ].position ];
			const oglModelPosition &v3 = pPositions[ pVertices[ face.GetVertex3() ].position ];
			
			primitive.minExtend = v1.position.Smallest( v2.position ).Smallest( v3.position );
			primitive.maxExtend = v1.position.Largest( v2.position ).Largest( v3.position );
			primitive.center = ( primitive.minExtend + primitive.maxExtend ) * 0.5f;
		}
	}
	
	try{
		// models have variable face count. the highest lod level typically used for ray tracing
		// should have low face count. if models are not optimized this can result in higher
		// face counts which is bad for performance. in tests a max depth of 12 (more can not
		// be used in shader ray tracing) seems to be the best solution. if low face count can
		// be guaranteed (for example auto-decimation) then lower max depth can be better.
		// occlusion meshes use 6 here
		pGIBVHLocal = new deoglGIBVHLocal( pModel.GetRenderThread() );
		pGIBVHLocal->BuildBVH( primitives, pFaceCount, 12 );
		
		if( pGIBVHLocal->GetBVH().GetRootNode() ){
			pGIBVHLocal->TBOAddVertices( pPositions, pPositionCount );
			pGIBVHLocal->TBOAddFaces( pFaces, pVertices, pTexCoords );
			pGIBVHLocal->TBOAddBVH();
		}
		
	}catch( const deException & ){
		if( pGIBVHLocal ){
			delete pGIBVHLocal;
			pGIBVHLocal = NULL;
		}
		if( primitives ){
			delete [] primitives;
		}
		throw;
	}
	
	if( primitives ){
		delete [] primitives;
	}
	
	// check for suboptimal configurations and warn the developer
	if( pFaceCount > 300 ){
		pModel.GetRenderThread().GetLogger().LogInfoFormat(
			"Model(%s,%i): High face count slows down global illumination (%d)."
			" Consider adding highest LOD variation with less than 300 faces.",
			pModel.GetFilename().GetString(), pLODIndex, pFaceCount );
	}
}



// Private functions
//////////////////////

void deoglModelLOD::pCleanUp(){
	if( pGIBVHLocal ){
		delete pGIBVHLocal;
	}
	
	if( pVBOBlockWithWeight ){
		pVBOBlockWithWeight->GetVBO()->RemoveBlock( pVBOBlockWithWeight );
		pVBOBlockWithWeight->FreeReference();
	}
	if( pVBOBlockWriteSkinnedVBO ){
		pVBOBlockWriteSkinnedVBO->GetVBO()->RemoveBlock( pVBOBlockWriteSkinnedVBO );
		pVBOBlockWriteSkinnedVBO->FreeReference();
	}
	if( pVBOBlockCalcNormalTangent ){
		pVBOBlockCalcNormalTangent->GetVBO()->RemoveBlock( pVBOBlockCalcNormalTangent );
		pVBOBlockCalcNormalTangent->FreeReference();
	}
	if( pVBOBlockPositionWeight ){
		pVBOBlockPositionWeight->GetVBO()->RemoveBlock( pVBOBlockPositionWeight );
		pVBOBlockPositionWeight->FreeReference();
	}
	if( pVBOBlock ){
		pVBOBlock->GetVBO()->RemoveBlock( pVBOBlock );
		pVBOBlock->FreeReference();
	}
	
	if( pIBO ){
		pglDeleteBuffers( 1, &pIBO );
	}
	
	if( pOctree ){
		delete pOctree;
	}
	
	if( pTexCoordSets ){
		delete [] pTexCoordSets;
	}
	if( pFaces ){
		delete [] pFaces;
	}
	if( pVertices ){
		delete [] pVertices;
	}
	if( pWeightsCounts ){
		delete [] pWeightsCounts;
	}
	if( pWeightsEntries ){
		delete [] pWeightsEntries;
	}
	
	if( pNegateTangents ){
		delete [] pNegateTangents;
	}
	if( pTangents ){
		delete [] pTangents;
	}
	if( pNormals ){
		delete [] pNormals;
	}
	if( pTexCoords ){
		delete [] pTexCoords;
	}
	if( pPositions ){
		delete [] pPositions;
	}
	if( pTextures ){
		delete [] pTextures;
	}
}

void deoglModelLOD::pCalcTangents(){
	decVector faceNormal, tangent, bitangent;
	bool negateTangent;
	decVector2 d1, d2;
	float len;
	int i, j;
	
	// reset tangents
	for( i=0; i<pPositionCount; i++ ){
		pPositions[ i ].normal.SetZero();
	}
	for( i=0; i<pNormalCount; i++ ){
		pNormals[ i ].SetZero();
	}
	for( i=0; i<pTangentCount; i++ ){
		pTangents[ i ].SetZero();
		pNegateTangents[ i ] = false;
	}
	for( i=0; i<pTexCoordSetCount; i++ ){
		bool * const tcsNegateTangents = pTexCoordSets[ i ].GetNegateTangents();
		decVector * const tcsTangents = pTexCoordSets[ i ].GetTangents();
		const int tcsTangentCount = pTexCoordSets[ i ].GetTangentCount();
		
		for( j=0; j<tcsTangentCount; j++ ){
			tcsTangents[ j ].SetZero();
			tcsNegateTangents[ j ] = false;
		}
	}
	
	// calculate normals and tangents
	for( i=0; i<pFaceCount; i++ ){
		// get positions
		const oglModelVertex &v1 = pVertices[ pFaces[ i ].GetVertex1() ];
		const oglModelVertex &v2 = pVertices[ pFaces[ i ].GetVertex2() ];
		const oglModelVertex &v3 = pVertices[ pFaces[ i ].GetVertex3() ];
		const decVector &p1 = pPositions[ v1.position ].position;
		const decVector &p2 = pPositions[ v2.position ].position;
		const decVector &p3 = pPositions[ v3.position ].position;
		const decVector2 &tc1 = pTexCoords[ v1.texcoord ];
		const decVector2 &tc2 = pTexCoords[ v2.texcoord ];
		const decVector2 &tc3 = pTexCoords[ v3.texcoord ];
		
		// calculate face normal
		faceNormal = ( p2 - p1 ) % ( p3 - p1 );
		len = faceNormal.Length();
		if( len != 0.0f ){
			faceNormal /= len;
			
		}else{
			faceNormal.Set( 0.0f, 1.0f, 0.0f );
		}
		
		// calculate face tangent
		d1 = tc2 - tc1;
		d2 = tc3 - tc1;
		
		tangent = ( p2 - p1 ) * d2.y - ( p3 - p1 ) * d1.y;
		len = tangent.Length();
		if( len != 0.0f ){
			tangent /= len;
			
		}else{
			tangent.Set( 1.0f, 0.0f, 0.0f );
		}
		
		bitangent = ( p3 - p1 ) * d1.x - ( p2 - p1 ) * d2.x;
		negateTangent = ( ( faceNormal % tangent ) * bitangent < 0.0f );
		
		// add face normal and tanget to vertices
		pPositions[ v1.position ].normal += faceNormal;
		pPositions[ v2.position ].normal += faceNormal;
		pPositions[ v3.position ].normal += faceNormal;
		pNormals[ v1.normal ] += faceNormal;
		pNormals[ v2.normal ] += faceNormal;
		pNormals[ v3.normal ] += faceNormal;
		pTangents[ v1.tangent ] += tangent;
		pTangents[ v2.tangent ] += tangent;
		pTangents[ v3.tangent ] += tangent;
		pNegateTangents[ v1.tangent ] = negateTangent;
		pNegateTangents[ v2.tangent ] = negateTangent;
		pNegateTangents[ v3.tangent ] = negateTangent;
		
		pFaces[ i ].SetFaceNormal( faceNormal );
		
		// process texture coordinate sets if existing
		for( j=0; j<pTexCoordSetCount; j++ ){
			const decVector2 * const tcsTexCoords = pTexCoordSets[ j ].GetTextureCoordinates();
			bool * const tcsNegateTangents = pTexCoordSets[ j ].GetNegateTangents();
			decVector * const tcsTangents = pTexCoordSets[ j ].GetTangents();
			
			const decVector2 &tcstc1 = tcsTexCoords[ v1.texcoord ];
			const decVector2 &tcstc2 = tcsTexCoords[ v2.texcoord ];
			const decVector2 &tcstc3 = tcsTexCoords[ v3.texcoord ];
			
			// calculate face tangent
			d1 = tcstc2 - tcstc1;
			d2 = tcstc3 - tcstc1;
			
			tangent = ( p2 - p1 ) * d2.y - ( p3 - p1 ) * d1.y;
			len = tangent.Length();
			if( len != 0.0f ){
				tangent /= len;
				
			}else{
				tangent.Set( 1.0f, 0.0f, 0.0f );
			}
			
			bitangent = ( p3 - p1 ) * d1.x - ( p2 - p1 ) * d2.x;
			negateTangent = ( ( faceNormal % tangent ) * bitangent < 0.0f );
			
			// add face normal and tanget to vertices
			tcsTangents[ v1.tangent ] += tangent;
			tcsTangents[ v2.tangent ] += tangent;
			tcsTangents[ v3.tangent ] += tangent;
			tcsNegateTangents[ v1.tangent ] = negateTangent;
			tcsNegateTangents[ v2.tangent ] = negateTangent;
			tcsNegateTangents[ v3.tangent ] = negateTangent;
		}
	}
	
	// normalize normals and tangents
	for( i=0; i<pPositionCount; i++ ){
		len = pPositions[ i ].normal.Length();
		
		if( len != 0.0f ){
			pPositions[ i ].normal /= len;
			
		}else{
			pPositions[ i ].normal.Set( 0.0f, 1.0f, 0.0f );
		}
	}
	
	for( i=0; i<pNormalCount; i++ ){
		len = pNormals[ i ].Length();
		
		if( len != 0.0f ){
			pNormals[ i ] /= len;
			
		}else{
			pNormals[ i ].Set( 0.0f, 1.0f, 0.0f );
		}
	}
	
	for( i=0; i<pTangentCount; i++ ){
		len = pTangents[ i ].Length();
		
		if( len != 0.0f ){
			pTangents[ i ] /= len;
			
		}else{
			pTangents[ i ].Set( 1.0f, 0.0f, 0.0f );
		}
	}
	
	for( i=0; i<pTexCoordSetCount; i++ ){
		decVector * const tcsTangents = pTexCoordSets[ i ].GetTangents();
		const int tcsTangentCount = pTexCoordSets[ i ].GetTangentCount();
		
		for( j=0; j<tcsTangentCount; j++ ){
			len = tcsTangents[ j ].Length();
			
			if( len != 0.0f ){
				tcsTangents[ j ] /= len;
				
			}else{
				tcsTangents[ j ].Set( 1.0f, 0.0f, 0.0f );
			}
		}
	}
}

struct sGroupVertex{
	int vertex;
	sGroupVertex *next;
};

void deoglModelLOD::pBuildArrays( const deModel &engModel ){
	const deModelLOD &engLod = *engModel.GetLODAt( pLODIndex );
	const int modelTexCoordSetCount = engLod.GetTextureCoordinatesSetCount();
	const int modelTexCoordCount = engLod.GetTextureCoordinatesCount();
	const int modelWeightGroupCount = engLod.GetWeightGroupCount();
	const int modelTangentCount = engLod.GetTangentCount();
	const int modelWeightCount = engLod.GetWeightCount();
	const int modelVertexCount = engLod.GetVertexCount();
	const int modelNormalCount = engLod.GetNormalCount();
	const int modelFaceCount = engLod.GetFaceCount();
	const deModelTextureCoordinatesSet * const modelTexCoordSets = engLod.GetTextureCoordinatesSets();
	const deModelWeight * const modelWeights = engLod.GetWeights();
	const deModelVertex * const modelVertices = engLod.GetVertices();
	const deModelFace * const modelFaces = engLod.GetFaces();
	const int * const modelWeightGroups = engLod.GetWeightGroups();
	int i, j;
	
	// calculate the number of weight sets
	pWeightsCount = 0;
	for( i=0; i<modelWeightGroupCount; i++ ){
		pWeightsCount += modelWeightGroups[ i ];
	}
	
	/*
	if( pWeightsCount > 0 ){
		ogl.LogInfoFormat( "Model(%s,%i): %i weight groups, %i weight sets, %i total weights.",
			engModel.GetFilename(), pLODIndex, modelWeightGroupCount, pWeightsCount, modelWeightCount );
	}
	*/
	
	// create weights counts array
	if( pWeightsCount > 0 ){
		pWeightsCounts = new int[ pWeightsCount ];
		int *walker = pWeightsCounts;
		
		for( i=0; i<modelWeightGroupCount; i++ ){
			const int count = modelWeightGroups[ i ];
			const int index = i + 1;
			
			for( j=0; j<count; j++ ){
				*(walker++) = index;
			}
		}
	}
	
	// create weights entries array and fill in the weights counts
	pWeightsEntryCount = modelWeightCount;
	
	if( modelWeightCount > 0 ){
		pWeightsEntries = new oglModelWeight[ modelWeightCount ];
		
		for( i=0; i<modelWeightCount; i++ ){
			const deModelWeight &in = modelWeights[ i ];
			oglModelWeight &out = pWeightsEntries[ i ];
			
			out.bone = in.GetBone();
			out.weight = in.GetWeight();
		}
	}
	
	// create positions array and fill it with vertex positions matching vertex indices in the faces
	if( modelVertexCount > 0 ){
		pPositions = new oglModelPosition[ modelVertexCount ];
		
		for( pPositionCount=0; pPositionCount<modelVertexCount; pPositionCount++ ){
			const deModelVertex &in = modelVertices[ pPositionCount ];
			oglModelPosition &out = pPositions[ pPositionCount ];
			
			out.position = in.GetPosition();
			out.weight = in.GetWeightSet();
		}
	}
	
	// create vertices array with the maximal number of possible vertices and set counter to 0.
	// then model vertices are grouped into "render vertices". this is required since model
	// vertices are grouped by position only whereas render vertices are grouped by position,
	// normals, tangent and texture coordinates. this grouping will produce a smaller number of
	// render vertices required. at the end the array will be trimmed down if the memory
	// difference is memory consumption has to be reduced as much as possible
	if( modelFaceCount > 0 ){
		pVertices = new oglModelVertex[ modelFaceCount * 3 ];
		pVertexCount = 0;
	}
	
	// add textures
	pTextures = new deoglModelTexture[ pTextureCount ];
	
	for( i=0; i<pTextureCount; i++ ){
		const deModelTexture &in = *engModel.GetTextureAt( i );
		deoglModelTexture &out = pTextures[ i ];
		
		out.SetDoubleSided( in.GetDoubleSided() );
		out.SetDecal( in.GetDecal() );
		out.SetDecalOffset( in.GetDecalOffset() );
		
		pDoubleSided |= in.GetDoubleSided();
		pDecal |= in.GetDecal();
	}
	
	// add texture coordinate sets and set the texture coordinate count
	if( modelTexCoordCount > 0 ){
		pTexCoords = new decVector2[ modelTexCoordCount ];
		pTexCoordCount = modelTexCoordCount;
		
		if( pTexCoordCount > 0 ){
			memcpy( pTexCoords, modelTexCoordSets[ 0 ].GetTextureCoordinates(),
				sizeof( decVector2 ) * pTexCoordCount );
		}
	}
	
	if( modelTexCoordSetCount > 1 ){
		pTexCoordSets = new deoglModelLODTexCoordSet[ modelTexCoordSetCount - 1 ];
		pTexCoordSetCount = modelTexCoordSetCount - 1;
		
		for( i=0; i<pTexCoordSetCount; i++ ){
			deoglModelLODTexCoordSet &tcs = pTexCoordSets[ i ];
			
			tcs.SetTextureCoordinateCount( pTexCoordCount, false );
			if( pTexCoordCount > 0 ){
				memcpy( tcs.GetTextureCoordinates(),
					modelTexCoordSets[ i + 1 ].GetTextureCoordinates(),
					sizeof( decVector2 ) * pTexCoordCount );
			}
		}
	}
	
	// add faces sorted by textures and group vertices. this can be very slow with a naive
	// implementation and large models. sortVertices stores the first point an original model
	// vertex / has been involved with. each additional involvement is added to a forward
	// linked list.
	if( modelFaceCount > 0 ){
		sGroupVertex **sortVertices = NULL;
		sGroupVertex *sortedVertices = NULL;
		
		try{
			int sortedVertexCount = 0;
			int k, f = 0;
			
			sortVertices = new sGroupVertex*[ modelVertexCount ];
			sortedVertices = new sGroupVertex[ modelFaceCount * 3 ];
			
			memset( sortVertices, 0, sizeof( sGroupVertex* ) * modelVertexCount );
			
			pFaces = new deoglModelFace[ modelFaceCount ];
			pFaceCount = modelFaceCount;
			
			for( i=0; i<pTextureCount; i++ ){
				deoglModelTexture &texture = pTextures[ i ];
				int texFaceCount = 0;
				
				texture.SetFirstFace( f );
				
				for( j=0; j<pFaceCount; j++ ){
					const deModelFace &modelFace = modelFaces[ j ];
					if( modelFace.GetTexture() != i ){
						continue;
					}
					
					/*if( f >= pFaceCount ){
						DETHROW( deeInvalidParam );
					}*/
					
					deoglModelFace &face = pFaces[ f ];
					face.SetIndex( f );
					face.SetTexture( i );
					
					for( k=0; k<3; k++ ){
						const int position = modelFace.GetVertexAt( k );
						const int texCoord = modelFace.GetTextureCoordinatesAt( k );
						const int normal = modelFace.GetNormalAt( k );
						const int tangent = modelFace.GetTangentAt( k );
						
						sGroupVertex *findVertex = sortVertices[ position ];
						sGroupVertex *lastVertex = NULL;
						while( findVertex ){
							const oglModelVertex &vertex = pVertices[ findVertex->vertex ];
							if( texCoord == vertex.texcoord && normal == vertex.normal
							&& tangent == vertex.tangent ){
								break;
							}
							lastVertex = findVertex;
							findVertex = findVertex->next;
						}
						
						if( findVertex ){
							face.SetVertexAt( k, findVertex->vertex );
							
						}else{
							oglModelVertex &vertex = pVertices[ pVertexCount ];
							vertex.position = position;
							vertex.texcoord = texCoord;
							vertex.normal = normal;
							vertex.tangent = tangent;
							
							sGroupVertex * const nextVertex = sortedVertices + sortedVertexCount++;
							nextVertex->vertex = pVertexCount;
							nextVertex->next = NULL;
							
							if( lastVertex ){
								lastVertex->next = nextVertex;
								
							}else{
								sortVertices[ position ] = nextVertex;
							}
							
							face.SetVertexAt( k, pVertexCount++ );
						}
					}
					
					f++;
					texFaceCount++;
				}
				
				texture.SetFaceCount( texFaceCount );
			}
			
			if( f != pFaceCount ){
				DETHROW( deeInvalidParam );
			}
			
			delete [] sortVertices;
			delete [] sortedVertices;
			
		}catch( const deException & ){
			if( sortVertices ){
				delete [] sortVertices;
			}
			if( sortedVertices ){
				delete [] sortedVertices;
			}
			throw;
		}
	}
	
	// optimize the memory consumption of the vertices array. required only if the difference
	// is large and on systems with low memory like mobile systems.
	if( pVertices ){
		oglModelVertex *optimized = NULL;
		
		if( pVertexCount > 0 ){
			optimized = new oglModelVertex[ pVertexCount ];
			memcpy( optimized, pVertices, sizeof( oglModelVertex ) * pVertexCount );
		}
		
		delete [] pVertices;
		pVertices = optimized;
	}
	
	// create normal array
	if( modelNormalCount > 0 ){
		pNormals = new decVector[ modelNormalCount ];
		pNormalCount = modelNormalCount;
	}
	
	// create tangent array
	if( modelTangentCount > 0 ){
		pTangents = new decVector[ modelTangentCount ];
		pNegateTangents = new bool[ modelTangentCount ];
		pTangentCount = modelTangentCount;
		
		for( i=0; i<pTexCoordSetCount; i++ ){
			pTexCoordSets[ i ].SetTangentCount( modelTangentCount );
		}
	}
	
	// debug
#if 0
	pModel->GetOgl()->LogInfoFormat( "Model(%s,%i): %i faces, %i points (%.1f%% less).", engModel.GetFilename(),
		pLODIndex, pFaceCount, pVertexCount, 100.0f * ( 1.0f - ( float )pVertexCount / ( float )( pFaceCount * 3 ) ) );
#endif
}

void deoglModelLOD::pOptimizeVertexCache(){
	deoglVCOptimizer optimizer;
	int i;
	
	//decTimer timer;
	//timer.Reset();
	
	// calculate the optimal ordering of faces and vertices
	optimizer.SetVertexCount( pVertexCount );
	
	optimizer.SetFaceCount( pFaceCount );
	for( i=0; i<pFaceCount; i++ ){
		optimizer.SetFaceAt( i , pFaces[ i ].GetVertex1(),
			pFaces[ i ].GetVertex2(), pFaces[ i ].GetVertex3() );
	}
	
	optimizer.SetGroupCount( pTextureCount );
	for( i=0; i<pTextureCount; i++ ){
		optimizer.SetGroupAt( i, pTextures[ i ].GetFirstFace(), pTextures[ i ].GetFaceCount() );
	}
	
	optimizer.Optimize();
	
	// apply the ordering. for this a new array is created for the vertices and faces and the
	// entries copied in the correct order. for the face in addition the vertex indices are
	// remapped to the new vertex order
	const deoglVCOptimizer::sVertex * const optimizerVertices = optimizer.GetVertices();
	const int * const reorderedVertices = optimizer.GetReorderedVertices();
	const int * const reorderedFaces = optimizer.GetReorderedFaces();
	oglModelVertex *vertices = NULL;
	deoglModelFace *faces = NULL;
	
	try{
		if( pVertexCount > 0 ){
			vertices = new oglModelVertex[ pVertexCount ];
			
			for( i=0; i<pVertexCount; i++ ){
				memcpy( vertices + i, pVertices + reorderedVertices[ i ], sizeof( oglModelVertex ) );
			}
		}
		
		if( pFaceCount > 0 ){
			faces = new deoglModelFace[ pFaceCount ];
			
			for( i=0; i<pFaceCount; i++ ){
				memcpy( faces + i, pFaces + reorderedFaces[ i ], sizeof( deoglModelFace ) );
				faces[ i ].SetIndex( i );
				faces[ i ].SetVertex1( optimizerVertices[ faces[ i ].GetVertex1() ].reorderedIndex );
				faces[ i ].SetVertex2( optimizerVertices[ faces[ i ].GetVertex2() ].reorderedIndex );
				faces[ i ].SetVertex3( optimizerVertices[ faces[ i ].GetVertex3() ].reorderedIndex );
			}
		}
		
		if( pFaces ){
			delete [] pFaces;
		}
		pFaces = faces;
		
		if( pVertices ){
			delete [] pVertices;
		}
		pVertices = vertices;
		
	}catch( const deException & ){
		if( faces ){
			delete [] faces;
		}
		if( vertices ){
			delete [] vertices;
		}
		throw;
	}
	
	//pModel->GetOgl()->LogInfoFormat( "model '%s' lod %i optimize in %ims", pModel.GetFilename().GetString(),
	//	pLODIndex, ( int )( timer.GetElapsedTime() * 1000.0f ) );
}

void deoglModelLOD::pWriteVBOData(){
	if( ! pVBOBlock ){
		DETHROW( deeInvalidParam );
	}
	
	deoglVBOWriterModel writerVBO( pModel.GetRenderThread() );
	int i, j, tcsCount;
	
	tcsCount = pTexCoordSetCount;
	if( tcsCount > 2 ){
		tcsCount = 2;
	}
	
	writerVBO.Reset( pVBOBlock );
	
	for( i=0; i<pVertexCount; i++ ){
		const oglModelVertex &vertex = pVertices[ i ];
		
		writerVBO.WritePoint( pPositions[ vertex.position ].position,
			pNormals[ vertex.normal ], pTangents[ vertex.tangent ],
			pNegateTangents[ vertex.tangent ], pTexCoords[ vertex.texcoord ],
			pPositions[ vertex.position ].normal );
		
		for( j=0; j<tcsCount; j++ ){
			const deoglModelLODTexCoordSet &tcs = pTexCoordSets[ j ];
			
			writerVBO.WriteTexCoordSetPoint( tcs.GetTangents()[ vertex.tangent ],
				tcs.GetNegateTangents()[ vertex.tangent ],
				tcs.GetTextureCoordinates()[ vertex.texcoord ] );
		}
	}
	
	for( i=0; i<pFaceCount; i++ ){
		writerVBO.WriteIndices( pFaces[ i ].GetVertex3(), pFaces[ i ].GetVertex2(), pFaces[ i ].GetVertex1() );
	}
}

void deoglModelLOD::pWriteVBODataPositionWeight(){
	if( ! pVBOBlockPositionWeight ){
		DETHROW( deeInvalidParam );
	}
	
	char * const vboData = ( char* )pVBOBlockPositionWeight->GetData();
	int i;
	
	for( i=0; i<pPositionCount; i++ ){
		const int offset = 16 * i;
		GLfloat * const dataPosition = ( GLfloat* )( vboData + offset );
		GLint * const dataWeight = ( GLint* )( vboData + offset + 12 );
		
		dataPosition[ 0 ] = ( GLfloat )pPositions[ i ].position.x;
		dataPosition[ 1 ] = ( GLfloat )pPositions[ i ].position.y;
		dataPosition[ 2 ] = ( GLfloat )pPositions[ i ].position.z;
		*dataWeight = ( GLint )pPositions[ i ].weight;
	}
}

void deoglModelLOD::pWriteVBODataCalcNormalTangent(){
	if( ! pVBOBlockCalcNormalTangent ){
		DETHROW( deeInvalidParam );
	}
	
	char * const vboData = ( char* )pVBOBlockCalcNormalTangent->GetData();
	int i;
	
	for( i=0; i<pFaceCount; i++ ){
		const int offset = 44 * i;
		GLint * const dataPositionIndex = ( GLint* )( vboData + offset );
		GLint * const dataNormalIndex = ( GLint* )( vboData + offset + 12 );
		GLint * const dataTangentIndex = ( GLint* )( vboData + offset + 24 );
		GLfloat * const dataTangentFactor = ( GLfloat* )( vboData + offset + 36 );
		
		const oglModelVertex &v1 = pVertices[ pFaces[ i ].GetVertex1() ];
		const oglModelVertex &v2 = pVertices[ pFaces[ i ].GetVertex2() ];
		const oglModelVertex &v3 = pVertices[ pFaces[ i ].GetVertex3() ];
		
		dataPositionIndex[ 0 ] = ( GLint )v1.position;
		dataPositionIndex[ 1 ] = ( GLint )v2.position;
		dataPositionIndex[ 2 ] = ( GLint )v3.position;
		
		dataNormalIndex[ 0 ] = ( GLint )v1.normal;
		dataNormalIndex[ 1 ] = ( GLint )v2.normal;
		dataNormalIndex[ 2 ] = ( GLint )v3.normal;
		
		dataTangentIndex[ 0 ] = ( GLint )v1.tangent;
		dataTangentIndex[ 1 ] = ( GLint )v2.tangent;
		dataTangentIndex[ 2 ] = ( GLint )v3.tangent;
		
		const decVector2 &tc1 = pTexCoords[ v1.texcoord ];
		const decVector2 &tc2 = pTexCoords[ v2.texcoord ];
		const decVector2 &tc3 = pTexCoords[ v3.texcoord ];
		
		dataTangentFactor[ 0 ] = tc3.y - tc1.y;
		dataTangentFactor[ 1 ] = tc2.y - tc1.y;
	}
}

void deoglModelLOD::pWriteVBODataWriteSkinnedVBO(){
	if( ! pVBOBlockWriteSkinnedVBO ){
		DETHROW( deeInvalidParam );
	}
	
	char * const vboData = ( char* )pVBOBlockWriteSkinnedVBO->GetData();
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		char * const vboDataVertex = vboData + ( i * 16 );
		
		*( ( GLint* )( vboDataVertex + 0 ) ) = ( GLint )pVertices[ i ].position;
		*( ( GLint* )( vboDataVertex + 4 ) ) = ( GLint )pVertices[ i ].normal;
		*( ( GLint* )( vboDataVertex + 8 ) ) = ( GLint )pVertices[ i ].tangent;
		*( ( GLfloat* )( vboDataVertex + 12 ) ) = ( GLfloat )( pNegateTangents[ pVertices[ i ].tangent ] ? -1.0f : 1.0f );
	}
}

void deoglModelLOD::pWriteVBODataWithWeight(){
	if( ! pVBOBlockWithWeight ){
		DETHROW( deeInvalidParam );
	}
	
	deoglVBOWriterModel writerVBO( pModel.GetRenderThread() );
	int i;
	
	writerVBO.Reset( pVBOBlockWithWeight );
	
	for( i=0; i<pVertexCount; i++ ){
		const oglModelVertex &vertex = pVertices[ i ];
		
		writerVBO.WritePoint( pPositions[ vertex.position ].position, pNormals[ vertex.normal ],
			pTangents[ vertex.tangent ], pNegateTangents[ vertex.tangent ],
			pTexCoords[ vertex.texcoord ], pPositions[ vertex.position ].normal );
		
		writerVBO.WriteWeight( pPositions[ vertex.position ].weight );
	}
	
// 	int maxCount = 0;
// 	for( i=0; i<pWeightsCount; i++ ){
// 		if( pWeightsCounts[ i ] > maxCount ) maxCount = pWeightsCounts[ i ];
// 	}
// 	pModel->GetOgl()->LogInfoFormat( "model %s bones=%i weights=%i(max %i) vertices=%i normals=%i tangents=%i",
// 		pModel.GetFilename().GetString(), pModel->GetModel()->GetBoneCount(), pWeightsCount, maxCount,
// 		pVertexCount, pNormalCount, pTangentCount );
}
