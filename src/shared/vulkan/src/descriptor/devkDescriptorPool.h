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

#ifndef _DEVKDESCRIPTORPOOL_H_
#define _DEVKDESCRIPTORPOOL_H_

#include "devkDescriptorPoolPool.h"
#include "devkDescriptorSetLayout.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class devkDevice;
class devkDescriptorPoolSlot;


/**
 * Vulkan descriptor pool. Manages a growing list of pools of the same configuration.
 * Once a pool has no more free slots a new pool is created. Descriptor sets are assigned
 * to free slots in the first pool with free slots. Upon deleting a descriptor set the
 * occupied slot is returned to the list of free slots.
 */
class devkDescriptorPool : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDescriptorPool> Ref;
	
	
private:
	devkDevice &pDevice;
	
	VkDescriptorPoolSize pPoolSize;
	VkDescriptorPoolCreateInfo pPoolCreateInfo;
	devkDescriptorSetLayout::Ref pLayout;
	
	decObjectList pPools;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create command pool. */
	devkDescriptorPool(devkDevice &device, VkDescriptorType type,
		devkDescriptorSetLayout *layout, int maxSetCount = 1000);
	
protected:
	/** Clean up queue. */
	virtual ~devkDescriptorPool();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Layout. */
	inline const devkDescriptorSetLayout::Ref &GetLayout() const{ return pLayout; }
	
	/** Get next free slot. */
	devkDescriptorPoolSlot *Get();
	/*@}*/
};

#endif

