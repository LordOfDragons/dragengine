/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEWMAUDIOSTREAM_H_
#define _DEWMAUDIOSTREAM_H_

#include <stdint.h>
#include <webm/webm_parser.h>


/**
 * Interface for audio stream decoders.
 */
class dewmAudioStream{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Clean up audio stream. */
	virtual ~dewmAudioStream() = default;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Bytes per sample. */
	virtual int GetBytesPerSample() const = 0;
	
	/** Sample rate. */
	virtual int GetSampleRate() const = 0;
	
	/** Channel count. */
	virtual int GetChannelCount() const = 0;
	
	/** Buffer sample size in bytes. */
	virtual int GetBufferSampleSize() const = 0;
	
	/** Open track. Returns true if the track was accepted and opened successfully. */
	virtual bool OpenTrack(const webm::TrackEntry &track) = 0;
	
	/** Copy available decoded samples into the resource buffer. */
	virtual void CopySamples() = 0;
	
	/** Decode the frame data already loaded into the frame buffer. */
	virtual void LoadFrameData(std::uint64_t frameSize) = 0;
	
	/** Rewind the stream to the beginning. */
	virtual void Rewind() = 0;
	/*@}*/
};

#endif
