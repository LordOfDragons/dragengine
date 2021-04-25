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

#include "deoglVAO.h"
#include "../vbo/deoglVBOLayout.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTUniqueKey.h"

#include <dragengine/common/exceptions.h>



// Class deoglVAO
///////////////////

// Constructor, destructor
////////////////////////////

deoglVAO::deoglVAO( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
	pVAO = 0;
	
	pIndexSize = 0;
	pIndexGLType = GL_NONE;
	
	pRenderTaskTrackingNumber = 0;
	pRenderTaskVAOIndex = 0;
	
	OGL_CHECK( renderThread, pglGenVertexArrays( 1, &pVAO ) );
	if( ! pVAO ){
		DETHROW( deeOutOfMemory );
	}
	
	pUniqueKey = renderThread.GetUniqueKey().Get();
}

deoglVAO::~deoglVAO(){
	if( pVAO ){
		pglDeleteVertexArrays( 1, &pVAO );
	}
	
	pRenderThread.GetUniqueKey().Return( pUniqueKey );
}



// Management
///////////////

void deoglVAO::SetIndexType( deoglVBOLayout::eIndexTypes indexType ){
	pIndexType = indexType;
	
	switch( indexType ){
	case deoglVBOLayout::eitUnsignedInt:
		pIndexSize = 4;
		pIndexGLType = GL_UNSIGNED_INT;
		break;
		
	case deoglVBOLayout::eitUnsignedShort:
		pIndexSize = 2;
		pIndexGLType = GL_UNSIGNED_SHORT;
		break;
		
	case deoglVBOLayout::eitUnsignedByte:
		pIndexSize = 1;
		pIndexGLType = GL_UNSIGNED_BYTE;
		break;
		
	default:
		// the values here are chosen this way to prevent a crash if by mistake indices are used although
		// eitNone has been specified in the layout. an index size of 0 causes allocation to fail should
		// a check be missing for pIndexSize equal to 0. also GL_NONE causes rendering to fail with an
		// opengl error instead of crashing
		pIndexSize = 0;
		pIndexGLType = GL_NONE;
	}
}



void deoglVAO::SetRenderTaskTrackingNumber( unsigned int trackingNumber ){
	pRenderTaskTrackingNumber = trackingNumber;
}

void deoglVAO::SetRenderTaskVAOIndex( int vaoIndex ){
	pRenderTaskVAOIndex = vaoIndex;
}
