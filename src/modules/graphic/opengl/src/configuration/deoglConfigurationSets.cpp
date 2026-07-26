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

#include "deoglConfigurationSets.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class deoglConfigurationSets
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConfigurationSets::deoglConfigurationSets(deoglConfiguration &configuration) :
pConfiguration(configuration),

// shadow quality
//
// cube map sizes above 1024 are brutal on the memory consumption especially if many
// point lights are close to the cameras. later on this can be modified to use higher
// resolution if the count of active point light shadow cubes is low
//
// the memory consumption of a cube map is 6x the memory consumption of a shadow map
// of the same size. if the cube map is half the size of the shadow map the memory
// consumption of the cube map is 1.5x the memory consumption of the shadow map
//
// for the time being half the shadow map size is used

pShadowQualityVeryHigh{
	.enable = true,
	.ditherShadows = true,
	.shadowMapSize = 4096,
	.shadowCubeSize = 2048,
	.shadowSkySize = 4096,
	.screenSpaceStepCount = 24
},
pShadowQualityHigh{
	.enable = true,
	.ditherShadows = true,
	.shadowMapSize = 2048,
	.shadowCubeSize = 1024,
	.shadowSkySize = 2048,
	.screenSpaceStepCount = 16
},
pShadowQualityMedium{
	.enable = true,
	.ditherShadows = true,
	.shadowMapSize = 1024,
	.shadowCubeSize = 512,
	.shadowSkySize = 1024,
	.screenSpaceStepCount = 10
},
pShadowQualityLow{
	.enable = true,
	.ditherShadows = true,
	.shadowMapSize = 512,
	.shadowCubeSize = 256,
	.shadowSkySize = 512,
	.screenSpaceStepCount = 6
},
pShadowQualityVeryLow{
	.enable = true,
	.ditherShadows = true,
	.shadowMapSize = 256,
	.shadowCubeSize = 128,
	.shadowSkySize = 256,
	.screenSpaceStepCount = 4
},
pShadowQualityOff{
	.enable = false,
	.ditherShadows = false,
	.shadowMapSize = 256,
	.shadowCubeSize = 128,
	.shadowSkySize = 256,
	.screenSpaceStepCount = 0
},


// GI quality

pGIQualityVeryHigh{
	.enable = true,
	.areaTrackerImportance = 1,
	.raysPerProbe = 256
},
pGIQualityHigh{
	.enable = true,
	.areaTrackerImportance = 2,
	.raysPerProbe = 128 // 192
},
pGIQualityMedium{
	.enable = true,
	.areaTrackerImportance = 3,
	.raysPerProbe = 64 // 192
},
pGIQualityLow{
	.enable = true,
	.areaTrackerImportance = 4,
	.raysPerProbe = 32 // 128
},
pGIQualityVeryLow{
	.enable = true,
	.areaTrackerImportance = 5,
	.raysPerProbe = 16 // 128
},
pGIQualityOff{
	.enable = false,
	.areaTrackerImportance = 6,
	.raysPerProbe = 16
},


// GI update speed

pGIUpdateSpeedVeryHigh{
	.probeCount = 2048
},
pGIUpdateSpeedHigh{
	.probeCount = 1024
},
pGIUpdateSpeedMedium{
	.probeCount = 512
},
pGIUpdateSpeedLow{
	.probeCount = 256
},
pGIUpdateSpeedVeryLow{
	.probeCount = 128
},
pGIUpdateSpeedOff{
	.probeCount = 128
}

{
}


// Management
///////////////

const deoglConfigurationSets::sShadowQuality &deoglConfigurationSets::ShadowQuality() const{
	return ShadowQuality(pConfiguration.GetShadowQuality());
}

const deoglConfigurationSets::sShadowQuality &deoglConfigurationSets::ShadowQuality(
deoglConfiguration::eShadowQuality quality) const{
	switch(quality){
	case deoglConfiguration::esqOff:
		return pShadowQualityOff;
		
	case deoglConfiguration::esqVeryLow:
		return pShadowQualityVeryLow;
		
	case deoglConfiguration::esqLow:
		return pShadowQualityLow;
		
	case deoglConfiguration::esqMedium:
		return pShadowQualityMedium;
		
	case deoglConfiguration::esqHigh:
		return pShadowQualityHigh;
		
	case deoglConfiguration::esqVeryHigh:
		return pShadowQualityVeryHigh;
		
	default:
		DETHROW(deeInvalidParam);
	}
}


const deoglConfigurationSets::sGIQuality &deoglConfigurationSets::GIQuality() const{
	return GIQuality(pConfiguration.GetGIQuality());
}

const deoglConfigurationSets::sGIQuality &deoglConfigurationSets::GIQuality(
deoglConfiguration::eGIQuality quality) const{
	switch(quality){
	case deoglConfiguration::egiqOff:
		return pGIQualityOff;
		
	case deoglConfiguration::egiqVeryLow:
		return pGIQualityVeryLow;
		
	case deoglConfiguration::egiqLow:
		return pGIQualityLow;
		
	case deoglConfiguration::egiqMedium:
		return pGIQualityMedium;
		
	case deoglConfiguration::egiqHigh:
		return pGIQualityHigh;
		
	case deoglConfiguration::egiqVeryHigh:
		return pGIQualityVeryHigh;
		
	default:
		DETHROW(deeInvalidParam);
	}
}


const deoglConfigurationSets::sGIUpdateSpeed &deoglConfigurationSets::GIUpdateSpeed() const{
	return GIUpdateSpeed(pConfiguration.GetGIUpdateSpeed());
}

const deoglConfigurationSets::sGIUpdateSpeed &deoglConfigurationSets::GIUpdateSpeed(
deoglConfiguration::eGIUpdateSpeed speed) const{
	switch(speed){
	case deoglConfiguration::egiusOff:
		return pGIUpdateSpeedOff;
		
	case deoglConfiguration::egiusVeryLow:
		return pGIUpdateSpeedVeryLow;
		
	case deoglConfiguration::egiusLow:
		return pGIUpdateSpeedLow;
		
	case deoglConfiguration::egiusMedium:
		return pGIUpdateSpeedMedium;
		
	case deoglConfiguration::egiusHigh:
		return pGIUpdateSpeedHigh;
		
	case deoglConfiguration::egiusVeryHigh:
		return pGIUpdateSpeedVeryHigh;
		
	default:
		DETHROW(deeInvalidParam);
	}
}
