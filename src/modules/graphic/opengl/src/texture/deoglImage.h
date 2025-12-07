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

#ifndef _DEOGLIMAGE_H_
#define _DEOGLIMAGE_H_

#include "deoglRImage.h"
#include "pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicImage.h>
#include <dragengine/threading/deMutex.h>

class deGraphicOpenGl;
class deImage;



/**
 * Image Peer.
 * 
 * Images can be used by two different types of parties.
 * 
 * The first use is by entities requiring the image data to be present as deoglRImage texture.
 * For these parties the deoglRImage texture is always created.
 * 
 * The second type of entities require only the pixel buffer data to do some processing work.
 * These entities have to call CreatePixelBuffer() to add a use to the pixel buffer. If the
 * pixel buffer is not present yet image data is retained and released if required and the
 * pixel buffer created. Afterwards the use count is incremented by one. Once the pixel buffer
 * is not required anymore ReleasePixelBuffer() has to be called and the use count is
 * decremented by one. If the use count drops to 0 the pixel buffer is released.
 * 
 * For the first usage type an own pixel buffer different from the second use case is created.
 * This is required since this pixel buffer is handed over to deoglRImage which in turn owns
 * now the pixel buffer.
 */
class deoglImage : public deBaseGraphicImage{
private:
	deGraphicOpenGl &pOgl;
	deImage &pImage;
	
	deoglRImage::Ref pRImage;
	
	deMutex pMutex;
	int pPixelBufferUseCount;
	deoglPixelBuffer::Ref pPixelBuffer;
	deoglPixelBuffer::Ref pPixelBufferRImageTexture;
	bool pDirtyTexture;
	
	decPointerSet pNotifyCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create image. */
	deoglImage(deGraphicOpenGl &ogl, deImage &image);
	
	/** Clean up image. */
	virtual ~deoglImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Image resource. */
	inline const deImage &GetImage() const{ return pImage; }
	
	/** Render image or nullptr if not created. */
	inline const deoglRImage::Ref &GetRImage() const{ return pRImage; }
	
	/**
	 * Update render thread counterpart if required.
	 * \warning Only allowed to be called from main thread.
	 */
	void SyncToRender();
	
	/** Mark texture dirty. */
	void MarkTextureDirty();
	
	/** Canvas to notify about dirty events. */
	inline decPointerSet &GetNotifyCanvas(){ return pNotifyCanvas; }
	inline const decPointerSet &GetNotifyCanvas() const{ return pNotifyCanvas; }
	
	
	
	/**
	 * Add use to pixel buffer ensuring pixel buffer is created.
	 * 
	 * If pixel buffer is not created image data is retained and released if required and
	 * the pixel buffer created. Increments use count by one.
	 * 
	 * \note Can be called from non-main threads.
	 */
	void CreatePixelBuffer();
	
	/**
	 * Remove use to pixel buffer releasing it if not used anymore.
	 * 
	 * Decrements use count by one. If use count drops to 0 pixel buffer is released.
	 * 
	 * \note Can be called from non-main threads.
	 */
	void ReleasePixelBuffer();
	
	/**
	 * Pixel buffer present only if CreatePixelBuffer() has been called.
	 * */
	inline const deoglPixelBuffer::Ref &GetPixelBuffer() const{ return pPixelBuffer; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Image data changed. */
	virtual void ImageDataChanged();
	
	/** Image data has been restored from original file. */
	virtual void ImageDataRestored();
	
	/**
	 * Graphic module requires image data to remain loaded.
	 * 
	 * Used to optimized memory consumption. Default implementation returns \em false.
	 */
	virtual bool RetainImageData();
	/*@}*/
	
private:
	deoglPixelBuffer::Ref pCreatePixelBuffer();
	void pCreatePixelBufferSafe(deoglPixelBuffer::Ref &pixelBuffer);
	void pRequiresSync();
};

#endif
