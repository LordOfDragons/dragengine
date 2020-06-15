/* 
 * FBX Model Module
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

#include "fbxModelModule.h"
#include "../shared/fbxScene.h"
#include "../shared/fbxNode.h"
#include "../shared/fbxProperty.h"
#include "../shared/property/fbxPropertyArrayDouble.h"
#include "../shared/property/fbxPropertyArrayFloat.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXModelCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *FBXModelCreateModule( deLoadableModule *loadableModule ){
	try{
		return new fbxModelModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
}



// Class fbxModelModule
/////////////////////////

// Constructor, destructor
////////////////////////////

fbxModelModule::fbxModelModule( deLoadableModule &loadableModule ) :
deBaseModelModule( loadableModule ),
pCacheTCSorter( NULL )
{
	//pCacheTCSorter = new deCacheHelper( &GetVFS(), decPath::CreatePathUnix( "/cache/local/tcsorter" ) );
}

fbxModelModule::~fbxModelModule(){
	if( pCacheTCSorter ){
		delete pCacheTCSorter;
	}
}



// Loading and Saving
///////////////////////

void fbxModelModule::LoadModel( decBaseFileReader &reader, deModel &model ){
	deModelLOD *modelLod = NULL;
	
	try{
		fbxScene scene( reader );
		scene.Prepare( *this );
		//scene.DebugPrintStructure( *this, true );
		
		// find model. we pick the first one we find
		const fbxNode * const nodeObjects = scene.GetNode()->FirstNodeNamed( "Objects" );
		if( ! nodeObjects ){
			DETHROW_INFO( deeInvalidParam, "missing node 'Objects'" );
		}
		
		const fbxNode * const nodeGeometry = nodeObjects->FirstNodeNamed( "Geometry" );
		if( ! nodeGeometry ){
			DETHROW_INFO( deeInvalidParam, "missing node 'Geometry'" );
		}
		
		modelLod = new deModelLOD;
		pLoadModelLod( scene, *nodeGeometry, model, *modelLod );
		model.AddLOD( modelLod );
		
	}catch( const deException & ){
		if( modelLod ){
			delete modelLod;
		}
		
		LogErrorFormat( "Failed reading file '%s' at file position %d",
			reader.GetFilename(), reader.GetPosition() );
		throw;
	}
}

void fbxModelModule::SaveModel(decBaseFileWriter &writer, const deModel &model){
	// nothing yet
}



// Private Functions
//////////////////////

void fbxModelModule::pLoadModelLod( const fbxScene &scene, const fbxNode &geometry,
deModel &model, deModelLOD &lod ){
	const fbxNode * const nodeVertices = geometry.FirstNodeNamed( "Vertices" );
	if( ! nodeVertices ){
		DETHROW_INFO( deeInvalidParam, "missing node 'Vertices'" );
	}
	
	const fbxNode * const nodePolygonVertexIndex = geometry.FirstNodeNamed( "PolygonVertexIndex" );
	if( ! nodePolygonVertexIndex ){
		DETHROW_INFO( deeInvalidParam, "missing node 'PolygonVertexIndex'" );
	}
	
	const fbxNode * const nodeLayerElementMaterial = geometry.FirstNodeNamed( "LayerElementMaterial" );
	if( ! nodeLayerElementMaterial ){
		DETHROW_INFO( deeInvalidParam, "missing node 'LayerElementMaterial'" );
	}
	
	deModelTexture * const texture = new deModelTexture( "skin", 512, 512 );
	model.AddTexture( texture );
	model.GetTextureCoordinatesSetList().Add( "default" );
	
	fbxProperty &propVertices = *nodeVertices->GetPropertyAt( 0 );
	pLoadModelVertices( scene, geometry, propVertices, model, lod );
	
	fbxProperty &propPolygonVertexIndex = *nodePolygonVertexIndex->GetPropertyAt( 0 );
	fbxProperty &propLayerElementMaterial = *nodeLayerElementMaterial->GetPropertyAt( 0 );
	pLoadModelFaces( scene, geometry, propPolygonVertexIndex, propLayerElementMaterial, model, lod );
}

void fbxModelModule::pLoadModelVertices( const fbxScene &scene, const fbxNode &,
fbxProperty &property, deModel &, deModelLOD &lod ){
	const float scale = 1.0f; //scene.GetScaleFactor() * scene.GetUnitScaleFactor();
	const int count = property.GetValueCount() / 3;
	lod.SetVertexCount( count );
	lod.SetNormalCount( count );
	lod.SetTangentCount( count );
	
	deModelVertex * const vertices = lod.GetVertices();
	int i, j;
	
	for( i=0, j=0; i<count; i++, j+=3 ){
		vertices[ i ].SetPosition( decVector(
			property.GetValueAtAsFloat( j ),
			property.GetValueAtAsFloat( j + 1 ),
			property.GetValueAtAsFloat( j + 2 ) ) * scale );
	}
}

void fbxModelModule::pLoadModelFaces( const fbxScene &scene, const fbxNode &geometry,
fbxProperty &propertyPolygonVertexIndex, fbxProperty &propertyLayerElementMaterial,
deModel &model, deModelLOD &lod ){
	const int polyVertIndexCount = propertyPolygonVertexIndex.GetValueCount();
	int i;
	
	// we have to first determine the number of required triangles. polygons can have any
	// number of vertices. for this reason the last vertex in the loop is negative with
	// all others positive
	int faceCount = 0;
	
	for( i=2; i<polyVertIndexCount; i++ ){
		faceCount++;
		if( propertyPolygonVertexIndex.GetValueAtAsInt( i ) < 0 ){
			i += 2;
		}
	}
	
	// determina the number of used materials
	
	// now build the faces
	int indexFace = 0;
	int faceIndex0 = propertyPolygonVertexIndex.GetValueAtAsInt( 0 );
	int faceIndexLast = propertyPolygonVertexIndex.GetValueAtAsInt( 1 );
	int texCoordIndex = 0;
	
	lod.SetFaceCount( faceCount );
	lod.SetTextureCoordinatesSetCount( 1 );
	lod.SetTextureCoordinatesCount( faceCount * 3 );
	
	deModelTextureCoordinatesSet &tcs = lod.GetTextureCoordinatesSetAt( 0 );
	tcs.SetTextureCoordinatesCount( faceCount * 3 );
	
	for( i=2; i<polyVertIndexCount; i++ ){
		int index = propertyPolygonVertexIndex.GetValueAtAsInt( i );
		bool initNextPolygon = false;
		if( index < 0 ){
			index ^= -1;
			initNextPolygon = i + 2 < polyVertIndexCount;
		}
		
		// continue polygon. each new vertex forms a new face with base and last vertex
		deModelFace &face = lod.GetFaceAt( indexFace++ );
		
		face.SetVertex1( faceIndex0 );
		face.SetVertex2( faceIndexLast );
		face.SetVertex3( index );
		
		face.SetNormal1( faceIndex0 );
		face.SetNormal2( faceIndexLast );
		face.SetNormal3( index );
		
		face.SetTangent1( faceIndex0 );
		face.SetTangent2( faceIndexLast );
		face.SetTangent3( index );
		
		face.SetTextureCoordinates1( texCoordIndex++ );
		face.SetTextureCoordinates2( texCoordIndex++ );
		face.SetTextureCoordinates3( texCoordIndex++ );
		
		face.SetTexture( 0 );
		
		faceIndexLast = index;
		
		// if this is the end of the polygon initialize the next one if possible
		if( initNextPolygon ){
			faceIndex0 = propertyPolygonVertexIndex.GetValueAtAsInt( i + 1 );
			faceIndexLast = propertyPolygonVertexIndex.GetValueAtAsInt( i + 2 );
			i += 2;
		}
	}
	
	if( indexFace != faceCount ){
		DETHROW( deeInvalidAction );
	}
}
