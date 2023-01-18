/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLVIDEODECODETHREAD_H_
#define _DEOGLVIDEODECODETHREAD_H_

#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/resources/video/deVideoReference.h>
#include <dragengine/resources/video/deVideoDecoderReference.h>
#include <dragengine/threading/deThread.h>
#include <dragengine/threading/deSemaphore.h>


/**
 * Video Decode Thread.
 */
class deoglVideoDecodeThread : public deThread{
public:
	const deVideoDecoderReference pDecoder;
	const deVideoReference pVideo;
	int pFrame;
	int pNextFrame;
	
	deoglPixelBuffer::Ref pPixelBufferDecode;
	deoglPixelBuffer::Ref pPixelBufferTexture;
	
	deMutex pMutex;
	deSemaphore pSemaphoreDecode;
	deSemaphore pSemaphoreFinished;
	bool pDecoding;
	bool pHasDecodedFrame;
	bool pShutDown;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new thread. */
	deoglVideoDecodeThread( deVideoDecoder *decoder, deVideo *video );
	
	/** Clean up thread. */
	virtual ~deoglVideoDecodeThread();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Start decoding a frame.
	 * 
	 * If a decoding is in progress the new decoding parameters are
	 * stored aside. Once the current decoding is finished the decoding continues with the
	 * stored aside parameters. This is done since stopping the thread could cut the decoder
	 * in a very bad situation causing various problems. This way decoding never can end
	 * up in an unhealthy state while guaranteeing the last set frame is always decoded and
	 * ready to be retrieved. Intermediate results are discarded.
	 */
	void StartDecode( int frame );
	
	/**
	 * Pixel buffer to upload to the texture. Waits for the decoding to finish.
	 * If no decoding is in progress nullptr is returned.
	 */
	deoglPixelBuffer::Ref GetTexturePixelBuffer();
	
	/**
	 * Set texture pixel buffer without deleting the old one. Pixel buffer can be nullptr
	 * in which case a new one is created the next time. Allows swaping pixel buffers with
	 * the render video player.
	 */
	void SetTexturePixelBuffer( deoglPixelBuffer *pixelBuffer );
	
	/**
	 * Wait for decoding to finish and clear decode parameters.
	 * 
	 * This removes all pointers to engine resources to avoid accessing invalid memory.
	 */
	void StopDecode();
	
	
	
	/** Run function of the thread */
	virtual void Run();
	
	/** Ensure pixel buffers are ready for decoding. */
	void PreparePixelBuffers();
	
	/** Decode frame. */
	void DecodeFrame();
	
	/** Set pixel buffer to the error result. */
	void SetErrorPixelBuffer();
	/*@}*/
	
	
	
private:
	void pSafelyShutDownThread();
};

#endif
