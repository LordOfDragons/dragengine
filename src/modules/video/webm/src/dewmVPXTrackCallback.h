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
	dewmVPXTrackCallback(deVideoWebm &module);
	
	/** Clean up callback. */
	~dewmVPXTrackCallback() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Set resource buffers to use for next frame. */
	void SetResBuffer(void *buffer);
	
	/** Rewind. */
	void Rewind();
	/*@}*/
	
	
	
protected:
	virtual bool pOpenTrack(const webm::TrackEntry &track);
	virtual void pProcessFrame(webm::Reader &reader, std::uint64_t &bytes_remaining);
	virtual void pProcessAdditional(const std::vector<unsigned char> &data);
};

#endif
