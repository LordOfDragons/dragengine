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
	VkDeviceSize pImageSize;
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

