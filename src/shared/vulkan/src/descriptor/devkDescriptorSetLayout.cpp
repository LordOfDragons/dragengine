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

#include "devkDescriptorSetLayout.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>



// class devkDescriptorSetLayout
//////////////////////////////////

devkDescriptorSetLayout::devkDescriptorSetLayout(devkDevice &device,
	const devkDescriptorSetLayoutConfiguration &configuration) :
pDevice(device),
pConfiguration(configuration),
pLayout(VK_NULL_HANDLE)
{
	VkDescriptorSetLayoutBinding *bindings = nullptr;
	try{
		if(configuration.GetLayoutBindingCount() > 0){
			bindings = new VkDescriptorSetLayoutBinding[configuration.GetLayoutBindingCount()];
			int i;
			for(i=0; i<configuration.GetLayoutBindingCount(); i++){
				bindings[i] = configuration.GetLayoutBindingAt(i);
			}
		}
		
		VkDescriptorSetLayoutCreateInfo layout{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
		layout.pBindings = bindings;
		layout.bindingCount = configuration.GetLayoutBindingCount();
		
		VK_CHECK(device.GetInstance().GetVulkan(), device.vkCreateDescriptorSetLayout(
			device.GetDevice(), &layout, VK_NULL_HANDLE, &pLayout));
		
		if(bindings){
			delete [] bindings;
		}
		
	}catch(const deException &){
		if(bindings){
			delete [] bindings;
		}
		throw;
	}
}

devkDescriptorSetLayout::~devkDescriptorSetLayout(){
	if(pLayout){
		pDevice.vkDestroyDescriptorSetLayout(pDevice.GetDevice(), pLayout, VK_NULL_HANDLE);
	}
}



// Management
///////////////

