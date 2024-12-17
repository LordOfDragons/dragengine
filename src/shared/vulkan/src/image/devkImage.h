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

#ifndef _DEVKIMAGE_H_
#define _DEVKIMAGE_H_

#include "devkImageConfiguration.h"
#include "../devkBasics.h"
#include "../queue/devkCommandBuffer.h"

#include <dragengine/deObject.h>

class devkDevice;
class devkCommandPool;


/**
 * Vulkan render pass.
 */
class devkImage : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkImage> Ref;
	
	
	
protected:
	devkDevice &pDevice;
	const devkImageConfiguration pConfiguration;
	
	VkImage pImage;
	uint32_t pImageSize;
	VkDeviceMemory pMemory;
	
	VkBuffer pBufferHost;
	VkDeviceMemory pBufferHostMemory;
	
	devkCommandBuffer::Ref pCommandBuffer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline. */
	devkImage( devkDevice &device, const devkImageConfiguration &configuration );
	
protected:
	/** Clean up pipeline. */
	virtual ~devkImage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Configuration. */
	inline const devkImageConfiguration &GetConfiguration() const{ return pConfiguration; }
	
	/** Image. */
	inline VkImage GetImage() const{ return pImage; }
	
	/** Host buffer or VK_NULL_HANDLE. */
	inline VkBuffer GetBufferHost() const{ return pBufferHost; }
	
	
	
	/** Copy data to host memory. */
	void SetData( const void *data );
	
	/** Copy data to host memory. */
	void SetData( const void *data, uint32_t offset, uint32_t size );
	
	/**
	 * Transfer data from host memory to device memory.
	 */
	void TransferToDevice(devkCommandBuffer &commandBuffer);
	
	/**
	 * Transfer data from host memory to device memory.
	 * \note Calls Wait() before starting the transfer.
	 * \note After call exist Wait() has to be called before using buffer.
	 */
	void TransferToDevice(devkCommandPool &pool);
	
	/**
	 * Transfer data from device memory to host memory.
	 */
	void FetchFromDevice(devkCommandBuffer &commandBuffer);
	
	/**
	 * Transfer data from device memory to host memory.
	 * \note Calls Wait() before starting the transfer.
	 * \note After call exist Wait() has to be called before using buffer.
	 */
	void FetchFromDevice(devkCommandPool &pool);
	
	/**
	 * Generate mipmap levels on device memoty.
	 * \note Calls Wait() before starting the transfer.
	 * \note After call exist Wait() has to be called before using buffer.
	 */
	void GenerateMipMaps(devkCommandPool &pool);
	
	/** Copy data from host memory. */
	void GetData( void *data );
	
	/** Copy data from host memory. */
	void GetData( void *data, uint32_t offset, uint32_t size );
	
	/** If command is active wait for command to be signaled. */
	void Wait();
	
	/** Drop resources used to transfer data if present. */
	void DropTransferResources();
	
	/** Ensure host buffer exists. */
	void EnsureHostBuffer();
	
	
	
	/** Add image memory barrier to transition image layout ignoring old content. */
	void BarrierLayoutTransition(devkCommandBuffer &commandBuffer, VkImageLayout newLayout);
	
	/** Add image memory barrier to transition image layout retaining old content. */
	void BarrierLayoutTransition(devkCommandBuffer &commandBuffer,
		VkImageLayout oldLayout, VkImageLayout newLayout);
	
	/** Add image memory barrier. */
	void Barrier(devkCommandBuffer &commandBuffer, VkAccessFlags sourceAccessMask,
		VkAccessFlags destAccessMask, VkPipelineStageFlags sourceStageMask,
		VkPipelineStageFlags destStageMask);
	
	/** Add image memory barrier between host writing and shader reading. */
	void BarrierHostShader(devkCommandBuffer &commandBuffer, VkPipelineStageFlags destStageMask);
	
	/** Add image memory barrier between shader writing and transfer reading. */
	void BarrierShaderTransfer(devkCommandBuffer &commandBuffer, VkPipelineStageFlags srcStageMask);
	
	/** Add image memory barrier between transfer writing and host reading. */
	void BarrierTransferHost(devkCommandBuffer &commandBuffer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateBuffer( VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
		VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size );
};

#endif

