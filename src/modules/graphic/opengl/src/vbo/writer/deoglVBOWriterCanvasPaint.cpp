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

#include "deoglVBOWriterCanvasPaint.h"
#include "../deoglSharedVBOBlock.h"

#include <dragengine/common/exceptions.h>



// Class deoglVBOWriterCanvasPaint
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVBOWriterCanvasPaint::deoglVBOWriterCanvasPaint( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pDataPoints( NULL ){
}

deoglVBOWriterCanvasPaint::~deoglVBOWriterCanvasPaint(){
}



// Management
///////////////

void deoglVBOWriterCanvasPaint::Reset( deoglSharedVBOBlock *vboBlock ){
	if( vboBlock ){
		pDataPoints = ( char* )vboBlock->GetData();
		
	}else{
		pDataPoints = NULL;
	}
}



void deoglVBOWriterCanvasPaint::WritePoint( const decVector2 &position ){
	if( ! pDataPoints ){
		DETHROW( deeInvalidParam );
	}
	
	GLfloat * const ptrPosition = ( GLfloat* )pDataPoints;
	ptrPosition[ 0 ] = ( GLfloat )position.x;
	ptrPosition[ 1 ] = ( GLfloat )position.y;
	
	pDataPoints += 8;
}
