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
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkDescriptorPool
/////////////////////////////

devkDescriptorPool::devkDescriptorPool( devkDevice &device, VkDescriptorType type,
devkDescriptorSetLayout *layout, int maxSetCount ) :
pDevice( device ),
pLayout( layout )
{
	if( ! layout ){
		DETHROW_INFO( deeNullPointer, "layout" );
	}
	
	memset( &pPoolSize, 0, sizeof( pPoolSize ) );
	pPoolSize.type = type;
	pPoolSize.descriptorCount = 1;
	
	memset( &pPoolCreateInfo, 0, sizeof( pPoolCreateInfo ) );
	pPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pPoolCreateInfo.poolSizeCount = 1;
	pPoolCreateInfo.pPoolSizes = &pPoolSize;
	pPoolCreateInfo.maxSets = maxSetCount;
}

devkDescriptorPool::~devkDescriptorPool(){
}



// Management
///////////////

devkDescriptorPoolSlot *devkDescriptorPool::Get(){
	// get next free slot from first pool with free slots
	const int count = pPools.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		devkDescriptorPoolPool * const pool = ( devkDescriptorPoolPool* )pPools.GetAt( i );
		devkDescriptorPoolSlot * const slot = pool->Get();
		if( slot ){
			return slot;
		}
	}
	
	// no pool has free slots. create a new pool and get slot
	devkDescriptorPoolPool::Ref pool( devkDescriptorPoolPool::Ref::With(
			new devkDescriptorPoolPool( *this, pPoolCreateInfo ) ) );
	pPools.Add( pool );
	
	devkDescriptorPoolSlot * const slot = pool->Get();
	if( ! slot ){
		DETHROW( deeOutOfMemory );
	}
	
	return slot;
}
