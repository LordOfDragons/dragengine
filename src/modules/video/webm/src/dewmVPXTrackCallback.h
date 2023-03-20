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

#ifndef _DEWMVPXTRACKCALLBACK_H_
#define _DEWMVPXTRACKCALLBACK_H_

#include "dewmTrackCallback.h"

#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>


/**
 * Callback for reading a VP9/VP8 WebM track.
 */
class dewmVPXTrackCallback : public dewmTrackCallback{
private:
	vpx_codec_iface_t *pInterface;
	int pWidth;
	int pHeight;
	bool pHasTransparency;
	int pStride;
	
	vpx_codec_ctx_t *pContext;
	vpx_codec_iter_t pIterator;
	
	vpx_codec_ctx_t *pContextTransparency;
	vpx_codec_iter_t pIteratorTransparency;
	
	void *pResBuffer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create callback. */
	dewmVPXTrackCallback( deVideoWebm &module );
	
	/** Clean up callback. */
	virtual ~dewmVPXTrackCallback();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set resource buffers to use for next frame. */
	void SetResBuffer( void *buffer );
	/*@}*/
	
	
	
protected:
	virtual bool pOpenTrack( const webm::TrackEntry &track );
	virtual void pProcessFrame( webm::Reader &reader, std::uint64_t &bytes_remaining );
	virtual void pProcessAdditional( const std::vector<unsigned char> &data );
};

#endif
