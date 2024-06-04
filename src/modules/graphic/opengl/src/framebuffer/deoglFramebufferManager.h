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

// include only once
#ifndef _DEOGLFRAMEBUFFERMANAGER_H_
#define _DEOGLFRAMEBUFFERMANAGER_H_

// predefinitions
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
private:
	deoglRenderThread &pRenderThread;
	
	deoglFramebuffer **pFBOs;
	int pFBOCount;
	int pFBOSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new framebuffer manager. */
	deoglFramebufferManager( deoglRenderThread &renderThread );
	/** Cleans up the framebuffer manager. */
	~deoglFramebufferManager();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of framebuffers. */
	inline int GetFBOCount() const{ return pFBOCount; }
	/** Retrieves the framebuffer for reading only at the given location. */
	const deoglFramebuffer *GetFBOAt( int index ) const;
	
	/** Retrieves the framebuffer for the given resolution. */
	deoglFramebuffer *GetFBOWithResolution( int width, int height );
	/*@}*/
};

// end of include only once
#endif
