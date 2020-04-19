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

#include "deoglCanvasPaint.h"
#include "render/deoglRCanvasPaint.h"
#include "../deGraphicOpenGl.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>



// Class deoglCanvasPaint
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasPaint::deoglCanvasPaint( deGraphicOpenGl &ogl, deCanvasPaint &canvas ) :
deoglCanvas( ogl, canvas ),
pCanvasPaint( canvas ),
pRCanvasPaint( NULL ){
}

deoglCanvasPaint::~deoglCanvasPaint(){
}



// Management
///////////////

void deoglCanvasPaint::DropRCanvas(){
	pRCanvasPaint = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasPaint::SyncContentToRender(){
	pRCanvasPaint->SetShapeType( pCanvasPaint.GetShapeType() );
	pRCanvasPaint->SetLineColor( pCanvasPaint.GetLineColor() );
	pRCanvasPaint->SetFillColor( pCanvasPaint.GetFillColor() );
	pRCanvasPaint->SetThickness( pCanvasPaint.GetThickness() );
	pRCanvasPaint->SetRoundCornerX( pCanvasPaint.GetRoundCornerX() );
	pRCanvasPaint->SetRoundCornerY( pCanvasPaint.GetRoundCornerY() );
	pRCanvasPaint->SetStartAngle( pCanvasPaint.GetStartAngle() );
	pRCanvasPaint->SetEndAngle( pCanvasPaint.GetEndAngle() );
	
	const int pointCount = pCanvasPaint.GetPointCount();
	pRCanvasPaint->SetPointCount( pointCount );
	if( pointCount > 0 ){
		decVector2 * const points = pRCanvasPaint->GetPoints();
		int i;
		
		for( i=0; i<pointCount; i++ ){
			points[ i ] = pCanvasPaint.GetPointAt( i );
		}
	}
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasPaint::CreateRCanvas(){
	pRCanvasPaint = new deoglRCanvasPaint( GetOgl().GetRenderThread() );
	return pRCanvasPaint;
}
