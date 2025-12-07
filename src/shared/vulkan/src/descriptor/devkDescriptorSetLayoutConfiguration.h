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

#ifndef _DEVKDESCRIPTORSETLAYOUTCONFIGURATION_H_
#define _DEVKDESCRIPTORSETLAYOUTCONFIGURATION_H_

#include "../devkBasics.h"


/**
 * Vulkan descriptor layout configuration.
 */
class devkDescriptorSetLayoutConfiguration{
private:
	VkDescriptorType pType;
	VkShaderStageFlags pShaderStageFlags;
	VkDescriptorSetLayoutBinding *pLayoutBindings;
	int pLayoutBindingCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor set layout configuration. */
	devkDescriptorSetLayoutConfiguration();
	
	/** Create copy of descriptor set layout configuration. */
	devkDescriptorSetLayoutConfiguration(const devkDescriptorSetLayoutConfiguration &configuration);
	
	/** Clean up descriptor set layout configuration. */
	~devkDescriptorSetLayoutConfiguration();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Type. */
	inline VkDescriptorType GetType() const{return pType;}
	
	/** Set Type. */
	void SetType(VkDescriptorType type);
	
	/** Shader stage flags. */
	inline VkShaderStageFlags GetShaderStageFlags() const{return pShaderStageFlags;}
	
	/** Set shader stage flags. */
	void SetShaderStageFlags(VkShaderStageFlags flags);
	
	
	
	/** Layout binding count. */
	inline int GetLayoutBindingCount() const{return pLayoutBindingCount;}
	
	/** Set layout binding count. */
	void SetLayoutBindingCount(int count);
	
	/** Layout binding at index. */
	const VkDescriptorSetLayoutBinding &GetLayoutBindingAt(int index) const;
	
	/** Set layout binding at index. */
	void SetLayoutBindingAt(int index, const VkDescriptorSetLayoutBinding &binding);
	
	void SetLayoutBindingAt(int index, int binding, VkDescriptorType type,
		VkShaderStageFlags flags = VK_SHADER_STAGE_ALL);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==(const devkDescriptorSetLayoutConfiguration &configuration) const;
	
	/** Copy Configurations. */
	devkDescriptorSetLayoutConfiguration &operator=(const devkDescriptorSetLayoutConfiguration &configuration);
	/*@}*/
};

#endif

