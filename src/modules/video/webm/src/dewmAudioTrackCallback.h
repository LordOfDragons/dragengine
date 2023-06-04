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
	dewmAudioTrackCallback( deVideoWebm &module );
	
	/** Clean up callback. */
	virtual ~dewmAudioTrackCallback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set resource buffers to use for next read. */
	void SetResBuffer( void *buffer, int samples );
	
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
	bool OpenTrack( const webm::TrackEntry &track );
	
	/** Update audio information. */
	void UpdateInfos( dewmInfos &infos );
	
	
	
	/** Advance resource buffer position. */
	void AdvanceResPosition( int bytes );
	
	/** Frame buffer. */
	inline const std::uint8_t *GetFrameBuffer() const{ return pGetBuffer(); }
	/*@}*/
	
	
	
protected:
	virtual bool pOpenTrack( const webm::TrackEntry &track );
	virtual void pProcessFrame( webm::Reader &reader, std::uint64_t &bytes_remaining );
	virtual void pEndSegment();
	
	
	
private:
	void pCopySamples();
	void pLoadFrameData( webm::Reader &reader, std::uint64_t &bytes_remaining );
};

#endif
