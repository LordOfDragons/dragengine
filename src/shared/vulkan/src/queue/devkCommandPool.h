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

#ifndef _DEVKCOMMANDPOOL_H_
#define _DEVKCOMMANDPOOL_H_

#include "../devkBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectLinkedList.h>

class devkDevice;
class devkCommandBuffer;


/**
 * Vulkan command pool.
 */
class devkCommandPool : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkCommandPool> Ref;
	
	
	
private:
	devkDevice &pDevice;
	
	VkCommandPool pPool;
	decObjectLinkedList pFreeCommandBuffers;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create command pool. */
	devkCommandPool( devkDevice &device, uint32_t queueFamily );
	
protected:
	/** Clean up queue. */
	virtual ~devkCommandPool();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Command pool. */
	inline VkCommandPool GetPool() const{ return pPool; }
	
	/** Get next free command buffer. */
	devkCommandBuffer *GetCommandBuffer();
	
	/** Return command buffer to pool. */
	void ReturnCommandBuffer( devkCommandBuffer *commandBuffer );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

