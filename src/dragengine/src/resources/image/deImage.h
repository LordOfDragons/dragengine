/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEIMAGE_H_
#define _DEIMAGE_H_

#include "../deFileResource.h"
#include "../../threading/deMutex.h"

class decXpmImage;
class deImageManager;
class deBaseGraphicImage;
class deResourceManager;


/** \brief 8-Bit Grayscale */
struct sGrayscale8{
	unsigned char value;
};

/** \brief 16-Bit Grayscale */
struct sGrayscale16{
	unsigned short value;
};

/** \brief 32-Bit Grayscale */
struct sGrayscale32{
	float value;
};


/** \brief 8-Bit Grayscale-Alpha */
struct sGrayscaleAlpha8{
	unsigned char value;
	unsigned char alpha;
};

/** \brief 16-Bit Grayscale-Alpha */
struct sGrayscaleAlpha16{
	unsigned short value;
	unsigned short alpha;
};

/** \brief 32-Bit Grayscale-Alpha */
struct sGrayscaleAlpha32{
	float value;
	float alpha;
};


/** \brief 8-Bit Red-Green-Blue */
struct sRGB8{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

/** \brief 16-Bit Red-Green-Blue */
struct sRGB16{
	unsigned short red;
	unsigned short green;
	unsigned short blue;
};

/** \brief 32-Bit Red-Green-Blue */
struct sRGB32{
	float red;
	float green;
	float blue;
};


/** \brief 8-Bit Red-Green-Blue-Alpha */
struct sRGBA8{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

/** \brief 16-Bit Red-Green-Blue-Alpha */
struct sRGBA16{
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	unsigned short alpha;
};

/** \brief 32-Bit Red-Green-Blue-Alpha */
struct sRGBA32{
	float red;
	float green;
	float blue;
	float alpha;
};


/**
 * \brief Image resource.
 * 
 * Image resources are retrieved from the deImageManager bei either loading an image from file
 * or creating a new one. Images loaded from file are treated read-only while created images
 * can be read or write.
 * 
 * If the image is loaded from a file the peers are asked if they need the image data to be
 * retained. If nobody needs the image data retained it is release to reduce memory consumption,
 * which is important on some platforms.
 * 
 * If the peers or any other party needs the image data back in memory it can be retained by
 * calling RetainImageData(). If the image data is no more required ReleaseImageData() has
 * to be called. Whenever nobody needs the image data anymore it is released from memory.
 * 
 * All image parameters except the image data itself can be always accessed safely. Due to the
 * retaining ability the image data itself (all GetData*() calls) can only be accessed when
 * the caller called RetainImageData() earlier. Otherwise GetData*() can return NULL if the
 * image data is currently not retained.
 * 
 * Calling RetainImageData() and ReleaseImageData() uses mutex locking internally to avoid
 * race conditions. Modifications of image data is only allowed by the game scripts during
 * main thread frame updating of the respective scripting module. For accessing the data no
 * locking is required since GetData() is only valid after RetainImageData() finished.
 */
class deImage : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deImage> Ref;
	
	
	
private:
	int pWidth;
	int pHeight;
	int pDepth;
	int pComponentCount;
	int pBitCount;
	unsigned char *pData;
	int pRetainImageData;
	deMutex pMutex;
	
	deBaseGraphicImage *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create image.
	 * \param[in] manager Resource manager to link to.
	 * \param[in] vfs Virtual file system.
	 * \param[in] filename Filename of the image.
	 * \param[in] width Width of the image at least 1.
	 * \param[in] height Height of the image at least 1.
	 * \param[in] components Number of components (1 to 4 inclusive).
	 * \param[in] bitCount Bit Count of each component (8, 16 or 32).
	 */
	deImage( deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime, int width, int height, int depth,
		int componentCount, int bitCount );
	
	/**
	 * \brief Create image object from XPM image.
	 * \param[in] manager Resource manager to link to.
	 * \param[in] vfs Virtual file system.
	 * \param[in] filename Filename of the image.
	 * \param[in] image Image data.
	 */
	deImage( deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime, decXpmImage *image );
	
	/**
	 * \brief Create image for internal loading.
	 * \warning This is a special internal constructor. Never ever call this on your own!
	 */
	deImage( deImageManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up resource.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deImage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Width in pixels. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Depth in pixels. */
	inline int GetDepth() const{ return pDepth; }
	
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Bit count of each component. */
	inline int GetBitCount() const{ return pBitCount; }
	
	
	
	/**
	 * \brief 8-bit Grayscale image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sGrayscale8 *GetDataGrayscale8() const;
	
	/**
	 * \brief 16-bit Grayscale image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sGrayscale16 *GetDataGrayscale16() const;
	
	/**
	 * \brief 32-bit Grayscale image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sGrayscale32 *GetDataGrayscale32() const;
	
	/**
	 * \brief 8-bit Grayscale-Alpha image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sGrayscaleAlpha8 *GetDataGrayscaleAlpha8() const;
	
	/**
	 * \brief 16-bit Grayscale-Alpha image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sGrayscaleAlpha16 *GetDataGrayscaleAlpha16() const;
	
	/**
	 * \brief 32-bit Grayscale-Alpha image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sGrayscaleAlpha32 *GetDataGrayscaleAlpha32() const;
	
	/**
	 * \brief 8-bit RGB image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sRGB8 *GetDataRGB8() const;
	
	/**
	 * \brief 16-bit RGB image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sRGB16 *GetDataRGB16() const;
	
	/**
	 * \brief 32-bit RGB image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sRGB32 *GetDataRGB32() const;
	
	/**
	 * \brief 8-bit RGBA image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sRGBA8 *GetDataRGBA8() const;
	
	/**
	 * \brief 16-bit RGBA image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sRGBA16 *GetDataRGBA16() const;
	
	/**
	 * \brief 32-bit RGBA image data.
	 * \see GetData().
	 * \throws deeInvalidParam Image format mismatch.
	 */
	sRGBA32 *GetDataRGBA32() const;
	
	/**
	 * \brief Direct image data pointer.
	 * 
	 * Returns NULL if image data is not retained. To make sure the image data is
	 * present call RetainImageData() and ReleaseImageData() when you do not need the
	 * data anymore in the future. Calls to RetainImageData() and ReleaseImageData()
	 * have to be balanced.
	 * 
	 * \warning This is a dangerous function. Use it only if you know what you are doing.
	 */
	inline void *GetData() const{ return pData; }
	
	/** \brief Notify peers image data changed. */
	void NotifyImageDataChanged();
	
	
	
	/**
	 * \brief Retain image data count.
	 * 
	 * For debug use.
	 */
	inline int GetRetainImageDataCount() const{ return pRetainImageData; }
	
	/**
	 * \brief Retain image data.
	 * 
	 * For use by script modules and engine users requiring to access image data. Increments
	 * retain counter by one. If the counter has been 0 the image data is loaded. Otherwise
	 * this call has no effect. Call ReleaseImageData() if you do not need the data anymore
	 * in the future. Calls to RetainImageData() and ReleaseImageData() have to be balanced.
	 * 
	 * \note If the image has been created with empty filename the retain counter
	 * is set to 1 instead of 0.
	 * 
	 * \note This call takes a lock on the image mutex while running.
	 */
	void RetainImageData();
	
	/**
	 * \brief Release image data.
	 * 
	 * For use by script modules and engine users requiring to access image data. Decrements
	 * retain counter by one. If the counter reaches 0 the image data is unloaded. Otherwise
	 * this call has no effect. Calls to RetainImageData() and ReleaseImageData() have to be
	 * balanced.
	 * 
	 * \note If the image has been created with empty filename the retain counter
	 * is set to 1 instead of 0.
	 * 
	 * \note This call takes a lock on the image mutex while running.
	 */
	void ReleaseImageData();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicImage *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicImage *peer );
	/*@}*/
	
	
	
	/** \name Special */
	/*@{*/
	/**
	 * \brief Finalize construction for internal loading.
	 * \warning This is a special internal constructor. Never ever call this on your own!
	 */
	void FinalizeConstruction( int width, int height, int depth, int componentCount, int bitCount );
	
	/**
	 * \brief Retain image data for peers requiring it.
	 * \warning This is for internal use only. Do not call on your own.
	 */
	void PeersRetainImageData();
	/*@}*/
};

#endif
