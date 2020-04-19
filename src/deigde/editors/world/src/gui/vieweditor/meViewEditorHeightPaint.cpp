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

#include "meViewEditorHeightPaint.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../collisions/meCLHeightPaint.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/common/utils/decCollisionFilter.h>



// Class meViewEditorHeightPaint
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorHeightPaint::meViewEditorHeightPaint( meView3D &view ) : meViewEditorNavigation( view ){
	pPainterElapsedTime = 0.0f;
	pPainting = false;
	
	pHeightPainter = new meCLHeightPaint( view.GetWorld() );
}

meViewEditorHeightPaint::~meViewEditorHeightPaint(){
	if( pHeightPainter ){
		delete pHeightPainter;
	}
}



// Management
///////////////

void meViewEditorHeightPaint::OnFrameUpdate( float elapsed ){
	if( pPainting ){
		meCamera &camera = GetActiveCamera();
		
		pPainterElapsedTime += elapsed;
		
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), pPaintPosition.x, pPaintPosition.y ) * 500.0f; // 500m
		pHeightPainter->SetRay( rayPosition, rayDirection );
		
		pHeightPainter->PreparePaint();
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmEditing );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmHeightTerrains );
		RayTestCollision( pHeightPainter, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
		
		pHeightPainter->Paint( pPainterElapsedTime );
		
		pPainterElapsedTime = 0.0f;
	}
}



// Callbacks
//////////////

void meViewEditorHeightPaint::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonPress( x, y, shift, control );
	
	meCamera &camera = GetActiveCamera();
	
	const decDVector rayPosition = GetMatrixView().GetPosition();
	const decVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) * 50.0f; // 50m
	pHeightPainter->SetRay( rayPosition, rayDirection );
	
	pPainterElapsedTime = 0.0f;
	
	pHeightPainter->BeginSession();
	
	pPaintPosition.Set( x, y );
	pPainting = true;
}

void meViewEditorHeightPaint::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pPainting ){
		pPainting = false;
		pHeightPainter->EndSession();
	}
}

void meViewEditorHeightPaint::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	if( pPainting ){
		/*
		meCamera *camera = pWorld->GetActiveCamera();
		decLayerMask layerMask;
		
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = camera.GetDirectionFor( GetViewWidth(), GetViewHeight(), x, y ) * 50.0f; // 50m
		pHeightPainter->SetRay( rayPosition, rayDirection );
		
		pHeightPainter->PreparePaint();
		layerMask.SetBit( meWorld::eclmHeightTerrains );
		TestCollisionWith( pHeightPainter, rayPosition, rayDirection, layerMask, 0 );
		pHeightPainter->Paint( pPainterElapsedTime );
		pPainterElapsedTime = 0.0f;
		
		RequestUpdate();
		*/
		
		pPaintPosition.Set( x, y );
	}
}
