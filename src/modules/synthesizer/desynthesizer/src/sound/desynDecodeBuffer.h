/* 
 * Drag[en]gine Synthesizer Module
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

#ifndef _DESYNDECODEBUFFER_H_
#define _DESYNDECODEBUFFER_H_

class deSoundDecoder;



/**
 * \brief Decode buffer.
 */
class desynDecodeBuffer{
private:
	char *pBuffer;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decode buffer. */
	desynDecodeBuffer( int size );
	
	/** \brief Clean up decode buffer. */
	~desynDecodeBuffer();
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
	 * Up to size bytes are decoded. If the size is larger than the buffer
	 * then the buffer is first resized to be large enough. Returns the number
	 * of bytes written to the buffer.
	 */
	int Decode( deSoundDecoder &decoder, int size );
	
	/**
	 * \brief Decode samples using decoder.
	 * 
	 * Up to size bytes are decoded. If the size is larger than the buffer then
	 * the buffer is first resized to be large enough. If the end of the stream
	 * has been reached before size bytes have been written then the decoding
	 * starts from the beginning to fill up the buffer with size bytes. Returns
	 * the number of bytes written to the buffer.
	 */
	int DecodeLooping( deSoundDecoder &decoder, int size );
	/*@}*/
};

#endif
