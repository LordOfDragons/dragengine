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

#include "devkDescriptorSetLayoutConfiguration.h"

#include <dragengine/common/exceptions.h>


// class devkDescriptorSetLayoutConfiguration
///////////////////////////////////////////////

devkDescriptorSetLayoutConfiguration::devkDescriptorSetLayoutConfiguration() :
pType(VK_DESCRIPTOR_TYPE_SAMPLER),
pShaderStageFlags(VK_SHADER_STAGE_ALL){
}

devkDescriptorSetLayoutConfiguration::devkDescriptorSetLayoutConfiguration(
	const devkDescriptorSetLayoutConfiguration &configuration) :
pType(VK_DESCRIPTOR_TYPE_SAMPLER),
pShaderStageFlags(VK_SHADER_STAGE_ALL)
{
	*this = configuration;
}

devkDescriptorSetLayoutConfiguration::~devkDescriptorSetLayoutConfiguration() = default;



// Management
///////////////

void devkDescriptorSetLayoutConfiguration::SetType(VkDescriptorType type){
	pType = type;
}

void devkDescriptorSetLayoutConfiguration::SetShaderStageFlags(VkShaderStageFlags flags){
	pShaderStageFlags = flags;
}

void devkDescriptorSetLayoutConfiguration::SetLayoutBindingCount(int count){
	DEASSERT_TRUE(count >= 0)
	
	pLayoutBindings.SetAll(count, {});
}

void devkDescriptorSetLayoutConfiguration::SetLayoutBindingAt(
int index, const VkDescriptorSetLayoutBinding &binding){
	DEASSERT_NULL(binding.pImmutableSamplers)
	
	pLayoutBindings[index] = binding;
}

void devkDescriptorSetLayoutConfiguration::SetLayoutBindingAt(
int index, int binding, VkDescriptorType type, VkShaderStageFlags flags){
	pLayoutBindings[index].binding = binding;
	pLayoutBindings[index].stageFlags = flags;
	pLayoutBindings[index].descriptorType = type;
	pLayoutBindings[index].descriptorCount = 1;
}



// Operators
//////////////

static bool operator==(const VkDescriptorSetLayoutBinding &a, const VkDescriptorSetLayoutBinding &b){
	return a.binding == b.binding
		&& a.descriptorType == b.descriptorType
		&& a.descriptorCount == b.descriptorCount
		&& a.stageFlags == b.stageFlags
		&& a.pImmutableSamplers == b.pImmutableSamplers;
}

bool devkDescriptorSetLayoutConfiguration::operator==(
const devkDescriptorSetLayoutConfiguration &configuration) const{
	return pType == configuration.pType
		&& pShaderStageFlags == configuration.pShaderStageFlags
		&& pLayoutBindings == configuration.pLayoutBindings;
}

devkDescriptorSetLayoutConfiguration &devkDescriptorSetLayoutConfiguration::operator=(
const devkDescriptorSetLayoutConfiguration &configuration){
	pType = configuration.pType;
	pShaderStageFlags = configuration.pShaderStageFlags;
	pLayoutBindings = configuration.pLayoutBindings;
	return *this;
}
