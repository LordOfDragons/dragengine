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

#include "deoglVBOLayout.h"
#include "deoglVBOAttribute.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglVBOLayout
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglVBOLayout::deoglVBOLayout() :
pSize(0),
pStride(0),
pIndexType(eitNone),
pIndexSize(0),
pIndexGLType(GL_NONE){
}

deoglVBOLayout::deoglVBOLayout(const deoglVBOLayout &layout) :
pSize(layout.pSize),
pStride(layout.pStride),
pAttributes(layout.pAttributes),
pIndexType(layout.pIndexType),
pIndexSize(layout.pIndexSize),
pIndexGLType(layout.pIndexGLType){
}

deoglVBOLayout::~deoglVBOLayout() = default;



// Management
///////////////

void deoglVBOLayout::SetSize(int size){
	DEASSERT_TRUE(size >= 0)
	pSize = size;
}

void deoglVBOLayout::SetStride(int stride){
	DEASSERT_TRUE(stride >= 0)
	pStride = stride;
}

void deoglVBOLayout::SetIndexType(eIndexTypes indexType){
	pIndexType = indexType;
	
	switch(indexType){
	case eitUnsignedInt:
		pIndexSize = 4;
		pIndexGLType = GL_UNSIGNED_INT;
		break;
		
	case eitUnsignedShort:
		pIndexSize = 2;
		pIndexGLType = GL_UNSIGNED_SHORT;
		break;
		
	case eitUnsignedByte:
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



// Attributes
///////////////

void deoglVBOLayout::SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int target){
	pAttributes[attribute].SetVAOAttributeAt(renderThread, target, pStride);
}

void deoglVBOLayout::SetVAOAttributeAt(deoglRenderThread &renderThread, int attribute, int target, int offset){
	pAttributes[attribute].SetVAOAttributeAt(renderThread, target, pStride, offset);
}



// Operators
//////////////

deoglVBOLayout &deoglVBOLayout::operator=(const deoglVBOLayout &layout){
	pSize = layout.pSize;
	pStride = layout.pStride;
	pIndexType = layout.pIndexType;
	pIndexSize = layout.pIndexSize;
	pIndexGLType = layout.pIndexGLType;
	pAttributes = layout.pAttributes;
	
	return *this;
}

bool deoglVBOLayout::operator==(const deoglVBOLayout &layout) const{
	return layout.pStride == pStride && layout.pAttributes == pAttributes;
}


// Debugging
//////////////

void deoglVBOLayout::PrintToConsole(deoglRenderThread &renderThread, const char* name){
	const char * const typeNames[] = {
		"float", "byte", "ubyte", "short", "ushort", "int", "uint",
		"ibyte", "iubyte", "ishort", "iushort", "iint", "iuint"};
	const char * const indexTypeNames[] = {"none", "uint", "ushort", "ubyte"};
	
	renderThread.GetLogger().LogInfoFormat("%s VBO Layout: size=%i stride=%i", name, pSize, pStride);
	
	pAttributes.VisitIndexed([&](int i, const deoglVBOAttribute &a){
		renderThread.GetLogger().LogInfoFormat(
			"- attribute %i: components=%i type=%s offset=%i indexType=%s indexSize=%i",
			i, a.GetComponentCount(), typeNames[a.GetDataType()], a.GetOffset(),
			indexTypeNames[pIndexType], pIndexSize);
	});
}
