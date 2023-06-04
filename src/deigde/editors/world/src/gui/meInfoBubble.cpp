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

#include "meInfoBubble.h"
#include "meView3D.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>



// Class meInfoBubble
///////////////////////

// Constructor, destructor
////////////////////////////

meInfoBubble::meInfoBubble( meView3D &view ) :
pView( view )
{
	deCanvasManager &manager = *view.GetEnvironment().GetEngineController()->GetEngine()->GetCanvasManager();
	
	pCanvasBubble.TakeOver( manager.CreateCanvasView() );
	pCanvasBubble->SetOrder( 1000.0f );
	pCanvasBubble->SetVisible( false );
	view.AddCanvas( pCanvasBubble );
	
	pCanvasBorder.TakeOver( manager.CreateCanvasPaint() );
	pCanvasBorder->SetShapeType( deCanvasPaint::estRectangle );
	pCanvasBorder->SetFillColor( decColor( 0.0f, 0.0f, 0.0f, 0.5f ) );
	pCanvasBorder->SetLineColor( decColor( 0.0f, 0.0f, 0.5f, 0.9f ) );
	pCanvasBorder->SetThickness( 1.0f );
	pCanvasBorder->SetOrder( 0.0f );
	pCanvasBubble->AddCanvas( pCanvasBorder );
	
	pCanvasContent.TakeOver( manager.CreateCanvasView() );
	pCanvasContent->SetOrder( 1.0f );
	pCanvasBubble->AddCanvas( pCanvasContent );
}

meInfoBubble::~meInfoBubble(){
	if( pCanvasBubble ){
		if( pCanvasBubble->GetParentView() ){
			pView.RemoveCanvas( pCanvasBubble );
		}
		pCanvasBubble->RemoveAllCanvas();
	}
}



// Management
///////////////

void meInfoBubble::ShowAt( const decPoint &position, ePlacement placement ){
	pResize();
	pCanvasBubble->SetPosition( pPlace( position, placement ) );
	pCanvasBubble->SetVisible( true );
}

void meInfoBubble::Hide(){
	pCanvasBubble->SetVisible( false );
}



// Private Functions
//////////////////////

void meInfoBubble::pResize(){
	const decPoint padding( 3, 3 );
	const decPoint size( pCanvasContent->GetSize() + padding * 2 );
	
	pCanvasContent->SetPosition( padding );
	pCanvasBorder->SetSize( size );
	pCanvasBubble->SetSize( size );
}

decPoint meInfoBubble::pPlace( const decPoint &position, ePlacement placement ){
	const decPoint &size = pCanvasBubble->GetSize();
	decPoint npos;
	
	switch( placement ){
	case epTopLeft:
		npos = position - size;
		break;
		
	case epTopRight:
		npos.x = position.x;
		npos.y = position.y - size.y;
		break;
		
	case epBottomLeft:
		npos.x = position.x - size.x;
		npos.y = position.y;
		break;
		
	case epBottomRight:
		npos = position;
		break;
	}
	
	return npos;
}
