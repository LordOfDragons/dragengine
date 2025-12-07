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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglShaderParameterBlock.h"
#include "deoglShaderParameterBlockList.h"

#include <dragengine/common/exceptions.h>



// Class deoglShaderParameterBlockList
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderParameterBlockList::deoglShaderParameterBlockList(){
}

deoglShaderParameterBlockList::~deoglShaderParameterBlockList(){
	pList.RemoveAll();
}



// Management
///////////////

int deoglShaderParameterBlockList::GetCount() const{
	return pList.GetCount();
}

deoglShaderParameterBlock *deoglShaderParameterBlockList::GetAt(int index) const{
	return (deoglShaderParameterBlock*)pList.GetAt(index);
}

int deoglShaderParameterBlockList::IndexOf(deoglShaderParameterBlock *block) const{
	return pList.IndexOf(block);
}

bool deoglShaderParameterBlockList::Has(deoglShaderParameterBlock *block) const{
	return pList.Has(block);
}

void deoglShaderParameterBlockList::Add(deoglShaderParameterBlock *block){
	pList.Add(block);
}

void deoglShaderParameterBlockList::Remove(deoglShaderParameterBlock *block){
	pList.RemoveFrom(pList.IndexOf(block));
}

void deoglShaderParameterBlockList::RemoveAll(){
	pList.RemoveAll();
}
