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

#include "deoglCanvasView.h"
#include "deoglCanvasCanvasView.h"
#include "render/deoglRCanvasCanvasView.h"
#include "render/deoglRCanvasView.h"
#include "../deGraphicOpenGl.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasCanvasView.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class deoglCanvasRenderTarget
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasCanvasView::deoglCanvasCanvasView( deGraphicOpenGl &ogl, deCanvasCanvasView &canvas ) :
deoglCanvas( ogl, canvas ),
pCanvasCanvasView( canvas ),
pRCanvasCanvasView( NULL ),
pCanvasView( NULL ){
}

deoglCanvasCanvasView::~deoglCanvasCanvasView(){
	if( pCanvasView ){
		pCanvasView->RemoveListener( this );
	}
}



// Management
///////////////

void deoglCanvasCanvasView::DropRCanvas(){
	pRCanvasCanvasView = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasCanvasView::SyncContentToRender(){
	if( pCanvasView ){
		pCanvasView->SyncToRender();
		pRCanvasCanvasView->SetCanvasView( pCanvasView->GetRCanvasView() );
		
	}else{
		pRCanvasCanvasView->SetCanvasView( NULL );
	}
	
	const float repeatScaleU = ( float )pCanvasCanvasView.GetRepeatX();
	const float repeatScaleV = ( float )pCanvasCanvasView.GetRepeatY();
	pRCanvasCanvasView->SetTCTransform( decTexMatrix2::CreateScale( repeatScaleU, repeatScaleV ) );
	pRCanvasCanvasView->SetTCClampMaximum( decVector2( repeatScaleU, repeatScaleV ) );
}

void deoglCanvasCanvasView::CanvasViewDestroyed(){
	pCanvasView = NULL;
}

void deoglCanvasCanvasView::CanvasViewRequiresSync(){
	SetDirtyParentPaint();
}



// Notifications
//////////////////

void deoglCanvasCanvasView::ContentChanged(){
	if( pCanvasView ){
		pCanvasView->RemoveListener( this );
	}
	
	if( pCanvasCanvasView.GetCanvasView() ){
		pCanvasView = ( deoglCanvasView* )pCanvasCanvasView.GetCanvasView()->GetPeerGraphic();
		pCanvasView->AddListener( this );
		
	}else{
		pCanvasView = NULL;
	}
	
	deoglCanvas::ContentChanged();
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasCanvasView::CreateRCanvas(){
	pRCanvasCanvasView = new deoglRCanvasCanvasView( GetOgl().GetRenderThread() );
	return pRCanvasCanvasView;
}
