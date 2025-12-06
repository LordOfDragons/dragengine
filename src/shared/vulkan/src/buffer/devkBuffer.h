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

#ifndef _DEVKBUFFER_H_
#define _DEVKBUFFER_H_

#include "../devkBasics.h"
#include "../queue/devkCommandBuffer.h"

#include <dragengine/deObject.h>

class devkDevice;
class devkCommandPool;


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
	
	devkCommandBuffer::Ref pCommandBuffer;
	
	
	
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
	/** Clean up buffer. */
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
	 */
	void TransferToDevice(devkCommandBuffer &commandBuffer);
	
	/**
	 * Transfer data from device memory to host memory.
	 */
	void FetchFromDevice(devkCommandBuffer &commandBuffer);
	
	/** Copy data from host memory. */
	void GetData( void *data );
	
	/** Copy data from host memory. */
	void GetData( void *data, uint32_t offset, uint32_t size );
	
	/** If command is active wait for command to be signaled. */
	void Wait();
	
	/**
	 * Begin awaitable command buffer.
	 * 
	 * Calls first Wait() to ensure a potential previous command buffer is finished.
	 * Caller has to fill the command buffer and submit it.
	 */
	devkCommandBuffer &BeginCommandBuffer(devkCommandPool &pool);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateBuffer( VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
		VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size );
};

#endif
