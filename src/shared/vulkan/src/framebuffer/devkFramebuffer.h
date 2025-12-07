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

#ifndef _DEVKFRAMEBUFFER_H_
#define _DEVKFRAMEBUFFER_H_

#include "devkFramebufferConfiguration.h"
#include "../devkBasics.h"
#include "../renderpass/devkRenderPass.h"

#include <dragengine/deObject.h>


/**
 * Vulkan framebuffer.
 */
class devkFramebuffer : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<devkFramebuffer> Ref;
	
	
protected:
	devkRenderPass::Ref pRenderPass;
	const devkFramebufferConfiguration pConfiguration;
	
	VkFramebuffer pFramebuffer;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create pipeline. */
	devkFramebuffer(devkRenderPass *renderPass, const devkFramebufferConfiguration &configuration);
	
protected:
	/** Clean up pipeline. */
	virtual ~devkFramebuffer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Render pass. */
	inline devkRenderPass &GetRenderPass() const{return pRenderPass;}
	
	/** Configuration. */
	inline const devkFramebufferConfiguration &GetConfiguration() const{return pConfiguration;}
	
	/** Framebuffer. */
	inline VkFramebuffer GetFramebuffer() const{return pFramebuffer;}
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

