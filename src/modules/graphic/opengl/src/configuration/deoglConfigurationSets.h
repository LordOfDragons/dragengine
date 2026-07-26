/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLCONFIGURATIONSETS_H_
#define _DEOGLCONFIGURATIONSETS_H_

#include "deoglConfiguration.h"

class deGraphicOpenGl;


// OpenGL configuration sets
//////////////////////////////

class deoglConfigurationSets{
public:
	struct sShadowQuality{
		bool enable;
		bool ditherShadows;
		int shadowMapSize;
		int shadowCubeSize;
		int shadowSkySize;
		int screenSpaceStepCount;
	};
	
	struct sGIQuality{
		bool enable;
		int areaTrackerImportance;
		int raysPerProbe;
	};
	
	struct sGIUpdateSpeed{
		int probeCount;
	};
	
	
private:
	deoglConfiguration &pConfiguration;
	
	sShadowQuality pShadowQualityVeryHigh, pShadowQualityHigh, pShadowQualityMedium,
		pShadowQualityLow, pShadowQualityVeryLow, pShadowQualityOff;
	
	sGIQuality pGIQualityVeryHigh, pGIQualityHigh, pGIQualityMedium,
		pGIQualityLow, pGIQualityVeryLow, pGIQualityOff;
	
	sGIUpdateSpeed pGIUpdateSpeedVeryHigh, pGIUpdateSpeedHigh, pGIUpdateSpeedMedium,
		pGIUpdateSpeedLow, pGIUpdateSpeedVeryLow, pGIUpdateSpeedOff;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create caches. */
	deoglConfigurationSets(deoglConfiguration &configuration);
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Get shadow quality configuration set. */
	const sShadowQuality &ShadowQuality() const;
	const sShadowQuality &ShadowQuality(deoglConfiguration::eShadowQuality quality) const;
	
	/** Get GI quality configuration set. */
	const sGIQuality &GIQuality() const;
	const sGIQuality &GIQuality(deoglConfiguration::eGIQuality quality) const;
	
	/** Get GI update speed configuration set. */
	const sGIUpdateSpeed &GIUpdateSpeed() const;
	const sGIUpdateSpeed &GIUpdateSpeed(deoglConfiguration::eGIUpdateSpeed speed) const;
	/*@}*/
};

#endif
