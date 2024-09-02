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
