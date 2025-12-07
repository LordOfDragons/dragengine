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

#ifndef _DEWMVORBISSTREAM_H_
#define _DEWMVORBISSTREAM_H_

#include <stdint.h>
#include <webm/callback.h>
#include <vorbis/codec.h>

class dewmAudioTrackCallback;


/**
 * Vorbis stream.
 */
class dewmVorbisStream{
private:
	dewmAudioTrackCallback &pCallback;
	
	int pBytesPerSample;
	int pSampleRate;
	int pChannelCount;
	
	int pBufferSampleSize;
	
	vorbis_info pInfo;
	vorbis_dsp_state pContext;
	vorbis_block pBlock;
	bool pInfoInited;
	bool pContextInited;
	bool pBlockInited;
	
	ogg_packet pPacket;
	uint8_t *pFillUpSample;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create vorbis stream. */
	dewmVorbisStream(dewmAudioTrackCallback &callback);
	
	/** Clean up vorbis stream. */
	~dewmVorbisStream();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Bytes per sample. */
	inline int GetBytesPerSample() const{return pBytesPerSample;}
	
	/** Sample rate. */
	inline int GetSampleRate() const{return pSampleRate;}
	
	/** Channel count. */
	inline int GetChannelCount() const{return pChannelCount;}
	
	/** Buffer sample size. */
	inline int GetBufferSampleSize() const{return pBufferSampleSize;}
	
	/** Open track. */
	bool OpenTrack(const webm::TrackEntry &track);
	
	/** Copy samples. */
	void CopySamples();
	
	/** Load frame data. */
	void LoadFrameData(std::uint64_t frameSize);
	
	/** Rewind. */
	void Rewind();
	
	/** Fill up buffer at end of segment. */
	void FillUpBuffer();
	/*@}*/
};

#endif
