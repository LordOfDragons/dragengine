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
#include "../queue/devkCommandPool.h"

#include <dragengine/deObject.h>

class devkDevice;
class devkQueue;


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
	
	VkFence pFence;
	bool pFenceActive;
	devkCommandPool::Ref pCommandPool;
	VkCommandBuffer pCommand;
	
	
	
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
	
	/** Drop resources used to transfer data if present. */
	void DropTransferResources();
	
	/** Ensure host buffer exists. */
	void EnsureHostBuffer();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateBuffer( VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperty,
		VkBuffer *buffer, VkDeviceMemory *memory, VkDeviceSize size );
	
	void pCreateFence();
};

#endif

