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

#ifndef _DEVKDESCRIPTORPOOL_H_
#define _DEVKDESCRIPTORPOOL_H_

#include "devkDescriptorPoolPool.h"
#include "devkDescriptorSetLayout.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/deTObjectReference.h>
#include <dragengine/common/collection/decObjectList.h>

class devkDevice;
class devkDescriptorPoolSlot;


/**
 * Vulkan descriptor pool. Manages a growing list of pools of the same configuration.
 * Once a pool has no more free slots a new pool is created. Descriptor sets are assigned
 * to free slots in the first pool with free slots. Upon deleting a descriptor set the
 * occupied slot is returned to the list of free slots.
 */
class devkDescriptorPool : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDescriptorPool> Ref;
	
	
	
private:
	devkDevice &pDevice;
	
	VkDescriptorPoolSize pPoolSize;
	VkDescriptorPoolCreateInfo pPoolCreateInfo;
	devkDescriptorSetLayout::Ref pLayout;
	
	decObjectList pPools;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create command pool. */
	devkDescriptorPool( devkDevice &device, VkDescriptorType type,
		devkDescriptorSetLayout *layout, int maxSetCount = 1000 );
	
protected:
	/** Clean up queue. */
	virtual ~devkDescriptorPool();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Layout. */
	inline devkDescriptorSetLayout *GetLayout() const{ return pLayout; }
	
	/** Get next free slot. */
	devkDescriptorPoolSlot *Get();
	/*@}*/
};

#endif

