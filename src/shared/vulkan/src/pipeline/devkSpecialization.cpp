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

#include "devkSpecialization.h"

#include <dragengine/common/exceptions.h>


// class devkSpecialization
/////////////////////////////

devkSpecialization::devkSpecialization() = default;

devkSpecialization::devkSpecialization(const void* data, int dataSize, int entryCount){
	SetEntryCount(entryCount);
	SetData(data, dataSize);
}

devkSpecialization::devkSpecialization(const devkSpecialization &configuration){
	*this = configuration;
}

devkSpecialization::~devkSpecialization() = default;



// Management
///////////////

void devkSpecialization::SetEntryCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pEntries.SetAll(count, {});
}

void devkSpecialization::SetEntryAt(int index, const VkSpecializationMapEntry &entry){
	DEASSERT_TRUE(entry.constantID >= 0)
	DEASSERT_TRUE(entry.offset >= 0)
	DEASSERT_TRUE(entry.size >= 0)
	
	pEntries[index] = entry;
}

void devkSpecialization::SetEntryAt(int index, int constantID, int offset, int size){
	VkSpecializationMapEntry entry{};
	entry.constantID = constantID;
	entry.offset = offset;
	entry.size = size;
	
	pEntries[index] = entry;
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
	DEASSERT_NOTNULL_IF(size > 0, data)
	
	pData.SetCountDiscard(size);
	
	if(size > 0){
		memcpy(pData.GetArrayPointer(), data, size);
	}
}



// Operators
//////////////

static bool operator==(const VkSpecializationMapEntry &a, const VkSpecializationMapEntry &b){
	return a.constantID == b.constantID
		&& a.offset == b.offset
		&& a.size == b.size;
}

bool devkSpecialization::operator==(const devkSpecialization &configuration) const{
	return pEntries == configuration.pEntries
		&& pData == configuration.pData;
}

devkSpecialization &devkSpecialization::operator=(
const devkSpecialization &configuration){
	pEntries = configuration.pEntries;
	pData = configuration.pData;
	return *this;
}
