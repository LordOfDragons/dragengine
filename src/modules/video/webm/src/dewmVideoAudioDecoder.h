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

#ifndef _DEWMVIDEOAUDIODECODER_H_
#define _DEWMVIDEOAUDIODECODER_H_

#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/systems/modules/video/deBaseVideoAudioDecoder.h>

#include <webm/webm_parser.h>

class deVideoWebm;
class dewmAudioTrackCallback;
class dewmWebmReader;



/**
 * Webm video decoder.
 */
class dewmVideoAudioDecoder : public deBaseVideoAudioDecoder{
private:
	deVideoWebm &pModule;
	
	dewmAudioTrackCallback *pCallback;
	dewmWebmReader *pReader;
	webm::WebmParser *pParser;
	
	int pCurSample;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create decoder. */
	dewmVideoAudioDecoder( deVideoWebm &module, decBaseFileReader *file );
	
	/** Clean up decoder. */
	virtual ~dewmVideoAudioDecoder();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** File position in samples from the beginning. */
	virtual int GetPosition();
	
	/** Set file position in samples from the beginning. */
	virtual void SetPosition( int position );
	
	/**
	 * Read chunk of sound data from current file position and advance.
	 * 
	 * Reads requested amount of samples into buffer. If successful the file position is
	 * advanced. Returns number of bytes written to the buffer. This can be less than the
	 * requested bytes required due to the number of requested samples because end of file
	 * has been reached. If reading fails an error is signaled using the engine error
	 * signaling and 0 returned.
	 */
	virtual int ReadSamples( void *buffer, int size );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pEnsureStreamOpen();
};

#endif
