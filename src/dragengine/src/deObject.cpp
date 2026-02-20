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

#include "deObject.h"
#include "common/exceptions.h"


// Class deObject::cWeakRefData
/////////////////////////////////

deObject::cWeakRefData::cWeakRefData(deObject *object) :
pObject(object),
pRefCount(1)
{
	DEASSERT_NOTNULL(object)
}

deObject::cWeakRefData::~cWeakRefData(){
	if(pObject){
		pObject->pWeakRefData = nullptr;
	}
}

void deObject::cWeakRefData::AddReference(){
	pRefCount++;
}

void deObject::cWeakRefData::FreeReference(){
	pRefCount--;
	if(pRefCount > 0){
		return;
	}
	
	if(pRefCount < 0){
		deeInvalidParam(__FILE__, __LINE__).PrintError();
		return;
	}
	
	delete this;
}


// Class deObject
///////////////////

// Constructor, destructor
////////////////////////////

deObject::deObject() :
pRefCount(1),
pWeakRefData(nullptr){
}

deObject::~deObject(){
	if(pWeakRefData){
		pWeakRefData->pObject = nullptr;
	}
}


// Management
///////////////

void deObject::AddReference(){
	pRefCount++;
}

void deObject::FreeReference(){
	pRefCount--;
	if(pRefCount > 0){
		return;
	}
	
	if(pRefCount < 0){
		deeInvalidParam(__FILE__, __LINE__).PrintError();
		return;
	}
	
	delete this;
}

deObject::cWeakRefData *deObject::AddWeakReference(){
	if(pWeakRefData){
		pWeakRefData->AddReference();
		
	}else{
		pWeakRefData = new cWeakRefData(this);
	}
	
	return pWeakRefData;
}
