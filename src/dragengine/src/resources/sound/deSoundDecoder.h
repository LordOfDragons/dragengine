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

#ifndef _DESOUNDDECODER_H_
#define _DESOUNDDECODER_H_

#include "../../deObject.h"
#include "deSoundReference.h"

class deSoundManager;
class deBaseSoundDecoder;


/**
 * \brief Sound decoder.
 *
 * Sound decoders decode sound data from a file reader. The decoding is done in chunks of sound
 * data as block of samples. The file reader is held open as long as the decoder exists. The
 * user decides if the data is read synchronous or asynchronous. The sound information is
 * not replicated in the decoder but found in the source sound resource.
 */
class deSoundDecoder : public deObject{
private:
	deSoundManager &pSoundManager;
	deSoundReference pSound;
	
	deBaseSoundDecoder *pPeerSound;
	
	deSoundDecoder *pLLManagerPrev;
	deSoundDecoder *pLLManagerNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound decoder. */
	deSoundDecoder( deSoundManager &manager, deSound *sound );
	
protected:
	/**
	 * \brief Clean up sound object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSoundDecoder();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sound. */
	deSound *GetSound() const{ return pSound; }
	
	
	
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
	inline deBaseSoundDecoder *GetPeerSound() const{ return pPeerSound; }
	
	/** \brief Set decoder peer. */
	void SetPeerSound( deBaseSoundDecoder *peer );
	/*@}*/
	
	
	
	/**
	 * \name Resource manager linked list
	 * \warning For internal use only. Never call on your own!
	 */
	/*@{*/
	/** \brief Previous resource in the resource manager linked list. */
	inline deSoundDecoder *GetLLManagerPrev() const{ return pLLManagerPrev; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerPrev( deSoundDecoder *resource );
	
	/** \brief Next resource in the resource manager linked list. */
	inline deSoundDecoder *GetLLManagerNext() const{ return pLLManagerNext; }
	
	/**
	 * \brief Set next resource in the resource manager linked list.
	 * \warning For internal use only. Never call on your own!
	 */
	void SetLLManagerNext( deSoundDecoder *resource );
	
	/**
	 * \brief Marks the resource leaking.
	 * \warning For internal use only. Never call on your own!
	 */
	void MarkLeaking();
	/*@}*/
};

#endif
