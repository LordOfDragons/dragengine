/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpHTSector.h"
#include "debpHeightTerrain.h"
#include "../../debpPhysicsBody.h"
#include "../../world/debpWorld.h"
#include "../../world/debpHeightTerrainShape.h"
#include "../../world/debpCollisionWorld.h"
#include "../../dePhysicsBullet.h"
#include "../../debpBulletShape.h"

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/common/exceptions.h>



// TODO the points array is temporary. since the height map contains all the
// informations the reading of values can be moved straight into the bullet
// height terrain shape. saves us a lot of memory we can use for better things



// Class debpHTSector
///////////////////////

// Constructor, destructor
////////////////////////////

debpHTSector::debpHTSector( debpHeightTerrain *heightTerrain, deHeightTerrainSector *sector ){
	if( ! heightTerrain || ! sector ) DETHROW( deeInvalidParam );
	
	pHeightTerrain = heightTerrain;
	pSector = sector;
	
	pMarked = false;
	
	pPoints = NULL;
	pDirtyPoints = false;
	
	pPhyBody = NULL;
	pShape = NULL;
	pBulletShape = NULL;
	
	pClusters = NULL;
	pClusterCount = 0;
	
	// init
	deHeightTerrain *engHT = heightTerrain->GetHeightTerrain();
	
	pImageDim = engHT->GetSectorResolution();
	
	float sectorDim = engHT->GetSectorSize();
	float halfSectorDim = sectorDim * 0.5f;
	float factor = sectorDim / ( float )pImageDim;
	
	const decPoint &scoord = pSector->GetSector();
	
	// speed notes ( ray-test )
	// 51: roughly 4ms with bad cases up to 13ms
	// 26: roughly 700ys with bad cases up to 8ms
	// 11: roughly 700ys with bad cases up to 3ms
	// 11 seems to be a good compromise. the smaller the clusters get the
	// more clusters are required. cluster boxes though can be themselves
	// placed in a quad-tree or another structure which should improve
	// performance even more
	//
	// NOTE the above measurements are from the old terrain class with
	// height terrains. in the new explicit height terrain class testing
	// can be done in a new way since the test location is easy to
	// determine
	
	//int maxPointsPerCluster = 11; //51;
	int maxPointsPerCluster = 11; //6; some deprecated methods could suffer => see deprecated on debpCollisionDetection::*HeightTerrain
	int clusterCount = ( ( pImageDim - 2 ) / ( maxPointsPerCluster - 1 ) ) + 1;
	int i, x, z, pcx, pcz, curx, curz;
	
	try{
		pMinExtend.x = ( double )sectorDim * ( ( double )scoord.x - 0.5 );
		pMinExtend.z = ( double )sectorDim * ( ( double )scoord.y - 0.5 );
		pMaxExtend.x = pMinExtend.x + ( double )sectorDim;
		pMaxExtend.z = pMinExtend.z + ( double )sectorDim;
		
		pClusters = new debpHTSCluster[ clusterCount * clusterCount ];
		if( ! pClusters ) DETHROW( deeOutOfMemory );
		
		pClusterCount = clusterCount;
		
		curz = 0;
		for( z=0; z<clusterCount; z++ ){
			if( z < clusterCount - 1 ){
				pcz = maxPointsPerCluster;
				
			}else{
				pcz = pImageDim - curz;
			}
			
			curx = 0;
			for( x=0; x<clusterCount; x++ ){
				if( x < clusterCount - 1 ){
					pcx = maxPointsPerCluster;
					
				}else{
					pcx = pImageDim - curx;
				}
				
				i = clusterCount * z + x;
				pClusters[ i ].firstPointX = curx;
				pClusters[ i ].firstPointZ = curz;
				pClusters[ i ].pointCountX = pcx;
				pClusters[ i ].pointCountZ = pcz;
				
				pClusters[ i ].center.x = ( ( ( float )curx + ( float )pcx * 0.5f ) * factor ) - halfSectorDim;
				pClusters[ i ].center.z = halfSectorDim - ( ( ( float )curz + ( float )pcz * 0.5f ) * factor );
				pClusters[ i ].halfExtends.x = ( ( float )pcx * 0.5f ) * factor;
				pClusters[ i ].halfExtends.z = ( ( float )pcz * 0.5f ) * factor;
				
				curx += maxPointsPerCluster - 1;
			}
			
			curz += maxPointsPerCluster - 1;
		}
		
		pCreatePoints();
		
		ParentWorldChanged();
		
	}catch( const deException &e ){
		e.PrintError();
		pCleanUp();
		throw;
	}
}

debpHTSector::~debpHTSector(){
	pCleanUp();
}



// Management
///////////////

debpHTSCluster &debpHTSector::GetClusterAt( int x, int z ) const{
	if( x < 0 || x >= pClusterCount || z < 0 || z >= pClusterCount ) DETHROW( deeInvalidParam );
	
	return pClusters[ pClusterCount * z + x ];
}

void debpHTSector::UpdateHeights( int fromX, int fromY, int toX, int toY ){
	pDirtyPoints = true;
}

void debpHTSector::Update(){
//	int d, decalCount = pTerrain->GetDecalCount();
//	debpDecal *bpDecal;
	
	if( pDirtyPoints ){
		pUpdatePoints();
		
		pDirtyPoints = false;
	}
	
/*	for( d=0; d<decalCount; d++ ){
		bpDecal = ( debpDecal* )pTerrain->GetDecalAt( d )->GetPhysicsDecal();
		
		bpDecal->Update();
	}*/
}

void debpHTSector::ParentWorldChanged(){
	debpWorld * const world = pHeightTerrain->GetParentWorld();
	
	if( world ){
		pPhyBody->SetDynamicsWorld( world->GetDynamicsWorld() );
		
	}else{
		pPhyBody->SetDynamicsWorld( NULL );
	}
}

void debpHTSector::SectorChanged(){
	// TODO the sector changes if height map changes or the texture changes.
	// only the height map change is interesting to us so to be safe we
	// should update the bullet shape as well as updating the heights
	
	pDirtyPoints = true;
}



// Private functions
//////////////////////

void debpHTSector::pCleanUp(){
	if( pPhyBody ) delete pPhyBody;
	
	if( pBulletShape ){
		pBulletShape->FreeReference();
	}
	
	if( pPoints ) delete [] pPoints;
	if( pClusters ) delete [] pClusters;
}



void debpHTSector::pCreatePoints(){
	float sectorDim = pHeightTerrain->GetHeightTerrain()->GetSectorSize();
	int i, x, z, count = pImageDim * pImageDim;
	float sizeStep = sectorDim / ( float )pImageDim;
	float sizeOffset = sectorDim * 0.5f;
	
	// create arrays
	if( count > 0 ){
		pPoints = new decVector[ count ];
		if( ! pPoints ) DETHROW( deeOutOfMemory );
	}
	
	// store the coordinates remaining the same
	i = 0;
	for( z=0; z<pImageDim; z++ ){
		for( x=0; x<pImageDim; x++ ){
			pPoints[ i ].x = ( float )( x ) * sizeStep - sizeOffset;
			pPoints[ i ].z = sizeOffset - ( float )( z ) * sizeStep;
			i++;
		}
	}
	
	// update the points so the heights are already initialized before
	// we create the height terrain shape. otherwise bullet totally
	// ignores this shape.
	pUpdatePoints();
	
	// create shape
	pShape = new debpHeightTerrainShape( this );
	pShape->setUserPointer( 0 ); // means no shape index
	
	pBulletShape = new debpBulletShape( pShape );
	
	// create body
	pPhyBody = new debpPhysicsBody;
	if( ! pPhyBody ) DETHROW( deeOutOfMemory );
	
	pPhyBody->SetResponseType( debpPhysicsBody::ertStatic );
	pPhyBody->SetOwnerHTSector( this );
	
	pPhyBody->SetShape( pBulletShape );
	pPhyBody->SetEnabled( true );
//		pPhyBody->SetEnabled( false );
//		pPhyBody->SetShape( NULL );
	
	// dirty
	pDirtyPoints = true;
}

void debpHTSector::pUpdatePoints(){
	deImage *heightImage = pSector->GetHeightImage();
	if( ! heightImage ) return;
	
	int i, count = pImageDim * pImageDim;
	float baseHeight = pSector->GetParentHeightTerrain()->GetBaseHeight();
	float scaling;
	
	if( heightImage->GetBitCount() == 8 ){
		sGrayscale8 *imageData = heightImage->GetDataGrayscale8();
		scaling = pSector->GetParentHeightTerrain()->GetHeightScaling() * HT_8BIT_PTOH;
		
		for( i=0; i<count; i++ ){
			pPoints[ i ].y = baseHeight + ( float )( imageData[ i ].value - HT_8BIT_BASE ) * scaling;
		}
		
	}else if( heightImage->GetBitCount() == 16 ){
		sGrayscale16 *imageData = heightImage->GetDataGrayscale16();
		scaling = pSector->GetParentHeightTerrain()->GetHeightScaling() * HT_16BIT_PTOH;
		
		for( i=0; i<count; i++ ){
			pPoints[ i ].y = baseHeight + ( float )( imageData[ i ].value - HT_16BIT_BASE ) * scaling;
		}
		
	}else{
		sGrayscale32 *imageData = heightImage->GetDataGrayscale32();
		scaling = pSector->GetParentHeightTerrain()->GetHeightScaling();
		
		for( i=0; i<count; i++ ){
			pPoints[ i ].y = baseHeight + imageData[ i ].value * scaling;
		}
	}
	
	// update bounding boxes
	count = pClusterCount * pClusterCount;
	int x, z;
	
	pMinExtend.y = baseHeight;
	pMaxExtend.y = baseHeight;
	
	for( i=0; i<count; i++ ){
		const int clusterFirstPointX = pClusters[ i ].firstPointX;
		const int clusterFirstPointZ = pClusters[ i ].firstPointZ;
		const int clusterPointCountX = pClusters[ i ].pointCountX;
		const int clusterPointCountZ = pClusters[ i ].pointCountZ;
		const int lastZ = clusterFirstPointZ + clusterPointCountZ;
		
		float minHeight = pPoints[ pImageDim * clusterFirstPointZ + clusterFirstPointX ].y;
		float maxHeight = minHeight;
		
		for( z=clusterFirstPointZ; z<lastZ; z++ ){
			const int firstX = pImageDim * z + clusterFirstPointX;
			const int lastX = firstX + clusterPointCountX;
			
			for( x=firstX; x<lastX; x++ ){
				if( pPoints[ x ].y < minHeight ){
					minHeight = pPoints[ x ].y;
					
				}else if( pPoints[ x ].y > maxHeight ){
					maxHeight = pPoints[ x ].y;
				}
			}
		}
		
		pClusters[ i ].center.y = ( minHeight + maxHeight ) * 0.5f;
		pClusters[ i ].halfExtends.y = ( maxHeight - minHeight ) * 0.5f;
		
		if( minHeight < pMinExtend.y ){
			pMinExtend.y = minHeight;
		}
		if( maxHeight > pMaxExtend.y ){
			pMaxExtend.y = maxHeight;
		}
	}
}
