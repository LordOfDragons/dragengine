/* 
 * Drag[en]gine Animated PNG Video Module
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

#ifndef _DEAPNGDECODER_H_
#define _DEAPNGDECODER_H_

#include <png.h>

#include <dragengine/systems/modules/video/deBaseVideoDecoder.h>

class deapngReader;
class deVideoApng;

class decBaseFileReader;



/**
 * \brief Animated PNG decoder.
 */
class deapngDecoder : public deBaseVideoDecoder{
private:
	deVideoApng &pModule;
	deapngReader *pReader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create decoder. */
	deapngDecoder( deVideoApng &module, decBaseFileReader *file );
	
	/** \brief Clean up decoder. */
	virtual ~deapngDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief File position in frames from the beginning. */
	virtual int GetPosition();
	
	/** \brief Set file position in frames from the beginning. */
	virtual void SetPosition( int position );
	
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
	virtual bool DecodeFrame( void *buffer, int size );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
