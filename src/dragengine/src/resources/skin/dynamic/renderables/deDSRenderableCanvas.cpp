/* 
 * Drag[en]gine Game Engine
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

#include "deDSRenderableCanvas.h"
#include "deDSRenderableVisitor.h"
#include "../../../canvas/deCanvasView.h"
#include "../../../../common/exceptions.h"



// Class deDSRenderableCanvas
///////////////////////////////

// Constructor, destructor
////////////////////////////

deDSRenderableCanvas::deDSRenderableCanvas( const char *name ) :
deDSRenderable( name ),
pComponentCount( 3 ),
pBitCount( 8 ){
}

deDSRenderableCanvas::~deDSRenderableCanvas(){
}



// Management
///////////////

void deDSRenderableCanvas::SetComponentCount( int componentCount ){
	if( componentCount < 1 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	pComponentCount = componentCount;
}

void deDSRenderableCanvas::SetBitCount( int bitCount ){
	if( bitCount != 8 && bitCount != 16 && bitCount != 32 ){
		DETHROW( deeInvalidParam );
	}
	pBitCount = bitCount;
}

void deDSRenderableCanvas::SetCanvas( deCanvasView *canvas ){
	pCanvas = canvas;
}



// Visiting
/////////////

void deDSRenderableCanvas::Visit( deDSRenderableVisitor &visitor ){
	visitor.VisitCanvas( *this );
}
