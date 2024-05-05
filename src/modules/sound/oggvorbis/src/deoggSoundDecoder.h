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
