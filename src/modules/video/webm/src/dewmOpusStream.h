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

#ifndef _DEWMOPUSSTREAM_H_
#define _DEWMOPUSSTREAM_H_

#include "dewmAudioStream.h"

#include <webm/callback.h>
#include <opus/opus_multistream.h>
#include <dragengine/common/collection/decTList.h>

class dewmAudioTrackCallback;


/**
 * Opus audio stream.
 */
class dewmOpusStream : public dewmAudioStream{
private:
	dewmAudioTrackCallback &pCallback;
	
	int pBytesPerSample;
	int pSampleRate;
	int pChannelCount;
	int pPreSkip;
	
	int pBufferSampleSize;
	
	OpusMSDecoder *pDecoder;
	
	/** Decoded PCM samples (int16, interleaved). */
	decTList<int16_t> pPcmBuffer;
	int pPcmBufferPos;
	int pPcmBufferCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create opus stream. */
	explicit dewmOpusStream(dewmAudioTrackCallback &callback);
	
	/** Clean up opus stream. */
	~dewmOpusStream() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Bytes per sample. */
	int GetBytesPerSample() const override;
	
	/** Sample rate. */
	int GetSampleRate() const override;
	
	/** Channel count. */
	int GetChannelCount() const override;
	
	/** Buffer sample size. */
	int GetBufferSampleSize() const override;
	
	/** Open track. */
	bool OpenTrack(const webm::TrackEntry &track) override;
	
	/** Copy samples. */
	void CopySamples() override;
	
	/** Load frame data. */
	void LoadFrameData(std::uint64_t frameSize) override;
	
	/** Rewind. */
	void Rewind() override;
	/*@}*/
};

#endif
