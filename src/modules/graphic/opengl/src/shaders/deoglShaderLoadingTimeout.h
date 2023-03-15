/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSHADERLOADINGTIMEOUT_H_
#define _DEOGLSHADERLOADINGTIMEOUT_H_

#include <dragengine/common/utils/decTimer.h>


/**
 * Shader loading timeout. Simplifies tracking timeout for loading shaders.
 * 
 * Top level object creates timeout on stack and hands down the reference.
 * 
 * The lowest level shader loading code has to first call CanLoad() to check if the
 * timeout elapsed. This function returns true if either this is the first shader to
 * be loaded or the timeout has not elapsed. This ensures at least one shader is
 * loaded without the timeout being checked.
 * 
 * After loading the shader the lowest level shader loading code has to call Loaded().
 * This clears the first flag. At this point TimedOut() still returns true. This
 * ensures loading one shader can not cause a timeout to be noticed up the call chain.
 * If this is not done functions up the call chain can potentially exit and another
 * try is done at creating shaders.
 * 
 * If later a lowest level shader loader calls CanLoad() and the timeout has elapsed
 * the TimedOut() function will return true. This way a timeout is noticed after
 * the first shader loading that has been prevented due to the timeout after at least
 * one shader has been loaded. This ensure loading never can stall because at least
 * one shader is loaded and no timeout is noticed then.
 */
class deoglShaderLoadingTimeout{
private:
	decTimer pTimer;
	float pInitialTimeout;
	float pTimeout;
	bool pFirst;
	bool pTimedOut;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader loading timeout. */
	deoglShaderLoadingTimeout( float timeout );
	
	/** Clean up shader loading timeout. */
	~deoglShaderLoadingTimeout();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Initial timeout. */
	inline float GetInitialTimeout() const{ return pInitialTimeout; }
	
	/** Remaining timeout. */
	inline float GetTimeout() const{ return pTimeout; }
	
	/** Timeout elapsed. */
	inline bool TimedOut() const{ return pTimedOut; }
	
	/** Timeout not elapsed. */
	inline bool NotTimedOut() const{ return ! pTimedOut; }
	
	/** First. */
	inline bool First() const{ return pFirst; }
	
	/** Check if shader can be loaded. If false is returned exit. */
	bool CanLoad();
	
	/** Shader loaded. */
	void Loaded();
	/*@}*/
};

#endif
