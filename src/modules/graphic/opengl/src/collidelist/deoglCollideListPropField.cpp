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

#include "deoglCollideListPropField.h"
#include "deoglCollideListPropFieldType.h"
#include "../propfield/deoglRPropField.h"
#include "../propfield/deoglRPropFieldType.h"
#include "../propfield/deoglPropFieldCluster.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideListPropField
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideListPropField::deoglCollideListPropField() :
pPropField(nullptr){
}

deoglCollideListPropField::deoglCollideListPropField(deoglRPropField *propField) :
pPropField(propField){
}

deoglCollideListPropField::deoglCollideListPropField(deoglCollideListPropField &&other) noexcept :
pPropField(other.pPropField),
pTypes(std::move(other.pTypes))
{
	other.pPropField = nullptr;
}

deoglCollideListPropField &deoglCollideListPropField::operator=(deoglCollideListPropField &&other) noexcept{
	if(this != &other){
		pPropField = other.pPropField;
		other.pPropField = nullptr;
		
		pTypes.RemoveAll();
		if(other.pTypes.IsNotEmpty()){
			pTypes.Swap(other.pTypes);
		}
	}
	
	return *this;
}


// Management
///////////////

void deoglCollideListPropField::Clear(){
	RemoveAllTypes();
	SetPropField(nullptr);
}

void deoglCollideListPropField::StartOcclusionTest(deoglOcclusionTest &occlusionTest,
const decDVector &referencePosition){
	DEASSERT_NOTNULL(pPropField)
	
	const decVector offset(pPropField->GetPosition() - referencePosition);
	pTypes.Visit([&](deoglCollideListPropFieldType &type){
		type.StartOcclusionTest(occlusionTest, offset);
	});
}

bool deoglCollideListPropField::GetIsEmpty() const{
	return pTypes.AllMatching([](const deoglCollideListPropFieldType &type){
		return type.GetIsEmpty();
	});
}

void deoglCollideListPropField::SetPropField(deoglRPropField *propField){
	RemoveAllTypes();
	pPropField = propField;
}


deoglCollideListPropFieldType &deoglCollideListPropField::GetTypeAt(int index){
	return pTypes.GetAt(index);
}

const deoglCollideListPropFieldType &deoglCollideListPropField::GetTypeAt(int index) const{
	return pTypes.GetAt(index);
}

deoglCollideListPropFieldType &deoglCollideListPropField::AddType(deoglRPropFieldType *type){
	pTypes.Add(deoglCollideListPropFieldType(type));
	return pTypes.Last();
}

void deoglCollideListPropField::RemoveAllTypes(){
	pTypes.RemoveAll();
}
