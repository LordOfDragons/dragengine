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

#ifndef _DEVIDEO_H_
#define _DEVIDEO_H_

#include "../deFileResource.h"
#include "../../common/math/decMath.h"

class deVideoManager;
class deBaseGraphicVideo;


/**
 * \brief Video class.
 *
 * Stores a video. Videos are sequences of images used to play short videos once
 * or in a loop. Videos can be of streaming or non-streaming type. For
 * non-streaming videos the data is loaded into a memory buffer shared. This is
 * the best choice for small videos which do not require a lot of memory or are
 * used in many places. For streaming videos no memory buffer is held. Streaming
 * videos use a Video Decoder object to retrieve the image data. This is the best
 * choice for large videos or videos which are used only a few times. The format
 * of the video data can be RGB or YUV. The same data structures as in the image
 * resource can be found. For the YUV format the mapping r=y, g=u and b=v is used.
 */
class deVideo : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVideo> Ref;
	
	
	
public:
	/** \brief Pixel format. */
	enum ePixelFormat{
		/** \brief Pixels are stored in the 4:4:4 format. */
		epf444,
		
		/** \brief Pixels are stored in the 4:2:2 format. */
		epf422,
		
		/** \brief Pixels are stored in the 4:2:0 format. */
		epf420,
		
		/** \brief Pixels are stored in the 4:4:4:4 format. */
		epf4444
	};
	
	
	
private:
	int pWidth;
	int pHeight;
	ePixelFormat pPixelFormat;
	int pFrameCount;
	int pFrameRate;
	float pPlayTime;
	decColorMatrix3 pColorConversionMatrix;
	
	int pBytesPerSample;
	int pSampleCount;
	int pSampleRate;
	int pChannelCount;
	
	deBaseGraphicVideo *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video. */
	deVideo( deVideoManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime, int width, int height, ePixelFormat pixelFormat,
		int frameRate, int frameCount, const decColorMatrix3 &colorConversionMatrix,
		int bytesPerSample, int sampleCount, int sampleRate, int channelCount );
	
	/**
	 * \brief Create video for internal loading.
	 * \warning For use by deResourceLoader only.
	 */
	deVideo( deVideoManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up video object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVideo();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Width in pixels. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Pixel format. */
	inline ePixelFormat GetPixelFormat() const{ return pPixelFormat; }
	
	/** \brief Number of frames. */
	inline int GetFrameCount() const{ return pFrameCount; }
	
	/** \brief Frame rate. */
	inline int GetFrameRate() const{ return pFrameRate; }
	
	/** \brief Play time in seconds. */
	inline float GetPlayTime() const{ return pPlayTime; }
	
	/** \brief Color conversion matrix. */
	inline const decColorMatrix3 &GetColorConversionMatrix() const{ return pColorConversionMatrix; }
	
	/** \brief Bytes per sample or 0 if no audio. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Number of samples or 0 if no audio. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Number of channels or 0 if no audio. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate or 0 if no audio. */
	inline int GetSampleRate() const{ return pSampleRate; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicVideo *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic( deBaseGraphicVideo *peer );
	/*@}*/
	
	
	
	/** \name Special */
	/*@{*/
	/**
	 * \brief Finalize construction after asynchronous loading.
	 * \warning For use by deResourceLoader only.
	 */
	void FinalizeConstruction( int width, int height, ePixelFormat pixelFormat,
		int frameRate, int frameCount, const decColorMatrix3 &colorConversionMatrix,
		int bytesPerSample, int sampleCount, int sampleRate, int channelCount );
	/*@}*/
};

#endif
