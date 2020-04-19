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

#include "meViewEditorSelect.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../collisions/meCLSelect.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class meViewEditorSelect
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorSelect::meViewEditorSelect( meView3D &view ) :
meViewEditorNavigation( view ),
pCLSelect( NULL )
{
	pColVol.TakeOver( view.GetEngine()->GetColliderManager()->CreateColliderVolume() );
	
	pCanvasSelect.TakeOver( view.GetEngine()->GetCanvasManager()->CreateCanvasPaint() );
	pCanvasSelect->SetShapeType( deCanvasPaint::estRectangle );
	pCanvasSelect->SetFillColor( decColor( 1.0f, 0.0f, 0.0f, 0.1f ) );
	pCanvasSelect->SetLineColor( decColor( 1.0f, 0.0f, 0.0f, 1.0f ) );
	pCanvasSelect->SetThickness( 1.0f );
	pCanvasSelect->SetOrder( 10.0f );
	pCanvasSelect->SetVisible( false );
	view.AddCanvas( pCanvasSelect );
	
	try{
		pCLSelect = new meCLSelect( *view.GetWorld() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meViewEditorSelect::~meViewEditorSelect(){
	pCleanUp();
}



// Management
///////////////

void meViewEditorSelect::UpdateRectSelection(){
	decLayerMask collisionCategory;
	collisionCategory.SetBit( meWorld::eclmEditing );
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit( meWorld::eclmDecals );
	collisionFilter.SetBit( meWorld::eclmObjects );
	collisionFilter.SetBit( meWorld::eclmObjectShape );
	collisionFilter.SetBit( meWorld::eclmAI );
	
	// determine the boundaries of the box
	const decPoint &dragCurrent = GetDragCurrent();
	const decPoint &dragOrigin = GetDragOrigin();
	decBoundary box;
	box.x1 = dragOrigin.x;
	box.y1 = dragOrigin.y;
	box.x2 = dragCurrent.x;
	box.y2 = dragCurrent.y;
	box.OrientateRect();
	
	const meCamera &camera = GetActiveCamera();
	const int height = GetViewHeight();
	const int width = GetViewWidth();
	
	decVector corner1( camera.GetDirectionFor( width, height, box.x1, box.y1 ) );
	decVector corner2( camera.GetDirectionFor( width, height, box.x2, box.y1 ) );
	decVector corner3( camera.GetDirectionFor( width, height, box.x2, box.y2 ) );
	decVector corner4( camera.GetDirectionFor( width, height, box.x1, box.y2 ) );
	
	const decVector view( GetMatrixView().TransformView().ToVector() );
	const float rectSelDist = GetWorldGuiParameters().GetRectSelDistance();
	corner1 *= rectSelDist / ( view * corner1 );
	corner2 *= rectSelDist / ( view * corner2 );
	corner3 *= rectSelDist / ( view * corner3 );
	corner4 *= rectSelDist / ( view * corner4 );
	
	pColVol->SetPosition( GetMatrixView().GetPosition() );
	pColVol->SetOrientation( decQuaternion() );
	pColVol->SetCollisionFilter( decCollisionFilter( collisionCategory, collisionFilter ) );
	
	decShapeHull * const shapeHull = new decShapeHull;
	shapeHull->SetPointCount( 5 );
	shapeHull->SetPointAt( 0, decVector() );
	shapeHull->SetPointAt( 1, corner1 );
	shapeHull->SetPointAt( 2, corner2 );
	shapeHull->SetPointAt( 3, corner3 );
	shapeHull->SetPointAt( 4, corner4 );
	
	decShapeList shapeList;
	shapeList.Add( shapeHull );
	( ( deColliderVolume& )( deCollider& )pColVol ).SetShapes( shapeList );
	
	// test with this frustum for selection
	pCLSelect->Prepare();
	ColliderTestCollision( pCLSelect, pColVol );
	pCLSelect->RunAction();
	
	// update canvas
	OnResize();
}



// Callbacks
//////////////

void meViewEditorSelect::OnResize(){
	if( ! GetDragLeftMouseButton() || ! pCLSelect || ! pCLSelect->GetRectSelect() ){
		pCanvasSelect->SetVisible( false );
		return;
	}
	
	const decPoint &dragOrigin = GetDragOrigin();
	const decPoint &dragCurrent = GetDragCurrent();
	decBoundary box;
	
	box.x1 = dragOrigin.x;
	box.y1 = dragOrigin.y;
	box.x2 = dragCurrent.x;
	box.y2 = dragCurrent.y;
	box.OrientateRect();
	
	pCanvasSelect->SetPosition( decPoint( box.x1, box.y1 ) );
	pCanvasSelect->SetSize( decPoint( box.x2 - box.x1, box.y2 - box.y1 ) );
	pCanvasSelect->SetVisible( true );
}

void meViewEditorSelect::OnLeftMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonPress( x, y, shift, control );
	
	if( GetDragLeftMouseButton() && pCLSelect ){
		const int elementMode = GetElementMode();
		
		pCLSelect->SetSingleSelect( ! shift );
		pCLSelect->SetRectSelect( false );
		
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmEditing );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmDecals );
		collisionFilter.SetBit( meWorld::eclmObjects );
		collisionFilter.SetBit( meWorld::eclmObjectShape );
		collisionFilter.SetBit( meWorld::eclmAI );
		
		pCLSelect->SetCanSelectObjects( elementMode == meWorldGuiParameters::eemObject );
		pCLSelect->SetCanSelectObjectShapes( elementMode == meWorldGuiParameters::eemObjectShape );
		pCLSelect->SetCanSelectDecals( elementMode == meWorldGuiParameters::eemDecal );
		pCLSelect->SetCanSelectNavSpaces( elementMode == meWorldGuiParameters::eemNavSpace );
		
		pCLSelect->SetCanHitObjects( elementMode == meWorldGuiParameters::eemObject );
		pCLSelect->SetCanHitObjectShapes( elementMode == meWorldGuiParameters::eemObjectShape );
		pCLSelect->SetCanHitDecals( elementMode == meWorldGuiParameters::eemDecal );
		pCLSelect->SetCanHitNavSpaces( elementMode == meWorldGuiParameters::eemNavSpace );
		
		pCLSelect->Reset();
		meWorldGuiParameters &guiparams = GetWorldGuiParameters();
		const decDVector rayPosition = GetMatrixView().GetPosition();
		const decVector rayDirection = GetActiveCamera().GetDirectionFor(
			GetViewWidth(), GetViewHeight(), x, y ) * guiparams.GetRectSelDistance();
		pCLSelect->SetRay( rayPosition, rayDirection );
		RayTestCollision( pCLSelect, rayPosition, rayDirection, decCollisionFilter( collisionCategory, collisionFilter ) );
		pCLSelect->RunAction();
		
		OnResize();
	}
}

void meViewEditorSelect::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pCLSelect ){
		pCLSelect->Reset();
		
		OnResize();
	}
}

void meViewEditorSelect::OnMouseMove( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnMouseMove( x, y, shift, control );
	
	if( GetDragLeftMouseButton() && pCLSelect ){
		if( ! pCLSelect->GetRectSelect() ){
			const int threshold = GetWorldGuiParameters().GetRectSelDragThreshold();
			const decPoint &dragDist = GetDragDistanceStart();
			
			if( abs( dragDist.x ) < threshold && abs( dragDist.y ) < threshold ){
				return;
				
			}else{
				pCLSelect->SetRectSelect( true );
				pCLSelect->StoreRectSelect();
			}
		}
		
		UpdateRectSelection();
	}
}

void meViewEditorSelect::OnMouseWheel( int steps, bool shift, bool control ){
	meViewEditorNavigation::OnMouseWheel( steps, shift, control );
	
	if( GetDragLeftMouseButton() && pCLSelect && pCLSelect->GetRectSelect() ){
		meWorldGuiParameters &guiparams = GetWorldGuiParameters();
		
		if( steps > 0 ){
			guiparams.SetRectSelDistance( guiparams.GetRectSelDistance() + guiparams.GetRectSelDistanceStep() );
			
		}else if( steps < 0 ){
			guiparams.SetRectSelDistance( guiparams.GetRectSelDistance() - guiparams.GetRectSelDistanceStep() );
		}
		
		GetWorld().NotifyEditingChanged();
		
		UpdateRectSelection();
	}
}



// Private Functions
//////////////////////

void meViewEditorSelect::pCleanUp(){
	if( pCanvasSelect && pCanvasSelect->GetParentView() ){
		GetView().RemoveCanvas( pCanvasSelect );
	}
	
	if( pCLSelect ){
		delete pCLSelect;
	}
}
