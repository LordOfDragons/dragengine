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
#include <stdint.h>

#include "deoglVBOAttribute.h"
#include "../deoglBasics.h"

#include <dragengine/common/exceptions.h>



// Class deoglVBOAttribute
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVBOAttribute::deoglVBOAttribute() :
pComponentCount(0),
pDataType(edtFloat),
pOffset(0){
}

deoglVBOAttribute::~deoglVBOAttribute(){
}



// Management
///////////////

void deoglVBOAttribute::SetComponentCount(int componentCount){
	if(componentCount < 0 || componentCount > 4){
		DETHROW(deeInvalidParam);
	}
	pComponentCount = componentCount;
}

void deoglVBOAttribute::SetDataType(eDataTypes dataType){
	pDataType = dataType;
}

void deoglVBOAttribute::SetOffset(int offset){
	if(offset < 0){
		DETHROW(deeInvalidParam);
	}
	pOffset = offset;
}



void deoglVBOAttribute::SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int stride) const{
	SetVAOAttributeAt(renderThread, attribute, stride, 0);
}

void deoglVBOAttribute::SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int stride, int offset) const{
	if(attribute < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(pComponentCount == 0){
		return;
	}
	
	const GLvoid * const pointer = (const GLvoid *)(intptr_t)(offset + (int)pOffset);
	
	OGL_CHECK(renderThread, pglEnableVertexAttribArray(attribute));
	
	switch(pDataType){
	case edtFloat:
		OGL_CHECK(renderThread, pglVertexAttribPointer(attribute,
			pComponentCount, GL_FLOAT, GL_FALSE, stride, pointer));
		break;
		
	case edtByte:
		OGL_CHECK(renderThread, pglVertexAttribPointer(attribute,
			pComponentCount, GL_BYTE, GL_TRUE, stride, pointer));
		break;
		
	case edtUByte:
		OGL_CHECK(renderThread, pglVertexAttribPointer(attribute,
			pComponentCount, GL_UNSIGNED_BYTE, GL_TRUE, stride, pointer));
		break;
		
	case edtShort:
		OGL_CHECK(renderThread, pglVertexAttribPointer(attribute,
			pComponentCount, GL_SHORT, GL_TRUE, stride, pointer));
		break;
		
	case edtUShort:
		OGL_CHECK(renderThread, pglVertexAttribPointer(attribute,
			pComponentCount, GL_UNSIGNED_SHORT, GL_TRUE, stride, pointer));
		break;
		
	case edtInt:
		OGL_CHECK(renderThread, pglVertexAttribPointer(attribute,
			pComponentCount, GL_INT, GL_TRUE, stride, pointer));
		break;
		
	case edtUInt:
		OGL_CHECK(renderThread, pglVertexAttribPointer(attribute,
			pComponentCount, GL_UNSIGNED_INT, GL_TRUE, stride, pointer));
		break;
		
	case edtIByte:
		OGL_CHECK(renderThread, pglVertexAttribIPointer(attribute,
			pComponentCount, GL_BYTE, stride, pointer));
		break;
		
	case edtIUByte:
		OGL_CHECK(renderThread, pglVertexAttribIPointer(attribute,
			pComponentCount, GL_UNSIGNED_BYTE, stride, pointer));
		break;
		
	case edtIShort:
		OGL_CHECK(renderThread, pglVertexAttribIPointer(attribute,
			pComponentCount, GL_SHORT, stride, pointer));
		break;
		
	case edtIUShort:
		OGL_CHECK(renderThread, pglVertexAttribIPointer(attribute,
			pComponentCount, GL_UNSIGNED_SHORT, stride, pointer));
		break;
		
	case edtIInt:
		OGL_CHECK(renderThread, pglVertexAttribIPointer(attribute,
			pComponentCount, GL_INT, stride, pointer));
		break;
		
	case edtIUInt:
		OGL_CHECK(renderThread, pglVertexAttribIPointer(attribute,
			pComponentCount, GL_UNSIGNED_INT, stride, pointer));
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}



// Operators
//////////////

deoglVBOAttribute &deoglVBOAttribute::operator=(const deoglVBOAttribute &attribute){
	pComponentCount = attribute.pComponentCount;
	pDataType = attribute.pDataType;
	pOffset = attribute.pOffset;
	return *this;
}

bool deoglVBOAttribute::operator==(const deoglVBOAttribute &attribute) const{
	return pComponentCount == attribute.pComponentCount
		&& pDataType == attribute.pDataType
		&& pOffset == attribute.pOffset;
}

bool deoglVBOAttribute::operator!=(const deoglVBOAttribute &attribute) const{
	return pComponentCount != attribute.pComponentCount
		|| pDataType != attribute.pDataType
		|| pOffset != attribute.pOffset;
}
