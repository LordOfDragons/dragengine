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

#include <stdlib.h>
#include <string.h>

#include "devkSpecialization.h"

#include <dragengine/common/exceptions.h>


// class devkSpecialization
/////////////////////////////

devkSpecialization::devkSpecialization() :
pEntries(nullptr),
pEntryCount(0),
pData(nullptr),
pDataSize(0){
}

devkSpecialization::devkSpecialization(const void* data, int dataSize, int entryCount) :
pEntries(nullptr),
pEntryCount(0),
pData(nullptr),
pDataSize(0)
{
	try{
		SetEntryCount(entryCount);
		SetData(data, dataSize);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkSpecialization::devkSpecialization(const devkSpecialization &configuration) :
pEntries(nullptr),
pEntryCount(0),
pData(nullptr),
pDataSize(0)
{
	*this = configuration;
}

devkSpecialization::~devkSpecialization(){
	pCleanUp();
}



// Management
///////////////

void devkSpecialization::SetEntryCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	if(pEntries){
		delete [] pEntries;
		pEntries = nullptr;
		pEntryCount = 0;
	}
	
	if(count == 0){
		return;
	}
	
	pEntries = new VkSpecializationMapEntry[ count ]{};
	pEntryCount = count;
}

const VkSpecializationMapEntry &devkSpecialization::GetEntryAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pEntryCount)
	
	return pEntries[index];
}

void devkSpecialization::SetEntryAt(int index, const VkSpecializationMapEntry &entry){
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pEntryCount)
	DEASSERT_TRUE(entry.constantID >= 0)
	DEASSERT_TRUE(entry.offset >= 0)
	DEASSERT_TRUE(entry.size >= 0)
	
	pEntries[ index ] = entry;
}

void devkSpecialization::SetEntryAt(int index, int constantID, int offset, int size){
	VkSpecializationMapEntry entry;
	entry.constantID = constantID;
	entry.offset = offset;
	entry.size = size;
	SetEntryAt(index, entry);
}

void devkSpecialization::SetEntryIntAt(int index, int constantID, int offset){
	SetEntryAt(index, constantID, offset, sizeof(int32_t));
}

void devkSpecialization::SetEntryUIntAt(int index, int constantID, int offset){
	SetEntryAt(index, constantID, offset, sizeof(uint32_t));
}

void devkSpecialization::SetEntryFloatAt(int index, int constantID, int offset){
	SetEntryAt(index, constantID, offset, sizeof(float));
}

void devkSpecialization::SetEntryBoolAt(int index, int constantID, int offset){
	SetEntryAt(index, constantID, offset, sizeof(bool));
}

void devkSpecialization::SetData(const void *data, int size){
	DEASSERT_TRUE(size >= 0)
	DEASSERT_NOTNULL_IF(size > 0, data)
	
	if(pData){
		delete [] pData;
		pData = nullptr;
		pDataSize = 0;
	}
	
	if(size == 0){
		return;
	}
	
	pData = new char[ size ];
	memcpy(pData, data, size);
	pDataSize = size;
}



// Operators
//////////////

bool devkSpecialization::operator==(const devkSpecialization &configuration) const{
	if(pEntryCount != configuration.pEntryCount
	|| pDataSize != configuration.pDataSize
	|| (pData && memcmp(pData, configuration.pData, pDataSize) != 0)){
		return false;
	}
	
	int i;
	for(i=0; i<pEntryCount; i++){
		if(pEntries[ i ].constantID != configuration.pEntries[ i ].constantID
		|| pEntries[ i ].offset != configuration.pEntries[ i ].offset
		|| pEntries[ i ].size != configuration.pEntries[ i ].size){
			return false;
		}
	}
	return true;
}

devkSpecialization &devkSpecialization::operator=(
const devkSpecialization &configuration){
	SetEntryCount(configuration.pEntryCount);
	if(configuration.pEntryCount > 0){
		memcpy(pEntries, configuration.pEntries,
			sizeof(VkSpecializationMapEntry) * configuration.pEntryCount);
	}
	SetData(configuration.GetData(), configuration.GetDataSize());
	return *this;
}



// Private Functions
//////////////////////

void devkSpecialization::pCleanUp(){
	if(pData){
		delete [] pData;
	}
	if(pEntries){
		delete [] pEntries;
	}
}
