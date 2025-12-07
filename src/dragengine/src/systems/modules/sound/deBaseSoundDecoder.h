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

#ifndef _DEBASESOUNDDECODER_H_
#define _DEBASESOUNDDECODER_H_

#include "../deBaseModule.h"

class decBaseFileReader;


/**
 * \brief Base sound decoder peer.
 *
 * Sound decoders decode sound data from a file reader. The decoding is done in chunks of sound
 * data as block of samples. The file reader is held open as long as the decoder exists. The
 * user decides if the data is read synchronous or asynchronous. The sound information is
 * not replicated in the decoder but found in the source sound resource.
 */
class DE_DLL_EXPORT deBaseSoundDecoder{
private:
	decBaseFileReader *pFile;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound decoder peer. */
	deBaseSoundDecoder(decBaseFileReader *file);
	
	/** \brief Clean up sound decoder object. */
	virtual ~deBaseSoundDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File reader. */
	inline decBaseFileReader *GetFile() const{return pFile;}
	
	/** \brief File position in samples from the beginning. */
	virtual int GetPosition() = 0;
	
	/** \brief Set file position in samples from the beginning. */
	virtual void SetPosition(int position) = 0;
	
	/**
	 * \brief Read chunk of sound data from current file position and advance.
	 * 
	 * Reads requested amount of samples into buffer. If successful the file position is
	 * advanced. Returns number of bytes written to the buffer. This can be less than the
	 * requested bytes required due to the number of requested samples because end of file
	 * has been reached. If reading fails an error is signaled using the engine error
	 * signaling and 0 returned.
	 */
	virtual int ReadSamples(void *buffer, int size) = 0;
	/*@}*/
};

#endif
