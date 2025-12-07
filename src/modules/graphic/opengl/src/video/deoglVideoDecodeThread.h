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

#ifndef _DEOGLVIDEODECODETHREAD_H_
#define _DEOGLVIDEODECODETHREAD_H_

#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoDecoder.h>
#include <dragengine/threading/deThread.h>
#include <dragengine/threading/deSemaphore.h>


/**
 * Video Decode Thread.
 */
class deoglVideoDecodeThread : public deThread{
public:
	const deVideoDecoder::Ref pDecoder;
	const deVideo::Ref pVideo;
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
	deoglVideoDecodeThread(deVideoDecoder *decoder, deVideo *video);
	
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
	void StartDecode(int frame);
	
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
	void SetTexturePixelBuffer(deoglPixelBuffer *pixelBuffer);
	
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
