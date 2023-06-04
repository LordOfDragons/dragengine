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

#ifndef _DEWMINFOS_H_
#define _DEWMINFOS_H_

#include <stdint.h>

#include <dragengine/systems/modules/video/deBaseVideoInfo.h>


/**
 * Webm video information.
 */
class dewmInfos : public deBaseVideoInfo{
public:
	/** Video codec. */
	enum eVideoCodec{
		evcVp9,
		evcVp8,
		evcUnsupported
	};
	
	/** Audio codec. */
	enum eAudioCodec{
		eacVorbis,
		eacOpus,
		eacUnsupported
	};
	
	
	
private:
	double pDuration;
	
	eVideoCodec pVideoCodec;
	uint64_t pVideoTrackNumber;
	
	eAudioCodec pAudioCodec;
	uint64_t pAudioTrackNumber;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create information object. */
	dewmInfos();
	
	/** Clean up information object. */
	~dewmInfos();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Duration in seconds. */
	inline double GetDuration() const{ return pDuration; }
	
	/** Set duration in seconds. */
	void SetDuration( double duration );
	
	
	
	/** Video codec. */
	inline eVideoCodec GetVideoCodec() const{ return pVideoCodec; }
	
	/** Set video codec. */
	void SetVideoCodec( eVideoCodec codec );
	
	/** Video track id. */
	inline uint64_t GetVideoTrackNumber() const{ return pVideoTrackNumber; }
	
	/** Set video track id. */
	void SetVideoTrackNumber( uint64_t number );
	
	
	
	/** Audio codec. */
	inline eAudioCodec GetAudioCodec() const{ return pAudioCodec; }
	
	/** Set audio codec. */
	void SetAudioCodec( eAudioCodec codec );
	
	/** Audio track id. */
	inline uint64_t GetAudioTrackNumber() const{ return pAudioTrackNumber; }
	
	/** Set audio track id. */
	void SetAudioTrackNumber( uint64_t number );
	/*@}*/
};

#endif
