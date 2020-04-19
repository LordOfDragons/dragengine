/* 
 * Drag[en]gine Animated PNG Video Module
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

#ifndef _DEAPNGREADER_H_
#define _DEAPNGREADER_H_

#include <png.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/video/deVideo.h>

class decString;
class decBaseFileReader;
class deVideoApng;



/**
 * \brief Animated PNG reader.
 */
class deapngReader{
private:
	deVideoApng &pModule;
	
	decBaseFileReader *pReader;
	
	png_structp pReadStruct;
	png_infop pInfoStruct;
	
	int pWidth;
	int pHeight;
	deVideo::ePixelFormat pPixelFormat;
	int pFrameCount;
	int pFrameRate;
	int pFirstFrame;
	int pPixelSize;
	int pRowLength;
	int pImageSize;
	
	int pCurFrame;
	
	png_bytep pAccumData;
	png_bytep *pAccumRows;
	png_bytep pFrameData;
	png_bytep *pFrameRows;
	
	bool pErrorState;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create reader. */
	deapngReader( deVideoApng &module, decBaseFileReader *reader );
	
	/** \brief Clean up reader. */
	~deapngReader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Animated PNG module. */
	inline deVideoApng &GetModule() const{ return pModule; }
	
	/** \brief File reader. */
	inline decBaseFileReader *GetReader() const{ return pReader; }
	
	/** \brief Width in pixels. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height in pixels. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Pixel format. */
	inline deVideo::ePixelFormat GetPixelFormat() const{ return pPixelFormat; }
	
	/** \brief Frame count. */
	inline int GetFrameCount() const{ return pFrameCount; }
	
	/** \brief Frame rate. */
	inline int GetFrameRate() const{ return pFrameRate; }
	
	/** \brief First frame (depends on first frame hidden). */
	inline int GetFirstFrame() const{ return pFirstFrame; }
	
	
	
	/** \brief Current frame. */
	inline int GetCurrentFrame() const{ return pCurFrame; }
	
	
	
	/** \brief Rewinds to the beginning. */
	void Rewind();
	
	/** \brief Seek to frame. */
	void SeekFrame( int frame );
	
	/** \brief Read frame image into accum image and advance to next frame. */
	void ReadImage();
	
	/** \brief Copy accum image. */
	void CopyAccumImage( void *buffer, int size ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pReadHeader();
	void pReadImage();
	void pEnterErrorState();
};

#endif
