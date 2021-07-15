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

#include "deoalMOVRayHitsFaces.h"
#include "deoalWOVRayHitsElement.h"
#include "../deoalRayTraceResult.h"
#include "../deoalRayTraceHitElement.h"
#include "../../../component/deoalAComponent.h"
#include "../../../component/deoalAComponentTexture.h"
#include "../../../model/deoalAModel.h"
#include "../../../model/deoalModelFace.h"
#include "../../../model/octree/deoalModelOctree.h"
#include "../../../utils/cache/deoalRayCache.h"
#include "../../../utils/cache/deoalRayCacheRay.h"
#include "../../../utils/cache/deoalRayCacheRayHit.h"
#include "../../../world/octree/deoalWorldOctree.h"

#include <dragengine/common/exceptions.h>



// Class deoalWOVRayHitsElement
/////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalWOVRayHitsElement::deoalWOVRayHitsElement() :
pResult( NULL ),
pThresholdTransmit( 1e-3f )
{
	(void)pThresholdTransmit; // not used yet
}

deoalWOVRayHitsElement::~deoalWOVRayHitsElement(){
}



// Visiting
/////////////

void deoalWOVRayHitsElement::SetResult( deoalRayTraceResult *result ){
	pResult = result;
}

#ifdef WOVRAYHITSELEMENT_DO_TIMING
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/model/deModel.h>
void deoalWOVRayHitsElement::StartTiming(){
	timingAll = 0.0f;
	timingComponent = 0.0f;
	timingComponentCount = 0;
	timingComponentOctree = 0.0f;
	timingComponentOctreeCount = 0;
	timingFaces = 0.0f;
	timingFacesCount = 0;
	rayAddCount = 0;
	rayFoundCount = 0;
	timerAll.Reset();
}

void deoalWOVRayHitsElement::EndTiming(){
	timingAll = timerAll.GetElapsedTime();
}

/*
Parallel-TraceSoundRays: pos=(20.141,1.600,12.176) range=60.000 rays=162
Result-TraceSoundRays: minExt=(-24.150,-0.000,-29.947) maxExt=(60.390,52.066,54.299) center=(18.120,26.033,12.176) size=(84.540,52.066,84.245) avgSize=73.617 mfp=3.740 echoDelay=0.011 volume=825.522 surface=570.679 abs=0.049 sabine=(6.246,28.160,53.223) revTime=(21.294,4.723,2.499) sepTimeFLR=0.0299 rays=765 segments=7096 time=344.59ms
WOVRHE: all=344.656 comp=2.272(20678) compOctree=283.926(20678)
Parallel-TraceSoundRays: pos=(0.000,1.600,-4.000) range=60.000 rays=162
Result-TraceSoundRays: minExt=(-13.000,-0.000,-8.000) maxExt=(46.227,2.500,2.900) center=(16.613,1.250,-2.550) size=(59.227,2.500,10.900) avgSize=24.209 mfp=2.553 echoDelay=0.007 volume=121.861 surface=165.160 abs=0.039 sabine=(3.355,6.517,9.814) revTime=(5.852,3.012,2.001) sepTimeFLR=0.0204 rays=441 segments=5991 time=48.28ms
WOVRHE: all=48.324 comp=0.987(8968) compOctree=23.297(8968)


Parallel-TraceSoundRays: pos=(20.141,1.600,12.176) range=60.000 rays=162
Result-TraceSoundRays: minExt=(-12.800,-0.000,0.100) maxExt=(52.800,4.500,22.800) center=(20.000,2.250,11.450) size=(65.600,4.500,22.700) avgSize=30.933 mfp=3.566 echoDelay=0.010 volume=824.507 surface=568.560 abs=0.049 sabine=(6.225,28.054,53.018) revTime=(21.338,4.735,2.506) sepTimeFLR=0.0285 rays=716 segments=6937 time=231.01ms
WOVRHE: all=231.065 comp=2.333(21561) compOctree=162.769(21561)
Parallel-TraceSoundRays: pos=(0.000,1.600,-4.000) range=60.000 rays=162
Result-TraceSoundRays: minExt=(-13.000,-0.000,-8.000) maxExt=(46.227,2.500,2.900) center=(16.613,1.250,-2.550) size=(59.227,2.500,10.900) avgSize=24.209 mfp=2.553 echoDelay=0.007 volume=121.861 surface=165.160 abs=0.039 sabine=(3.355,6.517,9.814) revTime=(5.852,3.012,2.001) sepTimeFLR=0.0204 rays=441 segments=5991 time=50.12ms
WOVRHE: all=50.179 comp=0.892(9090) compOctree=20.291(9090)
*/

/*
more rays, 1 transmission
WOVRHE: all=50.224 comp=0.834(9266) compOctree=12.141(9266)
WOVRHE: all=140.667 comp=1.796(19406) compOctree=107.826(19406)

more rays, 0 transmission
WOVRHE: all=16.330 comp=0.486(3930) compOctree=5.165(3930)
WOVRHE: all=61.499 comp=0.788(8005) compOctree=48.080(8005)

less rays, 1 transmission
WOVRHE: all=10.913 comp=0.302(2442) compOctree=3.144(2442)
WOVRHE: all=36.483 comp=0.555(4614) compOctree=27.711(4614)

less rays, 0 transmission
WOVRHE: all=4.059 comp=0.126(990) compOctree=1.316(990)
WOVRHE: all=15.271 comp=0.197(1986) compOctree=11.848(1986)

extreme test,parallel:
more rays, 1 transmission: 61.06ms
more rays, 0 transmission: 27.09ms
less rays, 1 transmission: 14.93ms
less rays, 0 transmission: 7.00ms
*/

/*
/model/hallway/corner.demodel: faces 32, (1.0,4.0)ys, count 289, total 445.0
/model/hallway/end.demodel: faces 38, (1.0,3.0)ys, count 299, total 473.0
/model/hallway/straight.demodel: faces 44, (1.0,23.0)ys, count 11711, total 23881.0
/model/prop/blinds1.demodel: faces 48, (0.0,6.0)ys, count 2765, total 5549.0
/model/prop/blinds2.demodel: faces 16, (0.0,10.0)ys, count 2681, total 2624.0
/model/prop/door1.demodel: faces 30, (1.0,14.0)ys, count 904, total 1282.0
/model/prop/entrance.demodel: faces 104, (2.0,15.0)ys, count 1506, total 5422.0
/model/prop/pillar1.demodel: faces 862, (5.0,54.0)ys, count 14126, total 158082.0
/model/prop/pole.demodel: faces 300, (5.0,15.0)ys, count 28, total 243.0
/model/prop/railing1.demodel: faces 1056, (9.0,40.0)ys, count 2417, total 39884.0
/model/prop/railing2.demodel: faces 1880, (7.0,54.0)ys, count 616, total 14349.0
/model/prop/roomLabel.demodel: faces 230, (4.0,20.0)ys, count 408, total 3064.0
/model/prop/speaker1.demodel: faces 412, (5.0,29.0)ys, count 3914, total 42727.0
/model/room/room1.demodel: faces 38, (0.0,16.0)ys, count 25572, total 41308.0
/model/room/room2.demodel: faces 38, (1.0,8.0)ys, count 3072, total 5249.0
/model/room/room3.demodel: faces 38, (0.0,15.0)ys, count 10599, total 16549.0

WOVRHE: all=221.431 comp=3.551(21561) compOctree=155.009(21561) face=129.434(3640072)
WOVRHE: all=50.884 comp=1.429(9095) compOctree=21.080(9095) face=16.367(442350)
*/

/*
- 125Hz: 2.7m
- 1kHz: 0.34m
- 2kHz: 0.17m
- 4kHz: 0.085m
*/
#endif

// #define USE_RAY_CACHE 1

/*
all=74.778 comp=12.619(11108) compOctree=17.106(11067) face=0.000(0) rayAdd=11067 rayFound=41
all=49.893 comp=12.842(8535) compOctree=14.355(8481) face=0.000(0) rayAdd=8481 rayFound=54
all=42.621 comp=13.245(7066) compOctree=13.230(7035) face=0.000(0) rayAdd=7035 rayFound=31
all=160.286 comp=27.660(17058) compOctree=98.597(17019) face=0.000(0) rayAdd=17019 rayFound=39
all=60.220 comp=19.764(7515) compOctree=24.293(7507) face=0.000(0) rayAdd=7507 rayFound=8
all=78.118 comp=29.009(9924) compOctree=28.842(9891) face=0.000(0) rayAdd=9891 rayFound=33

12.65ms => 26.34ms
13.05ms => 24.05ms
10.94ms => 21.03ms
55.94ms => 54.87ms
9.51ms => 22.98ms
13.09ms => 33.27ms

<0.5% on the initial casting. on subsequent casting this can go up to 2%.
2% at best is though nothing to warrent the troubles.
furthermore it's slower as expected due to the mutex locking. 
 */
#include "../../../audiothread/deoalAudioThread.h"
#include "../../../audiothread/deoalATLogger.h"

void deoalWOVRayHitsElement::VisitComponent( deoalAComponent *component ){
	// WARNING everything in here has to be thread-safe
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
		timingComponentCount++;
		timerComponent.Reset();
	#endif
	
	deoalAModel &model = *component->GetModel();
	
	if( ! model.GetRTBVH() && ! component->GetBVH() ){
		return;
	}
	
	const decDMatrix &invMatrix = component->GetInverseMatrix();
	const decVector rayOrigin( invMatrix * GetRayOrigin() );
	const decVector rayDirection( invMatrix.TransformNormal( GetRayDirection() ) );
	
	// check if ray is cached in the model
	#ifdef USE_RAY_CACHE
		const decVector cacheRayOrigin( pClampToBox( component->GetModel()->GetMinExtend(),
			component->GetModel()->GetMaxExtend(), rayOrigin, rayDirection ) );
		const decVector cacheRayDirection( rayOrigin + rayDirection - cacheRayOrigin );
		const float cacheRayDirectionLen = cacheRayDirection.Length();
		const deoalRayCacheRay * const cachedRay =
			model.GetRayCache().FindRay( cacheRayOrigin, cacheRayDirection );
		
		// NOTE if a cached ray is present it has to be long enough. by using a too short
		//      cached ray we might miss collisions which results in bad problems
		if( cachedRay && cachedRay->GetLength() >= cacheRayDirectionLen - 0.001f ){
			const float distanceOffset = decMath::max( ( cacheRayDirection / cacheRayDirectionLen )
				* ( cachedRay->GetOrigin() - rayOrigin ), 0.0f );
			const decDMatrix &matrix = component->GetMatrix();
			const bool hasScaling = component->GetHasScaling();
			const int hitCount = cachedRay->GetHitCount();
			int i;
			
			// add hits but only those shorter than the required length. required since the
			// cached ray can be longer. by not adding hits beyond the range we avoid not
			// required calculations
			for( i=0; i<hitCount; i++ ){
				const deoalRayCacheRayHit &hit = cachedRay->GetHitAt( i );
				if( hit.GetDistance() > cacheRayDirectionLen ){
					continue;
				}
				
				const decDVector point( matrix * hit.GetPoint() );
				decVector normal( matrix.TransformNormal( hit.GetNormal() ) );
				if( hasScaling ){
					normal.Normalize();
				}
				
				pResult->AddElement( distanceOffset + hit.GetDistance(), point, normal,
					component, hit.GetFaceIndex(), hit.GetForwardFacing() );
			}
			
			#ifdef WOVRAYHITSELEMENT_DO_TIMING
			rayFoundCount++;
			#endif
			return;
		}
	#endif
	
	// ray is not cached or too short. do ray tracing to find the results
	deoalMOVRayHitsFaces visitor( *component, model );
	
	#ifdef USE_RAY_CACHE
		visitor.SetRay( cacheRayOrigin, cacheRayDirection );
		pComponentResult.RemoveAllElements();
		visitor.SetResult( &pComponentResult );
		visitor.SetResultInWorldSpace( false );
	#else
		visitor.SetRay( rayOrigin, rayDirection );
		visitor.SetResult( pResult );
		visitor.SetResultInWorldSpace( true );
	#endif
	
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
		timingComponent += timerComponent.GetElapsedTime();
	#endif
	
	if( component->GetBVH() ){
		visitor.VisitBVH( *component->GetBVH() );
		
	}else{
		visitor.VisitBVH( *model.GetRTBVH() );
	}
	
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
		const float elapsed = timerComponent.GetElapsedTime();
		timingComponentOctree += elapsed; //timerComponent.GetElapsedTime();
		timingComponentOctreeCount++;
		#ifdef MOVRAYHITSFACES_DO_TIMIING
			timingFaces += visitor.timing;
			timingFacesCount += visitor.timingCount;
		#endif
	#endif
	
	// determine the maximum length of the cache. this takes into account the hit element
	// transmission properties to stop the cach ray as early as possible
	#ifdef USE_RAY_CACHE
	
	const int elementCount = pComponentResult.GetElementCount();
	float maxLength = cacheRayDirection.Length();
	int i, maxElementCount = elementCount;
	
	#if 0
	// disabled because the same model can be used with different components using different
	// textures producing different transmission values. to re-use the cached result for
	// all non-dynamic components requires to store them for full transmissiona ll the way.
	// not a problem actually since values are transmission applied by the cache user and
	// the used ray length has no influence outside the model
	if( elementCount > 0 ){
		float transmissionLow = 0.0f;
		float transmissionMedium = 0.0f;
		float transmissionHigh = 0.0f;
		bool inTransmission = false;
		float gainLow = 1.0f;
		float gainMedium = 1.0f;
		float gainHigh = 1.0f;
		float distanceFront;
		
		for( i=0; i<elementCount; i++ ){
			const deoalRayTraceHitElement &element = pComponentResult.GetElementAt( i );
			
			if( inTransmission ){
				if( element.GetForwardFacing() ){
					continue;
				}
				
				const float thickness = element.GetDistance() - distanceFront;
				gainLow = decMath::linearStep( thickness, 0.0f, transmissionLow, gainLow, 0.0f );
				gainMedium = decMath::linearStep( thickness, 0.0f, transmissionMedium, gainMedium, 0.0f );
				gainHigh = decMath::linearStep( thickness, 0.0f, transmissionHigh, gainHigh, 0.0f );
				inTransmission = false;
				
				if( gainLow <= pThresholdTransmit || gainMedium <= pThresholdTransmit
				|| gainHigh <= pThresholdTransmit ){
					break;
				}
				
			}else{
				if( ! element.GetForwardFacing() ){
					continue;
				}
				
				if( element.GetComponent() ){
					const deoalAComponentTexture &texture = element.GetComponent()->GetTextureAt(
						element.GetComponent()->GetModel()->GetFaceAt(
							element.GetComponentFace() ).GetTexture() );
					
					distanceFront = element.GetDistance();
					transmissionLow = texture.GetTransmissionLow();
					transmissionMedium = texture.GetTransmissionMedium();
					transmissionHigh = texture.GetTransmissionHigh();
					gainLow *= 1.0f - texture.GetAbsorptionLow();
					gainMedium *= 1.0f - texture.GetAbsorptionMedium();
					gainHigh *= 1.0f - texture.GetAbsorptionHigh();
					inTransmission = true;
					
					if( gainLow <= pThresholdTransmit || gainMedium <= pThresholdTransmit
					|| gainHigh <= pThresholdTransmit ){
						break;
					}
					
				}else{
					break;
				}
			}
		}
		
		maxElementCount = i;
		if( i < elementCount ){
			maxLength = pComponentResult.GetElementAt( i ).GetDistance();
		}
	}
	#endif
	
	// add the result to the ray cache
	#ifdef WOVRAYHITSELEMENT_DO_TIMING
	rayAddCount++;
	#endif
	
	// cache the ray. removes all matching rays before adding the new one. to prevent race
	// conditions if more than one task tries to replace the same ray the longest one is kept
	model.GetRayCache().CacheRay( cacheRayOrigin, cacheRayDirection,
		maxLength, pComponentResult, maxElementCount );
	
	// add the elements to the result
	const float distanceOffset = cacheRayDirection.Normalized() * ( cacheRayOrigin - rayOrigin );
	const decDMatrix &matrix = component->GetMatrix();
	const bool hasScaling = component->GetHasScaling();
	
	for( i=0; i<maxElementCount; i++ ){
		const deoalRayTraceHitElement &element = pComponentResult.GetElementAt( i );
		
		const decDVector point( matrix * element.GetPoint() );
		decVector normal( matrix.TransformNormal( element.GetNormal() ) );
		if( hasScaling ){
			normal.Normalize();
		}
		
		pResult->AddElement( distanceOffset + element.GetDistance(), point, normal,
			element.GetComponent(), element.GetComponentFace(), element.GetForwardFacing() );
	}
	
	#endif
}



// Protected functions
////////////////////////

decVector deoalWOVRayHitsElement::pClampToBox( const decVector &minExtend,
const decVector &maxExtend, const decVector &rayOrigin, const decVector &rayDirection ){
	if( rayOrigin >= minExtend && rayOrigin <= maxExtend ){
		return rayOrigin;
	}
	
	float bestLambda = 1.0f;
	float lambda;
	
	if( fabsf( rayDirection.x ) > FLOAT_SAFE_EPSILON ){
		const float invRayDirection = 1.0f / rayDirection.x;
		
		lambda = invRayDirection * ( minExtend.x - rayOrigin.x );
		if( lambda >= 0.0f && lambda <= 1.0f ){
			const float y = rayOrigin.y + rayDirection.y * lambda;
			if( y >= minExtend.y && y <= maxExtend.y ){
				const float z = rayOrigin.z + rayDirection.z * lambda;
				if( z >= minExtend.z && z <= maxExtend.z ){
					if( lambda < bestLambda ){
						bestLambda = lambda;
					}
				}
			}
		}
		
		lambda = invRayDirection * ( maxExtend.x - rayOrigin.x );
		if( lambda >= 0.0f && lambda <= 1.0f ){
			const float y = rayOrigin.y + rayDirection.y * lambda;
			if( y >= minExtend.y && y <= maxExtend.y ){
				const float z = rayOrigin.z + rayDirection.z * lambda;
				if( z >= minExtend.z && z <= maxExtend.z ){
					if( lambda < bestLambda ){
						bestLambda = lambda;
					}
				}
			}
		}
	}
	
	if( fabsf( rayDirection.y ) > FLOAT_SAFE_EPSILON ){
		const float invRayDirection = 1.0f / rayDirection.y;
		
		lambda = invRayDirection * ( maxExtend.y - rayOrigin.y );
		if( lambda >= 0.0f && lambda <= 1.0f ){
			const float x = rayOrigin.x + rayDirection.x * lambda;
			if( x >= minExtend.x && x <= maxExtend.x ){
				const float z = rayOrigin.z + rayDirection.z * lambda;
				if( z >= minExtend.z && z <= maxExtend.z ){
					if( lambda < bestLambda ){
						bestLambda = lambda;
					}
				}
			}
		}
		
		lambda = invRayDirection * ( minExtend.y - rayOrigin.y );
		if( lambda >= 0.0f && lambda <= 1.0f ){
			const float x = rayOrigin.x + rayDirection.x * lambda;
			if( x >= minExtend.x && x <= maxExtend.x ){
				const float z = rayOrigin.z + rayDirection.z * lambda;
				if( z >= minExtend.z && z <= maxExtend.z ){
					if( lambda < bestLambda ){
						bestLambda = lambda;
					}
				}
			}
		}
	}
	
	if( fabsf( rayDirection.z ) > FLOAT_SAFE_EPSILON ){
		const float invRayDirection = 1.0f / rayDirection.z;
		
		lambda = invRayDirection * ( maxExtend.z - rayOrigin.z );
		if( lambda >= 0.0f && lambda <= 1.0f ){
			const float x = rayOrigin.x + rayDirection.x * lambda;
			if( x >= minExtend.x && x <= maxExtend.x ){
				const float y = rayOrigin.y + rayDirection.y * lambda;
				if( y >= minExtend.y && y <= maxExtend.y ){
					if( lambda < bestLambda ){
						bestLambda = lambda;
					}
				}
			}
		}
		
		lambda = invRayDirection * ( minExtend.z - rayOrigin.z );
		if( lambda >= 0.0f && lambda <= 1.0f ){
			const float x = rayOrigin.x + rayDirection.x * lambda;
			if( x >= minExtend.x && x <= maxExtend.x ){
				const float y = rayOrigin.y + rayDirection.y * lambda;
				if( y >= minExtend.y && y <= maxExtend.y ){
					if( lambda < bestLambda ){
						bestLambda = lambda;
					}
				}
			}
		}
	}
	
	return rayOrigin + rayDirection * bestLambda;
}
