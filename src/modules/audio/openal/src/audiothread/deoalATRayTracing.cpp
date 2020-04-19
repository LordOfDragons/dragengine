/* 
 * Drag[en]gine OpenAL Audio Module
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalAudioThread.h"
#include "deoalATRayTracing.h"
#include "deoalATLogger.h"
#include "../deAudioOpenAL.h"
#include "../environment/raytrace/deoalIcoSphere.h"
#include "../environment/raytrace/deoalRayTraceConfig.h"
#include "../microphone/deoalAMicrophone.h"

#include <dragengine/common/exceptions.h>



// Class deoalATRayTracing
////////////////////////////

// Constructor, destructor
////////////////////////////

deoalATRayTracing::deoalATRayTracing( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),

pConfigTraceSoundRays( NULL ),
pConfigRoomEstimate( NULL ),
pConfigTraceSoundRaysSensor( NULL )
{
	try{
		pCreateRayTraceConfigs();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoalATRayTracing::~deoalATRayTracing(){
	pCleanUp();
}



// Management
///////////////

void deoalATRayTracing::ConfigurationChanged(){
	pCreateRayTraceConfigs();
}



// Private Functions
//////////////////////

void deoalATRayTracing::pCleanUp(){
	if( pConfigTraceSoundRays ){
		delete pConfigTraceSoundRays;
	}
	if( pConfigRoomEstimate ){
		delete pConfigRoomEstimate;
	}
	if( pConfigTraceSoundRaysSensor ){
		delete pConfigTraceSoundRaysSensor;
	}
}

void deoalATRayTracing::pCreateRayTraceConfigs(){
	const deoalConfiguration &config = pAudioThread.GetConfiguration();
	
	// configuration for tracing sound rays
	if( pConfigTraceSoundRays ){
		delete pConfigTraceSoundRays;
		pConfigTraceSoundRays = NULL;
	}
	pConfigTraceSoundRays = new deoalRayTraceConfig;
	pConfigTraceSoundRays->SetRaysEquallySpaced( config.GetSoundTraceRayCount() );
	pConfigTraceSoundRays->Rotate( 5.0f, 7.0f, 3.0f );
	
	// configuration for estimating room parameters
	if( pConfigRoomEstimate ){
		delete pConfigRoomEstimate;
		pConfigRoomEstimate = NULL;
	}
	pConfigRoomEstimate = new deoalRayTraceConfig;
	pConfigRoomEstimate->SetRaysEquallySpaced( config.GetEstimateRoomRayCount() );
	
	// configuration for tracing sound rays for sound sensor meters
	if( pConfigTraceSoundRaysSensor ){
		delete pConfigTraceSoundRaysSensor;
		pConfigTraceSoundRaysSensor = NULL;
	}
	pConfigTraceSoundRaysSensor = new deoalRayTraceConfig;
	pConfigTraceSoundRaysSensor->SetRaysEquallySpaced( 24 ); //config.GetSoundTraceRayCount() );
	pConfigTraceSoundRaysSensor->Rotate( 5.0f, 7.0f, 3.0f );
	
	// sound tracing configuration for microphone usage
	pConfigSoundTracingMicrophone.addRayMinLength = 0.2f; // 0.1f
	pConfigSoundTracingMicrophone.maxBounceCount = config.GetSoundTraceMaxBounceCount();
	pConfigSoundTracingMicrophone.maxTransmitCount = config.GetSoundTraceMaxTransmitCount();
	pConfigSoundTracingMicrophone.thresholdReflect = 1e-3f; // 1e-4f
	pConfigSoundTracingMicrophone.thresholdTransmit = 1e-3f;
	pConfigSoundTracingMicrophone.rtConfig = pConfigTraceSoundRays;
	#ifdef LISTENER_CENTRIC_RAY_CAST
	pConfigSoundTracingMicrophone.inverseRayTracing = true;
	#else
	pConfigSoundTracingMicrophone.inverseRayTracing = false;
	#endif
	
	// sound tracing configuration for metering usage
	pConfigSoundTracingMetering = pConfigSoundTracingMicrophone;
	pConfigSoundTracingMetering.maxBounceCount = 10;
	pConfigSoundTracingMetering.maxTransmitCount = 2;
	pConfigSoundTracingMetering.rtConfig = pConfigTraceSoundRaysSensor;
	pConfigSoundTracingMetering.inverseRayTracing = true;
}


#if 0

void deoalATRayTracing::pCreateRayTraceConfigs(){
	const deoalConfiguration &config = pAudioThread.GetConfiguration();
	deoalRayTraceConfig *probeConfig = NULL;
	
	try{
		/*
		pSubdivideIcoSphere: vertices=42 openingAngle=37.9
		pSubdivideIcoSphere: vertices=162 openingAngle=20.1
		pIncreaseProbeConfigLevels: level=1 vertices=642 openingAngle=10.2
		pIncreaseProbeConfigLevels: level=2 vertices=2562 openingAngle=5.1
		*/
		pCreateInitialIcoSphere();
		
		probeConfig = new deoalRayTraceConfig;
		probeConfig->SetFromVertices( pIcoSphere.GetVertices(),
			pIcoSphere.GetVertexCount(), pIcoSphere.GetOpeningAngle() );
		pProbeConfigs.Add( probeConfig );
		probeConfig = NULL;
		
		// create refinemenet levels untli the ray count is around ~10k.
		while( pIcoSphere.GetVertexCount() + pIcoSphere.GetEdgeCount() < 10000 ){
			// level 0: 162 rays
			// level 1: 642 rays
			// level 2: 2562 rays
			// level 3: 10242 rays
			// level 4: 40962 rays
			// level 5: 163842 rays
			pIncreaseProbeConfigLevels();
		}
		
		// create configs
		/*
		deoalIcoSphere ico( deoalIcoSphere::BaseLevel() ); // 12 rays
		ico = ico.Subdivide(); // 42 rays
		pConfigTraceSoundRays = new deoalRayTraceConfig;
		pConfigTraceSoundRays->SetFromIcoSphere( ico );
// 		pConfigTraceSoundRays->SetFromIcoSphere( ico.Subdivide() );
// 		pConfigTraceSoundRays->SetFromIcoSphere( ico.Subdivide().Subdivide() );
		pConfigTraceSoundRays->Rotate( 5.0f, 7.0f, 3.0f );
		
		ico = ico.Subdivide(); // 162 rays
		pConfigRoomEstimate = new deoalRayTraceConfig;
		pConfigRoomEstimate->SetFromIcoSphere( ico );
		*/
		
		pConfigTraceSoundRays = new deoalRayTraceConfig;
		pConfigTraceSoundRays->SetRaysEquallySpaced( config.GetSoundTraceRayCount() );
		pConfigTraceSoundRays->Rotate( 5.0f, 7.0f, 3.0f );
		
		pConfigRoomEstimate = new deoalRayTraceConfig;
		pConfigRoomEstimate->SetRaysEquallySpaced( config.GetEstimateRoomRayCount() );
		
	}catch( const deException & ){
		if( probeConfig ){
			delete probeConfig;
		}
		throw;
	}
}

#endif


#if 0
void deoalEnvProbeList::pCreateInitialIcoSphere(){
	// level 0 is an ico-sphere subdivided 2 times. produces 162 rays.
	const int subDivCount = 1; // 40 vertices
// 	const int subDivCount = 2; // 162 vertices
// 	const int subDivCount = 3; // 642 vertices
// 	const int subDivCount = 4; // 2562 vertices
	
	pIcoSphere = deoalIcoSphere::BaseLevel(); // 12 vertices
	
	int i;
	for( i=0; i<subDivCount; i++ ){
		pSubdivideIcoSphere();
	}
}

void deoalEnvProbeList::pSubdivideIcoSphere(){
	/*
	ico-sphere propagation.
	a=(12,30,20) # (vertexCount, edgeCount, faceCount)
	def tt(a):
		return (a[0]+a[1], a[1]*2+a[2]*3, a[2]*4)
	for i in range(10):
		a=tt(a)
		print(a)
		
	   (42, 120, 80)
	L0 (162, 480, 320)
	L1 (642, 1920, 1280)
	L2 (2562, 7680, 5120)
	L3 (10242, 30720, 20480)
	L4 (40962, 122880, 81920)
	L5 (163842, 491520, 327680)
	
	// using first ico sphere vertex count as base update batch size.
	L0: rays per probe update = 162, 1 update
	L1: rays per probe update = 162, 4 updates (last 156 rays)
	L2: rays per probe update = 162, 16 updates (last 132 rays)
	L3: rays per probe update = 162, 64 updates (last 36 rays)
	L4: rays per probe update = 162, 253 updates (last 138 rays)
	L5: rays per probe update = 162, 1012 updates (last 60)
	
	// using 1/3^level ico sphere vertex count as base update batch size.
	L0: rays per probe update = 162, 1 update
	L1: rays per probe update = 214, 3 updates
	L2: rays per probe update = 284, 9 updates (last +6 rays)
	L3: rays per probe update = 379, 27 updates (last +9 rays)
	L4: rays per probe update = 505, 81 updates (last +57 rays)
	L5: rays per probe update = 674, 243 updates (last +60 rays)
	*/
	
	pIcoSphere = pIcoSphere.Subdivide();
	pWorld.GetAudioThread().GetLogger().LogInfoFormat(
		"pSubdivideIcoSphere: vertices=%d openingAngle=%.1f",
		pIcoSphere.GetVertexCount(), pIcoSphere.GetOpeningAngle() / DEG2RAD );
}

void deoalEnvProbeList::pIncreaseProbeConfigLevels(){
	deoalRayTraceConfig *probeConfig = NULL;
	int i;
	
	int levelCount = 1;
	for( i=0; i<=pIcoSphereLevel; i++ ){
		levelCount *= 3;
	}
	
	pIcoSphere = pIcoSphere.Subdivide();
	pIcoSphereLevel++;
	
	try{
		const int vertexCount = pIcoSphere.GetVertexCount();
		const decVector * const vertices = pIcoSphere.GetVertices();
		const float openingAngle = pIcoSphere.GetOpeningAngle();
		int raysPerLevel = vertexCount / levelCount;
		int firstRay = 0;
		
		for( i=0; i<levelCount; i++ ){
			if( i == levelCount - 1 ){
				raysPerLevel = vertexCount - firstRay;
			}
			
			probeConfig = new deoalRayTraceConfig;
			probeConfig->SetFromVertices( vertices + firstRay, raysPerLevel, openingAngle );
			pProbeConfigs.Add( probeConfig );
			probeConfig = NULL;
			
			firstRay += raysPerLevel;
		}
		
	}catch( const deException & ){
		if( probeConfig ){
			delete probeConfig;
		}
		throw;
	}
	
	pWorld.GetAudioThread().GetLogger().LogInfoFormat(
		"pIncreaseProbeConfigLevels: level=%d vertices=%d openingAngle=%.1f",
		pIcoSphereLevel, pIcoSphere.GetVertexCount(), pIcoSphere.GetOpeningAngle() / DEG2RAD );
}
#endif
