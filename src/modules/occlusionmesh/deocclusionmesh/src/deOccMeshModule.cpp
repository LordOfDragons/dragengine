/* 
 * Drag[en]gine Occlusion Mesh Module
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

#include "deOccMeshModule.h"
#include "deoccmWeightSet.h"
#include "deoccmWeightSetList.h"

#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshBone.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshVertex.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshWeight.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/exceptions.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *DEOccMeshCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *DEOccMeshCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deOccMeshModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Definitions
////////////////



// Structures
///////////////

struct sWeight{
	int bone;
	float weight;
};



// Class deOccMeshModule
//////////////////////////

// Constructor, destructor
////////////////////////////

deOccMeshModule::deOccMeshModule( deLoadableModule &loadableModule ) :
deBaseOcclusionMeshModule( loadableModule ){
}

deOccMeshModule::~deOccMeshModule(){
}



// Loading and Saving
///////////////////////

void deOccMeshModule::LoadOcclusionMesh( decBaseFileReader &file, deOcclusionMesh &occmesh ){
	pLoadMesh( file, occmesh );
}

void deOccMeshModule::SaveOcclusionMesh( decBaseFileWriter &file, const deOcclusionMesh &occmesh ){
	DETHROW( deeInvalidAction );
}

bool deOccMeshModule::IsCompressed( decBaseFileReader& ){
	return false;
}



// Private Functions
//////////////////////

void deOccMeshModule::pLoadMesh( decBaseFileReader &reader, deOcclusionMesh &mesh ){
	const char *signature = "Drag[en]gine Occlusion Mesh";
	char sigbuf[ 27 ];
	sMeshInfos infos;
	
	// check signature
	reader.Read( &sigbuf, 27 );
	if( strncmp( sigbuf, signature, 27 ) != 0 ){
		DETHROW( deeInvalidFormat );
	}
	
	// read infos
	infos.version = ( int )reader.ReadUShort();
	infos.flags = ( int )reader.ReadUShort();
	infos.boneCount = ( int )reader.ReadUShort();
	infos.weightSetList = NULL;
	
	try{
		infos.weightSetList = new deoccmWeightSetList;
		
		if( infos.version == 1 ){
			infos.weightsCount = ( int )reader.ReadUShort();
			infos.vertexCount = ( int )reader.ReadUShort();
			infos.cornerCount = ( int )reader.ReadUShort();
			infos.faceCount = ( int )reader.ReadUShort();
			infos.doubleSidedFaceCount = ( int )reader.ReadUShort();
			
			mesh.SetBoneCount( infos.boneCount );
			mesh.SetVertexCount( infos.vertexCount );
			mesh.SetCornerCount( infos.cornerCount );
			mesh.SetFaceCount( infos.faceCount );
			mesh.SetDoubleSidedFaceCount( infos.doubleSidedFaceCount );
			
			pLoadBones( reader, mesh, infos );
			pLoadWeights( reader, mesh, infos );
			pLoadVertices( reader, mesh, infos );
			pLoadFaces( reader, mesh, infos );
		}
		
		delete infos.weightSetList;
		
	}catch( const deException & ){
		if( infos.weightSetList ){
			delete infos.weightSetList;
		}
		
		throw;
	}
}

void deOccMeshModule::pLoadBones( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos ){
	decString name;
	int b, parent;
	
	for( b=0; b<infos.boneCount; b++ ){
		deOcclusionMeshBone &bone = mesh.GetBoneAt( b );
		
		reader.ReadString8Into( name );
		if( mesh.HasBoneNamed( name.GetString() ) ){
			DETHROW( deeInvalidFormat );
		}
		
		bone.SetName( name.GetString() );
		bone.SetPosition( reader.ReadVector() );
		bone.SetOrientation( reader.ReadQuaternion() );
		
		parent = ( int )( reader.ReadUShort() ) - 1;
		if( parent >= infos.boneCount || parent == b ){
			DETHROW( deeInvalidFormat );
		}
		bone.SetParent( parent );
	}
}

void deOccMeshModule::pLoadWeights( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos ){
	deoccmWeightSet *weightSet = NULL;
	int w, b, boneCount;
	float factor;
	int bone;
	
	try{
		for( w=0; w<infos.weightsCount; w++ ){
			weightSet = new deoccmWeightSet;
			
			boneCount = ( int )reader.ReadByte();
			for( b=0; b<boneCount; b++ ){
				bone = ( int )reader.ReadUShort();
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
	
	mesh.SetWeightGroupCount( weightGroupCount );
	mesh.SetWeightCount( weightCount );
	
	deOcclusionMeshWeight * const meshWeights = mesh.GetWeights();
	int * const meshWeightGroups = mesh.GetWeightGroups();
	weightCount = 0;
	weightSetIndex = 0;
	
	for( i=0; i<weightGroupCount; i++ ){
		const int tempCount = i + 1;
		
		meshWeightGroups[ i ] = 0;
		
		for( j=0; j<weightSetCount; j++ ){
			deoccmWeightSet &weightSet = *infos.weightSetList->GetAt( j );
			
			if( weightSet.GetCount() == tempCount ){
				for( k=0; k<tempCount; k++ ){
					meshWeights[ weightCount + k ].SetBone( weightSet.GetBoneAt( k ) );
					meshWeights[ weightCount + k ].SetWeight( weightSet.GetWeightAt( k ) );
				}
				
				weightSet.SetGroupedIndex( weightSetIndex );
				
				weightSetIndex++;
				weightCount += tempCount;
				meshWeightGroups[ i ]++;
			}
		}
	}
}

void deOccMeshModule::pLoadVertices( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos ){
	deOcclusionMeshVertex * const vertices = mesh.GetVertices();
	int v, weights;
	
	for( v=0; v<infos.vertexCount; v++ ){
		deOcclusionMeshVertex &vertex = vertices[ v ];
		
		weights = ( int )reader.ReadUShort() - 1;
		if( weights >= infos.weightsCount ){
			DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
		}
		
		if( weights < 0 ){
			vertex.SetWeightSet( -1 );
			
		}else{
			vertex.SetWeightSet( infos.weightSetList->GetAt( weights )->GetGroupedIndex() );
		}
		
		vertex.SetPosition( reader.ReadVector() );
	}
}

void deOccMeshModule::pLoadFaces( decBaseFileReader &reader, deOcclusionMesh &mesh, sMeshInfos &infos ){
	unsigned short * const corners = mesh.GetCorners();
	unsigned short * const faces = mesh.GetFaces();
	int f, c, cornerCount;
	int cindex = 0;
	
	// NOTE: possible optimization could be to store corner vertex indices only as byte if the
	// mesh does contain at most 256 vertices. in general though occlusion mesh files are already
	// rather small to begin with
	
	for( f=0; f<infos.faceCount; f++ ){
		cornerCount = ( int )reader.ReadByte();
		
		for( c=0; c<cornerCount; c++ ){
			if( cindex == infos.cornerCount ){
				DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
			}
			corners[ cindex++ ] = reader.ReadUShort();
		}
		
		faces[ f ] = ( unsigned short )cornerCount;
	}
	
	if( cindex < infos.cornerCount ){
		DETHROW_INFO( deeInvalidFileFormat, reader.GetFilename() );
	}
}
