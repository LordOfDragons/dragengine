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

#include "devkDescriptorSet.h"
#include "devkDescriptorPool.h"
#include "devkDescriptorPoolSlot.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkDescriptorSet
////////////////////////////

devkDescriptorSet::devkDescriptorSet(devkDescriptorPool &pool) :
pPool(pool),
pSlot(pool.Get()),
pLayout(VK_NULL_HANDLE),
pSet(pSlot->GetSet()),
pBindingCount(0)
{
	const devkDescriptorSetLayoutConfiguration &configuration = pool.GetLayout()->GetConfiguration();
	const int count = configuration.GetLayoutBindings().GetCount();
	if(count > 0){
		pBindings.SetAll(count, {});
		pWriteSets.SetAll(count, {});
		
		int i;
		for(i=0; i<count; i++){
			const VkDescriptorSetLayoutBinding &binding = configuration.GetLayoutBindings()[i];
			
			pBindings[i].buffer = VK_NULL_HANDLE;
			pBindings[i].offset = 0;
			pBindings[i].range = VK_WHOLE_SIZE;
			
			pWriteSets[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			pWriteSets[i].dstSet = pSet;
			pWriteSets[i].dstBinding = binding.binding;
			pWriteSets[i].descriptorCount = 1;
			pWriteSets[i].descriptorType = binding.descriptorType;
			pWriteSets[i].pBufferInfo = &pBindings[i];
		}
		
		pBuffers.SetAll(count, {});
		
		pBindingCount = count;
	}
}

devkDescriptorSet::~devkDescriptorSet() = default;



// Management
///////////////

void devkDescriptorSet::SetBinding(int index, devkBuffer *buffer,
VkDeviceSize offset, VkDeviceSize range){
	if(index < 0){
		DETHROW_INFO(deeInvalidParam, "index < 0");
	}
	if(index >= pBindingCount){
		DETHROW_INFO(deeInvalidParam, "index >= bindingCount");
	}
	
	pBuffers[index] = buffer;
	
	if(buffer){
		pBindings[index].buffer = buffer->GetBuffer();
		pBindings[index].offset = offset;
		pBindings[index].range = range;
		
	}else{
		pBindings[index].buffer = VK_NULL_HANDLE;
		pBindings[index].offset = 0;
		pBindings[index].range = VK_WHOLE_SIZE;
	}
}

void devkDescriptorSet::ClearBinding(int index){
	if(index < 0){
		DETHROW_INFO(deeInvalidParam, "index < 0");
	}
	if(index >= pBindingCount){
		DETHROW_INFO(deeInvalidParam, "index >= bindingCount");
	}
	
	pBuffers[index] = nullptr;
	
	pBindings[index].buffer = VK_NULL_HANDLE;
	pBindings[index].offset = 0;
	pBindings[index].range = VK_WHOLE_SIZE;
}

void devkDescriptorSet::ClearAllBindings(){
	int i;
	for(i=0; i<pBindingCount; i++){
		pBuffers[i] = nullptr;
		
		pBindings[i].buffer = VK_NULL_HANDLE;
		pBindings[i].offset = 0;
		pBindings[i].range = VK_WHOLE_SIZE;
	}
}

void devkDescriptorSet::Update(){
	if(pBindingCount == 0){
		return;
	}
	
	devkDevice &device = pPool.GetDevice();
	device.vkUpdateDescriptorSets(device.GetDevice(), pBindingCount,
		pWriteSets.GetArrayPointer(), 0, VK_NULL_HANDLE);
}
