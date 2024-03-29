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

#ifndef _DEVKCOMMANDBUFFER_H_
#define _DEVKCOMMANDBUFFER_H_

#include "../devkBasics.h"
#include "../devkTPoolReference.h"
#include "../queue/devkCommandPool.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectLinkedList.h>
#include <dragengine/common/math/decMath.h>

class devkDevice;
class devkBuffer;
class devkPipeline;
class devkDescriptorSet;
class devkQueue;
class devkRenderPass;
class devkFramebuffer;
class devkImage;


/**
 * Vulkan command buffer.
 */
class devkCommandBuffer : public deObject{
public:
	/** Reference. */
	typedef devkTPoolReference<devkCommandBuffer> Ref;
	
	
	
private:
	devkCommandPool &pPool;
	
	VkCommandBuffer pBuffer;
	bool pRecording;
	const devkPipeline *pBoundPipeline;
	const devkRenderPass *pActiveRenderPass;
	
	VkFence pFence;
	bool pFenceActive;
	
	decObjectLinkedList::cListEntry pLLPool;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create command buffer. */
	devkCommandBuffer( devkCommandPool &pool );
	
protected:
	/** Clean up command buffer. */
	virtual ~devkCommandBuffer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Command pool. */
	inline devkCommandPool &GetPool() const{ return pPool; }
	
	/** Command buffer. */
	inline VkCommandBuffer GetBuffer() const{ return pBuffer; }
	
	
	
	/** Begin recording command buffer. */
	void Begin();
	
	/** Add buffer memory barrier. */
	void Barrier( const devkBuffer &buffer, bool useDeviceBuffer, VkAccessFlags sourceAccessMask,
		VkAccessFlags destAccessMask, VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags destStageMask );
	
	/** Add buffer memory barrier between host writing and shader reading. */
	void BarrierHostShader( const devkBuffer &buffer, VkPipelineStageFlags destStageMask );
	
	/** Add buffer memory barrier between shader writing and transfer reading. */
	void BarrierShaderTransfer( const devkBuffer &buffer, VkPipelineStageFlags srcStageMask );
	
	/** Add buffer memory barrier between transfer writing and host reading. */
	void BarrierTransferHost( const devkBuffer &buffer );
	
	/** Add image memory barrier. */
	void Barrier( const devkImage &image, VkAccessFlags sourceAccessMask, VkAccessFlags destAccessMask,
		VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags destStageMask );
	
	/** Add image memory barrier between host writing and shader reading. */
	void BarrierHostShader( const devkImage &image, VkPipelineStageFlags destStageMask );
	
	/** Add image memory barrier between shader writing and transfer reading. */
	void BarrierShaderTransfer( const devkImage &image, VkPipelineStageFlags srcStageMask );
	
	/** Add image memory barrier between transfer writing and host reading. */
	void BarrierTransferHost( const devkImage &image );
	
	/** Bind pipeline. */
	void BindPipeline( const devkPipeline &pipeline );
	
	/** Bind descriptor set. */
	void BindDescriptorSet( int bindPoint, const devkDescriptorSet &descriptorSet );
	
	/** Bind vertex buffers. */
	void BindVertexBuffer( int bindPoint, const devkBuffer &buffer, int offset = 0 );
	
	/** Dispatch compute shader. */
	void DispatchCompute( const decPoint3 &group );
	void DispatchCompute( int groupX, int groupY, int groupZ );
	
	/** Begin render pass. */
	void BeginRenderPass( const devkRenderPass &renderPass, const devkFramebuffer &framebuffer );
	
	void BeginRenderPass( const devkRenderPass &renderPass, const devkFramebuffer &framebuffer,
		const decPoint &position, const decPoint &size );
	
	/** Set stencil compare mask. */
	void SetStencilCompareMask( uint32_t front );
	void SetStencilCompareMask( uint32_t front, uint32_t back );
	
	/** Set stencil write mask. */
	void SetStencilWriteMask( uint32_t front );
	void SetStencilWriteMask( uint32_t front, uint32_t back );
	
	/** Set stencil reference. */
	void SetStencilReference( uint32_t front );
	void SetStencilReference( uint32_t front, uint32_t back );
	
	/** Draw. */
	void Draw( int vertexCount, int instanceCount, int firstVertex = 0, int firstInstance = 0 );
	
	/** End render pass. */
	void EndRenderPass();
	
	/** Write buffer data from host memory to device memory. */
	void WriteBuffer( const devkBuffer &buffer );
	
	/** Read buffer data from device memory to host memory. */
	void ReadBuffer( const devkBuffer &buffer );
	
	/** Read image data from device memory to host memory. */
	void ReadImage( devkImage &image );
	
	/** End recording command buffer. */
	void End();
	
	/** Submit to queue. */
	void Submit( devkQueue &queue );
	
	
	
	/**
	 * Check if fence is signaled.
	 * \param[in] reset If true reset fence and set it inactive after wait finished.
	 */
	bool IsFinished( bool reset = false );
	
	/**
	 * If fence is active wait for fence to be signaled.
	 * \param[in] reset If true reset fence and set it inactive after wait finished.
	 */
	void Wait( bool reset = false );
	
	/** Return to pool. */
	void ReturnToPool();
	
	/** Command pool linked list. */
	inline decObjectLinkedList::cListEntry &GetLLPool(){ return pLLPool; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

