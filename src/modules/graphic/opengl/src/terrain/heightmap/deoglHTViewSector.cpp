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
#include "deoglHTViewSectorCluster.h"
#include "deoglRHTSector.h"
#include "deoglRHeightTerrain.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../utils/collision/deoglDCollisionVolume.h"

#include <dragengine/common/exceptions.h>



// Class deoglHTView
//////////////////////

// Constructor, destructor
////////////////////////////

deoglHTViewSector::deoglHTViewSector( deoglHTView &view, deoglRHTSector &sector ) :
pView( view ),
pSector( sector ),
pClusters( NULL ),
pClusterCount( 0 )
{
	const int count = sector.GetClusterCount();
	decPoint i;
	
	pClusters = new deoglHTViewSectorCluster*[ count * count ];
	
	for( i.y=0; i.y<count; i.y++ ){
		for( i.x=0; i.x<count; i.x++ ){
			pClusters[ pClusterCount++ ] = new deoglHTViewSectorCluster( *this, i );
		}
	}
}

deoglHTViewSector::~deoglHTViewSector(){
	if( pClusters ){
		while( pClusterCount > 0 ){
			delete pClusters[ --pClusterCount ];
		}
		delete [] pClusters;
	}
}



// Management
///////////////

deoglHTViewSectorCluster &deoglHTViewSector::GetClusterAt( int index ) const{
	if( index < 0 || index >= pClusterCount ){
		DETHROW( deeInvalidParam );
	}
	return *pClusters[ index ];
}

deoglHTViewSectorCluster &deoglHTViewSector::GetClusterAt( const decPoint &coordinates ) const{
	if( coordinates.x < 0 || coordinates.x >= pSector.GetClusterCount()
	|| coordinates.y < 0 || coordinates.y >= pSector.GetClusterCount() ){
		DETHROW( deeInvalidParam );
	}
	
	return *pClusters[ pSector.GetClusterCount() * coordinates.y + coordinates.x ];
}

void deoglHTViewSector::ResetClusters(){
	int i;
	for( i=0; i<pClusterCount; i++ ){
		pClusters[ i ]->Reset();
	}
}

void deoglHTViewSector::UpdateLODLevels( const decVector &camera ){
	deoglHTSCluster *clusters = pSector.GetClusters();
	int clusterCount = pSector.GetClusterCount();
	int c, x, z, base, neighborLOD;
	decVector difference;
	float squareDist;
	
	// determine the new LOD levels based solely on the camera position without rules
	for( c=0; c<pClusterCount; c++ ){
		//if( pClusters[ c ].lodLevel != -1 ){
			deoglHTViewSectorCluster &cluster = *pClusters[ c ];
			if( clusters[ c ].GetNoLOD() ){
				cluster.SetLodLevel( 0 );
				
			}else{
				difference = camera - clusters[ c ].GetCenter();
				squareDist = difference * difference;
				
				if( squareDist < 1e4f ){
					cluster.SetLodLevel( 0 );
					
				}else if( squareDist < 4e4f ){
					cluster.SetLodLevel( 1 );
					
				}else if( squareDist < 9e4f ){
					cluster.SetLodLevel( 2 );
					
				}else if( squareDist < 16e4f ){
					cluster.SetLodLevel( 3 );
					
				}else{
					cluster.SetLodLevel( 4 );
				}
			}
			
			cluster.SetBorderTarget( deoglHTViewSectorCluster::ebLeft, deoglHTViewSectorCluster::ebtLeft );
			cluster.SetBorderTarget( deoglHTViewSectorCluster::ebTop, deoglHTViewSectorCluster::ebtTop );
			cluster.SetBorderTarget( deoglHTViewSectorCluster::ebRight, deoglHTViewSectorCluster::ebtRight );
			cluster.SetBorderTarget( deoglHTViewSectorCluster::ebBottom, deoglHTViewSectorCluster::ebtBottom );
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
			deoglHTViewSectorCluster &cluster = *pClusters[ base ];
			
			if( cluster.GetLodLevel() >= 0 ){
				if( x > 0 ){
					neighborLOD = pClusters[ base - 1 ]->GetLodLevel();
					if( neighborLOD >= 0 && neighborLOD < cluster.GetLodLevel() ){
						cluster.SetBorderTarget( deoglHTViewSectorCluster::ebLeft,
							deoglHTViewSectorCluster::ebtFixLeft );
					}
				}
				
				if( z > 0 ){
					neighborLOD = pClusters[ base - clusterCount ]->GetLodLevel();
					if( neighborLOD >= 0 && neighborLOD < cluster.GetLodLevel() ){
						cluster.SetBorderTarget( deoglHTViewSectorCluster::ebTop,
							deoglHTViewSectorCluster::ebtFixTop );
					}
				}
				
				if( x < clusterCount - 1 ){
					neighborLOD = pClusters[ base + 1 ]->GetLodLevel();
					if( neighborLOD >= 0 && neighborLOD < cluster.GetLodLevel() ){
						cluster.SetBorderTarget( deoglHTViewSectorCluster::ebRight,
							deoglHTViewSectorCluster::ebtFixRight );
					}
				}
				
				if( z < clusterCount - 1 ){
					neighborLOD = pClusters[ base + clusterCount ]->GetLodLevel();
					if( neighborLOD >= 0 && neighborLOD < cluster.GetLodLevel() ){
						cluster.SetBorderTarget( deoglHTViewSectorCluster::ebBottom,
							deoglHTViewSectorCluster::ebtFixBottom );
					}
				}
			}
		}
	}
}

bool deoglHTViewSector::IsVisible() const{
	int i;
	for( i=0; i<pClusterCount; i++ ){
		if( pClusters[ i ]->GetLodLevel() >= 0 ){
			return true;
		}
	}
	
	return false;
}

void deoglHTViewSector::UpdateAllRTSInstances(){
	int i;
	for( i=0; i<pClusterCount; i++ ){
		pClusters[ i ]->UpdateRTSInstances();
	}
}



// Private functions
//////////////////////

void deoglHTViewSector::pLimitNeighborLODLevels( int x, int z ){
	int clusterCount = pSector.GetClusterCount();
	int base = clusterCount * z + x;
	deoglHTViewSectorCluster &cluster = *pClusters[ base ];
	
	if( cluster.GetLodLevel() >= 0 && cluster.GetLodLevel() < HTSC_MAX_LOD ){
		int nextLODLevel = cluster.GetLodLevel() + 1;
		
		if( x > 0 ){
			if( pClusters[ base - 1 ]->GetLodLevel() > nextLODLevel ){
				pClusters[ base - 1 ]->SetLodLevel( nextLODLevel );
				pLimitNeighborLODLevels( x - 1, z );
			}
		}
		
		if( z > 0 ){
			if( pClusters[ base - clusterCount ]->GetLodLevel() > nextLODLevel ){
				pClusters[ base - clusterCount ]->SetLodLevel( nextLODLevel );
				pLimitNeighborLODLevels( x, z - 1 );
			}
		}
		
		if( x < clusterCount - 1 ){
			if( pClusters[ base + 1 ]->GetLodLevel() > nextLODLevel ){
				pClusters[ base + 1 ]->SetLodLevel( nextLODLevel );
				pLimitNeighborLODLevels( x + 1, z );
			}
		}
		
		if( z < clusterCount - 1 ){
			if( pClusters[ base + clusterCount ]->GetLodLevel() > nextLODLevel ){
				pClusters[ base + clusterCount ]->SetLodLevel( nextLODLevel );
				pLimitNeighborLODLevels( x, z + 1 );
			}
		}
	}
}
