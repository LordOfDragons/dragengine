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

#ifndef _DEOALDECODEBUFFER_H_
#define _DEOALDECODEBUFFER_H_

#include <dragengine/common/math/decMath.h>

class deSoundDecoder;



/**
 * \brief OpenAL decode buffer.
 */
class deoalDecodeBuffer{
private:
	char *pBuffer;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decode buffer. */
	deoalDecodeBuffer( int size );
	
	/** \brief Clean up decode buffer. */
	~deoalDecodeBuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Buffer. */
	inline char *GetBuffer() const{ return pBuffer; }
	
	/** \brief Size of buffer. */
	inline int GetSize() const{ return pSize; }
	
	/** \brief Set size of buffer. */
	void SetSize( int size );
	
	/**
	 * \brief Decode samples using decoder.
	 * 
	 * Up to size bytes are decoded. If the size is larger than the buffer then the buffer
	 * is first resized to be large enough. Returns the number of bytes written to the buffer.
	 */
	int Decode( deSoundDecoder &decoder, int size );
	
	/**
	 * \brief Decode samples using decoder.
	 * 
	 * Up to size bytes are decoded. If the size is larger than the buffer then the buffer
	 * is first resized to be large enough. If the end of the stream has been reached before
	 * size bytes have been written then the decoding starts from the beginning to fill up
	 * the buffer with size bytes. Returns the number of bytes written to the buffer.
	 */
	int DecodeLooping( deSoundDecoder &decoder, int size );
	/*@}*/
};

#endif
