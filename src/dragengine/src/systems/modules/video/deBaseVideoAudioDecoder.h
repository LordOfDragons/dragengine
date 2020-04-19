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

#ifndef _DEBASEVIDEOAUDIODECODER_H_
#define _DEBASEVIDEOAUDIODECODER_H_

#include "../deBaseModule.h"
#include "../../../common/file/decBaseFileReaderReference.h"


/**
 * \brief Base video audio decoder peer.
 *
 * Video audio decoders decode video audio data from a file reader. The decoding is done in
 * chunks of audio data as block of samples. The file reader is held open as long as the
 * decoder exists. The user decides if the data is read synchronous or asynchronous.
 * The sound information is not replicated in the decoder but found in the source video resource.
 */
class deBaseVideoAudioDecoder{
private:
	decBaseFileReaderReference pFile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video audio decoder peer. */
	deBaseVideoAudioDecoder( decBaseFileReader *file );
	
	/** \brief Clean up sound decoder object. */
	virtual ~deBaseVideoAudioDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File reader. */
	inline decBaseFileReader *GetFile() const{ return pFile; }
	
	/** \brief File position in samples from the beginning. */
	virtual int GetPosition() = 0;
	
	/** \brief Set file position in samples from the beginning. */
	virtual void SetPosition( int position ) = 0;
	
	/**
	 * \brief Read chunk of sound data from current file position and advance.
	 * 
	 * Reads requested amount of samples into buffer. If successful the file position is
	 * advanced. Returns number of bytes written to the buffer. This can be less than the
	 * requested bytes required due to the number of requested samples because end of file
	 * has been reached. If reading fails an error is signaled using the engine error
	 * signaling and 0 returned.
	 */
	virtual int ReadSamples( void *buffer, int size ) = 0;
	/*@}*/
};

#endif
