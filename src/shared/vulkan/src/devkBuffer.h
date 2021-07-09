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

#ifndef _DEVKBUFFER_H_
#define _DEVKBUFFER_H_

#include "devkBasics.h"
#include "devkCommandPool.h"

#include <dragengine/deObject.h>
#include <dragengine/deTObjectReference.h>

class devkDevice;
class devkQueue;


/**
 * Vulkan device buffer. Supports staging data in host memory to transfer to device memory
 * as well as fetching data from device memory into host memory. Buffers can not be resized
 * once created.
 */
class devkBuffer : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkBuffer> Ref;
	
	
	
private:
	devkDevice &pDevice;
	
	uint32_t pSize;
	
	VkBuffer pBuffer;
	VkDeviceMemory pBufferMemory;
	
	VkBuffer pBufferHost;
	VkDeviceMemory pBufferHostMemory;
	
	VkFence pFence;
	bool pFenceActive;
	devkCommandPool::Ref pCommandPool;
	VkCommandBuffer pCommand;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create queue. */
	devkBuffer( devkDevice &device, VkDeviceSize size );
	
protected:
	/** Clean up queue. */
	virtual ~devkBuffer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Device buffer. */
	inline VkBuffer GetBuffer() const{ return pBuffer; }
	
	/** Size. */
	inline uint32_t GetSize() const{ return pSize; }
	
	/** Copy data to host memory. Size has to equal buffer size. */
	void SetData( const void *data, uint32_t size );
	
	/** Copy data to host memory. */
	void SetData( const void *data, uint32_t offset, uint32_t size );
	
	/**
	 * Transfer data from host memory to device memory.
	 * \note Calls Wait() before starting the transfer.
	 * \note After call exist Wait() has to be called before using buffer.
	 */
	void TransferToDevice( devkCommandPool *pool, devkQueue &queue );
	
	/**
	 * If fence is active wait for fence to be signaled.
	 * \param[in] reset If true reset fence and set it inactive after wait finished.
	 */
	void Wait( bool reset = false );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateBuffer( VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
		VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size );
};

#endif

