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

#include "deoglOcclusionMesh.h"
#include "deoglROcclusionMesh.h"
#include "../../deGraphicOpenGl.h"

#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/common/exceptions.h>



// Class deoglOcclusionMesh
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionMesh::deoglOcclusionMesh( deGraphicOpenGl &ogl, const deOcclusionMesh &occlusionMesh ) :
pOgl( ogl ),
pOcclusionMesh( occlusionMesh ),
pROcclusionMesh( NULL )
{
	try{
		pROcclusionMesh = new deoglROcclusionMesh( ogl.GetRenderThread(), occlusionMesh );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglOcclusionMesh::~deoglOcclusionMesh(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglOcclusionMesh::pCleanUp(){
	if( pROcclusionMesh ){
		pROcclusionMesh->FreeReference();
	}
}
