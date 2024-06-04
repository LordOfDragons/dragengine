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

#ifndef _DEOXRSWAPCHAIN_H_
#define _DEOXRSWAPCHAIN_H_

#include "deoxrBasics.h"
#include "graphicapi/deoxrGraphicApiOpenGL.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoxrSession;


/**
 * Oxr swapchain.
 */
class deoxrSwapchain : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrSwapchain> Ref;
	
	/** Image. */
	struct sImage{
		uint32_t openglImage;
		deoxrGraphicApiOpenGL::Framebuffer::Ref openglFbo;
	};
	
	/** Type. */
	enum eType{
		etColor,
		etDepth
	};
	
	
	
private:
	deoxrSession &pSession;
	
	const eType pType;
	decPoint pSize;
	
	XrSwapchain pSwapchain;
	int64_t pImageFormat;
	
	sImage *pImages;
	int pImageCount;
	
	uint32_t pAcquiredImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create space. */
	deoxrSwapchain( deoxrSession &session, const decPoint &size, eType type );
	
protected:
	/** Clean up space. */
	virtual ~deoxrSwapchain();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Session. */
	inline deoxrSession &GetSession() const{ return pSession; }
	
	/** Type. */
	inline eType GetType() const{ return pType; }
	
	/** Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** Swapchain. */
	inline XrSwapchain GetSwapchain() const{ return pSwapchain; }
	
	/** Image format. */
	inline int64_t GetImageFormat() const{ return pImageFormat; }
	
	/** Count of images. */
	inline int GetImageCount() const{ return pImageCount; }
	
	/** Image at index. */
	const sImage &GetImageAt( int index ) const;
	
	/** Acquire image. */
	void AcquireImage();
	
	/** Release image. */
	void ReleaseImage();
	
	/** Acquired image. */
	inline uint32_t GetAcquiredImage() const{ return pAcquiredImage; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pGetImages();
};

#endif

