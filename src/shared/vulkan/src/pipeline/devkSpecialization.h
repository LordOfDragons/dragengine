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

#ifndef _DEVSPECIALIZATION_H_
#define _DEVSPECIALIZATION_H_

#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTList.h>


/**
 * Vulkan specialization. Defines the layout of specialization only.
 */
class devkSpecialization : public deObject{
public:
	/** Reference. */
	using Ref = deTObjectReference<devkSpecialization>;
	
	
private:
	decTList<VkSpecializationMapEntry> pEntries;
	decTList<char> pData;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create specialization configuration. */
	devkSpecialization();
	
	/** Create specialization configuration. */
	devkSpecialization(const void *data, int dataSize, int entryCount);
	
	/** Create copy of specialization configuration. */
	devkSpecialization(const devkSpecialization &configuration);
	
protected:
	/** Clean up specialization configuration. */
	~devkSpecialization() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Entries. */
	inline const decTList<VkSpecializationMapEntry> &GetEntries() const{ return pEntries; }
	
	/** Set count of entries. */
	void SetEntryCount(int count);
	
	/** Set entry. */
	void SetEntryAt(int index, const VkSpecializationMapEntry &entry);
	
	/** Set entry. */
	void SetEntryAt(int index, int constantID, int offset, int size);
	void SetEntryIntAt(int index, int constantID, int offset);
	void SetEntryUIntAt(int index, int constantID, int offset);
	void SetEntryFloatAt(int index, int constantID, int offset);
	void SetEntryBoolAt(int index, int constantID, int offset);
	
	/** Data. */
	inline const void *GetData() const{ return pData.GetArrayPointer(); }
	
	/** Data size. */
	inline int GetDataSize() const{ return pData.GetCount(); }
	
	/** Set data. */
	void SetData(const void *data, int size);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==(const devkSpecialization &configuration) const;
	
	/** Copy configuration. */
	devkSpecialization &operator=(const devkSpecialization &configuration);
	/*@}*/
};

#endif

