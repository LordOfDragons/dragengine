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
	inline double GetDuration() const{return pDuration;}
	
	/** Set duration in seconds. */
	void SetDuration(double duration);
	
	
	
	/** Video codec. */
	inline eVideoCodec GetVideoCodec() const{return pVideoCodec;}
	
	/** Set video codec. */
	void SetVideoCodec(eVideoCodec codec);
	
	/** Video track id. */
	inline uint64_t GetVideoTrackNumber() const{return pVideoTrackNumber;}
	
	/** Set video track id. */
	void SetVideoTrackNumber(uint64_t number);
	
	
	
	/** Audio codec. */
	inline eAudioCodec GetAudioCodec() const{return pAudioCodec;}
	
	/** Set audio codec. */
	void SetAudioCodec(eAudioCodec codec);
	
	/** Audio track id. */
	inline uint64_t GetAudioTrackNumber() const{return pAudioTrackNumber;}
	
	/** Set audio track id. */
	void SetAudioTrackNumber(uint64_t number);
	/*@}*/
};

#endif
