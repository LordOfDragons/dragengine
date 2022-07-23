/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
	deoglRTFrameCounterTracker( const deoglRenderThread &renderThread );
	
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
	inline uint32_t GetFrameCounter() const{ return pFrameCounter; }
	
	/**
	 * Elapsed frames between the last two calls to Update(). Valid if Update has been called
	 * at least two times. Otherwise returns 0.
	 */
	inline uint32_t GetElapsedFrames() const{ return pElapsedFrames; }
	
	/**
	 * At least one frame elapsed since between the last two calls to Update(). Valid if
	 * Update() has been called at least two times. Otherwise returns 0.
	 */
	inline bool HasElapsedFrames() const{ return pElapsedFrames > 0; }
	/*@}*/
};

#endif
