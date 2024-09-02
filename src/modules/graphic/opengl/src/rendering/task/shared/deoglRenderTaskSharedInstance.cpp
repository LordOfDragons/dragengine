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
