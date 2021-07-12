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
	devkPipeline *pBoundPipeline;
	
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
	void Barrier( devkBuffer *buffer, VkAccessFlags sourceAccessMask, VkAccessFlags destAccessMask,
		VkPipelineStageFlags sourceStageMask, VkPipelineStageFlags destStageMask );
	
	/** Add buffer memory barrier from host to shader. */
	void BarrierHostShader( devkBuffer *buffer, VkPipelineStageFlags destStageMask );
	
	/** Bind pipeline. */
	void BindPipeline( devkPipeline *pipeline );
	
	/** Bind descriptor set. */
	void BindDescriptorSet( int bindPoint, devkDescriptorSet *descriptorSet );
	
	/** Dispatch compute shader. */
	void DispatchCompute( const decPoint3 &group );
	void DispatchCompute( int groupX, int groupY, int groupZ );
	
	
	
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

