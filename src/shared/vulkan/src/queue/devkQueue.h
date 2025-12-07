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

#ifndef _DEVKQUEUE_H_
#define _DEVKQUEUE_H_

#include "../devkBasics.h"

#include <dragengine/deObject.h>

class devkDevice;
class devkCommandPool;


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
	uint32_t pIndex;
	
	VkQueue pQueue;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create queue. */
	devkQueue(devkDevice &device, uint32_t family, uint32_t index, VkQueue queue);
	
protected:
	/** Clean up queue. */
	~devkQueue() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Family. */
	inline uint32_t GetFamily() const{ return pFamily; }
	
	/** Index. */
	inline uint32_t GetIndex() const{ return pIndex; }
	
	/** Queue. */
	inline VkQueue GetQueue() const{ return pQueue; }
	inline operator VkQueue() const{ return pQueue; }
	
	/** Create command pool compatible with this queue and all queues of the same family. */
	deTObjectReference<devkCommandPool> CreateCommandPool();
	
	/** Wait for queue to be idle. */
	void WaitIdle();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

