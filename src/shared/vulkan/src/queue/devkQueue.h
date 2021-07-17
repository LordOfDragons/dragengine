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

#ifndef _DEVKQUEUE_H_
#define _DEVKQUEUE_H_

#include "devkCommandPool.h"
#include "../devkBasics.h"

#include <dragengine/deObject.h>

class devkDevice;


/**
 * Vulkan device queue.
 */
class devkQueue : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkQueue> Ref;
	
	
	
private:
	devkDevice &pDevice;
	uint32_t pFamily;
	
	VkQueue pQueue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create queue. */
	devkQueue( devkDevice &device, uint32_t family, VkQueue queue );
	
protected:
	/** Clean up queue. */
	virtual ~devkQueue();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Family. */
	inline uint32_t GetFamily() const{ return pFamily; }
	
	/** Queue. */
	inline VkQueue GetQueue() const{ return pQueue; }
	
	/** Create command pool compatible with this queue and all queues of the same family. */
	devkCommandPool::Ref CreateCommandPool();
	
	/** Wait for queue to be idle. */
	void WaitIdle();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

