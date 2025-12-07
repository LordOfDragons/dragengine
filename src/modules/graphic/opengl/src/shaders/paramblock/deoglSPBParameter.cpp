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

#include "deoglSPBParameter.h"

#include <dragengine/common/exceptions.h>



// Class deoglSPBParameter
////////////////////////////

// Constructor, destructor
////////////////////////////


deoglSPBParameter::deoglSPBParameter() :
pValueType(evtFloat),
pComponentCount(1),
pVectorCount(1),
pArrayCount(1),

pOffset(0),
pStride(0),
pArrayStride(0),
pDataSize(0){
}

deoglSPBParameter::~deoglSPBParameter(){
}



// Management
///////////////

void deoglSPBParameter::SetValueType(eValueTypes valueType){
	if(valueType < evtFloat || valueType > evtBool){
		DETHROW(deeInvalidParam);
	}
	
	pValueType = valueType;
}

void deoglSPBParameter::SetComponentCount(int componentCount){
	if(componentCount < 1 || componentCount > 4){
		DETHROW(deeInvalidParam);
	}
	
	pComponentCount = componentCount;
}

void deoglSPBParameter::SetVectorCount(int vectorCount){
	if(vectorCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	pVectorCount = vectorCount;
}

void deoglSPBParameter::SetArrayCount(int arrayCount){
	if(arrayCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	pArrayCount = arrayCount;
}

void deoglSPBParameter::SetAll(eValueTypes valueType, int componentCount,
int vectorCount, int arrayCount){
	SetValueType(valueType);
	SetComponentCount(componentCount);
	SetVectorCount(vectorCount);
	SetArrayCount(arrayCount);
}



void deoglSPBParameter::SetOffset(int offset){
	if(offset < 0){
		DETHROW(deeInvalidParam);
	}
	
	pOffset = offset;
}

void deoglSPBParameter::SetStride(int stride){
	if(stride < 0){
		DETHROW(deeInvalidParam);
	}
	
	pStride = stride;
}

void deoglSPBParameter::SetArrayStride(int arrayStride){
	if(arrayStride < 0){
		DETHROW(deeInvalidParam);
	}
	
	pArrayStride = arrayStride;
}

void deoglSPBParameter::SetDataSize (int size){
	if(size < 0){
		DETHROW(deeInvalidParam);
	}
	
	pDataSize = size;
}
