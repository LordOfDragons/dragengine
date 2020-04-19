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

#include "deOcclusionMesh.h"
#include "deOcclusionMeshManager.h"
#include "deOcclusionMeshBone.h"
#include "deOcclusionMeshVertex.h"
#include "deOcclusionMeshWeight.h"
#include "../../systems/modules/graphic/deBaseGraphicOcclusionMesh.h"
#include "../../common/exceptions.h"



// Class deOcclusionMesh
//////////////////////////

// Constructor, destructor
////////////////////////////

deOcclusionMesh::deOcclusionMesh( deOcclusionMeshManager *manager, deVirtualFileSystem *vfs,
	const char *filename, TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),

pBones( NULL ),
pWeights( NULL ),
pWeightGroups( NULL ),
pVertices( NULL ),
pCorners( NULL ),
pFaces( NULL ),

pBoneCount( 0 ),
pWeightCount( 0 ),
pWeightGroupCount( 0 ),
pVertexCount( 0 ),
pCornerCount( 0 ),
pFaceCount( 0 ),
pDoubleSidedFaceCount( 0 ),

pPeerGraphic ( NULL ){
}

deOcclusionMesh::~deOcclusionMesh(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pFaces ){
		delete [] pFaces;
	}
	if( pCorners ){
		delete [] pCorners;
	}
	if( pVertices ){
		delete [] pVertices;
	}
	if( pWeightGroups ){
		delete [] pWeightGroups;
	}
	if( pWeights ){
		delete [] pWeights;
	}
	if( pBones ){
		delete [] pBones;
	}
}



// Management
///////////////

bool deOcclusionMesh::Verify() const{
	bool *visited = NULL;
	int i, j, parent;
	bool success = true;
	
	// verify that all bones are free of cyclic parentship
	if( pBoneCount > 0 ){
		try{
			visited = new bool[ pBoneCount ];
			
			// check bone parents are valid
			for( i=0; i<pBoneCount; i++ ){
				for( j=0; j<pBoneCount; j++ ){
					visited[ j ] = false;
				}
				
				parent = pBones[ i ].GetParent();
				while( parent != -1 ){
					if( parent < -1 || parent >= pBoneCount || visited[ parent ] ){
						success = false;
						break;
					}
					visited[ parent ] = true;
					parent = pBones[ parent ].GetParent();
				}
				
				if( ! success ){
					break;
				}
			}
			
			// clean up
			delete [] visited;
			
		}catch( const deException & ){
			if( visited ){
				delete [] visited;
			}
			throw;
		}
	}
	
	// finished
	return success;
}

void deOcclusionMesh::Prepare(){
}



// Bones
//////////

void deOcclusionMesh::SetBoneCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pBoneCount ){
		if( pBones ){
			delete [] pBones;
		}
		pBones = NULL;
		pBoneCount = 0;
		
		if( count > 0 ){
			pBones = new deOcclusionMeshBone[ count ];
			if( ! pBones ){
				DETHROW( deeOutOfMemory );
			}
			pBoneCount = count;
		}
	}
}

deOcclusionMeshBone &deOcclusionMesh::GetBoneAt( int index ) const{
	if( index < 0 || index >= pBoneCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pBones[ index ];
}

int deOcclusionMesh::IndexOfBoneNamed( const char *name ) const{
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ].GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool deOcclusionMesh::HasBoneNamed( const char *name ) const{
	int i;
	
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ].GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}



// Weight Sets
////////////////

void deOcclusionMesh::SetWeightCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pWeightCount ){
		if( pWeights ){
			delete [] pWeights;
		}
		pWeights = NULL;
		pWeightCount = 0;
		
		if( count > 0 ){
			pWeights = new deOcclusionMeshWeight[ count ];
			pWeightCount = count;
		}
	}
}

deOcclusionMeshWeight &deOcclusionMesh::GetWeightAt( int index ) const{
	if( index < 0 || index >= pWeightCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pWeights[ index ];
}

void deOcclusionMesh::SetWeightGroupCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pWeightGroupCount ){
		if( pWeightGroups ){
			delete [] pWeightGroups;
		}
		pWeightGroups = NULL;
		pWeightGroupCount = 0;
		
		if( count > 0 ){
			pWeightGroups = new int[ count ];
			pWeightGroupCount = count;
		}
	}
}

int deOcclusionMesh::GetWeightGroupAt( int index ) const{
	if( index < 0 || index >= pWeightGroupCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pWeightGroups[ index ];
}

void deOcclusionMesh::SetWeightGroupAt( int index, int weightSetCount ) const{
	if( index < 0 || index >= pWeightGroupCount || weightSetCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pWeightGroups[ index ] = weightSetCount;
}



// Vertices
/////////////

void deOcclusionMesh::SetVertexCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pVertexCount ){
		if( pVertices ){
			delete [] pVertices;
		}
		pVertices = NULL;
		pVertexCount = 0;
		
		if( count > 0 ){
			pVertices = new deOcclusionMeshVertex[ count ];
			if( ! pVertices ){
				DETHROW( deeOutOfMemory );
			}
			pVertexCount = count;
		}
	}
}

deOcclusionMeshVertex &deOcclusionMesh::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pVertices[ index ];
}



// Corners
////////////

void deOcclusionMesh::SetCornerCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pCornerCount ){
		if( pCorners ){
			delete [] pCorners;
		}
		pCorners = NULL;
		pCornerCount = 0;
		
		if( count > 0 ){
			pCorners = new unsigned short[ count ];
			if( ! pCorners ){
				DETHROW( deeOutOfMemory );
			}
			pCornerCount = count;
		}
	}
}

unsigned short deOcclusionMesh::GetCornerAt( int index ) const{
	if( index < 0 || index >= pCornerCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pCorners[ index ];
}

void deOcclusionMesh::SetCornerAt( int index, unsigned short vertexIndex ){
	if( index < 0 || index >= pCornerCount ){
		DETHROW( deeInvalidParam );
	}
	
	pCorners[ index ] = vertexIndex;
}



// Faces
//////////

void deOcclusionMesh::SetFaceCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pFaceCount ){
		if( pFaces ){
			delete [] pFaces;
		}
		pFaces = NULL;
		pFaceCount = 0;
		
		if( count > 0 ){
			pFaces = new unsigned short[ count ];
			if( ! pFaces ){
				DETHROW( deeOutOfMemory );
			}
			pFaceCount = count;
		}
	}
}

unsigned short deOcclusionMesh::GetFaceAt( int index ) const{
	if( index < 0 || index >= pFaceCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFaces[ index ];
}

void deOcclusionMesh::SetFaceAt( int index, unsigned short cornerCount ){
	if( index < 0 || index >= pFaceCount ){
		DETHROW( deeInvalidParam );
	}
	
	pFaces[ index ] = cornerCount;
}

void deOcclusionMesh::SetDoubleSidedFaceCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pDoubleSidedFaceCount = count;
}



// System Peers
/////////////////

void deOcclusionMesh::SetPeerGraphic( deBaseGraphicOcclusionMesh *peer ){
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
}
