/* 
 * Drag[en]gine Model Module
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

#include "deModelModule.h"
#include "demdlWeightSet.h"
#include "demdlWeightSetList.h"
#include "demdlTexCoordSorter.h"

#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/deEngine.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEModelCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *DEModelCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deModelModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Definitions
////////////////

#define FLAG_TEX_DOUBLE_SIDED		0x1
#define FLAG_TEX_DECAL				0x2

#define FLAG_HAS_LOD_ERROR			0x1
#define FLAG_LARGE_MODEL			0x2



// Structures
///////////////

struct sWeight{
	int bone;
	float weight;
};



// Class deModelModule
////////////////////////

// Constructor, destructor
////////////////////////////

deModelModule::deModelModule( deLoadableModule &loadableModule ) :
deBaseModelModule( loadableModule ){
	pCacheTCSorter = NULL;
	
	// create cache
	deVirtualFileSystem &vfs = GetVFS();
	decPath path;
	
	path.SetFromUnix( "/cache/local/tcsorter" );
	pCacheTCSorter = new deCacheHelper( &vfs, path );
}

deModelModule::~deModelModule(){
	if( pCacheTCSorter ){
		delete pCacheTCSorter;
	}
}



// Loading and Saving
///////////////////////

void deModelModule::LoadModel( decBaseFileReader &reader, deModel &model ){
#ifdef OS_ANDROID
	decTimer timer;
#endif
	
	const char *infosSigOld = "Drag[en]gine Model Low  ";
	char checkSig[ 24 ];
	
	reader.Read( &checkSig, 24 );
	reader.SetPosition( 0 );
	
	if( strncmp( checkSig, infosSigOld, 24 ) == 0 ){
		LogWarnFormat( "Loading '%s' using old format (DEPRECATED)", reader.GetFilename() );
		pLoadVersion0( reader, model );
		
	}else{
		pLoadModel( reader, model );
	}
	
#ifdef OS_ANDROID
	LogInfoFormat( "Loaded '%s' in %dms", reader.GetFilename(), ( int )( timer.GetElapsedTime() * 1e3f ) );
#endif
}

void deModelModule::SaveModel(decBaseFileWriter &writer, const deModel &model){
	// nothing yet
}



// Private Functions
//////////////////////

void deModelModule::pLoadVersion0( decBaseFileReader &reader, deModel &model ){
	const char *infosSig = "Drag[en]gine Model Low  ";
	decString readStr;
	char checkSig[ 24 ];
	int i, j, k, version, /*reserved, */count;
	int width, height, parent, boneCount;
	int boneIndex, vertexIndex, normalIndex, textureIndex, weightsIndex;
	int vertexCount = 0, normalCount = 0;
	demdlWeightSetList weightSetList;
	demdlWeightSet *weightSet;
	int weightGroupCount;
	int weightSetCount;
	int weightCount;
	int faceCount;
	decVector2 texPos;
	deModelBone *bone;
	deModelTexture *texture;
	deModelLOD *lod = NULL;
	int textureFlags;
	float curWeight;
	demdlTexCoordSorter texCoordSorter;
	
	// add a lod mesh
	lod = new deModelLOD;
	model.AddLOD( lod );
	
	// check infos
	reader.Read( &checkSig, 24 );
	if( strncmp( checkSig, infosSig, 24 ) != 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	version = reader.ReadShort();
	if( version != 1 ){
		DETHROW( deeInvalidFormat );
	}
	/*reserved = */reader.ReadShort();
	
	// model informations
	reader.ReadString16();
	reader.ReadUShort(); // dummy: property list
	
	// bones
	count = reader.ReadShort();
	for( i=0; i<count; i++ ){
		// name
		readStr = reader.ReadString8();
		if( model.HasBoneNamed(readStr) ){
			DETHROW( deeInvalidFormat );
		}
		
		bone = new deModelBone( readStr );
		model.AddBone( bone );
		
		// position
		bone->SetPosition( reader.ReadVector() );
		
		// orientation
		bone->SetOrientation( decMatrix::CreateRotation( reader.ReadVector() * DEG2RAD ).ToQuaternion() );
		
		// parent index
		parent = reader.ReadShort();
		if( parent < -1 || parent >= count || parent == i ){
			DETHROW( deeInvalidFormat );
		}
		bone->SetParent( parent );
		
		// properties
		reader.ReadUShort(); // dummy: property list
	}
	
	// textures
	count = reader.ReadShort();
	for( i=0; i<count; i++ ){
		// name
		readStr = reader.ReadString8();
		if( model.HasTextureNamed( readStr ) ){
			DETHROW( deeInvalidFormat );
		}
		
		// width
		width = reader.ReadShort();
		if( width < 1 ){
			DETHROW( deeInvalidFormat );
		}
		
		// height
		height = reader.ReadShort();
		if( height < 1 ){
			DETHROW( deeInvalidFormat );
		}
		
		texture = new deModelTexture( readStr, width, height );
		model.AddTexture( texture );
		
		// flags
		textureFlags = reader.ReadUShort();
		if( ( textureFlags & FLAG_TEX_DOUBLE_SIDED ) == FLAG_TEX_DOUBLE_SIDED ){
			texture->SetDoubleSided( true );
		}
	}
	
	// vertices
	vertexCount = reader.ReadShort();
	if( vertexCount < 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	lod->SetVertexCount( vertexCount );
	
	for( i=0; i<vertexCount; i++ ){
		deModelVertex &vertex = lod->GetVertexAt( i );
		
		// weights
		boneCount = reader.ReadByte();
		if( boneCount > 0 ){
			weightSet = new demdlWeightSet;
			
			try{
				for( j=0; j<boneCount; j++ ){
					boneIndex = reader.ReadShort();
					if( boneIndex < 0 || boneIndex >= model.GetBoneCount() ){
						DETHROW( deeInvalidFormat );
					}
					
					curWeight = ( float )reader.ReadShort() / 1000.0f;
					weightSet->Set( boneIndex, curWeight );
				}
				
				weightSet->Normalize();
				weightsIndex = weightSetList.IndexOfEqual( *weightSet );
				
				if( weightsIndex == -1 ){
					weightSetList.Add( weightSet );
					weightSet = NULL;
					
					vertex.SetWeightSet( weightSetList.GetCount() - 1 );
					
				}else{
					delete weightSet;
					weightSet = NULL;
					
					vertex.SetWeightSet( weightsIndex );
				}
				
			}catch( const deException & ){
				if( weightSet ){
					delete weightSet;
				}
				throw;
			}
		}
		
		// position
		vertex.SetPosition( reader.ReadVector() );
	}
	
	// sort weight sets by the number of weights stored inside from the lowest to the highest
	weightGroupCount = weightSetList.GetLargestWeightCount();
	weightSetCount = weightSetList.GetCount();
	weightCount = 0;
	for( i=0; i<weightSetCount; i++ ){
		weightCount += weightSetList.GetAt( i )->GetCount();
	}
	
	lod->SetWeightGroupCount( weightGroupCount );
	lod->SetWeightCount( weightCount );
	
	deModelWeight * const modelWeights = lod->GetWeights();
	int * const modelWeightGroups = lod->GetWeightGroups();
	int weightSetIndex = 0;
	weightCount = 0;
	
	for( i=0; i<weightGroupCount; i++ ){
		const int tempCount = i + 1;
		
		modelWeightGroups[ i ] = 0;
		
		for( j=0; j<weightSetCount; j++ ){
			demdlWeightSet &weightSet = *weightSetList.GetAt( j );
			
			if( weightSet.GetCount() == tempCount ){
				for( k=0; k<tempCount; k++ ){
					modelWeights[ weightCount + k ].SetBone( weightSet.GetBoneAt( k ) );
					modelWeights[ weightCount + k ].SetWeight( weightSet.GetWeightAt( k ) );
				}
				
				weightSet.SetGroupedIndex( weightSetIndex );
				
				weightSetIndex++;
				weightCount += tempCount;
				modelWeightGroups[ i ]++;
			}
		}
	}
	
	// adjust the weight set of all vertices to point to the new location
	for( i=0; i<vertexCount; i++ ){
		deModelVertex &vertex = lod->GetVertexAt( i );
		
		if( vertex.GetWeightSet() != -1 ){
			vertex.SetWeightSet( weightSetList.GetAt( vertex.GetWeightSet() )->GetGroupedIndex() );
		}
	}
	
	// normal and tangent count
	normalCount = reader.ReadShort(); // normal count
	if( normalCount < 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	lod->SetNormalCount( normalCount );
	lod->SetTangentCount( normalCount );
	
	// we have to hack for the time being since the number of faces depends on the triangle and
	// quad count but the counts are not stored in a header but inside the data out of reach
	const int filePosition = reader.GetPosition();
	
	count = reader.ReadShort();
	faceCount = count;
	for( i=0; i<count; i++ ){
		reader.ReadShort();
		reader.ReadShort();
		reader.ReadShort();
		reader.ReadShort();
		reader.ReadShort();
		reader.ReadShort();
		reader.ReadShort();
		reader.ReadVector2();
		reader.ReadVector2();
		reader.ReadVector2();
	}
	
	count = reader.ReadShort();
	faceCount += count * 2;
	
	lod->SetFaceCount( faceCount );
	lod->SetTextureCoordinatesSetCount( 1 );
	model.GetTextureCoordinatesSetList().Add( "" ); // default tc set name
	deModelFace * const modelFaces = lod->GetFaces();
	
	faceCount = 0;
	
	reader.SetPosition( filePosition );
	
	// prepare the texture coordinate sorter
	texCoordSorter.Resize( lod->GetFaceCount(), 1 );
	
	// triangles
	count = reader.ReadShort();
	if( count < 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	for( i=0; i<count; i++ ){
		deModelFace &face1 = modelFaces[ faceCount ];
		
		// texture index
		textureIndex = reader.ReadShort();
		if( textureIndex < 0 || textureIndex >= model.GetTextureCount() ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTexture( textureIndex );
		
		// vertex index 1
		vertexIndex = reader.ReadShort();
		if( vertexIndex < 0 || vertexIndex >= vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex1( vertexIndex );
		
		// vertex index 2
		vertexIndex = reader.ReadShort();
		if( vertexIndex < 0 || vertexIndex >= vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex2( vertexIndex );
		
		// vertex index 3
		vertexIndex = reader.ReadShort();
		if( vertexIndex < 0 || vertexIndex >= vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex3( vertexIndex );
		
		// normal index 1
		normalIndex = reader.ReadShort();
		if( normalIndex < 0 || normalIndex >= normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal1( normalIndex );
		face1.SetTangent1( normalIndex );
		
		// normal index 2
		normalIndex = reader.ReadShort();
		if( normalIndex < 0 || normalIndex >= normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal2( normalIndex );
		face1.SetTangent2( normalIndex );
		
		// normal index 3
		normalIndex = reader.ReadShort();
		if( normalIndex < 0 || normalIndex >= normalCount ){
			DETHROW( deeInvalidFormat );
		}
		
		face1.SetNormal3( normalIndex );
		face1.SetTangent3( normalIndex );
		
		// texture coordinates
		texCoordSorter.SetFaceTexCoordAt( faceCount, 0, 0, reader.ReadVector2() );
		texCoordSorter.SetFaceTexCoordAt( faceCount, 1, 0, reader.ReadVector2() );
		texCoordSorter.SetFaceTexCoordAt( faceCount, 2, 0, reader.ReadVector2() );
		//face1.SetTextureCoordinates1( reader.ReadVector2() );
		//face1.SetTextureCoordinates2( reader.ReadVector2() );
		//face1.SetTextureCoordinates3( reader.ReadVector2() );
		
		faceCount++;
	}
	
	// quad faces
	count = reader.ReadShort();
	if( count < 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	for( i=0; i<count; i++ ){
		deModelFace &face1 = modelFaces[ faceCount ];
		deModelFace &face2 = modelFaces[ faceCount + 1 ];
		
		// texture index
		textureIndex = reader.ReadShort();
		if( textureIndex < 0 || textureIndex >= model.GetTextureCount() ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTexture( textureIndex );
		face2.SetTexture( textureIndex );
		
		// vertex index 1
		vertexIndex = reader.ReadShort();
		if( vertexIndex < 0 || vertexIndex >= vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex1( vertexIndex );
		face2.SetVertex1( vertexIndex );
		
		// vertex index 2
		vertexIndex = reader.ReadShort();
		if( vertexIndex < 0 || vertexIndex >= vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex2( vertexIndex );
		
		// vertex index 3
		vertexIndex = reader.ReadShort();
		if( vertexIndex < 0 || vertexIndex >= vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex3( vertexIndex );
		face2.SetVertex2( vertexIndex );
		
		// vertex index 4
		vertexIndex = reader.ReadShort();
		if( vertexIndex < 0 || vertexIndex >= vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetVertex3( vertexIndex );
		
		// normal index 1
		normalIndex = reader.ReadShort();
		if( normalIndex < 0 || normalIndex >= normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal1( normalIndex );
		face1.SetTangent1( normalIndex );
		face2.SetNormal1( normalIndex );
		face2.SetTangent1( normalIndex );
		
		// normal index 2
		normalIndex = reader.ReadShort();
		if( normalIndex < 0 || normalIndex >= normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal2( normalIndex );
		face1.SetTangent2( normalIndex );
		
		// normal index 3
		normalIndex = reader.ReadShort();
		if( normalIndex < 0 || normalIndex >= normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal3( normalIndex );
		face1.SetTangent3( normalIndex );
		face2.SetNormal2( normalIndex );
		face2.SetTangent2( normalIndex );
		
		// normal index 4
		normalIndex = reader.ReadShort();
		if( normalIndex < 0 || normalIndex >= normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetNormal3( normalIndex );
		face2.SetTangent3( normalIndex );
		
		// texture position 1
		texPos = reader.ReadVector2();
		texCoordSorter.SetFaceTexCoordAt( faceCount, 0, 0, texPos );
		texCoordSorter.SetFaceTexCoordAt( faceCount + 1, 0, 0, texPos );
		//face1.SetTextureCoordinates1( texPos );
		//face2.SetTextureCoordinates1( texPos );
		
		// texture position 2
		texPos = reader.ReadVector2();
		texCoordSorter.SetFaceTexCoordAt( faceCount, 1, 0, texPos );
		//face1.SetTextureCoordinates2( texPos );
		
		// texture position 3
		texPos = reader.ReadVector2();
		texCoordSorter.SetFaceTexCoordAt( faceCount, 2, 0, texPos );
		texCoordSorter.SetFaceTexCoordAt( faceCount + 1, 1, 0, texPos );
		//face1.SetTextureCoordinates3( texPos );
		//face2.SetTextureCoordinates2( texPos );
		
		// texture position 4
		texPos = reader.ReadVector2();
		texCoordSorter.SetFaceTexCoordAt( faceCount + 1, 2, 0, texPos );
		//face2.SetTextureCoordinates3( texPos );
		
		faceCount += 2;
	}
	
	// sort texture coordinates and assign the indices
	deModelTextureCoordinatesSet &tcset = lod->GetTextureCoordinatesSetAt( 0 );
	
	texCoordSorter.Sort();
	
	count = texCoordSorter.GetTexCoordCount();
	lod->SetTextureCoordinatesCount( count );
	tcset.SetTextureCoordinatesCount( count );
	for( i=0; i<count; i++ ){
		tcset.SetTextureCoordinatesAt( i, texCoordSorter.GetTexCoordAt( i, 0 ) );
	}
	
	count = texCoordSorter.GetFaceCount();
	for( i=0; i<count; i++ ){
		deModelFace &face = lod->GetFaceAt( i );
		
		face.SetTextureCoordinates1( texCoordSorter.GetFaceCornerAt( i, 0 ) );
		face.SetTextureCoordinates2( texCoordSorter.GetFaceCornerAt( i, 1 ) );
		face.SetTextureCoordinates3( texCoordSorter.GetFaceCornerAt( i, 2 ) );
	}
}

void deModelModule::pLoadModel( decBaseFileReader &reader, deModel &model ){
	const char *signature = "Drag[en]gine Model";
	demdlTexCoordSorter texCoordSorter;
	deModelLOD *lod = NULL;
	char sigbuf[ 18 ];
	sModelInfos infos;
	int i;
	
	// check signature
	reader.Read( &sigbuf, 18 );
	if( strncmp( sigbuf, signature, 18 ) != 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	// read infos
	infos.filename = reader.GetFilename();
	infos.weightSetList = NULL;
	
	infos.version = reader.ReadUShort();
	infos.flags = reader.ReadUShort();
	
	try{
		infos.weightSetList = new demdlWeightSetList;
		
		switch( infos.version ){
		case 1:{
			infos.boneCount = reader.ReadUShort();
			infos.textureCount = reader.ReadUShort();
			infos.texCoordSetCount = ( int )reader.ReadUShort() + 1; // because base set is not counted but has to for engine
			infos.normalCount = reader.ReadUShort();
			infos.tangentCount = reader.ReadUShort();
			infos.weightsCount = reader.ReadUShort();
			infos.vertexCount = reader.ReadUShort();
			infos.triangleCount = reader.ReadUShort();
			infos.quadCount = reader.ReadUShort();
			
			infos.isLargeModel = ( ( infos.flags & 0x1 ) == 0x1 );
			infos.faceCount = infos.triangleCount + infos.quadCount * 2;
			infos.texCoordSorter = &texCoordSorter;
			
			lod = new deModelLOD;
			model.AddLOD( lod );
			
			lod->SetVertexCount( infos.vertexCount );
			lod->SetFaceCount( infos.faceCount );
			lod->SetNormalCount( infos.normalCount );
			lod->SetTangentCount( infos.tangentCount );
			lod->SetTextureCoordinatesSetCount( infos.texCoordSetCount );
			
			texCoordSorter.Resize( infos.faceCount, infos.texCoordSetCount );
			
			pLoadBones( reader, model, infos );
			pLoadTextures( reader, model, infos );
			pLoadTexCoordSetsOld( reader, model, infos );
			pLoadWeights( reader, model, infos, *lod );
			pLoadVertices( reader, model, infos, *lod );
			pLoadTrianglesOld( reader, model, infos, *lod );
			pLoadQuadsOld( reader, model, infos, *lod );
			
			pUpdateFaceTexCoordIndices( model, infos, *lod );
			}break;
			
		case 2:
		case 3:
		case 4:{
			infos.boneCount = reader.ReadUShort();
			infos.textureCount = reader.ReadUShort();
			infos.texCoordSetCount = ( int )reader.ReadUShort() + 1; // because base set is not counted but has to for engine
			infos.texCoordSorter = &texCoordSorter;
			
			const int lodMeshCount = reader.ReadUShort();
			
			infos.isLargeModel = ( ( infos.flags & 0x1 ) == 0x1 );
			
			pLoadBones( reader, model, infos );
			pLoadTextures( reader, model, infos );
			pLoadTexCoordSetsOld( reader, model, infos );
			
			for( i=0; i<lodMeshCount; i++ ){
				const int flags = infos.version >= 4 ? reader.ReadByte() : 0;
				float lodError = 0.01f;
				
				const bool hasLodError = ( ( flags & FLAG_HAS_LOD_ERROR ) == FLAG_HAS_LOD_ERROR );
				
				if( hasLodError ){
					lodError = reader.ReadFloat(); // lod error
				}
				
				if( infos.isLargeModel ){
					infos.normalCount = reader.ReadInt();
					infos.tangentCount = reader.ReadInt();
					infos.weightsCount = reader.ReadInt();
					infos.vertexCount = reader.ReadInt();
					infos.triangleCount = reader.ReadInt();
					infos.quadCount = reader.ReadInt();
					
				}else{
					infos.normalCount = reader.ReadUShort();
					infos.tangentCount = reader.ReadUShort();
					infos.weightsCount = reader.ReadUShort();
					infos.vertexCount = reader.ReadUShort();
					infos.triangleCount = reader.ReadUShort();
					infos.quadCount = reader.ReadUShort();
				}
				
				infos.faceCount = infos.triangleCount + infos.quadCount * 2;
				infos.weightSetList->RemoveAll();
				
				lod = new deModelLOD;
				model.AddLOD( lod );
				
				lod->SetVertexCount( infos.vertexCount );
				lod->SetFaceCount( infos.faceCount );
				lod->SetNormalCount( infos.normalCount );
				lod->SetTangentCount( infos.tangentCount );
				lod->SetTextureCoordinatesSetCount( infos.texCoordSetCount );
				
				lod->SetLodError( lodError );
				lod->SetHasLodError( hasLodError );
				
				texCoordSorter.Resize( infos.faceCount, infos.texCoordSetCount );
				
				pLoadWeights( reader, model, infos, *lod );
				pLoadVertices( reader, model, infos, *lod );
				pLoadTrianglesOld( reader, model, infos, *lod );
				pLoadQuadsOld( reader, model, infos, *lod );
				
				pUpdateFaceTexCoordIndices( model, infos, *lod );
			}
			}break;
			
		case 5:{
			infos.boneCount = reader.ReadUShort();
			infos.textureCount = reader.ReadUShort();
			infos.texCoordSetCount = reader.ReadUShort();
			const int lodMeshCount = reader.ReadUShort();
			
			pLoadBones( reader, model, infos );
			pLoadTextures( reader, model, infos );
			pLoadTexCoordSets( reader, model, infos );
			
			for( i=0; i<lodMeshCount; i++ ){
				const int flags = reader.ReadByte();
				
				const bool hasLodError = ( ( flags & FLAG_HAS_LOD_ERROR ) == FLAG_HAS_LOD_ERROR );
				float lodError = 0.01f;
				if( hasLodError ){
					lodError = reader.ReadFloat(); // lod error
				}
				
				infos.isLargeModel = ( ( flags & FLAG_LARGE_MODEL ) == FLAG_LARGE_MODEL );
				
				if( infos.isLargeModel ){
					infos.normalCount = reader.ReadInt();
					infos.tangentCount = reader.ReadInt();
					infos.weightsCount = reader.ReadInt();
					infos.vertexCount = reader.ReadInt();
					infos.triangleCount = reader.ReadInt();
					infos.quadCount = reader.ReadInt();
					
				}else{
					infos.normalCount = reader.ReadUShort();
					infos.tangentCount = reader.ReadUShort();
					infos.weightsCount = reader.ReadUShort();
					infos.vertexCount = reader.ReadUShort();
					infos.triangleCount = reader.ReadUShort();
					infos.quadCount = reader.ReadUShort();
				}
				
				infos.faceCount = infos.triangleCount + infos.quadCount * 2;
				infos.weightSetList->RemoveAll();
				
				lod = new deModelLOD;
				model.AddLOD( lod );
				
				lod->SetVertexCount( infos.vertexCount );
				lod->SetFaceCount( infos.faceCount );
				lod->SetNormalCount( infos.normalCount );
				lod->SetTangentCount( infos.tangentCount );
				lod->SetTextureCoordinatesSetCount( infos.texCoordSetCount );
				
				lod->SetLodError( lodError );
				lod->SetHasLodError( hasLodError );
				
				pLoadWeights( reader, model, infos, *lod );
				pLoadVertices( reader, model, infos, *lod );
				pLoadTexCoords( reader, infos, *lod );
				pLoadTriangles( reader, infos, *lod );
				pLoadQuads( reader, infos, *lod );
			}
			}break;
			
		default:
			DETHROW( deeInvalidParam );
		}
		
		delete infos.weightSetList;
		
	}catch( const deException & ){
		if( infos.weightSetList ){
			delete infos.weightSetList;
		}
		
		throw;
	}
}

void deModelModule::pLoadBones( decBaseFileReader &reader, deModel &model, sModelInfos &infos ){
	deModelBone *bone = NULL;
	decString name;
	int b, parent;
	
	try{
		for( b=0; b<infos.boneCount; b++ ){
			name = reader.ReadString8();
			if( model.HasBoneNamed( name ) ){
				DETHROW( deeInvalidFormat );
			}
			
			bone = new deModelBone( name );
			
			bone->SetPosition( reader.ReadVector() );
			bone->SetOrientation( decMatrix::CreateRotation( reader.ReadVector() * DEG2RAD ).ToQuaternion() );
			
			parent = ( int )( reader.ReadUShort() ) - 1;
			if( parent >= infos.boneCount || parent == b ){
				DETHROW( deeInvalidFormat );
			}
			bone->SetParent( parent );
			
			model.AddBone( bone );
			bone = NULL;
		}
		
	}catch( const deException & ){
		if( bone ){
			delete bone;
		}
		throw;
	}
}

void deModelModule::pLoadTextures( decBaseFileReader &reader, deModel &model, sModelInfos &infos ){
	deModelTexture *texture = NULL;
	int t,  width, height;
	decString name;
	int textureFlags;
	
	try{
		for( t=0; t<infos.textureCount; t++ ){
			name = reader.ReadString8();
			if( model.HasTextureNamed( name ) ){
				DETHROW( deeInvalidFormat );
			}
			
			width = reader.ReadUShort();
			if( width < 1 ){
				DETHROW( deeInvalidFormat );
			}
			height = reader.ReadUShort();
			if( height < 1 ){
				DETHROW( deeInvalidFormat );
			}
			
			texture = new deModelTexture( name, width, height );
			
			textureFlags = reader.ReadUShort();
			if( ( textureFlags & FLAG_TEX_DOUBLE_SIDED ) == FLAG_TEX_DOUBLE_SIDED ){
				texture->SetDoubleSided( true );
			}
			
			if( ( textureFlags & FLAG_TEX_DECAL ) == FLAG_TEX_DECAL ){
				texture->SetDecal( true );
				texture->SetDecalOffset( reader.ReadByte() );
			}
			
			model.AddTexture( texture );
			texture = NULL;
		}
		
	}catch( const deException & ){
		if( texture ){
			delete texture;
		}
		throw;
	}
}

void deModelModule::pLoadTexCoordSetsOld( decBaseFileReader &reader, deModel &model, sModelInfos &infos ){
	decString name;
	int t;
	
	// add default texture coordinate set name
	model.GetTextureCoordinatesSetList().Add( "" );
	
	// store the texture coordinate set names in the model checking for duplicate errors
	for( t=1; t<infos.texCoordSetCount; t++ ){
		name = reader.ReadString8();
		if( model.GetTextureCoordinatesSetList().Has( name ) ){
			DETHROW( deeInvalidFormat );
		}
		
		model.GetTextureCoordinatesSetList().Add( name );
	}
}

void deModelModule::pLoadTexCoordSets( decBaseFileReader &reader, deModel &model, sModelInfos &infos ){
	int i;
	
	for( i=0; i<infos.texCoordSetCount; i++ ){
		const decString name( reader.ReadString8() );
		if( model.GetTextureCoordinatesSetList().Has( name ) ){
			DETHROW( deeInvalidFormat );
		}
		model.GetTextureCoordinatesSetList().Add( name );
	}
}

void deModelModule::pLoadWeights( decBaseFileReader &reader, deModel &model, sModelInfos &infos, deModelLOD &lodMesh ){
	demdlWeightSet *weightSet = NULL;
	int w, b, boneCount;
	float factor;
	int bone;
	
	try{
		for( w=0; w<infos.weightsCount; w++ ){
			weightSet = new demdlWeightSet;
			
			boneCount = ( int )reader.ReadByte();
			for( b=0; b<boneCount; b++ ){
				bone = reader.ReadUShort();
				if( bone >= infos.boneCount ){
					DETHROW( deeInvalidFormat );
				}
				
				factor = ( float )reader.ReadUShort() / 1000.0f;
				
				weightSet->Set( bone, factor );
			}
			
			weightSet->Normalize();
			
			infos.weightSetList->Add( weightSet );
			weightSet = NULL;
		}
		
	}catch( const deException & ){
		if( weightSet ){
			delete weightSet;
		}
		
		throw;
	}
	
	// add weights sorted by the number of weights stored inside from the lowest to the highest
	const int weightGroupCount = infos.weightSetList->GetLargestWeightCount();
	const int weightSetCount = infos.weightSetList->GetCount();
	int i, j, k, weightCount = 0, weightSetIndex = 0;
	
	for( i=0; i<weightSetCount; i++ ){
		weightCount += infos.weightSetList->GetAt( i )->GetCount();
	}
	
	lodMesh.SetWeightGroupCount( weightGroupCount );
	lodMesh.SetWeightCount( weightCount );
	
	deModelWeight * const modelWeights = lodMesh.GetWeights();
	int * const modelWeightGroups = lodMesh.GetWeightGroups();
	weightCount = 0;
	weightSetIndex = 0;
	
	for( i=0; i<weightGroupCount; i++ ){
		const int tempCount = i + 1;
		
		modelWeightGroups[ i ] = 0;
		
		for( j=0; j<weightSetCount; j++ ){
			demdlWeightSet &weightSet = *infos.weightSetList->GetAt( j );
			
			if( weightSet.GetCount() == tempCount ){
				for( k=0; k<tempCount; k++ ){
					modelWeights[ weightCount + k ].SetBone( weightSet.GetBoneAt( k ) );
					modelWeights[ weightCount + k ].SetWeight( weightSet.GetWeightAt( k ) );
				}
				
				weightSet.SetGroupedIndex( weightSetIndex );
				
				weightSetIndex++;
				weightCount += tempCount;
				modelWeightGroups[ i ]++;
			}
		}
	}
}

void deModelModule::pLoadVertices( decBaseFileReader &reader, deModel &model, sModelInfos &infos, deModelLOD &lodMesh ){
	const int indexOffset = ( infos.version >= 3 ) ? -1 : 0; // hack until format is final
	deModelVertex * const vertices = lodMesh.GetVertices();
	int v, weights;
	
	for( v=0; v<infos.vertexCount; v++ ){
		deModelVertex &vertex = vertices[ v ];
		
		if( infos.isLargeModel ){
			weights = reader.ReadInt() + indexOffset;
			
		}else{
			weights = ( int )reader.ReadUShort() + indexOffset;
		}
		
		if( infos.weightsCount == 0 ){
			vertex.SetWeightSet( -1 );
			
		}else{
			if( weights >= infos.weightsCount ){
				DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
			}
			
			if( weights == -1 ){
				vertex.SetWeightSet( -1 );
				
			}else{
				vertex.SetWeightSet( infos.weightSetList->GetAt( weights )->GetGroupedIndex() );
			}
		}
		
		vertex.SetPosition( reader.ReadVector() );
	}
}

void deModelModule::pLoadTexCoords( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh ){
	int i, j;
	
	for( i=0; i<infos.texCoordSetCount; i++ ){
		deModelTextureCoordinatesSet &tcset = lodMesh.GetTextureCoordinatesSetAt( i );
		int count;
		
		if( infos.isLargeModel ){
			count = reader.ReadInt();
			
		}else{
			count = reader.ReadUShort();
		}
		
		if( i == 0 ){
			lodMesh.SetTextureCoordinatesCount( count );
		}
		tcset.SetTextureCoordinatesCount( count );
		
		decVector2 * const texCoords = tcset.GetTextureCoordinates();
		for( j=0; j<count; j++ ){
			texCoords[ j ] = reader.ReadVector2();
		}
	}
}

void deModelModule::pLoadTrianglesOld( decBaseFileReader &reader, deModel &model,
sModelInfos &infos, deModelLOD &lodMesh ){
	deModelFace * const faces = lodMesh.GetFaces();
	int i, tcs, index;
	
	for( i=0; i<infos.triangleCount; i++ ){
		deModelFace &face = faces[ i ];
		
		// texture
		index = reader.ReadUShort();
		if( index >= infos.textureCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTexture( index );
		
		// vertices
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetVertex1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetVertex2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetVertex3( index );
		
		// normals
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetNormal1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetNormal2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetNormal3( index );
		
		// tangents
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTangent1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTangent2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTangent3( index );
		
		// texture coordinates
		for( tcs=0; tcs<infos.texCoordSetCount; tcs++ ){
			infos.texCoordSorter->SetFaceTexCoordAt( i, 0, tcs, reader.ReadVector2() );
			infos.texCoordSorter->SetFaceTexCoordAt( i, 1, tcs, reader.ReadVector2() );
			infos.texCoordSorter->SetFaceTexCoordAt( i, 2, tcs, reader.ReadVector2() );
		}
	}
}

void deModelModule::pLoadQuadsOld( decBaseFileReader &reader, deModel &model, sModelInfos &infos, deModelLOD &lodMesh ){
	deModelFace * const faces = lodMesh.GetFaces();
	decVector2 texCoord;
	int i, tcs, index;
	
	for( i=0; i<infos.quadCount; i++ ){
		deModelFace &face1 = faces[ infos.triangleCount + i * 2 ];
		deModelFace &face2 = faces[ infos.triangleCount + i * 2 + 1 ];
		
		// texture
		index = reader.ReadUShort();
		if( index >= infos.textureCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTexture( index );
		face2.SetTexture( index );
		
		// vertices
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex1( index );
		face2.SetVertex1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex3( index );
		face2.SetVertex2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetVertex3( index );
		
		// normals
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal1( index );
		face2.SetNormal1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal3( index );
		face2.SetNormal2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetNormal3( index );
		
		// tangents
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTangent1( index );
		face2.SetTangent1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTangent2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTangent3( index );
		face2.SetTangent2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetTangent3( index );
		
		// texture coordinates
		index = infos.triangleCount + i * 2;
		
		for( tcs=0; tcs<infos.texCoordSetCount; tcs++ ){
			texCoord = reader.ReadVector2();
			infos.texCoordSorter->SetFaceTexCoordAt( index, 0, tcs, texCoord );
			infos.texCoordSorter->SetFaceTexCoordAt( index + 1, 0, tcs, texCoord );
			
			texCoord = reader.ReadVector2();
			infos.texCoordSorter->SetFaceTexCoordAt( index, 1, tcs, texCoord );
			
			texCoord = reader.ReadVector2();
			infos.texCoordSorter->SetFaceTexCoordAt( index, 2, tcs, texCoord );
			infos.texCoordSorter->SetFaceTexCoordAt( index + 1, 1, tcs, texCoord );
			
			texCoord = reader.ReadVector2();
			infos.texCoordSorter->SetFaceTexCoordAt( index + 1, 2, tcs, texCoord );
		}
	}
}

void deModelModule::pLoadTriangles( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh ){
	deModelFace * const faces = lodMesh.GetFaces();
	int i, tcs, index;
	
	for( i=0; i<infos.triangleCount; i++ ){
		deModelFace &face = faces[ i ];
		
		// texture
		index = reader.ReadUShort();
		if( index >= infos.textureCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTexture( index );
		
		// vertices
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetVertex1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetVertex2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetVertex3( index );
		
		// normals
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetNormal1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetNormal2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetNormal3( index );
		
		// tangents
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTangent1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTangent2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face.SetTangent3( index );
		
		// texture coordinates
		for( tcs=0; tcs<infos.texCoordSetCount; tcs++ ){
			deModelTextureCoordinatesSet &tcset = lodMesh.GetTextureCoordinatesSetAt( tcs );
			
			// corner 1
			if( infos.isLargeModel ){
				index = reader.ReadInt();
			}else{
				index = reader.ReadUShort();
			}
			if( index >= tcset.GetTextureCoordinatesCount() ){
				DETHROW( deeInvalidFormat );
			}
			face.SetTextureCoordinates1( index );
			
			// corner 2
			if( infos.isLargeModel ){
				index = reader.ReadInt();
			}else{
				index = reader.ReadUShort();
			}
			if( index >= tcset.GetTextureCoordinatesCount() ){
				DETHROW( deeInvalidFormat );
			}
			face.SetTextureCoordinates2( index );
			
			// corner 3
			if( infos.isLargeModel ){
				index = reader.ReadInt();
			}else{
				index = reader.ReadUShort();
			}
			if( index >= tcset.GetTextureCoordinatesCount() ){
				DETHROW( deeInvalidFormat );
			}
			face.SetTextureCoordinates3( index );
		}
	}
}

void deModelModule::pLoadQuads( decBaseFileReader &reader, sModelInfos &infos, deModelLOD &lodMesh ){
	deModelFace * const faces = lodMesh.GetFaces();
	decVector2 texCoord;
	int i, tcs, index;
	
	for( i=0; i<infos.quadCount; i++ ){
		deModelFace &face1 = faces[ infos.triangleCount + i * 2 ];
		deModelFace &face2 = faces[ infos.triangleCount + i * 2 + 1 ];
		
		// texture
		index = reader.ReadUShort();
		if( index >= infos.textureCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTexture( index );
		face2.SetTexture( index );
		
		// vertices
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex1( index );
		face2.SetVertex1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetVertex3( index );
		face2.SetVertex2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.vertexCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetVertex3( index );
		
		// normals
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal1( index );
		face2.SetNormal1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetNormal3( index );
		face2.SetNormal2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.normalCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetNormal3( index );
		
		// tangents
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTangent1( index );
		face2.SetTangent1( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTangent2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face1.SetTangent3( index );
		face2.SetTangent2( index );
		
		if( infos.isLargeModel ){
			index = reader.ReadInt();
		}else{
			index = reader.ReadUShort();
		}
		if( index >= infos.tangentCount ){
			DETHROW( deeInvalidFormat );
		}
		face2.SetTangent3( index );
		
		// texture coordinates
		for( tcs=0; tcs<infos.texCoordSetCount; tcs++ ){
			deModelTextureCoordinatesSet &tcset = lodMesh.GetTextureCoordinatesSetAt( tcs );
			
			// corner 1
			if( infos.isLargeModel ){
				index = reader.ReadInt();
			}else{
				index = reader.ReadUShort();
			}
			if( index >= tcset.GetTextureCoordinatesCount() ){
				DETHROW( deeInvalidFormat );
			}
			face1.SetTextureCoordinates1( index );
			face2.SetTextureCoordinates1( index );
			
			// corner 2
			if( infos.isLargeModel ){
				index = reader.ReadInt();
			}else{
				index = reader.ReadUShort();
			}
			if( index >= tcset.GetTextureCoordinatesCount() ){
				DETHROW( deeInvalidFormat );
			}
			face1.SetTextureCoordinates2( index );
			
			// corner 3
			if( infos.isLargeModel ){
				index = reader.ReadInt();
			}else{
				index = reader.ReadUShort();
			}
			if( index >= tcset.GetTextureCoordinatesCount() ){
				DETHROW( deeInvalidFormat );
			}
			face1.SetTextureCoordinates3( index );
			face2.SetTextureCoordinates2( index );
			
			// corner 4
			if( infos.isLargeModel ){
				index = reader.ReadInt();
			}else{
				index = reader.ReadUShort();
			}
			if( index >= tcset.GetTextureCoordinatesCount() ){
				DETHROW( deeInvalidFormat );
			}
			face2.SetTextureCoordinates3( index );
		}
	}
}


void deModelModule::pUpdateFaceTexCoordIndices( deModel &model, sModelInfos &infos, deModelLOD &lodMesh ){
	// check if there exists a cached copy of the data
	decBaseFileReader *cacheReader = NULL;
	deVirtualFileSystem &vfs = *model.GetVirtualFileSystem();
	bool hasCached = false;
	decPath cachePath;
	decString cacheID;
	int i;
	
	cachePath.SetFromUnix( model.GetFilename() );
	
	cacheID = model.GetFilename();
	for( i=0; i<model.GetLODCount(); i++ ){
		if( model.GetLODAt( i ) == &lodMesh ){
			cacheID.AppendFormat( ";%i", i );
			break;
		}
	}
	
	pCacheMutex.Lock();
	#ifdef OS_ANDROID
	LogInfoFormat( "TCSorting '%s' (faces=%d sets=%d)", infos.filename,
		infos.texCoordSorter->GetFaceCount(), infos.texCoordSorter->GetTexCoordSetCount() );
	decTimer timer;
	#endif
	
	try{
		cacheReader = pCacheTCSorter->Read( cacheID );
		
		if( cacheReader ){
			// check the cache version in case we upgraded
			if( cacheReader->ReadByte() != 0x1 ){
				// cache file outdated
				cacheReader->FreeReference();
				cacheReader = NULL;
				pCacheTCSorter->Delete( cacheID );
				//LogInfoFormat( "TCSorter: '%s' cache ignored due to different version", cacheID.GetString() );
			}
			
		//}else{
		//	LogInfoFormat( "TCSorter: '%s' cache not found", cacheID.GetString() );
		}
		
		if( cacheReader ){
			// check if the cache is still valid
			const TIME_SYSTEM checkTime = ( TIME_SYSTEM )cacheReader->ReadUInt();
			
			if( vfs.GetFileModificationTime( cachePath ) != checkTime ){
				// cache file outdated
				cacheReader->FreeReference();
				cacheReader = NULL;
				pCacheTCSorter->Delete( cacheID );
				//LogInfoFormat( "TCSorter: '%s' cache ignored due to different mod-time", cacheID.GetString() );
			}
		}
		
		if( cacheReader ){
			// read cached data
			const int texCoordSetCount = lodMesh.GetTextureCoordinatesSetCount();
			int j, count;
			
			count = cacheReader->ReadInt();
			lodMesh.SetTextureCoordinatesCount( count );
			for( i=0; i<texCoordSetCount; i++ ){
				lodMesh.GetTextureCoordinatesSetAt( i ).SetTextureCoordinatesCount( count );
			}
			
			for( i=0; i<texCoordSetCount; i++ ){
				deModelTextureCoordinatesSet &tcset = lodMesh.GetTextureCoordinatesSetAt( i );
				
				for( j=0; j<count; j++ ){
					tcset.SetTextureCoordinatesAt( j, cacheReader->ReadVector2() );
				}
			}
			
			count = cacheReader->ReadInt();
			for( i=0; i<count; i++ ){
				deModelFace &face = lodMesh.GetFaceAt( i );
				
				face.SetTextureCoordinates1( cacheReader->ReadInt() );
				face.SetTextureCoordinates2( cacheReader->ReadInt() );
				face.SetTextureCoordinates3( cacheReader->ReadInt() );
			}
			
			// done
			cacheReader->FreeReference();
			hasCached = true;
			#ifdef OS_ANDROID
			LogInfoFormat( "TCSorting.LoadCache '%s' (tcs=%d) in %dms", infos.filename,
				infos.texCoordSorter->GetTexCoordCount(), ( int )( timer.GetElapsedTime() * 1e3f ) );
			#endif
		}
		
		pCacheMutex.Unlock();
		
	}catch( const deException & ){
		if( cacheReader ){
			cacheReader->FreeReference();
		}
		pCacheTCSorter->Delete( cacheID );
		pCacheMutex.Unlock();
		LogInfoFormat( "Cache file '%s' damaged, discarding", cacheID.GetString() );
		hasCached = false;
	}
	
	// update tex-coord sorting if not cached
	if( hasCached ){
		return;
	}
	
	demdlTexCoordSorter &texCoordSorter = *infos.texCoordSorter;
	const int texCoordSetCount = lodMesh.GetTextureCoordinatesSetCount();
	int j, count;
	
	// sort texture coordinates and assign the indices
	texCoordSorter.Sort();
#ifdef OS_ANDROID
	LogInfoFormat( "TCSorting.Sorted '%s' in %dms", infos.filename, ( int )( timer.GetElapsedTime() * 1e3f ) );
#endif
	
	count = texCoordSorter.GetTexCoordCount();
	lodMesh.SetTextureCoordinatesCount( count );
	for( i=0; i<texCoordSetCount; i++ ){
		lodMesh.GetTextureCoordinatesSetAt( i ).SetTextureCoordinatesCount( count );
	}
	
	for( i=0; i<texCoordSetCount; i++ ){
		deModelTextureCoordinatesSet &tcset = lodMesh.GetTextureCoordinatesSetAt( i );
		
		for( j=0; j<count; j++ ){
			tcset.SetTextureCoordinatesAt( j, texCoordSorter.GetTexCoordAt( j, i ) );
		}
	}
	
	count = texCoordSorter.GetFaceCount();
	for( i=0; i<count; i++ ){
		deModelFace &face = lodMesh.GetFaceAt( i );
		
		face.SetTextureCoordinates1( texCoordSorter.GetFaceCornerAt( i, 0 ) );
		face.SetTextureCoordinates2( texCoordSorter.GetFaceCornerAt( i, 1 ) );
		face.SetTextureCoordinates3( texCoordSorter.GetFaceCornerAt( i, 2 ) );
	}
#ifdef OS_ANDROID
	LogInfoFormat( "TCSorting.Sorted '%s' (tcs=%d) in %dms", infos.filename,
		texCoordSorter.GetTexCoordCount(), ( int )( timer.GetElapsedTime() * 1e3f ) );
#endif
	
	// cache the result
	decBaseFileWriter *cacheWriter = NULL;
	pCacheMutex.Lock();
	
	try{
		cacheWriter = pCacheTCSorter->Write( cacheID );
		
		// write cache version
		cacheWriter->WriteByte( 0x1 );
		
		// write file modification time to reject the cached file if the source model changed
		cacheWriter->WriteUInt( ( unsigned int )vfs.GetFileModificationTime( cachePath ) );
		
		// write data
		const int texCoordSetCount = lodMesh.GetTextureCoordinatesSetCount();
		int i, j, count;
		
		count = texCoordSorter.GetTexCoordCount();
		cacheWriter->WriteInt( count );
		for( i=0; i<texCoordSetCount; i++ ){
			for( j=0; j<count; j++ ){
				cacheWriter->WriteVector2( texCoordSorter.GetTexCoordAt( j, i ) );
			}
		}
		
		count = texCoordSorter.GetFaceCount();
		cacheWriter->WriteInt( count );
		for( i=0; i<count; i++ ){
			cacheWriter->WriteInt( texCoordSorter.GetFaceCornerAt( i, 0 ) );
			cacheWriter->WriteInt( texCoordSorter.GetFaceCornerAt( i, 1 ) );
			cacheWriter->WriteInt( texCoordSorter.GetFaceCornerAt( i, 2 ) );
		}
		
		// done
		cacheWriter->FreeReference();
		cacheWriter = NULL;
		
		//LogInfoFormat( "TCSorter: '%s' written to cache", cacheID.GetString() );
		
		pCacheMutex.Unlock();
		
	}catch( const deException &e ){
		if( cacheWriter ){
			cacheWriter->FreeReference();
		}
		pCacheTCSorter->Delete( cacheID );
		pCacheMutex.Unlock();
		LogInfoFormat( "Writing cache file '%s' failed, deleted", cacheID.GetString() );
		LogException( e );
		throw;
	}
#ifdef OS_ANDROID
	LogInfoFormat( "TCSorting.WriteCache '%s' in %dms", infos.filename, ( int )( timer.GetElapsedTime() * 1e3f ) );
#endif
}
