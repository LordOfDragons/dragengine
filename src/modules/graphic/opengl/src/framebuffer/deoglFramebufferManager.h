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

#ifndef _DEOGLFRAMEBUFFERMANAGER_H_
#define _DEOGLFRAMEBUFFERMANAGER_H_

#include <dragengine/common/collection/decObjectList.h>

class deoglRenderThread;
class deoglFramebuffer;


/**
 * Framebuffer manager.
 * Manages framebuffers. Hands out framebuffers of a given size creating
 * or reusing framebuffers to keep one framebuffer for each resolution
 * asked for. This avoids slowdown due to changing the resolution of
 * framebuffer attachments. On some systems the opposite is true though.
 * If the use only one FBO flag is set only one FBO is created and handed
 * out.
 */
class deoglFramebufferManager{
public:
	/** Framebuffer usage. */
	class Usage{
	private:
		deoglFramebuffer *pFBO;
		
		
	public:
		/** \name Constructors and Destructors */
		/*@{*/
		/** Create empty usage. */
		Usage();
		
		/** Create usage for framebuffer. */
		explicit Usage(const Usage &usage);
		
		/** Cleans up usage of framebuffer. */
		~Usage();
		/*@}*/
		
		
		/** \name Management */
		/*@{*/
		/** Framebuffer or nullptr. */
		inline deoglFramebuffer* Get() const{ return pFBO; }
		inline operator deoglFramebuffer*() const{ return pFBO; }
		inline deoglFramebuffer* operator->() const { return pFBO; }
		
		/** Check if empty. */
		inline bool IsNull() const{ return pFBO == nullptr; }
		inline bool IsNotNull() const{ return pFBO != nullptr; }
		inline operator bool() const{ return pFBO != nullptr; }
		
		/** Clear. */
		void Clear();
		
		/** Copy operator. */
		Usage &operator=(const Usage &usage);
		/*@}*/
		
		
	private:
		Usage(deoglFramebuffer *fbo);
		friend class deoglFramebufferManager;
	};
	
	
private:
	deoglRenderThread &pRenderThread;
	decObjectList pFBOs;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create framebuffer manager. */
	deoglFramebufferManager(deoglRenderThread &renderThread);
	
	/** Clean up framebuffer manager. */
	~deoglFramebufferManager() = default;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Count of framebuffers. */
	int GetFBOCount() const;
	
	/**
	 * Framebuffer for reading only at the given location.
	 * 
	 * \warning Debug use only.
	 */
	const deoglFramebuffer &GetFBOAt(int index) const;
	
	/** Framebuffer for the given resolution. */
	Usage GetFBOWithResolution(int width, int height);
	/*@}*/
};

// end of include only once
#endif
