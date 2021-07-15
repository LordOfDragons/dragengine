/* 
 * Drag[en]gine Shared Vulkan
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	devkDescriptorSetLayoutConfiguration( const devkDescriptorSetLayoutConfiguration &configuration );
	
	/** Clean up descriptor set layout configuration. */
	~devkDescriptorSetLayoutConfiguration();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Type. */
	inline VkDescriptorType GetType() const{ return pType; }
	
	/** Set Type. */
	void SetType( VkDescriptorType type );
	
	/** Shader stage flags. */
	inline VkShaderStageFlags GetShaderStageFlags() const{ return pShaderStageFlags; }
	
	/** Set shader stage flags. */
	void SetShaderStageFlags( VkShaderStageFlags flags );
	
	
	
	/** Layout binding count. */
	inline int GetLayoutBindingCount() const{ return pLayoutBindingCount; }
	
	/** Set layout binding count. */
	void SetLayoutBindingCount( int count );
	
	/** Layout binding at index. */
	const VkDescriptorSetLayoutBinding &GetLayoutBindingAt( int index ) const;
	
	/** Set layout binding at index. */
	void SetLayoutBindingAt( int index, const VkDescriptorSetLayoutBinding &binding );
	
	void SetLayoutBindingAt( int index, int binding, VkDescriptorType type,
		VkShaderStageFlags flags = VK_SHADER_STAGE_ALL );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==( const devkDescriptorSetLayoutConfiguration &configuration ) const;
	
	/** Copy Configurations. */
	devkDescriptorSetLayoutConfiguration &operator=( const devkDescriptorSetLayoutConfiguration &configuration );
	/*@}*/
};

#endif

