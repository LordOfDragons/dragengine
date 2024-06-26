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

#ifndef _DEOGLENVMAPFADER_H_
#define _DEOGLENVMAPFADER_H_

#include "../deoglBasics.h"

class deoglEnvironmentMap;


/**
 * Environment Map Fader
 * Provides support to fade between environment maps while they change. Two environment maps are
 * stored at most each time. The first environment map is the active one. If a different environment
 * map is set it is taken as the fading one. The fading is done over time. Once the fading
 * environment map reached full strength it is set as the active one and the fading one set to NULL.
 * Fading is not started before the previous fading has finished. This guarantees no popping but it
 * can lead to a longer usage of an environment map if moving too fast. The fading speed can be
 * adjusted to provide a smoother fading if moving slow and quicker fading if moving fast.
 */
class deoglEnvMapFader{
public:
	deoglEnvironmentMap *pEnvMapActive;
	deoglEnvironmentMap *pEnvMapFading;
	deoglEnvironmentMap *pEnvMapDelayed;
	float pFadePerTime;
	float pBlendFactor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new environment map fader. */
	deoglEnvMapFader();
	/** Cleans up the environment map fader. */
	~deoglEnvMapFader();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the active environment map or NULL if not existing. */
	inline deoglEnvironmentMap *GetActiveEnvMap() const{ return pEnvMapActive; }
	/** Retrieves the fading environment map or NULL if not fading. */
	inline deoglEnvironmentMap *GetFadingEnvMap() const{ return pEnvMapFading; }
	/** Retrieves the delayed environment map or NULL if not fading. */
	inline deoglEnvironmentMap *GetDelayedEnvMap() const{ return pEnvMapDelayed; }
	/**
	 * Retrieves the blend factor for the active environment map. The blend factor for the
	 * fading environment map is 1 minus blend factor.
	 */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	
	/** Retrieves the amount of fading per second. */
	inline float GetFadePerTime() const{ return pFadePerTime; }
	/** Sets the amount of fading per second. */
	void SetFadePerTime( float fadePerTime );
	
	/**
	 * Switch the active environment map. If a fading is in progress the environment map
	 * is put aside to start fading once the current fading has finished. If no fading
	 * is in progress the environment map is set as the fading environment map and fading
	 * starts.
	 */
	void FadeTo( deoglEnvironmentMap *envmap );
	/** Determines if fading is in progess. */
	bool IsFading() const;
	/**
	 * Invalidates an environment map. This drop all references to the environment map to avoid
	 * memory problems. This function has to be called before an environment map is about to
	 * be deleted or if the contained texture is about to get invalid. This operation can lead
	 * to popping.
	 */
	void Drop( deoglEnvironmentMap *envmap );
	/** Drop all environment maps. */
	void DropAll();
	
	/** Update fading. */
	void Update( float elapsed );
	/*@}*/
};

#endif
