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

#ifndef _DEOGLRTFRAMECOUNTTRACKER_H_
#define _DEOGLRTFRAMECOUNTTRACKER_H_

#include <stdint.h>


class deoglRenderThread;


/**
 * Render thread frame count tracker.
 */
class deoglRTFrameCounterTracker{
private:
	const deoglRenderThread &pRenderThread;
	uint32_t pFrameCounter;
	uint32_t pElapsedFrames;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread frame counter tracker. */
	deoglRTFrameCounterTracker(const deoglRenderThread &renderThread);
	
	/** Clean up render thread frame counter tracker. */
	~deoglRTFrameCounterTracker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Update tracker. Stores the elapsed frames since the last time Update() has been called.
	 */
	void Update();
	
	/**
	 * Frame counter.
	 */
	inline uint32_t GetFrameCounter() const{return pFrameCounter;}
	
	/**
	 * Elapsed frames between the last two calls to Update(). Valid if Update has been called
	 * at least two times. Otherwise returns 0.
	 */
	inline uint32_t GetElapsedFrames() const{return pElapsedFrames;}
	
	/**
	 * At least one frame elapsed since between the last two calls to Update(). Valid if
	 * Update() has been called at least two times. Otherwise returns 0.
	 */
	inline bool HasElapsedFrames() const{return pElapsedFrames > 0;}
	/*@}*/
};

#endif
