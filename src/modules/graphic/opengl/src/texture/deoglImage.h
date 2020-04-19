/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEOGLIMAGE_H_
#define _DEOGLIMAGE_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicImage.h>
#include <dragengine/threading/deMutex.h>

class deoglPixelBuffer;
class deoglRImage;
class deGraphicOpenGl;
class deImage;



/**
 * \brief Image Peer.
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
	
	deoglRImage *pRImage;
	
	deMutex pMutex;
	int pPixelBufferUseCount;
	deoglPixelBuffer *pPixelBuffer;
	deoglPixelBuffer *pPixelBufferRImageTexture;
	bool pDirtyTexture;
	
	decPointerSet pNotifyCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create image. */
	deoglImage( deGraphicOpenGl &ogl, deImage &image );
	
	/** \brief Clean up image. */
	virtual ~deoglImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Image resource. */
	inline const deImage &GetImage() const{ return pImage; }
	
	/** \brief Render image or \em NULL if not created. */
	inline deoglRImage *GetRImage() const{ return pRImage; }
	
	/**
	 * \brief Update render thread counterpart if required.
	 * \warning Only allowed to be called from main thread.
	 */
	void SyncToRender();
	
	/** \brief Mark texture dirty. */
	void MarkTextureDirty();
	
	/** \brief Canvas to notify about dirty events. */
	inline decPointerSet &GetNotifyCanvas(){ return pNotifyCanvas; }
	inline const decPointerSet &GetNotifyCanvas() const{ return pNotifyCanvas; }
	
	
	
	/**
	 * \brief Add use to pixel buffer ensuring pixel buffer is created.
	 * 
	 * If pixel buffer is not created image data is retained and released if required and
	 * the pixel buffer created. Increments use count by one.
	 * 
	 * \note Can be called from non-main threads.
	 */
	void CreatePixelBuffer();
	
	/**
	 * \brief Remove use to pixel buffer releasing it if not used anymore.
	 * 
	 * Decrements use count by one. If use count drops to 0 pixel buffer is released.
	 * 
	 * \note Can be called from non-main threads.
	 */
	void ReleasePixelBuffer();
	
	/**
	 * \brief Pixel buffer present only if CreatePixelBuffer() has been called.
	 * */
	inline deoglPixelBuffer *GetPixelBuffer() const{ return pPixelBuffer; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Image data changed. */
	virtual void ImageDataChanged();
	
	/** \brief Image data has been restored from original file. */
	virtual void ImageDataRestored();
	
	/**
	 * \brief Graphic module requires image data to remain loaded.
	 * 
	 * Used to optimized memory consumption. Default implementation returns \em false.
	 */
	virtual bool RetainImageData();
	/*@}*/
	
private:
	void pCleanUp();
	
	deoglPixelBuffer *pCreatePixelBuffer();
	void pCreatePixelBufferSafe( deoglPixelBuffer* &pixelBuffer );
	void pRequiresSync();
};

#endif
