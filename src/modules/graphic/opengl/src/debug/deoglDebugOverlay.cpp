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
#include <string.h>
#include <stdlib.h>

#include "deoglDebugOverlay.h"
#include "../deGraphicOpenGl.h"
#include "../canvas/deoglCanvas.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTContext.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/systems/deGraphicSystem.h>



// Class deoglDebugOverlay
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugOverlay::deoglDebugOverlay( deGraphicOpenGl &ogl ) :
pOgl( ogl ){
}

deoglDebugOverlay::~deoglDebugOverlay(){
}



// Management
///////////////

void deoglDebugOverlay::PrepareOverlay( deCanvasView &canvasView ){
	deoglRenderThread &renderThread = pOgl.GetRenderThread();
	
	if( ! canvasView.GetRootCanvas() ){
		renderThread.SetCanvasDebugOverlay( NULL );
		return;
	}
	
	const deRenderWindow * const renderWindow = pOgl.GetGameEngine()->GetGraphicSystem()->GetRenderWindow();
	if( ! renderWindow ){
		renderThread.SetCanvasDebugOverlay( NULL );
		return;
	}
	
	canvasView.SetSize( decPoint( renderWindow->GetWidth(), renderWindow->GetHeight() ) );
	
	pSortViews( canvasView );
	pAlignViews();
	
	deoglCanvas &oglCanvas = *( ( deoglCanvas* )canvasView.GetPeerGraphic() );
	oglCanvas.SyncToRender();
	
	renderThread.SetCanvasDebugOverlay( oglCanvas.GetRCanvas() );
}



// Private Functions
//////////////////////

void deoglDebugOverlay::pSortViews( deCanvasView &canvasView ){
	if( canvasView.GetCanvasCount() == 0 ){
		pList1.RemoveAll();
		pList2.RemoveAll();
		return;
	}
	
	pList1.RemoveAll();
	
	deCanvas *childCanvas = canvasView.GetRootCanvas();
	while( childCanvas ){
		pList1.Add( childCanvas );
		childCanvas = childCanvas->GetLLViewNext();
	}
	
	pList2.RemoveAll();
	float bestOrder;
	int i;
	
	while( pList1.GetCount() > 0 ){
		const int count = pList1.GetCount();
		int bestIndex = -1;
		
		for( i=0; i<count; i++ ){
			const float order = ( ( deCanvas* )pList1.GetAt( i ) )->GetOrder();
			if( bestIndex == -1 || order < bestOrder ){
				bestOrder = order;
				bestIndex = i;
			}
		}
		
		pList2.Add( pList1.GetAt( bestIndex ) );
		pList1.RemoveFrom( bestIndex );
	}
}

void deoglDebugOverlay::pAlignViews(){
	// for the time being just take each canvas and align it from left to right using
	// the order value to sort them
	decPoint position( 5, 5 );
	
	const int count = pList2.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deCanvas &canvas = *( ( deCanvas* )pList2.GetAt( i ) );
		canvas.SetPosition( position );
		position.x += canvas.GetSize().x + 5;
	}
}
