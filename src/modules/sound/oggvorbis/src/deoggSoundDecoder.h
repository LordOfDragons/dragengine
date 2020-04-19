/* 
 * Drag[en]gine OGG Vorbis Sound Module
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

#ifndef _DEOGGSOUNDDECODER_H_
#define _DEOGGSOUNDDECODER_H_

#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <dragengine/systems/modules/sound/deBaseSoundDecoder.h>
#include <dragengine/common/math/decMath.h>

class deSoundOGG;



/**
 * @brief OGG Vorbis Sound Decoder.
 * Sound decoder to read OGG Vorbis sound files.
 */
class deoggSoundDecoder : public deBaseSoundDecoder{
private:
	deSoundOGG *pModule;
	
	OggVorbis_File pOggFile;
	int pBytesPerSample;
	int pSampleRate;
	int pSampleCount;
	int pChannelCount;
	int pSigned;
	int pSection;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new ogg sound stream object. */
	deoggSoundDecoder( deSoundOGG *module, decBaseFileReader *file );
	/** Cleans up the ogg sound stream object. */
	virtual ~deoggSoundDecoder();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the position in samples. */
	virtual int GetPosition();
	/** Sets the position in samples from the beginning. */
	virtual void SetPosition( int position );
	/**
	 * Reads a chunk of sound data from the file. Reads the given number
	 * of sample bytes into the provided buffer and advances the position.
	 * Returns the number of bytes written to the buffer. This can be less
	 * than the requested amount if the end of file has been reached. An
	 * error is signaled using the engine error signaling and 0 returned.
	 */
	virtual int ReadSamples( void *buffer, int size );
	/*@}*/
};

// end of include only once
#endif
