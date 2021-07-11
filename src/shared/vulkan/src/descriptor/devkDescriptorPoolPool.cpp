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

#include "devkDescriptorPool.h"
#include "devkDescriptorPoolPool.h"
#include "devkDescriptorPoolSlot.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkDescriptorPoolPool
/////////////////////////////////

devkDescriptorPoolPool::devkDescriptorPoolPool( devkDescriptorPool &owner,
	const VkDescriptorPoolCreateInfo &poolCreateInfo ) :
pOwner( owner ),
pOutOfMemory( false )
{
	try{
		deSharedVulkan &vulkan = owner.GetDevice().GetInstance().GetVulkan();
		VkDevice device = owner.GetDevice().GetDevice();
		
		VK_CHECK( vulkan, owner.GetDevice().vkCreateDescriptorPool(
			device, &poolCreateInfo, nullptr, &pPool ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkDescriptorPoolPool::~devkDescriptorPoolPool(){
	pCleanUp();
}



// Management
///////////////

devkDescriptorPoolSlot * devkDescriptorPoolPool::Get(){
	// return next free slot if present
	if( pFreeSlots.GetCount() > 0 ){
		const int index = pFreeSlots.GetCount() - 1;
		devkDescriptorPoolSlot * const slot = ( devkDescriptorPoolSlot* )pFreeSlots.GetAt( index );
		slot->AddReference(); // caller holds reference
		pFreeSlots.RemoveFrom( index );
		return slot;
	}
	
	// if the pool ran out of memory do not try allocating new sets
	if( pOutOfMemory ){
		return nullptr;
	}
	
	// no free slots present. try allocate a new descriptor set. this can fail if
	// the pool has not enough free memory left
	deSharedVulkan &vulkan = pOwner.GetDevice().GetInstance().GetVulkan();
	VkDevice device = pOwner.GetDevice().GetDevice();
	
	const VkDescriptorSetLayout setLayouts[ 1 ] = { pOwner.GetLayout()->GetLayout() };
	VkDescriptorSetAllocateInfo allocInfo;
	memset( &allocInfo, 0, sizeof( allocInfo ) );
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pPool;
	allocInfo.pSetLayouts = setLayouts;
	allocInfo.descriptorSetCount = 1;
	
	VkDescriptorSet set = nullptr;
	VkResult result = pOwner.GetDevice().vkAllocateDescriptorSets( device, &allocInfo, &set );
	
	switch( result ){
	case VK_SUCCESS:
		break;
		
	case VK_ERROR_OUT_OF_POOL_MEMORY:
	case VK_ERROR_FRAGMENTED_POOL:
		pOutOfMemory = true;
		return nullptr;
		
	default:
		VK_CHECK( vulkan, result );
	}
	
	// create slot for set and return it. caller holds reference
	return new devkDescriptorPoolSlot( *this, set );
}

void devkDescriptorPoolPool::Return( devkDescriptorPoolSlot *slot ){
	if( ! slot ){
		DETHROW_INFO( deeNullPointer, "slot" );
	}
	if( &slot->GetPool() != this ){
		DETHROW_INFO( deeInvalidParam, "slot pool mismatch" );
	}
	
	pFreeSlots.Add( slot );
	slot->FreeReference(); // caller held reference
}



// Private Functions
//////////////////////

void devkDescriptorPoolPool::pCleanUp(){
	if( pPool ){
		pOwner.GetDevice().vkDestroyDescriptorPool( pOwner.GetDevice().GetDevice(), pPool, nullptr );
	}
}
