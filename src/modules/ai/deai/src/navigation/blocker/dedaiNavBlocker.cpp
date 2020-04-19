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

#include "dedaiNavBlocker.h"
#include "../dedaiNavSpace.h"
#include "../spaces/dedaiSpace.h"
#include "../heightterrain/dedaiHeightTerrain.h"
#include "../heightterrain/dedaiHeightTerrainNavSpace.h"
#include "../layer/dedaiLayer.h"
#include "../../deDEAIModule.h"
#include "../../world/dedaiWorld.h"
#include "../../devmode/dedaiDeveloperMode.h"
#include "../../utils/dedaiShapeToConvexVolume.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/deEngine.h>



// Class dedaiNavBlocker
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiNavBlocker::dedaiNavBlocker( deDEAIModule &deai, const deNavigationBlocker &blocker ) :
pDEAI( deai ),
pNavBlocker( blocker ),

pParentWorld( NULL ),

pDirtyMatrix( true ),
pDirtyExtends( true ),
pDirtyShape( true ),

pLayer( NULL ),

pDebugDrawer( NULL ),
pDDSBlocker( NULL )
{
	ShapeChanged();
}

dedaiNavBlocker::~dedaiNavBlocker(){
	pCleanUp();
}



// Management
///////////////

void dedaiNavBlocker::SetParentWorld( dedaiWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	pInvalidateLayerBlocking();
	if( pParentWorld && pDebugDrawer ){
		pParentWorld->GetWorld().RemoveDebugDrawer( pDebugDrawer );
	}
	
	pParentWorld = world;
	
	if( world ){
		pLayer = world->GetLayer( pNavBlocker.GetLayer() );
		if( pDebugDrawer ){
			world->GetWorld().AddDebugDrawer( pDebugDrawer );
		}
		
	}else{
		pLayer = NULL;
	}
	
	pDirtyShape = true;
	pInvalidateLayerBlocking();
	
	UpdateDDSBlocker();
}



const decDMatrix &dedaiNavBlocker::GetMatrix(){
	if( pDirtyMatrix ){
		pUpdateMatrices();
	}
	
	return pMatrix;
}

const decDMatrix &dedaiNavBlocker::GetInverseMatrix(){
	if( pDirtyMatrix ){
		pUpdateMatrices();
	}
	
	return pInvMatrix;
}

const decDVector &dedaiNavBlocker::GetMinimumExtends(){
	if( pDirtyExtends ){
		pUpdateExtends();
	}
	
	return pMinExtends;
}

const decDVector &dedaiNavBlocker::GetMaximumExtends(){
	if( pDirtyExtends ){
		pUpdateExtends();
	}
	
	return pMaxExtends;
}



void dedaiNavBlocker::Prepare(){
	if( ! pParentWorld || ! pDirtyShape ){
		return;
	}
	
	pUpdateBlocker();
	pDirtyShape = false;
}



void dedaiNavBlocker::UpdateDDSBlocker(){
	// check if the developer mode is enabled and navigation spaces have to be shown
	const dedaiDeveloperMode &devmode = pDEAI.GetDeveloperMode();
	
	if( devmode.GetEnabled() && devmode.GetShowBlockers() && pNavBlocker.GetEnabled() ){
		// ensure the debug drawer exists
		if( ! pDebugDrawer ){
			pDebugDrawer = pDEAI.GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer();
			pDebugDrawer->SetXRay( true );
			
			if( pParentWorld ){
				pParentWorld->GetWorld().AddDebugDrawer( pDebugDrawer );
			}
		}
		
		// update the position and orientation of the debug drawer
		pDebugDrawer->SetPosition( pNavBlocker.GetPosition() );
		pDebugDrawer->SetOrientation( pNavBlocker.GetOrientation() );
		pDebugDrawer->SetScale( pNavBlocker.GetScaling() );
		
		// ensure the debug drawer shapes exists
		bool updateShapes = false;
		
		if( ! pDDSBlocker ){
			pDDSBlocker = new deDebugDrawerShape;
			pDDSBlocker->SetFillColor( decColor( 0.0f, 0.5f, 1.0f, 0.1f ) );
			pDDSBlocker->SetEdgeColor( decColor( 0.0f, 0.5f, 1.0f, 0.8f ) );
			pDebugDrawer->AddShape( pDDSBlocker );
			updateShapes = true;
		}
		
		// update the shapes if required
		if( updateShapes ){
			UpdateDDSBlockerShape();
		}
		
	}else{
		// if the debug drawer exists remove it
		if( pDebugDrawer ){
			if( pParentWorld ){
				pParentWorld->GetWorld().RemoveDebugDrawer( pDebugDrawer );
			}
			
			pDDSBlocker = NULL;
			
			pDebugDrawer->FreeReference();
			pDebugDrawer = NULL;
		}
	}
}

void dedaiNavBlocker::UpdateDDSBlockerShape(){
	if( ! pDDSBlocker ){
		return;
	}
	
	const decShapeList &shapeList = pNavBlocker.GetShapeList();
	const int count = shapeList.GetCount();
	
	pDDSBlocker->RemoveAllFaces();
	pDDSBlocker->GetShapeList().RemoveAll();
	
	if( count > 0 ){
		decShape *shape = NULL;
		int i;
		
		try{
			for( i=0; i<count; i++ ){
				shape = shapeList.GetAt( i )->Copy();
				pDDSBlocker->GetShapeList().Add( shape );
				shape = NULL;
			}
			
		}catch( const deException & ){
			if( shape ){
				delete shape;
			}
			throw;
		}
	}
	
	pDebugDrawer->NotifyShapeContentChanged();
}



// Notifications
//////////////////

void dedaiNavBlocker::PositionChanged(){
	pDirtyMatrix = true;
	pDirtyExtends = true;
	
	pInvalidateLayerBlocking();
	
	if( pDebugDrawer ){
		pDebugDrawer->SetPosition( pNavBlocker.GetPosition() );
	}
	
	// TODO
	// for each navigation space:
	//    if navigation space matches type and layer of blocker:
	//       blockerChanged( blockerOldAABBMin, blockerOldAABBMax )
	//       blockerChanged( blockerNewAABBMin, blockerNewAABBMax )
	// 
	// navigation space :: blockerChanged( minAABB, maxAABB ):
	// for each face:
	//    face overlaps AABB:
	//       mark face dirty for blocker recalculation
	//       remove all generated blocker faces for this face
	// 
	// navigation space :: update:
	// if dirtyBlockerFaces:
	//    for each blocker:
	//       if blocker overlaps navigation space:
	//          blockerShapeList.extend( blocker.shapes )
	//    for each face:
	//       if face is blockerDirty:
	//          generate blocker faces for face. for this take the
	//          face and send it through all blockerShapeList.
	// 
	// all generated vertices, edges, corners and faces are added
	// to the navigation mesh. after the initial generation process
	// the vertex, edge, corner and face count is stored. any vertex
	// corner, edge and face beyond this value is a dynamic blocker
	// vertex, corner, edge, face. this way all blocker data can be
	// deleted by just going back to the saved counts
}

void dedaiNavBlocker::OrientationChanged(){
	pDirtyMatrix = true;
	pDirtyExtends = true;
	
	pInvalidateLayerBlocking();
	
	if( pDebugDrawer ){
		pDebugDrawer->SetOrientation( pNavBlocker.GetOrientation() );
	}
}

void dedaiNavBlocker::ScalingChanged(){
	pDirtyMatrix = true;
	pDirtyExtends = true;
	
	pInvalidateLayerBlocking();
	
	if( pDebugDrawer ){
		pDebugDrawer->SetScale( pNavBlocker.GetScaling() );
	}
}

void dedaiNavBlocker::LayerChanged(){
	if( ! pParentWorld ){
		return;
	}
	
	pInvalidateLayerBlocking();
	pLayer = pParentWorld->GetLayer( pNavBlocker.GetLayer() );
	pInvalidateLayerBlocking();
}

void dedaiNavBlocker::SpaceTypeChanged(){
	pInvalidateLayerBlocking();
}

void dedaiNavBlocker::EnabledChanged(){
	pInvalidateLayerBlocking();
	UpdateDDSBlocker();
}

void dedaiNavBlocker::ShapeChanged(){
	pDirtyShape = true;
	pDirtyExtends = true;
	
	pUpdateConvexVolumeList();
	pInvalidateLayerBlocking();
}

void dedaiNavBlocker::BlockingPriorityChanged(){
	pInvalidateLayerBlocking();
}



// Private Functions
//////////////////////

void dedaiNavBlocker::pCleanUp(){
	SetParentWorld( NULL );
	
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
}

void dedaiNavBlocker::pUpdateMatrices(){
	pMatrix.SetWorld( pNavBlocker.GetPosition(), pNavBlocker.GetOrientation() );
	pInvMatrix = pMatrix.Invert();
	pDirtyMatrix = false;
}

void dedaiNavBlocker::pUpdateExtends(){
	const int volumeCount = pConvexVolumeList.GetVolumeCount();
	bool first = true;
	
	if( volumeCount > 0 ){
		const decDMatrix &matrix = GetMatrix();
		int i, j, k;
		
		for( i=0; i< volumeCount; i++ ){
			const decConvexVolume &volume = *pConvexVolumeList.GetVolumeAt( i );
			const int faceCount = volume.GetFaceCount();
			
			for( j=0; j<faceCount; j++ ){
				const decConvexVolumeFace &face = *volume.GetFaceAt( j );
				const int vertexCount = face.GetVertexCount();
				
				for( k=0; k<vertexCount; k++ ){
					const decDVector position = matrix * decDVector( volume.GetVertexAt( face.GetVertexAt( k ) ) );
					
					if( first ){
						pMinExtends = position;
						pMaxExtends = position;
						first = false;
						
					}else{
						pMinExtends.SetSmallest( position );
						pMaxExtends.SetLargest( position );
					}
				}
			}
		}
	}
	
	if( first ){
		pMinExtends = pNavBlocker.GetPosition();
		pMaxExtends = pMinExtends;
	}
}

void dedaiNavBlocker::pUpdateBlocker(){
	if( ! pDDSBlocker ){
		return;
	}
	
	UpdateDDSBlockerShape();
}

void dedaiNavBlocker::pUpdateConvexVolumeList(){
	dedaiShapeToConvexVolume visitor;
	visitor.SetList( &pConvexVolumeList );
	
	pConvexVolumeList.RemoveAllVolumes();
	
	const decShapeList &shapeList = pNavBlocker.GetShapeList();
	const int shapeCount = shapeList.GetCount();
	int i;
	
	for( i=0; i<shapeCount; i++ ){
		shapeList.GetAt( i )->Visit( visitor );
	}
}

void dedaiNavBlocker::pInvalidateLayerBlocking(){
	if( ! pLayer ){
		return;
	}
	
	pLayer->InvalidateBlocking( pNavBlocker.GetSpaceType(), GetMinimumExtends(), GetMaximumExtends() );
}
