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

#ifndef _DEVKDESCRIPTORSETLAYOUTMANAGER_H_
#define _DEVKDESCRIPTORSETLAYOUTMANAGER_H_

#include <dragengine/common/collection/decObjectList.h>

class devkDevice;
class devkDescriptorSetLayout;
class devkDescriptorSetLayoutConfiguration;


/**
 * Descriptor set layout manager.
 */
class devkDescriptorSetLayoutManager{
private:
	devkDevice &pDevice;
	
	decObjectList pLayouts;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create descriptor set layout manager. */
	devkDescriptorSetLayoutManager(devkDevice &device);
	
	/** Clean up descriptor set layouts manager. */
	~devkDescriptorSetLayoutManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Device. */
	inline devkDevice &GetDevice() const{ return pDevice; }
	
	/** Count of descriptor set layouts. */
	int GetCount() const;
	
	/** Descriptor set layout at index. Caller does not hold reference. */
	devkDescriptorSetLayout *GetAt(int index) const;
	
	/** Descriptor set layout with configuration creating it if absent. Caller does not hold reference. */
	devkDescriptorSetLayout *GetWith(const devkDescriptorSetLayoutConfiguration &configuration);
	
	/** Descriptor set layout with configuration is present. */
	bool HasWith(const devkDescriptorSetLayoutConfiguration &configuration) const;
	
	/** Remove all descriptor set layouts. */
	void Clear();
	/*@}*/
};

#endif
