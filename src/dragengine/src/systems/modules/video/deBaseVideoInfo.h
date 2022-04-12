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

#ifndef _DEBASEVIDEOINFOS_H_
#define _DEBASEVIDEOINFOS_H_

#include "../deBaseModule.h"
#include "../../../common/math/decMath.h"
#include "../../../resources/video/deVideo.h"


/**
 * \brief Video information.
 */
class DE_DLL_EXPORT deBaseVideoInfo{
private:
	int pWidth;
	int pHeight;
	deVideo::ePixelFormat pPixelFormat;
	int pFrameCount;
	int pFrameRate;
	decColorMatrix3 pColorConversionMatrix;
	bool pIsCompressed;
	
	int pBytesPerSample;
	int pSampleCount;
	int pSampleRate;
	int pChannelCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create info. */
	deBaseVideoInfo();
	
	/** \brief Clean up info. */
	~deBaseVideoInfo();
	/*@}*/
	
	
	
	/** \name Video Information */
	/*@{*/
	/** \brief Width in pixels. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Set width in pixels. */
	void SetWidth( int width );
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Set height in pixels. */
	void SetHeight( int height );
	
	/** \brief Pixel format. */
	inline deVideo::ePixelFormat GetPixelFormat() const{ return pPixelFormat; }
	
	/** \brief Set pixel format. */
	void SetPixelFormat( deVideo::ePixelFormat pixelFormat );
	
	/** \brief Number of frames. */
	inline int GetFrameCount() const{ return pFrameCount; }
	
	/** \brief Set number of frames. */
	void SetFrameCount( int frameCount );
	
	/** \brief Frame rate. */
	inline int GetFrameRate() const{ return pFrameRate; }
	
	/** \brief Set frame rate. */
	void SetFrameRate( int frameRate );
	
	/** \brief Color conversion matrix. */
	inline const decColorMatrix3 &GetColorConversionMatrix() const{ return pColorConversionMatrix; }
	
	/** \brief Set color conversion matrix. */
	void SetColorConversionMatrix( const decColorMatrix3 &matrix );
	
	/**
	 * \brief File is compressed.
	 * \version 1.12
	 */
	inline bool GetIsCompressed() const{ return pIsCompressed; }
	
	/**
	 * \brief File is compressed.
	 * \version 1.12
	 */
	void SetIsCompressed( bool isCompressed );
	/*@}*/
	
	
	
	/** \name Audio Information */
	/*@{*/
	/** \brief Bytes per sample or 0 if no audio. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Set bytes per sample or 0 if no audio. */
	void SetBytesPerSample( int bytesPerSample );
	
	/** \brief Number of samples or 0 if no audio. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Set number of samples or 0 if no audio. */
	void SetSampleCount( int sampleCount );
	
	/** \brief Number of channels or 0 if no audio. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Set number of channels or 0 if no audio. */
	void SetChannelCount( int channelCount );
	
	/** \brief Sample rate or 0 if no audio. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Set sample rate or 0 if no audio. */
	void SetSampleRate( int sampleRate );
	/*@}*/
};

#endif
