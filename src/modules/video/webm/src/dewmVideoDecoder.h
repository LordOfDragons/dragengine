/* 
 * Drag[en]gine WebM Video Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _dewmVIDEODECODER_H_
#define _dewmVIDEODECODER_H_

#include <dragengine/systems/modules/video/deBaseVideoDecoder.h>

#include <webm/webm_parser.h>

class deVideoWebm;
class dewmVPXTrackCallback;
class dewmWebmReader;


/**
 * Webm video decoder.
 */
class dewmVideoDecoder : public deBaseVideoDecoder{
private:
	deVideoWebm &pModule;
	
	dewmVPXTrackCallback *pCallback;
	dewmWebmReader *pReader;
	webm::WebmParser *pParser;
	
	int pCurFrame;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create decoder. */
	dewmVideoDecoder( deVideoWebm &module, decBaseFileReader *file );
	
	/** Clean up decoder. */
	virtual ~dewmVideoDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** File position in frames from the beginning. */
	virtual int GetPosition();
	
	/** Set file position in frames from the beginning. */
	virtual void SetPosition( int position );
	
	/**
	 * Decode next frame into buffer and advances file position.
	 * 
	 * Depending on the pixel format one or two buffers are provided. \em buffer1 is
	 * always used and is of size width/height. \em buffer2 is only provided if one
	 * of the reduced pixel formats is used. In this case \em buffer1 contains 1
	 * color plane and \em buffer2 2 color planes with half or quarter size.
	 * Otherwise \em buffer1 contains all three color planes. In all cases the color
	 * planes are interleaved.
	 * 
	 * If successful the file position is advanced. Returns \em true if the frame
	 * has been decoded successfully. Otherwise \em fals is returned and an error
	 * is signaled using the engine error signaling.
	 */
	virtual bool DecodeFrame( void *buffer1, int size1, void *buffer2, int size2 );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
