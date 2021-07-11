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

#include "devkCommandPool.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkCommandPool
//////////////////////////

devkCommandPool::devkCommandPool( devkDevice &device, uint32_t queueFamily ) :
pDevice( device ),
pPool( nullptr )
{
	try{
		VK_IF_CHECK( deSharedVulkan &vulkan = device.GetInstance().GetVulkan() );
		
		VkCommandPoolCreateInfo info;
		memset( &info, 0, sizeof( info ) );
		
		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.queueFamilyIndex = queueFamily;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		
		VK_CHECK( vulkan, device.vkCreateCommandPool( device.GetDevice(), &info, nullptr, &pPool ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkCommandPool::~devkCommandPool(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void devkCommandPool::pCleanUp(){
	if( pPool ){
		pDevice.vkDestroyCommandPool( pDevice.GetDevice(), pPool, nullptr );
	}
}
