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

#include "deoglCanvas.h"
#include "deoglCanvasView.h"
#include "render/deoglRCanvas.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class deoglCanvas
//////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvas::deoglCanvas( deGraphicOpenGl &ogl, deCanvas &canvas ) :
pOgl( ogl ),
pCanvas( canvas ),

pRCanvas( NULL ),
pDirtyGeometry( true ),
pDirtyTransform( true ),
pDirtyColorTransform( true ),
pDirtyParameters( true ),
pDirtyContent( true ),
pDirtyParentPaint( false ),
pDirtyMaskContent( false )
{
}

deoglCanvas::~deoglCanvas(){
	if( pRCanvas ){
		pRCanvas->FreeReference();
	}
}



// Management
///////////////

void deoglCanvas::DropRCanvas(){
	if( pRCanvas ){
		pRCanvas->FreeReference();
		pRCanvas = NULL;
	}
}

void deoglCanvas::SetDirtyParentPaint(){
	if( pDirtyParentPaint ){
		return;
	}
	
	pDirtyParentPaint = true;
	
	if( pCanvas.GetParentMask() ){
		( ( deoglCanvas* )pCanvas.GetParentMask()->GetPeerGraphic() )->SetDirtyMaskContent();
	}
	if( pCanvas.GetParentView() ){
		( ( deoglCanvasView* )pCanvas.GetParentView()->GetPeerGraphic() )->SetDirtyPaint();
	}
}

void deoglCanvas::SetDirtyMaskContent(){
	pDirtyMaskContent = true;
	SetDirtyParentPaint();
	
	if( pRCanvas ){
		pRCanvas->DirtyMaskRenderTarget();
	}
}



void deoglCanvas::SyncToRender(){
	if( ! pRCanvas ){
		pRCanvas = CreateRCanvas();
		if( ! pRCanvas ){
			DETHROW( deeInvalidParam );
		}
		
		pDirtyTransform = true;
		pDirtyColorTransform = true;
		pDirtyParameters = true;
		pDirtyContent = true;
	}
	
	if( pDirtyGeometry ){
		pDirtyTransform = true; // position is calculated into the transform
		
		pRCanvas->SetPosition( decVector2( pCanvas.GetPosition() ) );
		pRCanvas->SetSize( decVector2( pCanvas.GetSize() ) );
		pDirtyGeometry = false;
	}
	
	if( pDirtyTransform ){
		pDirtyTransform = false;
		
		pRCanvas->SetTransform( pCanvas.GetTransform()
			* decTexMatrix2::CreateTranslation( decVector2( pCanvas.GetPosition() ) ) );
	}
	
	if( pDirtyColorTransform ){
		pDirtyColorTransform = false;
		
		pRCanvas->SetColorTransform( pCanvas.GetColorTransform() );
	}
	
	if( pDirtyParameters ){
		pDirtyParameters = false;
		
		pRCanvas->SetOrder( pCanvas.GetOrder() );
		pRCanvas->SetTransparency( pCanvas.GetTransparency() );
		pRCanvas->SetVisible( pCanvas.GetVisible() );
		pRCanvas->SetBlendMode( pCanvas.GetBlendMode() );
		
		if( pCanvas.GetMask() ){
			deoglCanvas &canvas = *( ( deoglCanvas* )pCanvas.GetMask()->GetPeerGraphic() );
			canvas.SyncToRender();
			pRCanvas->SetMask( canvas.pRCanvas );
			
		}else{
			pRCanvas->SetMask( nullptr );
		}
	}
	
	if( pDirtyContent ){
		pDirtyContent = false;
		SyncContentToRender();
	}
	
	pDirtyParentPaint = false;
}

void deoglCanvas::SyncContentToRender(){
}



// Notifications
//////////////////

void deoglCanvas::PositionChanged(){
	pDirtyGeometry = true;
	SetDirtyParentPaint();
}

void deoglCanvas::SizeChanged(){
	pDirtyGeometry = true;
	SetDirtyParentPaint();
}

void deoglCanvas::TransformChanged(){
	pDirtyTransform = true;
	SetDirtyParentPaint();
}

void deoglCanvas::ColorTransformChanged(){
	pDirtyColorTransform = true;
	SetDirtyParentPaint();
}

void deoglCanvas::VisibleChanged(){
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::OrderChanged(){
	deCanvasView * const parentView = pCanvas.GetParentView();
	if( parentView ){
		( ( deoglCanvasView* )parentView->GetPeerGraphic() )->ChildOrderChanged();
	}
	
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::TransparencyChanged(){
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::BlendModeChanged(){
	pDirtyParameters = true;
	SetDirtyParentPaint();
}

void deoglCanvas::MaskChanged(){
	pDirtyParameters = true;
	pDirtyContent = true;
	SetDirtyParentPaint();
}

void deoglCanvas::ContentChanged(){
	pDirtyContent = true;
	SetDirtyParentPaint();
}
