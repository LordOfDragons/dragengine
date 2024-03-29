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

#include "../devkBasics.h"
#include "../queue/devkCommandPool.h"

#include <dragengine/deObject.h>

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
	VkBufferUsageFlagBits pUsage;
	
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
	/**
	 * Create buffer.
	 * 
	 * Typical usages:
	 * - VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT : UBO
	 * - VK_BUFFER_USAGE_STORAGE_BUFFER_BIT : SSBO
	 * - VK_BUFFER_USAGE_INDEX_BUFFER_BIT : IBO
	 * - VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : VBO
	 * - VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT
	 * - VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT : ?
	 * - VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT : ?
	 */
	devkBuffer( devkDevice &device, VkDeviceSize size, VkBufferUsageFlagBits usage );
	
protected:
	/** Clean up queue. */
	virtual ~devkBuffer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Buffer. */
	inline VkBuffer GetBuffer() const{ return pBuffer; }
	
	/** Host buffer. */
	inline VkBuffer GetBufferHost() const{ return pBufferHost; }
	
	/** Size. */
	inline uint32_t GetSize() const{ return pSize; }
	
	/** Usage. */
	inline VkBufferUsageFlagBits GetUsage() const{ return pUsage; }
	
	/** Copy data to host memory. */
	void SetData( const void *data );
	
	/** Copy data to host memory. */
	void SetData( const void *data, uint32_t offset, uint32_t size );
	
	/**
	 * Transfer data from host memory to device memory.
	 * \note Calls Wait() before starting the transfer.
	 * \note After call exist Wait() has to be called before using buffer.
	 */
	void TransferToDevice( devkCommandPool *pool, devkQueue &queue );
	
	/** Copy data from host memory. */
	void GetData( void *data );
	
	/** Copy data from host memory. */
	void GetData( void *data, uint32_t offset, uint32_t size );
	
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

