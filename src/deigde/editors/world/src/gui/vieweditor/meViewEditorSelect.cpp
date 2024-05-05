/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meViewEditorSelect.h"
#include "../meView3D.h"
#include "../meWindowMain.h"
#include "../../collisions/meCLSelect.h"
#include "../../collisions/meCLClosestElement.h"
#include "../../configuration/meConfiguration.h"
#include "../../world/meCamera.h"
#include "../../world/meWorld.h"
#include "../../world/meWorldGuiParameters.h"
#include "../../world/object/meObject.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/world/deWorld.h>



// Class meViewEditorSelect
/////////////////////////////

// Constructor, destructor
////////////////////////////

meViewEditorSelect::meViewEditorSelect( meView3D &view ) :
meViewEditorNavigation( view ),
pCLSelect( nullptr ),
pCLClosest ( nullptr )
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
	
	pInfoBubble.TakeOver( new meInfoBubble( view ) );
	
	pInfoBubbleText.TakeOver( view.GetEngine()->GetCanvasManager()->CreateCanvasText() );
	pInfoBubbleText->SetFont( deFont::Ref::New( view.GetEngine()->GetFontManager()->
		LoadFont( "/igde/fonts/sans_10.defont", "/" ) ) );
	pInfoBubbleText->SetFontSize( ( float )pInfoBubbleText->GetFont()->GetLineHeight() );
	pInfoBubbleText->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
	pInfoBubbleText->SetOrder( 0.0f );
	pInfoBubble->GetCanvasContent()->AddCanvas( pInfoBubbleText );
	
	try{
		pCLSelect = new meCLSelect( *view.GetWorld() );
		pCLClosest = new meCLClosestElement( *view.GetWorld() );
		
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
		
		pInfoBubble->Hide();
		OnResize();
	}
}

void meViewEditorSelect::OnLeftMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnLeftMouseButtonRelease( x, y, shift, control );
	
	if( pCLSelect ){
		pCLSelect->Reset();
		
		OnResize();
		pUpdateInfoBubble( x, y );
	}
}

void meViewEditorSelect::OnRightMouseButtonPress( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnRightMouseButtonPress( x, y, shift, control );
	
	pInfoBubble->Hide();
}

void meViewEditorSelect::OnRightMouseButtonRelease( int x, int y, bool shift, bool control ){
	meViewEditorNavigation::OnRightMouseButtonRelease( x, y, shift, control );
	
	pUpdateInfoBubble( x, y );
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
		
	}else{
		pUpdateInfoBubble( x, y );
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

void meViewEditorSelect::OnMousLeave(){
	meViewEditorNavigation::OnMousLeave();
	
	pInfoBubble->Hide();
}



// Private Functions
//////////////////////

void meViewEditorSelect::pCleanUp(){
	if( pCanvasSelect && pCanvasSelect->GetParentView() ){
		GetView().RemoveCanvas( pCanvasSelect );
	}
	
	if( pCLClosest ){
		delete pCLClosest;
	}
	if( pCLSelect ){
		delete pCLSelect;
	}
}

void meViewEditorSelect::pUpdateInfoBubble( int x, int y ){
	if( GetDragLeftMouseButton() || GetDragRightMouseButton() || ! pCLClosest ){
		return;
	}
	if( GetElementMode() != meWorldGuiParameters::eemObject ){
		return;
	}
	
	decLayerMask collisionCategory;
	collisionCategory.SetBit( meWorld::eclmEditing );
	
	decLayerMask collisionFilter;
	collisionFilter.SetBit( meWorld::eclmObjects );
	
	UpdateMatrices();
	
	pCLClosest->Reset();
	
	pCLClosest->SetTestObjects( true );
	
	meWorldGuiParameters &guiparams = GetWorldGuiParameters();
	const decDVector rayPosition = GetMatrixView().GetPosition();
	const decPoint size( GetViewWidth(), GetViewHeight() );
	const decVector rayDirection = GetActiveCamera().GetDirectionFor(
		size.x, size.y, x, y ) * guiparams.GetRectSelDistance();
	RayTestCollision( pCLClosest, rayPosition, rayDirection,
		decCollisionFilter( collisionCategory, collisionFilter ) );
	
	if( pCLClosest->GetHasHit() ){
		const meObject &object = *pCLClosest->GetHitObject();
		decString text;
		text.Format( "%s (%s)", object.GetClassName().GetString(), object.GetID().ToHexString().GetString() );
		const decPoint textSize( pInfoBubbleText->GetFont()->TextSize( text ) );
		
		pInfoBubbleText->SetText( text );
		pInfoBubbleText->SetSize( textSize );
		pInfoBubble->GetCanvasContent()->SetSize( textSize );
		
		decPoint position = decPoint( x + 32, y );
		meInfoBubble::ePlacement placement = meInfoBubble::epTopRight;
		
		if( position.x + textSize.x + 6 > size.x ){
			position.x = x - 32;
			placement = meInfoBubble::epTopLeft;
		}
		
		if( position.y - textSize.y - 6 < 0 ){
			position.y = y + 64;
			placement = placement == meInfoBubble::epTopRight
				? meInfoBubble::epBottomRight : meInfoBubble::epBottomLeft;
		}
		
		pInfoBubble->ShowAt( position, placement );
		
	}else{
		pInfoBubble->Hide();
	}
}
