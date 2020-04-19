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

#ifndef _DESYNSHAREDBUFFER_H_
#define _DESYNSHAREDBUFFER_H_



/**
 * \brief Shared buffer.
 */
class desynSharedBuffer{
private:
	float *pBuffer;
	int pSize;
	bool pInUse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create shared buffer. */
	desynSharedBuffer();
	
	/** \brief Clean up buffer. */
	~desynSharedBuffer();
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
	void SetSize( int size );
	
	/** \brief Buffer is in use. */
	inline bool GetInUse() const{ return pInUse; }
	
	/**
	 * \brief Set if buffer is in use.
	 * \details For use by desynSharedBufferList only. Use desynSharedBuffer::ReleaseBuffer
	 *          to return the buffer to the list of available shared buffers.
	 */
	void SetInUse( bool inUse );
	/*@}*/
};

#endif
