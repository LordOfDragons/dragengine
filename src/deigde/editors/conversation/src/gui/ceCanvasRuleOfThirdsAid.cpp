/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceCanvasRuleOfThirdsAid.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/engine/igdeEngineController.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasPaintReference.h>



// Class ceCanvasRuleOfThirdsAid
///////////////////////////////

// Constructor, destructor
////////////////////////////

ceCanvasRuleOfThirdsAid::ceCanvasRuleOfThirdsAid( igdeEnvironment &environment ) :
pEnvironment( environment )
{
	pCreateCanvas();
}

ceCanvasRuleOfThirdsAid::~ceCanvasRuleOfThirdsAid(){
}



// Management
///////////////

void ceCanvasRuleOfThirdsAid::Resize(){
	// simply copy of the size of the parent to our canvas since we fully fill it
	if( ! pCanvasView->GetParentView() ){
		return;
	}
	
	pCanvasView->SetSize( pCanvasView->GetParentView()->GetSize() );
	
	pCreateAid();
}

void ceCanvasRuleOfThirdsAid::SetVisible( bool visible ){
	if( pCanvasView ){
		pCanvasView->SetVisible( visible );
	}
}



// Private Functions
//////////////////////

void ceCanvasRuleOfThirdsAid::pCreateCanvas(){
	pCanvasView.TakeOver( pEnvironment.GetEngineController()->GetEngine()->GetCanvasManager()->CreateCanvasView() );
	pCanvasView->SetSize( decPoint( 100, 100 ) );
	pCanvasView->SetVisible( false );
	pCanvasView->SetTransparency( 0.5f );
	pCanvasView->SetOrder( 20.0f );
	
	pCreateAid();
}

void ceCanvasRuleOfThirdsAid::pCreateAid(){
	const decPoint &size = pCanvasView->GetSize();
	const int thirdX1 = size.x / 3;
	const int thirdX2 = size.x * 2 / 3;
	const int thirdY1 = size.y / 3;
	const int thirdY2 = size.y * 2 / 3;
	
	pCanvasView->RemoveAllCanvas();
	
	pAddLine( 0, thirdY1, size.x, thirdY1 );
	pAddLine( 0, thirdY2, size.x, thirdY2 );
	pAddLine( thirdX1, 0, thirdX1, size.y );
	pAddLine( thirdX2, 0, thirdX2, size.y );
	
	pAddFocus( thirdX1, thirdY1 );
	pAddFocus( thirdX2, thirdY1 );
	pAddFocus( thirdX1, thirdY2 );
	pAddFocus( thirdX2, thirdY2 );
}

void ceCanvasRuleOfThirdsAid::pAddLine( int x1, int y1, int x2, int y2 ){
	const int lineWidth = 3;
	const int strippleSize = lineWidth;
	const int strippleGap = lineWidth;
	const int strippleStride = strippleSize + strippleGap;
	const float sinkInner = ( float )strippleStride;
	const float sinkOuter = ( float )strippleStride * 4.0f;
	const decColor strippleColor( 1.0f, 1.0f, 1.0f );
	const float alphaNonSink = 1.0f;
	const float alphaSink = 0.2f;
	float sinkBlend;
	
	if( x1 == x2 ){
		const int strippleX1 = x1 - lineWidth / 2;
		const int strippleX2 = strippleX1 + lineWidth;
		const float sink1 = ( float )( y1 + ( y2 - y1 ) ) / 3.0f;
		const float sink2 = ( float )( y1 + ( y2 - y1 ) ) * 2.0f / 3.0f;
		int y;
		
		for( y=y1; y<y2; y+=strippleStride ){
			sinkBlend = decMath::linearStep( fabsf( ( float )y - sink1 ), sinkInner, sinkOuter );
			sinkBlend *= decMath::linearStep( fabsf( ( float )y - sink2 ), sinkInner, sinkOuter );
			
			pAddRect( strippleX1, y, strippleX2, y + strippleSize, strippleColor,
				decMath::mix( alphaSink, alphaNonSink, sinkBlend ) );
		}
		
	}else{
		const int strippleY1 = y1 - lineWidth / 2;
		const int strippleY2 = strippleY1 + lineWidth;
		const float sink1 = ( float )( x1 + ( x2 - x1 ) ) / 3.0f;
		const float sink2 = ( float )( x1 + ( x2 - x1 ) ) * 2.0f / 3.0f;
		int x;
		
		for( x=x1; x<x2; x+=strippleStride ){
			sinkBlend = decMath::linearStep( fabsf( ( float )x - sink1 ), sinkInner, sinkOuter );
			sinkBlend *= decMath::linearStep( fabsf( ( float )x - sink2 ), sinkInner, sinkOuter );
			
			pAddRect( x, strippleY1, x + strippleSize, strippleY2, strippleColor,
				decMath::mix( alphaSink, alphaNonSink, sinkBlend ) );
		}
	}
}

void ceCanvasRuleOfThirdsAid::pAddFocus( int x, int y ){
	const int lineWidth = 3;
	const int focusThickness = 2;
	const int focusSize = lineWidth * 3 + focusThickness;
	const decColor focusColor( 1.0f, 1.0f, 1.0f );
	const int focusX1 = x - focusSize / 2;
	const int focusY1 = y - focusSize / 2;
	const int focusX2 = focusX1 + focusSize;
	const int focusY2 = focusY1 + focusSize;
	
	pAddRect( focusX1, focusY1, focusX2, focusY1 + focusThickness, focusColor, 1.0f );
	pAddRect( focusX1, focusY1 + focusThickness, focusX1 + focusThickness, focusY2, focusColor, 1.0f );
	pAddRect( focusX1 + focusThickness, focusY2 - focusThickness, focusX2, focusY2, focusColor, 1.0f );
	pAddRect( focusX2 - focusThickness, focusY1 + focusThickness, focusX2, focusY2 - focusThickness, focusColor, 1.0f );
}

void ceCanvasRuleOfThirdsAid::pAddRect( int x1, int y1, int x2, int y2, const decColor &color, float transparency ){
	deCanvasPaintReference canvasPaint;
	canvasPaint.TakeOver( pEnvironment.GetEngineController()->GetEngine()->GetCanvasManager()->CreateCanvasPaint() );
	canvasPaint->SetShapeType( deCanvasPaint::estRectangle );
	canvasPaint->SetThickness( 0 );
	canvasPaint->SetLineColor( decColor( color, 0.0f ) /*color*/ );
	canvasPaint->SetFillColor( color );
	canvasPaint->SetOrder( pCanvasView->GetCanvasCount() );
	canvasPaint->SetTransparency( transparency );
	canvasPaint->SetPosition( decPoint( x1, y1 ) );
	canvasPaint->SetSize( decPoint( x2 - x1, y2 - y1 ) );
	pCanvasView->AddCanvas( canvasPaint );
}
