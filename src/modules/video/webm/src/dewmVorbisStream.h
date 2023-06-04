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
	dewmVorbisStream( dewmAudioTrackCallback &callback );
	
	/** Clean up vorbis stream. */
	~dewmVorbisStream();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** Channel count. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** Buffer sample size. */
	inline int GetBufferSampleSize() const{ return pBufferSampleSize; }
	
	/** Open track. */
	bool OpenTrack( const webm::TrackEntry &track );
	
	/** Copy samples. */
	void CopySamples();
	
	/** Load frame data. */
	void LoadFrameData( std::uint64_t frameSize );
	
	/** Rewind. */
	void Rewind();
	
	/** Fill up buffer at end of segment. */
	void FillUpBuffer();
	/*@}*/
};

#endif
