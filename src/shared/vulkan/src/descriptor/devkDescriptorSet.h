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

#ifndef _DEVKDESCRIPTORSET_H_
#define _DEVKDESCRIPTORSET_H_

#include "devkDescriptorPoolSlot.h"
#include "../devkBasics.h"
#include "../devkDevice.h"
#include "../buffer/devkBuffer.h"

#include <dragengine/deObject.h>
#include <dragengine/deTObjectReference.h>

class devkDescriptorPool;


/**
 * Vulkan descriptor set. Constructor obtains a set from the pool and returns it to the pool
 * in the destructor. Bindings are created at construction time and set to nullptr. Assign
 * bindings then call Update() to write bindings to descriptor set.
 */
class devkDescriptorSet : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDescriptorSet> Ref;
	
	
	
private:
	devkDescriptorPool &pPool;
	
	devkDescriptorPoolSlot::Ref pSlot;
	
	VkDescriptorSetLayout pLayout;
	VkDescriptorSet pSet;
	
	VkDescriptorBufferInfo *pBindings;
	VkWriteDescriptorSet *pWriteSets;
	devkBuffer::Ref *pBuffers;
	int pBindingCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor set. */
	devkDescriptorSet( devkDescriptorPool &pool );
	
protected:
	/** Clean up descriptor set. */
	virtual ~devkDescriptorSet();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pool. */
	inline devkDescriptorPool &GetPool() const{ return pPool; }
	
	/** Layout. */
	inline VkDescriptorSetLayout GetLayout() const{ return pLayout; }
	
	/** Descriptor set. */
	inline VkDescriptorSet GetSet() const{ return pSet; }
	
	/** Set binding. */
	void SetBinding( int index, devkBuffer *buffer,
		VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE );
	
	/** Clear binding. */
	void ClearBinding( int index );
	
	/** Clear all bindings. */
	void ClearAllBindings();
	
	/** Write bindings to descriptor set. */
	void Update();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

