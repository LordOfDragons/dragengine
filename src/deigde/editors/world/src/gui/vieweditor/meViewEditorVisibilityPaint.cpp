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

#include "meViewEditorVisibilityPaint.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../collisions/meCLVisibilityPaint.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decCollisionFilter.h>



// Class meViewEditorVisibilityPaint
////////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorVisibilityPaint::meViewEditorVisibilityPaint( meView3D &view ) :
meViewEditorNavigation( view ){
	pVisPainter = new meCLVisibilityPaint( view.GetWorld() );
}

meViewEditorVisibilityPaint::~meViewEditorVisibilityPaint(){
	if( pVisPainter ){
		delete pVisPainter;
	}
}



// Management
///////////////



// Callbacks
//////////////

void meViewEditorVisibilityPaint::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonPress( x, y, shift, control );
	
	meCamera &camera = GetActiveCamera();
	decLayerMask layerMask;
	
	const decDVector rayPosition = GetMatrixView().GetPosition();
	const decVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) * 500.0f; // 500m
	pVisPainter->SetRay( rayPosition, rayDirection );
	
	pVisPainter->BeginSession();
	pVisPainter->PreparePaint();
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit( meWorld::eclmEditing );
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit( meWorld::eclmHeightTerrains );
	
	RayTestCollision( pVisPainter, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
	
	pVisPainter->Paint();
}

void meViewEditorVisibilityPaint::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( GetDragLeftMouseButton() ){
		pVisPainter->EndSession();
	}
}

void meViewEditorVisibilityPaint::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	if( GetDragLeftMouseButton() ){
		meCamera &camera = GetActiveCamera();
		
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) * 50.0f; // 50m
		pVisPainter->SetRay( rayPosition, rayDirection );
		
		pVisPainter->PreparePaint();
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmEditing );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmHeightTerrains );
		
		RayTestCollision( pVisPainter, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
		
		pVisPainter->Paint();
	}
}
