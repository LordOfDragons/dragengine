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

#ifndef _DEBASEVIDEODECODER_H_
#define _DEBASEVIDEODECODER_H_

#include "../deBaseModule.h"

class decBaseFileReader;


/**
 * \brief Base video image decoder.
 *
 * Video decoders decode video data from a file reader. The decoding is done as individual
 * frames of video data. The file reader is held open as long as the decoder exists. The
 * user decides if the data is read synchronous or asynchronous. The video information is
 * not replicated in the decoder but found in the source video resource.
 */
class DE_DLL_EXPORT deBaseVideoDecoder{
private:
	decBaseFileReader *pFile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video image decoder. */
	deBaseVideoDecoder( decBaseFileReader *file );
	
	/** \brief Clean up video image decoder. */
	virtual ~deBaseVideoDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File. */
	inline decBaseFileReader *GetFile() const{ return pFile; }
	
	/** \brief File position in frames from the beginning. */
	virtual int GetPosition() = 0;
	
	/** \brief Set file position in frames from the beginning. */
	virtual void SetPosition( int position ) = 0;
	
	/**
	 * \brief Decode next frame into buffer and advances file position.
	 * 
	 * Depending on the deVideo::GetComponentCount() \em buffer contains 1 to 4 components
	 * per color in the order R, G, B and A. The parameter \em size is provided as fail
	 * check for the decoder to ensure he is expecting the correct buffer size.
	 * 
	 * If successful the file position is advanced. Returns true if the frame
	 * has been decoded successfully. Otherwise \em fals is returned and an error
	 * is signaled using the engine error signaling.
	 */
	virtual bool DecodeFrame( void *buffer, int size ) = 0;
	/*@}*/
};

#endif
