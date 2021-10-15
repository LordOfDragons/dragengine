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

#include <stdlib.h>
#include <string.h>

#include "devkDescriptorSetLayout.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>



// class devkDescriptorSetLayout
//////////////////////////////////

devkDescriptorSetLayout::devkDescriptorSetLayout( devkDevice &device,
	const devkDescriptorSetLayoutConfiguration &configuration ) :
pDevice( device ),
pConfiguration( configuration ),
pLayout( VK_NULL_HANDLE )
{
	VkDescriptorSetLayoutBinding layoutBindings;
	memset( &layoutBindings, 0, sizeof( layoutBindings ) );
	layoutBindings.descriptorType = configuration.GetType();
	layoutBindings.stageFlags = configuration.GetShaderStageFlags();
	layoutBindings.binding = 0;
	layoutBindings.descriptorCount = 1;
	
	VkDescriptorSetLayoutBinding *bindings = nullptr;
	try{
		if( configuration.GetLayoutBindingCount() > 0 ){
			bindings = new VkDescriptorSetLayoutBinding[ configuration.GetLayoutBindingCount() ];
			int i;
			for( i=0; i<configuration.GetLayoutBindingCount(); i++ ){
				bindings[ i ] = configuration.GetLayoutBindingAt( i );
			}
		}
		
		VkDescriptorSetLayoutCreateInfo layout;
		memset( &layout, 0, sizeof( layout ) );
		layout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout.pBindings = bindings;
		layout.bindingCount = configuration.GetLayoutBindingCount();
		
		VK_CHECK( device.GetInstance().GetVulkan(), device.vkCreateDescriptorSetLayout(
			device.GetDevice(), &layout, VK_NULL_HANDLE, &pLayout ) );
		
		if( bindings ){
			delete [] bindings;
		}
		
	}catch( const deException & ){
		if( bindings ){
			delete [] bindings;
		}
		throw;
	}
}

devkDescriptorSetLayout::~devkDescriptorSetLayout(){
	if( pLayout ){
		pDevice.vkDestroyDescriptorSetLayout( pDevice.GetDevice(), pLayout, VK_NULL_HANDLE );
	}
}



// Management
///////////////

