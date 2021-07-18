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

#ifndef _DEVIDEOAUDIODECODER_H_
#define _DEVIDEOAUDIODECODER_H_

#include "deVideoReference.h"
#include "../../deObject.h"

class deVideoManager;
class deBaseVideoAudioDecoder;


/**
 * \brief Video audio decoder.
 *
 * Video audio decoders decode video audio data from a file reader. The decoding is done in
 * chunks of audio data as block of samples. The file reader is held open as long as the
 * decoder exists. The user decides if the data is read synchronous or asynchronous.
 * The audio information is not replicated in the decoder but found in the source video resource.
 */
class deVideoAudioDecoder : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deVideoAudioDecoder> Ref;
	
	
	
private:
	deVideoManager &pVideoManager;
	deVideoReference pVideo;
	
	deBaseVideoAudioDecoder *pPeerVideo;
	
	deVideoAudioDecoder *pLLManagerPrev;
	deVideoAudioDecoder *pLLManagerNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound decoder. */
	deVideoAudioDecoder( deVideoManager &manager, deVideo *sound );
	
protected:
	/**
	 * \brief Clean up sound object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deVideoAudioDecoder();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Video. */
	deVideo *GetVideo() const{ return pVideo; }
	
	
	
	/** \brief File position in samples from the beginning. */
	int GetPosition();
	
	/** \brief Set file position in samples from the beginning. */
	void SetPosition( int position );
	
	/**
	 * \brief Read chunk of sound data from current file position and advance.
	 * 
	 * Reads requested amount of samples into buffer. If successful the file position is
	 * advanced. Returns number of bytes written to the buffer. This can be less than the
	 * requested bytes required due to the number of requested samples because end of file
	 * has been reached. If reading fails an error is signaled using the engine error
	 * signaling and 0 returned.
	 */
	int ReadSamples( void *buffer, int size );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Decoder peer. */
	inline deBaseVideoAudioDecoder *GetPeerVideo() const{ return pPeerVideo; }
	
	/** \brief Set decoder peer. */
	void SetPeerVideo( deBaseVideoAudioDecoder *peer );
	/*@}*/
	
	
	
	/**
	 * \name Resource manager linked list
	 * \warning For internal use only. Never call on your own!
	 */
	/*@{*/
	/** \brief Previous resource in the resource manager linked list. */
	inline deVideoAudioDecoder *GetLLManagerPrev() const{ return pLLManagerPrev; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerPrev( deVideoAudioDecoder *resource );
	
	/** \brief Next resource in the resource manager linked list. */
	inline deVideoAudioDecoder *GetLLManagerNext() const{ return pLLManagerNext; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerNext( deVideoAudioDecoder *resource );
	
	/**
	 * \brief Marks the resource leaking.
	 * \warning For internal use only. Never call on your own!
	 */
	void MarkLeaking();
	/*@}*/
};

#endif
