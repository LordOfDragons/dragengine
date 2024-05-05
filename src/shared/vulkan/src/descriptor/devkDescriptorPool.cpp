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
	devkDescriptorPoolPool::Ref pool( devkDescriptorPoolPool::Ref::New(
			new devkDescriptorPoolPool( *this, pPoolCreateInfo ) ) );
	pPools.Add( pool );
	
	devkDescriptorPoolSlot * const slot = pool->Get();
	if( ! slot ){
		DETHROW( deeOutOfMemory );
	}
	
	return slot;
}
