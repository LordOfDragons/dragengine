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

#ifndef _DESOUNDDECODER_H_
#define _DESOUNDDECODER_H_

#include "../../deObject.h"
#include "deSound.h"

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
class DE_DLL_EXPORT deSoundDecoder : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSoundDecoder> Ref;
	
	
	
private:
	deSoundManager &pSoundManager;
	deSound::Ref pSound;
	
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
