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

deoglSPBParameter::deoglSPBParameter(eValueTypes valueType, int componentCount,
	int vectorCount, int arrayCount) :
deoglSPBParameter()
{
	SetValueType(valueType);
	SetComponentCount(componentCount);
	SetVectorCount(vectorCount);
	SetArrayCount(arrayCount);
}

deoglSPBParameter::deoglSPBParameter(const deoglSPBParameter &parameter){
	*this = parameter;
}

deoglSPBParameter &deoglSPBParameter::operator=(const deoglSPBParameter &parameter){
	if(this != &parameter){
		pValueType = parameter.pValueType;
		pComponentCount = parameter.pComponentCount;
		pVectorCount = parameter.pVectorCount;
		pArrayCount = parameter.pArrayCount;
		pOffset = parameter.pOffset;
		pStride = parameter.pStride;
		pArrayStride = parameter.pArrayStride;
		pDataSize = parameter.pDataSize;
		pMembers = parameter.pMembers;
	}
	
	return *this;
}

deoglSPBParameter::deoglSPBParameter(deoglSPBParameter &&parameter) noexcept{
	*this = std::move(parameter);
}

deoglSPBParameter &deoglSPBParameter::operator=(deoglSPBParameter &&parameter) noexcept{
	if(this != &parameter){
		pValueType = parameter.pValueType;
		pComponentCount = parameter.pComponentCount;
		pVectorCount = parameter.pVectorCount;
		pArrayCount = parameter.pArrayCount;
		pOffset = parameter.pOffset;
		pStride = parameter.pStride;
		pArrayStride = parameter.pArrayStride;
		pDataSize = parameter.pDataSize;
		pMembers = std::move(parameter.pMembers);
	}
	
	return *this;
}

deoglSPBParameter::~deoglSPBParameter() = default;


// Management
///////////////

void deoglSPBParameter::SetValueType(eValueTypes valueType){
	pValueType = valueType;
}

void deoglSPBParameter::SetComponentCount(int componentCount){
	DEASSERT_TRUE(componentCount >= 1)
	DEASSERT_TRUE(componentCount <= 4)
	
	pComponentCount = componentCount;
}

void deoglSPBParameter::SetVectorCount(int vectorCount){
	DEASSERT_TRUE(vectorCount >= 1)
	
	pVectorCount = vectorCount;
}

void deoglSPBParameter::SetArrayCount(int arrayCount){
	DEASSERT_TRUE(arrayCount >= 1)
	
	pArrayCount = arrayCount;
}

void deoglSPBParameter::SetAll(eValueTypes valueType, int componentCount,
int vectorCount, int arrayCount){
	SetValueType(valueType);
	SetComponentCount(componentCount);
	SetVectorCount(vectorCount);
	SetArrayCount(arrayCount);
}

void deoglSPBParameter::SetStruct(const ParameterList &members, int arrayCount){
	SetValueType(evtStruct);
	SetComponentCount(1);
	SetVectorCount(1);
	SetArrayCount(arrayCount);
	pMembers = members;
}

void deoglSPBParameter::SetStruct(ParameterList &&members, int arrayCount){
	SetValueType(evtStruct);
	SetComponentCount(1);
	SetVectorCount(1);
	SetArrayCount(arrayCount);
	pMembers = std::move(members);
}

void deoglSPBParameter::SetOffset(int offset){
	DEASSERT_TRUE(offset >= 0)
	
	pOffset = offset;
}

void deoglSPBParameter::SetStride(int stride){
	DEASSERT_TRUE(stride >= 0)
	
	pStride = stride;
}

void deoglSPBParameter::SetArrayStride(int arrayStride){
	DEASSERT_TRUE(arrayStride >= 0)
	
	pArrayStride = arrayStride;
}

void deoglSPBParameter::SetDataSize(int size){
	DEASSERT_TRUE(size >= 0)
	
	pDataSize = size;
}
