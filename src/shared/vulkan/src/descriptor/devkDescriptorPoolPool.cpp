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

#include "devkDescriptorPool.h"
#include "devkDescriptorPoolPool.h"
#include "devkDescriptorPoolSlot.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkDescriptorPoolPool
/////////////////////////////////

devkDescriptorPoolPool::devkDescriptorPoolPool(devkDescriptorPool &owner,
	const VkDescriptorPoolCreateInfo &poolCreateInfo) :
pOwner(owner),
pOutOfMemory(false)
{
	try{
		VK_IF_CHECK(deSharedVulkan &vulkan = owner.GetDevice().GetInstance().GetVulkan());
		VkDevice device = owner.GetDevice().GetDevice();
		
		VK_CHECK(vulkan, owner.GetDevice().vkCreateDescriptorPool(
			device, &poolCreateInfo, VK_NULL_HANDLE, &pPool));
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

devkDescriptorPoolPool::~devkDescriptorPoolPool(){
	pCleanUp();
}



// Management
///////////////

devkDescriptorPoolSlot::Ref devkDescriptorPoolPool::Get(){
	// return next free slot if present
	if(pFreeSlots.GetCount() > 0){
		const int index = pFreeSlots.GetCount() - 1;
		devkDescriptorPoolSlot::Ref slot((devkDescriptorPoolSlot*)pFreeSlots.GetAt(index));
		pFreeSlots.RemoveFrom(index);
		return slot;
	}
	
	// if the pool ran out of memory do not try allocating new sets
	if(pOutOfMemory){
		return nullptr;
	}
	
	// no free slots present. try allocate a new descriptor set. this can fail if
	// the pool has not enough free memory left
	VK_IF_CHECK(deSharedVulkan &vulkan = pOwner.GetDevice().GetInstance().GetVulkan());
	VkDevice device = pOwner.GetDevice().GetDevice();
	
	const VkDescriptorSetLayout setLayouts[1] = {pOwner.GetLayout()->GetLayout()};
	VkDescriptorSetAllocateInfo allocInfo{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
	allocInfo.descriptorPool = pPool;
	allocInfo.pSetLayouts = setLayouts;
	allocInfo.descriptorSetCount = 1;
	
	VkDescriptorSet set = VK_NULL_HANDLE;
	VkResult result = pOwner.GetDevice().vkAllocateDescriptorSets(device, &allocInfo, &set);
	
	switch(result){
	case VK_SUCCESS:
		break;
		
	case VK_ERROR_OUT_OF_POOL_MEMORY:
	case VK_ERROR_FRAGMENTED_POOL:
		pOutOfMemory = true;
		return nullptr;
		
	default:
		#ifdef VK_CHECKCOMMANDS
		VK_CHECK(vulkan, result);
		#endif
		DETHROW(deeInvalidAction);
	}
	
	// create slot for set and return it. caller holds reference
	return devkDescriptorPoolSlot::Ref(new devkDescriptorPoolSlot(*this, set));
}

void devkDescriptorPoolPool::Return(devkDescriptorPoolSlot *slot){
	if(!slot){
		DETHROW_INFO(deeNullPointer, "slot");
	}
	if(&slot->GetPool() != this){
		DETHROW_INFO(deeInvalidParam, "slot pool mismatch");
	}
	
	pFreeSlots.Add(slot);
	slot->FreeReference(); // caller held reference
}



// Private Functions
//////////////////////

void devkDescriptorPoolPool::pCleanUp(){
	if(pPool){
		pOwner.GetDevice().vkDestroyDescriptorPool(pOwner.GetDevice().GetDevice(), pPool, VK_NULL_HANDLE);
	}
}
