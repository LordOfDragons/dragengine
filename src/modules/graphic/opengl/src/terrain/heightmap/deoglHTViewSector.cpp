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
#include "deoglHTView.h"
#include "deoglHTViewSector.h"
#include "deoglRHTSector.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>



// Class deoglHTView
//////////////////////

// Constructor, destructor
////////////////////////////

deoglHTViewSector::deoglHTViewSector( deoglHTView &view, deoglRHTSector &sector ) :
pView( view ),
pSector( sector )
{
	int c, count = sector.GetClusterCount() * sector.GetClusterCount();
	
	pClusters = new sHTVSCluster[ count ];
	
	for( c=0; c<count; c++ ){
		pClusters[ c ].lodLevel = 0;
		pClusters[ c ].borders[ 0 ] = 0;
		pClusters[ c ].borders[ 1 ] = 0;
		pClusters[ c ].borders[ 2 ] = 0;
		pClusters[ c ].borders[ 3 ] = 0;
	}
}

deoglHTViewSector::~deoglHTViewSector(){
	if( pClusters ) delete [] pClusters;
}



// Management
///////////////

sHTVSCluster &deoglHTViewSector::GetClusterAt( int x, int z ) const{
	if( x < 0 || x >= pSector.GetClusterCount() || z < 0 || z >= pSector.GetClusterCount() ) DETHROW( deeInvalidParam );
	
	return pClusters[ pSector.GetClusterCount() * z + x ];
}

void deoglHTViewSector::ResetClusters(){
	int c, count = pSector.GetClusterCount() * pSector.GetClusterCount();
	
	for( c=0; c<count; c++ ){
		pClusters[ c ].lodLevel = 0;
		pClusters[ c ].borders[ 0 ] = 0;
		pClusters[ c ].borders[ 1 ] = 0;
		pClusters[ c ].borders[ 2 ] = 0;
		pClusters[ c ].borders[ 3 ] = 0;
	}
}
/*
void deoglHTViewSector::DetermineVisibilityUsing( deoglDCollisionVolume *collisionVolume ){
	if( ! collisionVolume ) DETHROW( deeInvalidParam );
	
	int c, count = pSector.GetClusterCount() * pSector.GetClusterCount();
	deoglHTSCluster *clusters = pSector.GetClusters();
	deoglDCollisionBox colBox;
	
	for( c=0; c<count; c++ ){
		colBox.SetCenter( clusters[ c ].GetCenter() );
		colBox.SetHalfSize( clusters[ c ].GetHalfExtends() );
		
		if( collisionVolume->BoxHitsVolume( &colBox ) ){
			pClusters[ c ].lodLevel = 0;
			
		}else{
			pClusters[ c ].lodLevel = -1;
		}
	}
}
*/
void deoglHTViewSector::UpdateLODLevels( const decVector &camera ){
	deoglHTSCluster *clusters = pSector.GetClusters();
	int clusterCount = pSector.GetClusterCount();
	int c, totalClusterCount = clusterCount * clusterCount;
	int x, z, base, neighborLOD;
	decVector difference;
	float squareDist;
	
	// determine the new LOD levels based solely on the camera position without rules
	for( c=0; c<totalClusterCount; c++ ){
		//if( pClusters[ c ].lodLevel != -1 ){
			if( clusters[ c ].GetNoLOD() ){
				pClusters[ c ].lodLevel = 0;
				
			}else{
				difference = camera - clusters[ c ].GetCenter();
				squareDist = difference * difference;
				
				if( squareDist < 1e4f ){
					pClusters[ c ].lodLevel = 0;
					
				}else if( squareDist < 4e4f ){
					pClusters[ c ].lodLevel = 1;
					
				}else if( squareDist < 9e4f ){
					pClusters[ c ].lodLevel = 2;
					
				}else if( squareDist < 16e4f ){
					pClusters[ c ].lodLevel = 3;
					
				}else{
					pClusters[ c ].lodLevel = 4;
				}
			}
			
			pClusters[ c ].borders[ ehtvsbLeft ] = ehtscbLeft;
			pClusters[ c ].borders[ ehtvsbTop ] = ehtscbTop;
			pClusters[ c ].borders[ ehtvsbRight ] = ehtscbRight;
			pClusters[ c ].borders[ ehtvsbBottom ] = ehtscbBottom;
		//}
	}
	
	// enforce now the LOD level rules
	for( z=0; z<clusterCount; z++ ){
		for( x=0; x<clusterCount; x++ ){
			pLimitNeighborLODLevels( x, z );
		}
	}
	
	// update borders
	for( z=0; z<clusterCount; z++ ){
		for( x=0; x<clusterCount; x++ ){
			base = clusterCount * z + x;
			
			if( pClusters[ base ].lodLevel >= 0 ){
				if( x > 0 ){
					neighborLOD = pClusters[ base - 1 ].lodLevel;
					if( neighborLOD >= 0 && neighborLOD < pClusters[ base ].lodLevel ){
						pClusters[ base ].borders[ ehtvsbLeft ] = ehtscbFixLeft;
					}
				}
				
				if( z > 0 ){
					neighborLOD = pClusters[ base - clusterCount ].lodLevel;
					if( neighborLOD >= 0 && neighborLOD < pClusters[ base ].lodLevel ){
						pClusters[ base ].borders[ ehtvsbTop ] = ehtscbFixTop;
					}
				}
				
				if( x < clusterCount - 1 ){
					neighborLOD = pClusters[ base + 1 ].lodLevel;
					if( neighborLOD >= 0 && neighborLOD < pClusters[ base ].lodLevel ){
						pClusters[ base ].borders[ ehtvsbRight ] = ehtscbFixRight;
					}
				}
				
				if( z < clusterCount - 1 ){
					neighborLOD = pClusters[ base + clusterCount ].lodLevel;
					if( neighborLOD >= 0 && neighborLOD < pClusters[ base ].lodLevel ){
						pClusters[ base ].borders[ ehtvsbBottom ] = ehtscbFixBottom;
					}
				}
			}
		}
	}
}

bool deoglHTViewSector::IsVisible() const{
	int i, count = pSector.GetClusterCount() * pSector.GetClusterCount();
	
	for( i=0; i<count; i++ ){
		if( pClusters[ i ].lodLevel >= 0 ){
			return true;
		}
	}
	
	return false;
}



// Private functions
//////////////////////

void deoglHTViewSector::pLimitNeighborLODLevels( int x, int z ){
	int clusterCount = pSector.GetClusterCount();
	int base = clusterCount * z + x;
	
	if( pClusters[ base ].lodLevel >= 0 && pClusters[ base ].lodLevel < HTSC_MAX_LOD ){
		int nextLODLevel = pClusters[ base ].lodLevel + 1;
		
		if( x > 0 ){
			if( pClusters[ base - 1 ].lodLevel > nextLODLevel ){
				pClusters[ base - 1 ].lodLevel = nextLODLevel;
				pLimitNeighborLODLevels( x - 1, z );
			}
		}
		
		if( z > 0 ){
			if( pClusters[ base - clusterCount ].lodLevel > nextLODLevel ){
				pClusters[ base - clusterCount ].lodLevel = nextLODLevel;
				pLimitNeighborLODLevels( x, z - 1 );
			}
		}
		
		if( x < clusterCount - 1 ){
			if( pClusters[ base + 1 ].lodLevel > nextLODLevel ){
				pClusters[ base + 1 ].lodLevel = nextLODLevel;
				pLimitNeighborLODLevels( x + 1, z );
			}
		}
		
		if( z < clusterCount - 1 ){
			if( pClusters[ base + clusterCount ].lodLevel > nextLODLevel ){
				pClusters[ base + clusterCount ].lodLevel = nextLODLevel;
				pLimitNeighborLODLevels( x, z + 1 );
			}
		}
	}
}
