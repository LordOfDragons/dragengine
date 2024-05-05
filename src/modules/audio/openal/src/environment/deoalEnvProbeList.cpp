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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoalEnvProbe.h"
#include "deoalEnvProbeList.h"
#include "raytrace/deoalRayTraceConfig.h"
#include "raytrace/parallel/deoalRTParallelEnvProbe.h"
#include "../audiothread/deoalAudioThread.h"
#include "../audiothread/deoalATLogger.h"
#include "../audiothread/deoalATRayTracing.h"
#include "../world/deoalAWorld.h"
#include "../world/octree/deoalWorldOctree.h"
#include "../world/octree/deoalRTWorldBVH.h"

#include <dragengine/common/exceptions.h>



// Class deoalEnvProbeList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalEnvProbeList::deoalEnvProbeList( deoalAWorld &world, double reuseDistance, int maxProbeCount ) :
pWorld( world ),
pAttenuationRefDist( 1.0f ),
pAttenuationRolloff( 1.0f ),
pAttenuationDistanceOffset( 0.0f ),
pRTWorldBVH( NULL ),
pRTConfig( NULL ),
pReuseDistance( reuseDistance ),
pReuseDistanceSquared( reuseDistance * reuseDistance ),
pLastUsedCounter( 1 ),
pMaxProbeCount( maxProbeCount ){
}

deoalEnvProbeList::~deoalEnvProbeList(){
	InvalidateAllProbes();
	
	const int count = pProbes.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		delete ( deoalEnvProbe* )pProbes.GetAt( i );
	}
}



// Manegement
///////////////

void deoalEnvProbeList::SetRange( float range ){
	InvalidateAllProbes();
	
	pRange = range;
}

void deoalEnvProbeList::SetAttenuation( float refDist, float rolloff, float distanceOffset ){
	InvalidateAllProbes();
	
	pAttenuationRefDist = refDist;
	pAttenuationRolloff = rolloff;
	pAttenuationDistanceOffset = distanceOffset;
}

void deoalEnvProbeList::SetLayerMask( const decLayerMask &layerMask ){
	InvalidateAllProbes();
	
	pLayerMask = layerMask;
	pVisitorInvalidateEnvProbes.SetLayerMask( layerMask );
}

void deoalEnvProbeList::SetRTWorldBVH( deoalRTWorldBVH *bvh ){
	InvalidateAllProbes();
	
	pRTWorldBVH = bvh;
}

void deoalEnvProbeList::SetRTConfig( const deoalATRayTracing::sConfigSoundTracing *config ){
	InvalidateAllProbes();
	
	pRTConfig = config;
}



void deoalEnvProbeList::PrepareProcessAudio(){
	pLastUsedCounter++;
}

deoalEnvProbe *deoalEnvProbeList::GetProbeEstimateRoom( const decDVector &position ){
	// TODO check if probe exists in the octree and return it if found
	const int count = pProbes.GetCount();
	deoalEnvProbe *bestProbe = NULL;
	double bestDistanceSquared = 0.0;
	
	int i;
	for( i=0; i<count; i++ ){
		deoalEnvProbe &probe = *( ( deoalEnvProbe* )pProbes.GetAt( i ) );
		if( ! probe.GetOctreeNode() ){
			continue; // invalidated
		}
		const double distanceSquared = ( probe.GetPosition() - position ).LengthSquared();
		if( distanceSquared > pReuseDistanceSquared ){
			continue;
		}
		// NOTE for estimated probes any probe is fine, estimated or not
		
		if( bestProbe && distanceSquared >= bestDistanceSquared ){
			continue;
		}
		bestProbe = &probe;
		bestDistanceSquared = distanceSquared;
	}
	
	if( bestProbe ){
		return bestProbe;
	}
	
	const deoalRayTraceConfig &rtconfig = *pWorld.GetAudioThread().GetRayTracing().GetConfigTraceSoundRays();
	
	// reuse first invalid probe if present
	for( i=0; i<count; i++ ){
		deoalEnvProbe * const probe = ( deoalEnvProbe* )pProbes.GetAt( i );
		if( probe->GetOctreeNode() ){
			continue;
		}
		
		probe->SetPosition( position );
		probe->SetLastUsed( pLastUsedCounter );
		probe->EstimateRoomParameters( pWorld, rtconfig );
		pWorld.GetOctree()->InsertEnvProbeIntoTree( probe, 8 );
		return probe;
	}
	
	// if maximum number of probes is in use invalidate and use the oldest probe
	if( count == pMaxProbeCount ){
		deoalEnvProbe *oldestProbe = ( deoalEnvProbe* )pProbes.GetAt( 0 );
		unsigned int oldestAge = pLastUsedCounter - oldestProbe->GetLastUsed();
		
		for( i=1; i<count; i++ ){
			deoalEnvProbe * const probe = ( deoalEnvProbe* )pProbes.GetAt( i );
			unsigned int age = pLastUsedCounter - probe->GetLastUsed();
			if( age > oldestAge ){
				oldestProbe = probe;
				oldestAge = age;
			}
		}
		
		oldestProbe->GetOctreeNode()->RemoveEnvProbe( oldestProbe );
		oldestProbe->Invalidate();
		oldestProbe->SetPosition( position );
		oldestProbe->SetLastUsed( pLastUsedCounter );
		oldestProbe->EstimateRoomParameters( pWorld, rtconfig );
		pWorld.GetOctree()->InsertEnvProbeIntoTree( oldestProbe, 8 );
		return oldestProbe;
	}
	
	// collect probe data and insert probe into octree
	deoalEnvProbe *probe = NULL;
	
	try{
		probe = new deoalEnvProbe( pWorld.GetAudioThread() );
		probe->SetPosition( position );
		probe->SetRange( pRange );
		probe->SetLayerMask( pLayerMask );
		probe->SetRTConfig( pRTConfig );
		probe->SetLastUsed( pLastUsedCounter );
		probe->EstimateRoomParameters( pWorld, rtconfig );
		pProbes.Add( probe );
		
	}catch( const deException & ){
		if( probe ){
			delete probe;
		}
		throw;
	}
	
	try{
		pWorld.GetOctree()->InsertEnvProbeIntoTree( probe, 8 );
		
	}catch( const deException & ){
		pProbes.RemoveFrom( pProbes.GetCount() - 1 );
		delete probe;
		throw;
	}
	
	return probe;
}

deoalEnvProbe *deoalEnvProbeList::GetProbeTraceSoundRays( const decDVector &position ){
	// TODO check if probe exists in the octree and return it if found
	//deoalRayTraceConfig *config = ( deoalRayTraceConfig* )pProbeConfigs.GetAt( 0 );
	deoalAudioThread &audiothread = pWorld.GetAudioThread();
	const int count = pProbes.GetCount();
	deoalEnvProbe *bestProbe = NULL;
	double bestDistanceSquared = 0.0;
	
	int i;
	for( i=0; i<count; i++ ){
		deoalEnvProbe &probe = *( ( deoalEnvProbe* )pProbes.GetAt( i ) );
		if( ! probe.GetOctreeNode() ){
			continue; // invalidated
		}
		if( probe.GetEstimated() ){
			continue;
		}
		const double distanceSquared = ( probe.GetPosition() - position ).LengthSquared();
		if( distanceSquared > pReuseDistanceSquared ){
			continue;
		}
		
		if( bestProbe && distanceSquared >= bestDistanceSquared ){
			continue;
		}
		bestProbe = &probe;
		bestDistanceSquared = distanceSquared;
	}
	
	const deoalRayTraceConfig &rtconfig = *audiothread.GetRayTracing().GetConfigTraceSoundRays();
	
	if( bestProbe ){
		if( bestProbe->GetEstimated() ){
			// probe has been only estimated so trace sound rays. we can reuse this probe but
			// we need to re-add it to the octree because tracing sound rays changes extends
			bestProbe->GetOctreeNode()->RemoveEnvProbe( bestProbe );
			bestProbe->Invalidate();
			bestProbe->SetAttenuation( pAttenuationRefDist, pAttenuationRolloff, pAttenuationDistanceOffset );
			bestProbe->SetLayerMask( pLayerMask );
			bestProbe->SetRTConfig( pRTConfig );
			bestProbe->SetLastUsed( pLastUsedCounter );
			bestProbe->TraceSoundRays( pWorld, pRTWorldBVH, rtconfig );
			pWorld.GetOctree()->IndexOfEnvProbe( bestProbe );
		}
		
		return bestProbe;
	}
	
	// reuse first invalid probe if present
	for( i=0; i<count; i++ ){
		deoalEnvProbe * const probe = ( deoalEnvProbe* )pProbes.GetAt( i );
		if( probe->GetOctreeNode() ){
			continue;
		}
		
		probe->SetPosition( position );
		probe->SetLastUsed( pLastUsedCounter );
		probe->TraceSoundRays( pWorld, pRTWorldBVH, rtconfig );
		pWorld.GetOctree()->InsertEnvProbeIntoTree( probe, 8 );
		return probe;
	}
	
	// if maximum number of probes is in use invalidate and use the oldest probe
	if( count == pMaxProbeCount ){
		deoalEnvProbe *oldestProbe = ( deoalEnvProbe* )pProbes.GetAt( 0 );
		unsigned int oldestAge = pLastUsedCounter - oldestProbe->GetLastUsed();
		
		for( i=1; i<count; i++ ){
			deoalEnvProbe * const probe = ( deoalEnvProbe* )pProbes.GetAt( i );
			unsigned int age = pLastUsedCounter - probe->GetLastUsed();
			if( age > oldestAge ){
				oldestProbe = probe;
				oldestAge = age;
			}
		}
		
		oldestProbe->GetOctreeNode()->RemoveEnvProbe( oldestProbe );
		oldestProbe->Invalidate();
		oldestProbe->SetPosition( position );
		oldestProbe->SetLastUsed( pLastUsedCounter );
		oldestProbe->TraceSoundRays( pWorld, pRTWorldBVH, rtconfig );
		pWorld.GetOctree()->InsertEnvProbeIntoTree( oldestProbe, 8 );
		return oldestProbe;
	}
	
	// collect probe data and insert probe into octree
	deoalEnvProbe *probe = NULL;
	
	try{
		probe = new deoalEnvProbe( pWorld.GetAudioThread() );
		probe->SetPosition( position );
		probe->SetRange( pRange );
		probe->SetAttenuation( pAttenuationRefDist, pAttenuationRolloff, pAttenuationDistanceOffset );
		probe->SetLayerMask( pLayerMask );
		probe->SetRTConfig( pRTConfig );
		probe->SetLastUsed( pLastUsedCounter );
		probe->TraceSoundRays( pWorld, pRTWorldBVH, rtconfig );
		pProbes.Add( probe );
		
	}catch( const deException & ){
		if( probe ){
			delete probe;
		}
		throw;
	}
	
	try{
		pWorld.GetOctree()->InsertEnvProbeIntoTree( probe, 8 );
		
	}catch( const deException & ){
		pProbes.RemoveFrom( pProbes.GetCount() - 1 );
		delete probe;
		throw;
	}
	
	return probe;
}

void deoalEnvProbeList::InvalidateProbesInside( const decDVector &minExtend,
const decDVector &maxExtend ){
	pVisitorInvalidateEnvProbes.SetExtends( minExtend, maxExtend );
	pWorld.GetOctree()->VisitNodesColliding( &pVisitorInvalidateEnvProbes, minExtend, maxExtend );
}

void deoalEnvProbeList::InvalidateAllProbes(){
	const int count = pProbes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoalEnvProbe * const envProbe = ( deoalEnvProbe* )pProbes.GetAt( i );
		if( envProbe->GetOctreeNode() ){
			envProbe->GetOctreeNode()->RemoveEnvProbe( envProbe );
			envProbe->Invalidate();
		}
	}
}

void deoalEnvProbeList::PrepareQuickDispose(){
	const int count = pProbes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoalEnvProbe* )pProbes.GetAt( i ) )->SetOctreeNode( NULL );
	}
}



int deoalEnvProbeList::GetProbeCount() const{
	return pProbes.GetCount();
}

int deoalEnvProbeList::GetValidProbeCount() const{
	const int count = pProbes.GetCount();
	int i, validCount = 0;
	
	for( i=0; i<count; i++ ){
		if( ( ( deoalEnvProbe* )pProbes.GetAt( i ) )->GetOctreeNode() ){
			validCount++;
		}
	}
	
	return validCount;
}



// Private Functions
//////////////////////

#if 0
void deoalEnvProbeList::pCreateProbeConfig(){
	// store constant somewhere. as measurement an ico-sphere can be used in Blender.
	// at subdivision level 2 the sphere has 42 points. this is rather coarse grained
	// and samples a room mostly along the ceiling and floor with walls receiving some
	// hits only. at subdivision level 2 the sphere has 162 points. this is rather fine
	// grained already and hits walls also with a broader band of samples. 162 is the
	// better solution to get a better understanding of the first order hits around the
	// probe. 42 is though a lot cheaper but is more of an approximation of the room.
	// right now a ray count of 42 is used. an option can be added to allow this value
	// to be selected by the user.
	// 
	// time for 42 rays: 0.9ms -> 3.5ms
	// 
	// this results in an average time per ray-check: 21ys -> 83ys
	// 
	// this is for a small test world. in a large game world this will be higher
	pProbeConfig = new deoalRayTraceConfig;
// 	pProbeConfig->SetRaysEquallySpaced( 42 ); // 162, 92, 42
// 	pProbeConfig->SetIcoLevel2();
	pProbeConfig->SetIcoLevel3();
// 	pProbeConfig->SetIcoLevel4();
// 	pProbeConfig->SetIcoLevel5();
	
	/*
	const int subDivLevels = 2; // 162
// 	const int subDivLevels = 3; // 642
// 	const int subDivLevels = 4; // 2562
// 	const int subDivLevels = 5; // 10242
	deoalIcoSphere *icoSphere = new deoalIcoSphere;
	for( int i; i<subDivLevels; i++ ){
		deoalIcoSphere * const prevIcoSphere = icoSphere;
		icoSphere = new deoalIcoSphere( *prevIcoSphere );
		delete prevIcoSphere;
	}
	pProbeConfig->SetFromVertices( icoSphere->GetVertices(),
		icoSphere->GetVertexCount(), icoSphere->GetOpeningAngle() );
	*/
	
	/*
	for( int i=0; i<42; i++ ){
		const decDVector &p = pProbeConfig->GetRayDirections()[ i ];
		pWorld.GetAudioThread().GetLogger().LogInfoFormat( "ProbeConfig Ray %d: (%.3f,%.3f,%.3f) [ %.1f, %.1f ]",
			i, p.x, p.y, p.z, atan2( p.z, p.x ) / DEG2RAD,
			atan2( p.y, decVector2( p.x, p.z ).Length() ) / DEG2RAD );
	}
	*/
	
	/*
	try{
		const deoalIcoSphere ico1;
		const deoalIcoSphere ico2( ico1 );
		const deoalIcoSphere ico3( ico2 );
		if( ico3.GetVertexCount() != pProbeConfig->GetRayCount() ) DETHROW( deeInvalidParam );
		const decVector * const v1 = pProbeConfig->GetRayDirections();
		const decVector * const v2 = ico3.GetVertices();
		const int vc = ico3.GetVertexCount();
		int i, j;
		for( i=0; i<vc; i++ ){
			pWorld.GetAudioThread().GetLogger().LogInfoFormat("%d: %f,%f,%f", i, v2[i].x, v2[i].y, v2[i].z);
			const decVector vcc(-v2[i].x, -v2[i].z, v2[i].y);
			for( j=0; j<vc; j++ ){
				if( vcc.IsEqualTo( v1[j], 0.025f ) ){
					break;
				}
			}
			if( j == vc ) DETHROW( deeInvalidParam );
		}
	}catch(const deException &e ){
		pWorld.GetAudioThread().LogException(e);
	}
	*/
}
#endif
