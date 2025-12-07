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

#ifndef _DEVKDESCRIPTORPOOLPOOL_H_
#define _DEVKDESCRIPTORPOOLPOOL_H_

#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class devkDescriptorPool;
class devkDescriptorPoolSlot;


/**
 * Vulkan descriptor pool pool.
 */
class devkDescriptorPoolPool : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDescriptorPoolPool> Ref;
	
	
private:
	devkDescriptorPool &pOwner;
	
	VkDescriptorPool pPool;
	
	decObjectList pFreeSlots;
	bool pOutOfMemory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor pool pool. */
	devkDescriptorPoolPool(devkDescriptorPool &owner, const VkDescriptorPoolCreateInfo &poolCreateInfo);
	
protected:
	/** Clean up descriptor pool pool. */
	virtual ~devkDescriptorPoolPool();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Owner. */
	inline devkDescriptorPool &GetOwner() const{return pOwner;}
	
	/** Get next free slot or nullptr if pool is full. */
	devkDescriptorPoolSlot *Get();
	
	/** Return slot to pool. */
	void Return(devkDescriptorPoolSlot *slot);
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

