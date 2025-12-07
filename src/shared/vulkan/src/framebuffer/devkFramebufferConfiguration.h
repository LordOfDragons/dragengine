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

#ifndef _DEVKFRAMEBUFFERCONFIGURATION_H_
#define _DEVKFRAMEBUFFERCONFIGURATION_H_

#include "../devkBasics.h"
#include "../image/devkImageView.h"

#include <dragengine/common/math/decMath.h>


/**
 * Vulkan framebuffer configuration.
 */
class devkFramebufferConfiguration{
private:
	int pAttachmentCount;
	devkImageView::Ref *pAttachments;
	decPoint pSize;
	int pLayerCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create framebuffer configuration. */
	devkFramebufferConfiguration();
	
	/** Create copy of framebuffer configuration. */
	devkFramebufferConfiguration(const devkFramebufferConfiguration &configuration);
	
	/** Clean up framebuffere configuration. */
	~devkFramebufferConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Attachment count. */
	inline int GetAttachmentCount() const{return pAttachmentCount;}
	
	/** Set attachment count. */
	void SetAttachmentCount(int count);
	
	/** Attachment at index. */
	devkImageView *GetAttachmentAt(int index) const;
	
	/** Set attachment at index. */
	void SetAttachmentAt(int index, devkImageView *attachment);
	
	/** Size. */
	inline const decPoint &GetSize() const{return pSize;}
	
	/** Set size. */
	void SetSize(const decPoint &size);
	
	/** Layer count. */
	inline int GetLayerCount() const{return pLayerCount;}
	
	/** Set layer count. */
	void SetLayerCount(int count);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==(const devkFramebufferConfiguration &configuration) const;
	
	/** Copy configuration. */
	devkFramebufferConfiguration &operator=(const devkFramebufferConfiguration &configuration);
	/*@}*/
};

#endif

