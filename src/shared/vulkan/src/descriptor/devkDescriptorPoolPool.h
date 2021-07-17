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

#ifndef _DEVKDESCRIPTORPOOLPOOL_H_
#define _DEVKDESCRIPTORPOOLPOOL_H_

#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>

class devkDescriptorPool;
class devkDescriptorPoolSlot;


/**
 * Vulkan descriptor pool pool.
 */
class devkDescriptorPoolPool : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDescriptorPoolPool> Ref;
	
	
	
private:
	devkDescriptorPool &pOwner;
	
	VkDescriptorPool pPool;
	
	decObjectList pFreeSlots;
	bool pOutOfMemory;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor pool pool. */
	devkDescriptorPoolPool( devkDescriptorPool &owner, const VkDescriptorPoolCreateInfo &poolCreateInfo );
	
protected:
	/** Clean up descriptor pool pool. */
	virtual ~devkDescriptorPoolPool();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Owner. */
	inline devkDescriptorPool &GetOwner() const{ return pOwner; }
	
	/** Get next free slot or nullptr if pool is full. */
	devkDescriptorPoolSlot *Get();
	
	/** Return slot to pool. */
	void Return( devkDescriptorPoolSlot *slot );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

