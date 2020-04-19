/* 
 * Drag[en]gine IGDE World Editor
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

#include "meViewEditorMaskPaint.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../collisions/meCLMaskPaint.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decCollisionFilter.h>



// Class meViewEditorMaskPaint
////////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorMaskPaint::meViewEditorMaskPaint( meView3D &view ) : meViewEditorNavigation( view ){
	pMaskPainter = new meCLMaskPaint( view.GetWorld() );
}

meViewEditorMaskPaint::~meViewEditorMaskPaint(){
	if( pMaskPainter ){
		delete pMaskPainter;
	}
}



// Management
///////////////



// Callbacks
//////////////

void meViewEditorMaskPaint::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonPress( x, y, shift, control );
	
	meCamera &camera = GetActiveCamera();
	meWorld &world = GetWorld();
	
	const decPoint3 activeSector; // = world.GetActiveSector();
	meHeightTerrainSector * const htsector = world.GetHeightTerrain()->GetSectorWith( decPoint( activeSector.x, activeSector.z ) );
	
	if( htsector && htsector->GetActiveTexture() ){
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) * 500.0f; // 500m
		pMaskPainter->SetRay( rayPosition, rayDirection );
		pMaskPainter->SetTexture( htsector, htsector->GetActiveTexture() );
		
		pMaskPainter->BeginSession();
		pMaskPainter->PreparePaint();
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmEditing );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmHeightTerrains );
		
		RayTestCollision( pMaskPainter, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
		pMaskPainter->Paint();
		
	}else{
		pMaskPainter->SetTexture( NULL, NULL );
	}
}

void meViewEditorMaskPaint::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pMaskPainter->CanPaint() ){
		pMaskPainter->EndSession();
	}
}

void meViewEditorMaskPaint::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	if( pMaskPainter->CanPaint() ){
		meCamera &camera = GetActiveCamera();
		
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) * 50.0f; // 50m
		pMaskPainter->SetRay( rayPosition, rayDirection );
		
		pMaskPainter->PreparePaint();
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmEditing );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmHeightTerrains );
		RayTestCollision( pMaskPainter, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
		pMaskPainter->Paint();
	}
}
