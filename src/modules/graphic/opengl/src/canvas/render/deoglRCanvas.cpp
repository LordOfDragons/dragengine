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

#include "deoglRCanvas.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvas
///////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvas::deoglRCanvas( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pOrder( 0.0f ),
pTransparency( 1.0f ),
pBlendSrc( GL_SRC_ALPHA ),
pBlendDest( GL_ONE_MINUS_SRC_ALPHA ),
pVisible( true ){
}

deoglRCanvas::~deoglRCanvas(){
}



// Management
///////////////

void deoglRCanvas::SetPosition( const decVector2 &position ){
	pPosition = position;
}

void deoglRCanvas::SetSize( const decVector2 &size ){
	pSize = size;
}

void deoglRCanvas::SetTransform( const decTexMatrix2 &transform ){
	pTransform = transform;
}

void deoglRCanvas::SetColorTransform( const decColorMatrix &transform ){
	pColorTransform = transform;
}

void deoglRCanvas::SetOrder( float order ){
	pOrder = order;
}

void deoglRCanvas::SetTransparency( float transparency ){
	pTransparency = transparency;
}

void deoglRCanvas::SetBlendSrc( GLenum blendSrc ){
	pBlendSrc = blendSrc;
}

void deoglRCanvas::SetBlendDest( GLenum blendDest ){
	pBlendDest = blendDest;
}

void deoglRCanvas::SetVisible( bool visible ){
	pVisible = visible;
}



void deoglRCanvas::PrepareForRender(){
}
