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

#ifndef _DEWMAUDIOTRACKCALLBACK_H_
#define _DEWMAUDIOTRACKCALLBACK_H_

#include "dewmTrackCallback.h"

class dewmVorbisStream;
class dewmInfos;


/**
 * Callback for reading a audio WebM track.
 */
class dewmAudioTrackCallback : public dewmTrackCallback{
private:
	uint8_t *pResBuffer;
	int pResSize;
	int pResPosition;
	int pSampleSize;
	
	dewmVorbisStream *pStreamVorbis;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create callback. */
	dewmAudioTrackCallback(deVideoWebm &module);
	
	/** Clean up callback. */
	virtual ~dewmAudioTrackCallback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set resource buffers to use for next read. */
	void SetResBuffer(void *buffer, int samples);
	
	/** Resource buffer. */
	inline uint8_t *GetResBuffer() const{ return pResBuffer; }
	
	/** Resource buffer size in samples. */
	inline int GetResSize() const{ return pResSize; }
	
	/** Resource buffer position in samples. */
	inline int GetResPosition() const{ return pResPosition; }
	
	/** Sample size. */
	inline int GetSampleSize() const{ return pSampleSize; }
	
	/** Stream is open. */
	bool IsStreamOpen();
	
	/** Rewind. */
	void Rewind();
	
	/** Open track. */
	bool OpenTrack(const webm::TrackEntry &track);
	
	/** Update audio information. */
	void UpdateInfos(dewmInfos &infos);
	
	
	
	/** Advance resource buffer position. */
	void AdvanceResPosition(int bytes);
	
	/** Frame buffer. */
	inline const std::uint8_t *GetFrameBuffer() const{ return pGetBuffer(); }
	/*@}*/
	
	
	
protected:
	virtual bool pOpenTrack(const webm::TrackEntry &track);
	virtual void pProcessFrame(webm::Reader &reader, std::uint64_t &bytes_remaining);
	virtual void pEndSegment();
	
	
	
private:
	void pCopySamples();
	void pLoadFrameData(webm::Reader &reader, std::uint64_t &bytes_remaining);
};

#endif
