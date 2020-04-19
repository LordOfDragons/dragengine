/* 
 * Drag[en]gine AI Module
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
#include <string.h>

#include "dedaiLayer.h"
#include "../dedaiNavigator.h"
#include "../heightterrain/dedaiHeightTerrain.h"
#include "../heightterrain/dedaiHeightTerrainSector.h"
#include "../heightterrain/dedaiHeightTerrainNavSpace.h"
#include "../spaces/dedaiSpace.h"
#include "../spaces/grid/dedaiSpaceGrid.h"
#include "../spaces/mesh/dedaiSpaceMesh.h"
#include "../blocker/dedaiNavBlocker.h"
#include "../dedaiNavSpace.h"
#include "../../world/dedaiWorld.h"
#include "../../deDEAIModule.h"
#include "../../devmode/dedaiDeveloperMode.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>



// Class dedaiLayer
/////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiLayer::dedaiLayer( dedaiWorld &world, int layer ) :
pWorld( world ),
pLayer( layer ),
pDirty( true )
{
}

dedaiLayer::~dedaiLayer(){
}



// Management
///////////////

void dedaiLayer::Update( float elapsed ){
	if( pWorld.GetDEAI().GetDeveloperMode().GetEnabled() ){
		Prepare();
	}
}

void dedaiLayer::Prepare(){
	if( ! pDirty ){
		return;
	}
	
	pUpdateCostTable();
	pNavSpacesPrepare();
	pNavSpacesPrepareLinks();
	pNavigatorsPrepare();
	
	pDirty = false;
}



void dedaiLayer::MarkDirty(){
	pDirty = true;
}



dedaiSpaceGridVertex *dedaiLayer::GetGridVertexClosestTo( const decDVector &position, float &distance ){
	dedaiSpaceGridVertex *bestVertex = NULL;
	float bestDistance = 0.0f;
	float testDistance;
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() != this ){
					continue;
				}
				
				dedaiSpaceGrid * const grid = space.GetGrid();
				if( ! grid ){
					continue;
				}
				
				const decVector testPosition( space.GetInverseMatrix() * position );
				dedaiSpaceGridVertex * const testVertex = grid->GetVertexClosestTo( testPosition, testDistance );
				if( testVertex && ( ! bestVertex || testDistance < bestDistance ) ){
					bestVertex = testVertex;
					bestDistance = testDistance;
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() != this ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpaceGrid * const grid = space.GetGrid();
		if( ! grid ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		const decVector testPosition( space.GetInverseMatrix() * position );
		dedaiSpaceGridVertex * const testVertex = grid->GetVertexClosestTo( testPosition, testDistance );
		if( testVertex && ( ! bestVertex || testDistance < bestDistance ) ){
			bestVertex = testVertex;
			bestDistance = testDistance;
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	if( bestVertex ){
		distance = bestDistance;
	}
	return bestVertex;
}

dedaiSpaceMeshFace *dedaiLayer::GetMeshFaceClosestTo( const decDVector &position, float &distance ){
	dedaiSpaceMeshFace *bestFace = NULL;
	float bestDistance = 0.0f;
	float testDistance;
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() != this ){
					continue;
				}
				
				dedaiSpaceMesh * const navmesh = space.GetMesh();
				
				if( ! navmesh ){
					continue;
				}
				
				const decVector testPosition( space.GetInverseMatrix() * position );
				dedaiSpaceMeshFace * const testFace = navmesh->GetFaceClosestTo( testPosition, testDistance );
				
				if( testFace && ( ! bestFace || testDistance < bestDistance ) ){
					bestFace = testFace;
					bestDistance = testDistance;
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() != this ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpaceMesh * const navmesh = space.GetMesh();
		if( ! navmesh ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
// 		if( ! ( navspace->GetMaximumExtends() >= testMinExtend && navspace->GetMinimumExtends() <= testMaxExtend ) ){
// 			engNavSpace = engNavSpace->GetLLWorldNext();
// 			continue;
// 		}
		
		const decVector testPosition( space.GetInverseMatrix() * position );
		dedaiSpaceMeshFace * const testFace = navmesh->GetFaceClosestTo( testPosition, testDistance );
		
		if( testFace && ( ! bestFace || testDistance < bestDistance ) ){
			bestFace = testFace;
			bestDistance = testDistance;
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	if( bestFace ){
		distance = bestDistance;
	}
	
	return bestFace;
}

dedaiSpaceMeshFace *dedaiLayer::GetNavMeshNearestPoint( const decDVector &point, float radius, decDVector &nearest ){
	const decDVector testMinExtend( point - decDVector( ( double )radius, ( double )radius, ( double )radius ) );
	const decDVector testMaxExtend( point + decDVector( ( double )radius, ( double )radius, ( double )radius ) );
	dedaiSpaceMeshFace *bestFace = NULL;
	float bestDistanceSquared = 0.0f;
	decVector testNearestPosition;
	float testNearestDistance;
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() != this ){
					continue;
				}
				
				dedaiSpaceMesh * const navmesh = space.GetMesh();
				if( ! navmesh ){
					continue;
				}
				
				if( ! ( space.GetMaximumExtends() >= testMinExtend && space.GetMinimumExtends() <= testMaxExtend ) ){
					continue;
				}
				
				const decVector testPoint( ( space.GetInverseMatrix() * point ) );
				dedaiSpaceMeshFace * const testFace = navmesh->NearestPoint( testPoint, radius, testNearestPosition, testNearestDistance );
				
				if( testFace && ( ! bestFace || testNearestDistance < bestDistanceSquared ) ){
					bestFace = testFace;
					nearest = space.GetMatrix() * decDVector( testNearestPosition );
					bestDistanceSquared = testNearestDistance;
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() != this ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpaceMesh * const navmesh = space.GetMesh();
		if( ! navmesh ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		if( ! ( space.GetMaximumExtends() >= testMinExtend && space.GetMinimumExtends() <= testMaxExtend ) ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		const decVector testPoint( ( space.GetInverseMatrix() * point ) );
		dedaiSpaceMeshFace * const testFace = navmesh->NearestPoint( testPoint, radius, testNearestPosition, testNearestDistance );
		
		if( testFace && ( ! bestFace || testNearestDistance < bestDistanceSquared ) ){
			bestFace = testFace;
			nearest = space.GetMatrix() * decDVector( testNearestPosition );
			bestDistanceSquared = testNearestDistance;
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	return bestFace;
}



void dedaiLayer::InvalidateBlocking(){
	// TODO use an per-layer octree to improve this
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() == this ){
					space.InvalidateBlocking();
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() == this ){
			space.InvalidateBlocking();
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	// NOTE tell also navigators?
	
	// update require
	pDirty = true;
}

void dedaiLayer::InvalidateBlocking( deNavigationSpace::eSpaceTypes type ){
	// TODO use an per-layer octree to improve this
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() == this && space.GetType() == type ){
					space.InvalidateBlocking();
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() == this && space.GetType() == type ){
			space.InvalidateBlocking();
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	// NOTE tell also navigators?
	
	// update require
	pDirty = true;
}

void dedaiLayer::InvalidateBlocking( deNavigationSpace::eSpaceTypes type,
const decDVector &boxMin, const decDVector &boxMax ){
	// TODO use an per-layer octree to improve this
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() != this || space.GetType() != type ){
					continue;
				}
				
				const decDVector &targetMinExtend = space.GetMinimumExtends();
				const decDVector &targetMaxExtend = space.GetMaximumExtends();
				if( targetMaxExtend >= boxMin && targetMinExtend <= boxMax ){
					space.InvalidateBlocking();
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() != this || space.GetType() != type ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		const decDVector &targetMinExtend = space.GetMinimumExtends();
		const decDVector &targetMaxExtend = space.GetMaximumExtends();
		if( targetMaxExtend >= boxMin && targetMinExtend <= boxMax ){
			space.InvalidateBlocking();
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	// NOTE tell also navigators?
	
	// update require
	pDirty = true;
}



void dedaiLayer::InvalidateLinks(){
	// TODO use an per-layer octree to improve this
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() == this ){
					space.ClearLinks();
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() == this ){
			space.ClearLinks();
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	// NOTE tell also navigators?
	
	// update require
	pDirty = true;
}

void dedaiLayer::InvalidateLinks( deNavigationSpace::eSpaceTypes type ){
	// TODO use an per-layer octree to improve this
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() == this && space.GetType() == type ){
					space.ClearLinks();
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() == this && space.GetType() == type ){
			space.ClearLinks();
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	// NOTE tell also navigators?
	
	// update require
	pDirty = true;
}

void dedaiLayer::InvalidateLinks( deNavigationSpace::eSpaceTypes type,
const decDVector &boxMin, const decDVector &boxMax ){
	// TODO use an per-layer octree to improve this
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() != this || space.GetType() != type ){
					continue;
				}
				
				const decDVector &targetMinExtend = space.GetMinimumExtends();
				const decDVector &targetMaxExtend = space.GetMaximumExtends();
				if( targetMaxExtend >= boxMin && targetMinExtend <= boxMax ){
					space.ClearLinks();
				}
			}
		}
	}
	
	// navigation spaces
	deNavigationSpace *engNavSpace = pWorld.GetWorld().GetRootNavigationSpace();
	while( engNavSpace ){
		dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
		if( ! navspace ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		dedaiSpace &space = *navspace->GetSpace();
		if( space.GetLayer() != this || space.GetType() != type ){
			engNavSpace = engNavSpace->GetLLWorldNext();
			continue;
		}
		
		const decDVector &targetMinExtend = space.GetMinimumExtends();
		const decDVector &targetMaxExtend = space.GetMaximumExtends();
		if( targetMaxExtend >= boxMin && targetMinExtend <= boxMax ){
			space.ClearLinks();
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
	
	// NOTE tell also navigators?
	
	// update require
	pDirty = true;
}



// Private functions
//////////////////////

void dedaiLayer::pUpdateCostTable(){
	if( ! pCostTable.GetChanged() ){
		return;
	}
	
	deNavigator *navigator = pWorld.GetWorld().GetRootNavigator();
	while( navigator ){
		if( navigator->GetLayer() == pLayer ){
			dedaiNavigator * const peer = ( dedaiNavigator* )navigator->GetPeerAI();
			if( peer ){
				peer->CostTableDefinitionChanged();
			}
		}
		navigator = navigator->GetLLWorldNext();
	}
	
	pCostTable.ClearChanged();
}

void dedaiLayer::pNavSpacesPrepare(){
	dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() == this ){
					space.Prepare();
				}
			}
		}
	}
	
	deNavigationBlocker *blocker = pWorld.GetWorld().GetRootNavigationBlocker();
	while( blocker ){
		if( blocker->GetLayer() == pLayer ){
			dedaiNavBlocker * const peer = ( dedaiNavBlocker* )blocker->GetPeerAI();
			if( peer ){
				peer->Prepare();
			}
		}
		blocker = blocker->GetLLWorldNext();
	}
	
	deNavigationSpace *navspace = pWorld.GetWorld().GetRootNavigationSpace();
	while( navspace ){
		if( navspace->GetLayer() == pLayer ){
			dedaiNavSpace * const peer = ( dedaiNavSpace* )navspace->GetPeerAI();
			if( peer ){
				peer->GetSpace()->Prepare();
			}
		}
		navspace = navspace->GetLLWorldNext();
	}
}

void dedaiLayer::pNavSpacesPrepareLinks(){
	dedaiHeightTerrain * const heightTerrain = pWorld.GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiSpace &space = *sector.GetNavSpaceAt( j )->GetSpace();
				if( space.GetLayer() == this ){
					space.PrepareLinks();
				}
			}
		}
	}
	
	deNavigationSpace *navspace = pWorld.GetWorld().GetRootNavigationSpace();
	while( navspace ){
		if( navspace->GetLayer() == pLayer ){
			dedaiNavSpace * const peer = ( dedaiNavSpace* )navspace->GetPeerAI();
			if( peer ){
				peer->GetSpace()->PrepareLinks();
			}
		}
		navspace = navspace->GetLLWorldNext();
	}
}

void dedaiLayer::pNavigatorsPrepare(){
	deNavigator *navigator = pWorld.GetWorld().GetRootNavigator();
	while( navigator ){
		if( navigator->GetLayer() == pLayer ){
			dedaiNavigator * const peer = ( dedaiNavigator* )navigator->GetPeerAI();
			if( peer ){
				peer->Prepare();
			}
		}
		navigator = navigator->GetLLWorldNext();
	}
}
