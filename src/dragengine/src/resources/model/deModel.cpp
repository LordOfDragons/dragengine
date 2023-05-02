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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deModel.h"
#include "deModelBone.h"
#include "deModelTexture.h"
#include "deModelLOD.h"
#include "deModelVertex.h"
#include "deModelFace.h"
#include "deModelManager.h"
#include "deModelWeight.h"
#include "deModelTextureCoordinatesSet.h"
#include "deModelVertexPositionSet.h"
#include "deModelLodVertexPositionSet.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/audio/deBaseAudioModel.h"
#include "../../systems/modules/graphic/deBaseGraphicModel.h"
#include "../../systems/modules/physics/deBasePhysicsModel.h"



// Class deModel
//////////////////

// Constructor, destructor
////////////////////////////

deModel::deModel( deModelManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),

pBones( NULL ),
pBoneCount( 0 ),
pBoneSize( 0 ),

pTextures( NULL ),
pTextureCount( 0 ),
pTextureSize( 0 ),

pLODs( NULL ),
pLODCount( 0 ),
pLODSize( 0 ),

pVertexPositionSets( nullptr ),
pVertexPositionSetCount( 0 ),
pVertexPositionSetSize( 0 ),

pPeerGraphic ( NULL ),
pPeerPhysics ( NULL ),
pPeerAudio ( NULL ){
}

deModel::~deModel(){
	if( pPeerAudio ){
		delete pPeerAudio;
		pPeerAudio = NULL;
	}
	if( pPeerPhysics ){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pLODs ){
		while( pLODCount > 0 ){
			pLODCount--;
			delete pLODs[ pLODCount ];
		}
		delete [] pLODs;
	}
	
	if( pTextures ){
		while( pTextureCount > 0 ){
			pTextureCount--;
			delete pTextures[ pTextureCount ];
		}
		delete [] pTextures;
	}
	
	if( pVertexPositionSets ){
		while( pVertexPositionSetCount > 0 ){
			pVertexPositionSetCount--;
			delete pVertexPositionSets[ pVertexPositionSetCount ];
		}
		delete [] pVertexPositionSets;
	}
	
	if( pBones ){
		while( pBoneCount > 0 ){
			pBoneCount--;
			delete pBones[ pBoneCount ];
		}
		delete [] pBones;
	}
}



// Management
///////////////

bool deModel::Verify(){
	bool *visited = nullptr;
	int i, j, parent;
	bool success = true;
	
	// verify there is at least one LOD mesh
	if( pLODCount < 1 ){
		return false;
	}
	
	// verify that all bones are free of cyclic parentship
	if( pBoneCount > 0 ){
		try{
			visited = new bool[ pBoneCount ];
			
			// check bone parents are valid
			for( i=0; i<pBoneCount; i++ ){
				for( j=0; j<pBoneCount; j++ ){
					visited[ j ] = false;
				}
				
				parent = pBones[ i ]->GetParent();
				while( parent != -1 ){
					if( parent < -1 || parent >= pBoneCount || visited[ parent ] ){
						success = false;
						break;
					}
					visited[ parent ] = true;
					parent = pBones[ parent ]->GetParent();
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
	
	// verify that each lod level has the correct number of texture coordinates sets and texture coordinates per set
	const int texCoordSetCount = pTextureCoordinatesSetList.GetCount();
	
	if( texCoordSetCount < 1 ){
		return false;
	}
	
	for( i=0; i<pLODCount; i++ ){
		const int texCoordCount = pLODs[ i ]->GetTextureCoordinatesCount();
		
		if( pLODs[ i ]->GetTextureCoordinatesSetCount() != texCoordSetCount ){
			return false;
		}
		
		for( j=0; j<texCoordSetCount; j++ ){
			if( pLODs[ i ]->GetTextureCoordinatesSetAt( j ).GetTextureCoordinatesCount() != texCoordCount ){
				return false;
			}
		}
	}
	
	// verify each lod level has the correct count of vertex position sets
	for( i=0; i<pLODCount; i++ ){
		if( pLODs[ i ]->GetVertexPositionSetCount() != pVertexPositionSetCount ){
			return false;
		}
	}
	
	// finished
	return success;
}

void deModel::Prepare(){
	pCalcBoneMatrices();
}



// Bones
//////////

int deModel::IndexOfBoneNamed( const char *name ) const{
	int i;
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ]->GetName() == name ){
			return i;
		}
	}
	return -1;
}

bool deModel::HasBoneNamed( const char *name ) const{
	int i;
	for( i=0; i<pBoneCount; i++ ){
		if( pBones[ i ]->GetName() == name ){
			return true;
		}
	}
	return false;
}

deModelBone *deModel::GetBoneAt( int index ) const{
	if( index < 0 || index >= pBoneCount ){
		DETHROW( deeInvalidParam );
	}
	return pBones[ index ];
}

void deModel::AddBone( deModelBone *bone ){
	if( ! bone ){
		DETHROW( deeInvalidParam );
	}
	
	if( pBoneCount == pBoneSize ){
		int i, newSize = pBoneSize * 3 / 2 + 1;
		deModelBone **newArray = new deModelBone*[ newSize ];
		if( pBones ){
			for( i=0; i<pBoneCount; i++ ) newArray[ i ] = pBones[ i ];
			delete [] pBones;
		}
		pBones = newArray;
		pBoneSize = newSize;
	}
	
	pBones[ pBoneCount++ ] = bone;
}



// Textures
/////////////

int deModel::IndexOfTextureNamed( const char *name ) const{
	int i;
	for( i=0; i<pTextureCount; i++ ){
		if( pTextures[ i ]->GetName() == name ){
			return i;
		}
	}
	return -1;
}

bool deModel::HasTextureNamed( const char *name ) const{
	int i;
	for( i=0; i<pTextureCount; i++ ){
		if( pTextures[ i ]->GetName() == name ){
			return true;
		}
	}
	return false;
}

deModelTexture *deModel::GetTextureAt( int index ) const{
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	return pTextures[ index ];
}

void deModel::AddTexture( deModelTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTextureCount == pTextureSize ){
		int i, newSize = pTextureSize * 3 / 2 + 1;
		deModelTexture **newArray = new deModelTexture*[ newSize ];
		if( pTextures ){
			for( i=0; i<pTextureCount; i++ ) newArray[ i ] = pTextures[ i ];
			delete [] pTextures;
		}
		pTextures = newArray;
		pTextureSize = newSize;
	}
	
	pTextures[ pTextureCount++ ] = texture;
}



// LODs
/////////

deModelLOD *deModel::GetLODAt( int index ) const{
	if( index < 0 || index >= pLODCount ){
		DETHROW( deeInvalidParam );
	}
	return pLODs[ index ];
}

void deModel::AddLOD( deModelLOD *lod ){
	if( ! lod ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLODCount == pLODSize ){
		int i, newSize = pLODSize * 3 / 2 + 1;
		deModelLOD **newArray = new deModelLOD*[ newSize ];
		if( pLODs ){
			for( i=0; i<pLODCount; i++ ) newArray[ i ] = pLODs[ i ];
			delete [] pLODs;
		}
		pLODs = newArray;
		pLODSize = newSize;
	}
	
	pLODs[ pLODCount++ ] = lod;
}



// Vertex position sets
/////////////////////////

int deModel::IndexOfVertexPositionSetNamed( const char *name ) const{
	int i;
	for( i=0; i<pVertexPositionSetCount; i++ ){
		if( pVertexPositionSets[ i ]->GetName() == name ){
			return i;
		}
	}
	return -1;
}

bool deModel::HasVertexPositionSetNamed( const char *name ) const{
	int i;
	for( i=0; i<pVertexPositionSetCount; i++ ){
		if( pVertexPositionSets[ i ]->GetName() == name ){
			return true;
		}
	}
	return false;
}

deModelVertexPositionSet *deModel::GetVertexPositionSetAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pVertexPositionSetCount )
	
	return pVertexPositionSets[ index ];
}

void deModel::AddVertexPositionSet( deModelVertexPositionSet *set ){
	DEASSERT_NOTNULL( set )
	
	if( pVertexPositionSetCount == pVertexPositionSetSize ){
		const int newSize = pVertexPositionSetSize * 3 / 2 + 1;
		int i;
		
		deModelVertexPositionSet ** const newArray = new deModelVertexPositionSet*[ newSize ];
		if( pVertexPositionSets ){
			for( i=0; i<pVertexPositionSetCount; i++ ){
				newArray[ i ] = pVertexPositionSets[ i ];
			}
			delete [] pVertexPositionSets;
		}
		pVertexPositionSets = newArray;
		pVertexPositionSetSize = newSize;
	}
	
	pVertexPositionSets[ pVertexPositionSetCount++ ] = set;
}



// System Peers
/////////////////

void deModel::SetPeerGraphic( deBaseGraphicModel *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	deBaseGraphicModel * const deletePeer = pPeerGraphic;
	
	pPeerGraphic = peer;
	
	if( deletePeer ){
		delete deletePeer;
	}
}

void deModel::SetPeerAudio ( deBaseAudioModel *peer ){
	if( peer == pPeerAudio ){
		return;
	}
	
	deBaseAudioModel * const deletePeer = pPeerAudio;
	
	pPeerAudio = peer;
	
	if( deletePeer ){
		delete deletePeer;
	}
}

void deModel::SetPeerPhysics( deBasePhysicsModel *peer ){
	if( peer == pPeerPhysics ){
		return;
	}
	
	deBasePhysicsModel * const deletePeer = pPeerPhysics;
	
	pPeerPhysics = peer;
	
	if( deletePeer ){
		delete deletePeer;
	}
}



// Private Functions
//////////////////////

void deModel::pCalcBoneMatrices(){
	if( pBoneCount == 0 ){
		return;
	}
	
	bool *calculated = NULL;
	int i;
	
	try{
		// create a temporary array to hold the calculated status
		calculated = new bool[ pBoneCount ];
		for( i=0; i<pBoneCount; i++ ){
			calculated[ i ] = false;
		}
		
		// keeps track of the count of bones in need of calculation
		int remaining = pBoneCount;
		
		// loop until there are no more remaining bones
		while( remaining > 0 ){
			// loop over all bones and calculate those not done so yet
			for( i=0; i<pBoneCount; i++ ){
				// if calculated skip the bone
				if( calculated[ i ] ){
					continue;
				}
				deModelBone &bone = *pBones[ i ];
				
				// check if the parent if present is calculated
				const int parent = bone.GetParent();
				if( parent != -1 && ! calculated[ parent ] ){
					continue;
				}
				
				// calculate the matrix
				decMatrix matrix( decMatrix::CreateWorld( bone.GetPosition(), bone.GetOrientation() ) );
				if( parent != -1 ){
					matrix *= pBones[ parent ]->GetMatrix();
				}
				bone.SetMatrix( matrix );
				
				// mark as calculate and decrease count of remaining bones by one
				calculated[ i ] = true;
				remaining--;
			}
		}
		
		// free temporary array
		delete [] calculated;
		
	}catch( const deException & ){
		if( calculated ){
			delete [] calculated;
		}
		throw;
	}
}
