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

#ifndef _DEOALSHAREDBUFFER_H_
#define _DEOALSHAREDBUFFER_H_



/**
 * \brief Shared buffer.
 */
class deoalSharedBuffer{
private:
	float *pBuffer;
	int pSize;
	bool pInUse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared buffer. */
	deoalSharedBuffer();
	
	/** \brief Clean up buffer. */
	~deoalSharedBuffer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Buffer. */
	inline float *GetBuffer() const{ return pBuffer; }
	
	/** \brief Number of floats in the buffer. */
	inline int GetSize() const{ return pSize; }
	
	/**
	 * \brief Set buffer size.
	 * \details Resizes buffer to fit new size.
	 */
	void SetSize(int size);
	
	/** \brief Buffer is in use. */
	inline bool GetInUse() const{ return pInUse; }
	
	/**
	 * \brief Set if buffer is in use.
	 * \details For use by deoalSharedBufferList only. Use deoalSharedBuffer::ReleaseBuffer
	 *          to return the buffer to the list of available shared buffers.
	 */
	void SetInUse(bool inUse);
	/*@}*/
};

#endif
