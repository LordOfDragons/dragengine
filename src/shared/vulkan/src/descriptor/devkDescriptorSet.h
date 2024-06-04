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

#ifndef _DEVKDESCRIPTORSET_H_
#define _DEVKDESCRIPTORSET_H_

#include "devkDescriptorPoolSlot.h"
#include "../devkBasics.h"
#include "../devkDevice.h"
#include "../buffer/devkBuffer.h"

#include <dragengine/deObject.h>

class devkDescriptorPool;


/**
 * Vulkan descriptor set. Constructor obtains a set from the pool and returns it to the pool
 * in the destructor. Bindings are created at construction time and set to nullptr. Assign
 * bindings then call Update() to write bindings to descriptor set.
 */
class devkDescriptorSet : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkDescriptorSet> Ref;
	
	
	
private:
	devkDescriptorPool &pPool;
	
	devkDescriptorPoolSlot::Ref pSlot;
	
	VkDescriptorSetLayout pLayout;
	VkDescriptorSet pSet;
	
	VkDescriptorBufferInfo *pBindings;
	VkWriteDescriptorSet *pWriteSets;
	devkBuffer::Ref *pBuffers;
	int pBindingCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor set. */
	devkDescriptorSet( devkDescriptorPool &pool );
	
protected:
	/** Clean up descriptor set. */
	virtual ~devkDescriptorSet();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pool. */
	inline devkDescriptorPool &GetPool() const{ return pPool; }
	
	/** Layout. */
	inline VkDescriptorSetLayout GetLayout() const{ return pLayout; }
	
	/** Descriptor set. */
	inline VkDescriptorSet GetSet() const{ return pSet; }
	
	/** Set binding. */
	void SetBinding( int index, devkBuffer *buffer,
		VkDeviceSize offset = 0, VkDeviceSize range = VK_WHOLE_SIZE );
	
	/** Clear binding. */
	void ClearBinding( int index );
	
	/** Clear all bindings. */
	void ClearAllBindings();
	
	/** Write bindings to descriptor set. */
	void Update();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

