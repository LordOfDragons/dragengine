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

#include "devkDescriptorSet.h"
#include "devkDescriptorPool.h"
#include "devkDescriptorPoolSlot.h"
#include "../devkDevice.h"
#include "../devkInstance.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class devkDescriptorSet
////////////////////////////

devkDescriptorSet::devkDescriptorSet( devkDescriptorPool &pool ) :
pPool( pool ),
pSlot( pool.Get() ),
pLayout( VK_NULL_HANDLE ),
pSet( pSlot->GetSet() ),
pBindings( nullptr ),
pWriteSets( nullptr ),
pBuffers( nullptr ),
pBindingCount( 0 )
{
	try{
		const devkDescriptorSetLayoutConfiguration &configuration = pool.GetLayout()->GetConfiguration();
		const int count = configuration.GetLayoutBindingCount();
		if( count > 0 ){
			pBindings = new VkDescriptorBufferInfo[ count ];
			
			pWriteSets = new VkWriteDescriptorSet[ count ];
			memset( pWriteSets, 0, sizeof( VkWriteDescriptorSet ) * count );
			
			int i;
			for( i=0; i<count; i++ ){
				const VkDescriptorSetLayoutBinding &binding = configuration.GetLayoutBindingAt( i );
				
				pBindings[ i ].buffer = VK_NULL_HANDLE;
				pBindings[ i ].offset = 0;
				pBindings[ i ].range = VK_WHOLE_SIZE;
				
				pWriteSets[ i ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				pWriteSets[ i ].dstSet = pSet;
				pWriteSets[ i ].dstBinding = binding.binding;
				pWriteSets[ i ].descriptorCount = 1;
				pWriteSets[ i ].descriptorType = binding.descriptorType;
				pWriteSets[ i ].pBufferInfo = pBindings + i;
			}
			
			pBuffers = new devkBuffer::Ref[ count ];
			
			pBindingCount = count;
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

devkDescriptorSet::~devkDescriptorSet(){
	pCleanUp();
}



// Management
///////////////

void devkDescriptorSet::SetBinding( int index, devkBuffer *buffer,
VkDeviceSize offset, VkDeviceSize range ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pBindingCount ){
		DETHROW_INFO( deeInvalidParam, "index >= bindingCount" );
	}
	
	pBuffers[ index ] = buffer;
	
	if( buffer ){
		pBindings[ index ].buffer = buffer->GetBuffer();
		pBindings[ index ].offset = offset;
		pBindings[ index ].range = range;
		
	}else{
		pBindings[ index ].buffer = VK_NULL_HANDLE;
		pBindings[ index ].offset = 0;
		pBindings[ index ].range = VK_WHOLE_SIZE;
	}
}

void devkDescriptorSet::ClearBinding( int index ){
	if( index < 0 ){
		DETHROW_INFO( deeInvalidParam, "index < 0" );
	}
	if( index >= pBindingCount ){
		DETHROW_INFO( deeInvalidParam, "index >= bindingCount" );
	}
	
	pBuffers[ index ] = nullptr;
	
	pBindings[ index ].buffer = VK_NULL_HANDLE;
	pBindings[ index ].offset = 0;
	pBindings[ index ].range = VK_WHOLE_SIZE;
}

void devkDescriptorSet::ClearAllBindings(){
	int i;
	for( i=0; i<pBindingCount; i++ ){
		pBuffers[ i ] = nullptr;
		
		pBindings[ i ].buffer = VK_NULL_HANDLE;
		pBindings[ i ].offset = 0;
		pBindings[ i ].range = VK_WHOLE_SIZE;
	}
}

void devkDescriptorSet::Update(){
	if( pBindingCount == 0 ){
		return;
	}
	
	devkDevice &device = pPool.GetDevice();
	device.vkUpdateDescriptorSets( device.GetDevice(), pBindingCount, pWriteSets, 0, VK_NULL_HANDLE );
}



// Private Functions
//////////////////////

void devkDescriptorSet::pCleanUp(){
	if( pBuffers ){
		delete [] pBuffers;
	}
	if( pWriteSets ){
		delete [] pWriteSets;
	}
	if( pBindings ){
		delete [] pBindings;
	}
}
