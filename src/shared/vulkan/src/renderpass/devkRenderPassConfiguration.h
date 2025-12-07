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

#ifndef _DEVKRENDERPASSCONFIGURATION_H_
#define _DEVKRENDERPASSCONFIGURATION_H_

#include "../devkBasics.h"

#include <dragengine/common/math/decMath.h>


/**
 * Vulkan render pass configuration.
 */
class devkRenderPassConfiguration{
public:
	/** Enter type. */
	enum pInType{
		eitClear,
		eitLoad,
		eitDiscard
	};
	
	/** End type. */
	enum eOutType{
		eotRender,
		eotShader,
		eotReadBack,
		eotDisplay,
		eotDiscard
	};
	
	/** Sub pass configuration. */
	struct sSubPass{
		int colorAttachments[8];
		int depthStencilAttachment;
	};
	
	
	
private:
	int pAttachmentCount;
	VkAttachmentDescription *pAttachments;
	VkClearValue *pClearValues;
	int pSubPassCount;
	sSubPass *pSubPasses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render pass configuration. */
	devkRenderPassConfiguration();
	
	/** Create copy of render pass configuration. */
	devkRenderPassConfiguration(const devkRenderPassConfiguration &configuration);
	
	/** Clean up render pass configuration. */
	~devkRenderPassConfiguration();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Attachment count. */
	inline int GetAttachmentCount() const{ return pAttachmentCount; }
	
	/** Set attachment count. */
	void SetAttachmentCount(int count);
	
	/** Attachment at index. */
	const VkAttachmentDescription &GetAttachmentAt(int index) const;
	
	/** Set attachment at index. */
	void SetAttachmentAt(int index, const VkAttachmentDescription &attachment);
	
	/** Set color attachment at index. */
	void SetColorAttachmentAt(int index, VkFormat format, pInType in, eOutType out);
	
	/** Clear value at index. */
	const VkClearValue &GetClearValueAt(int index) const;
	
	/** Set clear value at index. */
	void SetClearValueAt(int index, const VkClearValue &clearValue);
	
	/** Set color clear value at index. */
	void SetClearValueColorFloatAt(int index, float red, float green, float blue, float alpha);
	void SetClearValueColorIntAt(int index, int red, int green, int blue, int alpha);
	void SetClearValueColorUIntAt(int index, int red, int green, int blue, int alpha);
	
	/** Set depth/stencil clear value at index. */
	void SetClearValueDepthAt(int index, float depth, int stencil);
	
	/** Internal use only. */
	inline const VkAttachmentDescription *GetAttachments() const{ return pAttachments; }
	inline const VkClearValue *GetClearValues() const{ return pClearValues; }
	
	
	
	/** Sub pass count. */
	inline int GetSubPassCount() const{ return pSubPassCount; }
	
	/** Set sub pass count. */
	void SetSubPassCount(int count);
	
	/** Sub pass at index. */
	const sSubPass &GetSubPassAt(int index) const;
	
	/** Set sub pass at index. */
	void SetSubPassAt(int index, const sSubPass &subpass);
	
	/** Set sub pass at index. */
	void SetSubPassAt(int index, int depthStencil, int color1 = -1, int color2 = -1,
		int color3 = -1, int color4 = -1, int color5 = -1, int color6 = -1,
		int color7 = -1, int color8 = -1);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Configurations are equal. */
	bool operator==(const devkRenderPassConfiguration &configuration) const;
	
	/** Copy configuration. */
	devkRenderPassConfiguration &operator=(const devkRenderPassConfiguration &configuration);
	/*@}*/
};

#endif

