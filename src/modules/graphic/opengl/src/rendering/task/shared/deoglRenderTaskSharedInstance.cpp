/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglRenderTaskSharedPool.h"
#include "deoglRenderTaskSharedInstance.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBElement.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskSharedInstance
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskSharedInstance::deoglRenderTaskSharedInstance( deoglRenderTaskSharedPool &pool, int index ) :
pPool( pool ),
pIndex( index ),

pParamBlock( NULL ),
pParamBlockSpecial( NULL ),

pFirstPoint( 0 ),
pPointCount( 0 ),
pFirstIndex( 0 ),
pIndexCount( 0 ),
pSubInstanceCount( 0 ),
pSubInstanceSPB( NULL ),
pPrimitiveType( GL_TRIANGLES ),
pTessPatchVertexCount( 3 ){
}

deoglRenderTaskSharedInstance::~deoglRenderTaskSharedInstance(){
}



// Management
///////////////

void deoglRenderTaskSharedInstance::SetParameterBlock( deoglShaderParameterBlock *block ){
	pParamBlock = block;
}

void deoglRenderTaskSharedInstance::SetParameterBlockSpecial( deoglShaderParameterBlock *block ){
	pParamBlockSpecial = block;
}

void deoglRenderTaskSharedInstance::SetFirstPoint( int firstPoint ){
	pFirstPoint = firstPoint;
}

void deoglRenderTaskSharedInstance::SetPointCount( int pointCount ){
	pPointCount = pointCount;
}

void deoglRenderTaskSharedInstance::SetFirstIndex( int firstIndex ){
	pFirstIndex = firstIndex;
}

void deoglRenderTaskSharedInstance::SetIndexCount( int indexCount ){
	pIndexCount = indexCount;
}

void deoglRenderTaskSharedInstance::SetSubInstanceCount( int subInstanceCount ){
	pSubInstanceCount = subInstanceCount;
}

void deoglRenderTaskSharedInstance::SetSubInstanceSPB( deoglSharedSPB *spb ){
	pSubInstanceSPB = spb;
}

void deoglRenderTaskSharedInstance::SetPrimitiveType( GLenum primitiveType ){
	pPrimitiveType = primitiveType;
}

void deoglRenderTaskSharedInstance::SetTessPatchVertexCount( int count ){
	pTessPatchVertexCount = count;
}



void deoglRenderTaskSharedInstance::Clear(){
	pParamBlock = NULL;
	pParamBlockSpecial = NULL;
	
	pFirstPoint = 0;
	pPointCount = 0;
	pFirstIndex = 0;
	pIndexCount = 0;
	pSubInstanceCount = 0;
	pSubInstanceSPB = NULL;
	pPrimitiveType = GL_TRIANGLES;
	pTessPatchVertexCount = 3;
}

void deoglRenderTaskSharedInstance::ReturnToPool(){
	pPool.ReturnInstance( this );
}
