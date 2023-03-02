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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglGI.h"
#include "deoglGICascade.h"
#include "deoglGIState.h"
#include "deoglGITraceRays.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../model/deoglRModel.h"
#include "../model/deoglModelLOD.h"
#include "../rendering/deoglRenderWorld.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../world/deoglRWorld.h"
#include "../utils/collision/deoglCollisionDetection.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionFrustum.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../shaders/paramblock/deoglSPBMapBufferRead.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIState
///////////////////////

// Constructor, destructor
////////////////////////////

// #define GISTATE_SPECIAL_DEBUG

deoglGIState::deoglGIState( deoglRenderThread &renderThread, const decVector &size ) :
pRenderThread( renderThread ),

pSize( size ),
pWorld( NULL ),

pProbeCount( 32, 8, 32 ),
pGridCoordClamp( pProbeCount - decPoint3( 1, 1, 1 ) ),
pStrideProbeCount( pProbeCount.x * pProbeCount.z ),
pRealProbeCount( pStrideProbeCount * pProbeCount.y ),

pIrradianceMapSize( 8 ),
pDistanceMapSize( 16 ),

pDepthSharpness( 50.0f ),
pHysteresis( 0.9f ), // 0.98 (paper)
pNormalBias( 0.25f ), // 0.25 or 0.2 in examples
pIrradianceGamma( 5.0f ),
pSelfShadowBias( 0.35f ), // higher when variance is higher (lower ray count)
	// paper 0.3*0.75. sdk examples 0.4, 0.1 or even 4.0 (pre-scaled by spacing).
	// examples unscaled: 0.1, 0.32, 0.33, 0.4 . 0.35 seems a good middle ground

pSizeTexIrradiance( pIrradianceMapSize ),
pSizeTexDistance( pDistanceMapSize ),

pIrradianceMapScale( 1.0f / ( ( pSizeTexIrradiance + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexIrradiance + 2 ) * pProbeCount.z + 2 ) ),
pDistanceMapScale( 1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.z + 2 ) ),

pCascades( NULL ),
pCascadeCount( 0 ),
pActiveCascade( 0 ),

pCascaceUpdateCycle( NULL ),
pCascaceUpdateCycleCount( 0 ),
pCascaceUpdateCycleIndex( 0 ),
pCameraForceToneMapAdaptionCount( 0 ),

pTexProbeIrradiance( renderThread ),
pTexProbeDistance( renderThread ),
pTexProbeOffset( renderThread ),
pClearMaps( true ),
pProbesHaveMoved( false ),

pProbesExtendsChanged( false ),

pInstances( *this ),
pRayCache( renderThread, 64, pRealProbeCount, 4 ),
pBVHStatic( renderThread ),
pBVHDynamic( renderThread )
{
	try{
		pInitCascades();
		pInitCascadeUpdateCycle();
		pInitUBOClearProbes();
		
		pCameraForceToneMapAdaptionCount = pCascaceUpdateCycleCount;
		
		#ifdef GI_USE_RAY_CACHE
		pPrepareProbeVBO();
		#endif
		
		pAreaTracker.SetHalfExtends( pCascades[ pCascadeCount - 1 ]->GetDetectionBox() );
		
		// update threshold defines the distance in meters before tracking is updated.
		// the GI State position is used to update the tracking position. this position
		// is updated step-wise matching the smallest cascade which is usually around
		// 1m spacing.
		// 
		// there are different possibilities how the threshold can be set.
		// 
		// one is to use the largest cascade probe spacing. for a 500m view distance
		// camera this is a GI State size of 1km. with 32 probes this a threshold of 32m.
		// with this threshold the cost for updating thet tracking stays in acceptable
		// boundaries of 1-2ms but the cost for updating the instances tracking can go
		// up to 10ms.
		// 
		// using a threshold of 10m instead keeps the cost for updating the instances
		// tracking below 1.5ms most of the time.
		// 
		// in general costs for finding content using area tracking is around 1-2ms
		// with most thresholds. hence each update run does cost 1ms for finding the
		// content. but the cost for updating the instances tracking increases quickly.
		// for this reason 8m is used to get a middle ground between updating not too
		// often (with a running player this is around 2s between updates) and keeping
		// the costs for updating instances tracking reasonable
		// 
		// all these measurements have been done for a city scene with roughly 12k
		// objects inside the tracking area and a camera view distance of 250m
		// (hence GI tracking area size of 500m)
		pAreaTracker.SetUpdateThreshold( 8.0 );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIState::~deoglGIState(){
	pCleanUp();
}



// Management
///////////////

void deoglGIState::SetWorld( deoglRWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pWorld = world;
	pAreaTracker.SetWorld( world );
	Invalidate();
	
	// in addition set all cascades to required full update of inside view probes
	int i;
	for( i=0; i<pCascadeCount; i++ ){
		pCascades[ i ]->SetRequiresFullUpdateInsideView( true );
	}
}

void deoglGIState::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	pAreaTracker.SetLayerMask( layerMask );
	Invalidate();
}

deoglGICascade &deoglGIState::GetCascadeAt( int index ) const{
	if( index < 0 || index >= pCascadeCount ){
		DETHROW( deeInvalidParam );
	}
	return *pCascades[ index ];
}

deoglGICascade &deoglGIState::GetActiveCascade() const{
	return *pCascades[ pActiveCascade ];
}

deoglGICascade & deoglGIState::GetLastCascade() const{
	return *pCascades[ pLastFrameCascade ];
}

deoglGICascade & deoglGIState::GetSkyShadowCascade() const{
	return *pCascades[ GetActiveCascade().GetSkyShadowCascade() ];
}

bool deoglGIState::CameraForceToneMapAdaption() const{
	return pCameraForceToneMapAdaptionCount > 0;
}



void deoglGIState::PrepareUBOClearProbes() const{
	deoglGICascade &cascade = GetActiveCascade();
	cascade.UpdateUBOParameters( pRenderThread.GetGI().NextUBOParameter(), 0, pBVHDynamic );
	cascade.PrepareUBOClearProbes( GetUBOClearProbes() );
}



// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) pRenderThread.GetLogger().LogInfoFormat("GIState." w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglGIState::ActivateNextCascade(){
	// store cascade as last frame cascade
	pLastFrameCascade = pActiveCascade;
	
	// update first all cascades requiring full update of all probes inside view. do this
	// starting at the largest cascade going down to the smallest to ensure valid lighting
	// results to be present as quickly as possible
	for( pActiveCascade=pCascadeCount-1; pActiveCascade>=0; pActiveCascade-- ){
		if( pCascades[ pActiveCascade ]->GetRequiresFullUpdateInsideView() ){
// 			pRenderThread.GetLogger().LogInfoFormat( "GIState: next cascade %d (requires full update inside view)", pActiveCascade );
			return;
		}
	}
	
	// update second all cascaded with invalid probes inside view. do this starting at the
	// largest cascade going down to the smallest to ensure valid lighting results to be
	// present as quickly as possible
	for( pActiveCascade=pCascadeCount-1; pActiveCascade>=0; pActiveCascade-- ){
		if( pCascades[ pActiveCascade ]->HasInvalidProbesInsideView() ){
// 			pRenderThread.GetLogger().LogInfoFormat( "GIState: next cascade %d (has invalid probes inside view)", pActiveCascade );
			return;
		}
	}
	
	// regular update of cascades using cascade cycle
	pActiveCascade = pCascaceUpdateCycle[ pCascaceUpdateCycleIndex++ ];
	if( pCascaceUpdateCycleIndex >= pCascaceUpdateCycleCount ){
		pCascaceUpdateCycleIndex = 0;
	}
// 	pRenderThread.GetLogger().LogInfoFormat( "GIState: next cascade %d (cycle)", pActiveCascade );
	
	// while doing the first loop of cascade updates force tone mapping adaption on camera
	// to avoid bad starting values due t non-lit geometry
	if( pCameraForceToneMapAdaptionCount > 0 ){
		pCameraForceToneMapAdaptionCount--;
	}
}

void deoglGIState::Update( const decDVector &cameraPosition, const deoglDCollisionFrustum &frustum ){
// 		pRenderThread.GetLogger().LogInfoFormat( "Update GIState %p (%g,%g,%g)",
// 			this, cameraPosition.x, cameraPosition.y, cameraPosition.z );
	
	// updates from last frame
	pUpdateProbeOffsetFromShader( GetLastCascade() );
	pUpdateProbeExtendsFromShader( GetLastCascade() );
	
	INIT_SPECIAL_TIMING
	// monitor configuration changes
	pRenderThread.GetGI().GetTraceRays().UpdateFromConfig();
	
	// update position
	deoglGICascade &cascade = GetActiveCascade();
	cascade.UpdatePosition( cameraPosition );
	SPECIAL_TIMER_PRINT("Update: UpdatePosition")
	
	pFindContent( cameraPosition );
	pBVHDynamic.SetPosition( pAreaTracker.GetPosition() );
	pBVHStatic.SetPosition( pAreaTracker.GetPosition() );
	SPECIAL_TIMER_PRINT("Update: FindContent")
// 		if(pAreaTracker.HasChanged()){
// 			pRenderThread.GetLogger().LogInfoFormat("GIState.FindContent: enter=%d leave=%d allLeaving=%d",
// 				pAreaTracker.GetEntering().GetComponentCount(), pAreaTracker.GetLeaving().GetComponentCount(), pAreaTracker.GetAllLeaving());
// 		}
	
	// track changes in static instances has to be done first
	pTrackInstanceChanges();
	SPECIAL_TIMER_PRINT("Update: TrackInstanceChanges")
	
	// prepare probes for tracing
	pPrepareTraceProbes( cascade, frustum );
	SPECIAL_TIMER_PRINT("Update: PrepareTraceProbes")
	
	#ifdef GI_USE_RAY_CACHE
	pPrepareRayCacheProbes( cascade );
	#endif
	SPECIAL_TIMER_PRINT("Update: PrepareRayCacheProbes")
}

void deoglGIState::PrepareUBOState() const{
	deoglGICascade &cascade = GetActiveCascade();
	const int count = cascade.GetUpdateProbeCount();
	cascade.UpdateUBOParameters( pRenderThread.GetGI().NextUBOParameter(), count, pBVHDynamic );
	pPrepareUBORayDirections();
	
	if( count > 0 ){
		cascade.UpdateUBOProbeIndices( pRenderThread.GetGI().NextUBOProbeIndex() );
		cascade.UpdateUBOProbePosition( pRenderThread.GetGI().NextUBOProbePosition() );
	}
}

void deoglGIState::PrepareUBOStateRayCache() const{
	deoglGICascade &cascade = GetActiveCascade();
	const int count = cascade.GetRayCacheProbeCount();
	cascade.UpdateUBOParameters( pRenderThread.GetGI().NextUBOParameter(), count, pBVHStatic );
	pPrepareUBORayDirections();
	
	if( count > 0 ){
		cascade.UpdateUBOProbeIndicesRayCache( pRenderThread.GetGI().NextUBOProbeIndex() );
		cascade.UpdateUBOProbePositionRayCache( pRenderThread.GetGI().NextUBOProbePosition() );
	}
}

void deoglGIState::Invalidate(){
	pInstances.Clear();
	
	int i;
	for( i=0; i<pCascadeCount; i++ ){
		pCascades[ i ]->Invalidate();
	}
	
	pClearMaps = true;
	pProbesHaveMoved = false;
	pProbesExtendsChanged = false;
}

void deoglGIState::ProbesMoved(){
	pProbesHaveMoved = true;
}

void deoglGIState::InvalidateArea( const decDVector &minExtend, const decDVector &maxExtend, bool hard ){
// 		pRenderThread.GetLogger().LogInfoFormat("InvalidateArea %s (%g,%g,%g) (%g,%g,%g)", hard ? "hard" : "soft",
// 			minExtend.x, minExtend.y, minExtend.z, maxExtend.x, maxExtend.y, maxExtend.z);
	if( ! ( maxExtend > minExtend ) ){
		return;
	}
	
	int i;
	for( i=0; i<pCascadeCount; i++ ){
		pCascades[ i ]->InvalidateArea( minExtend, maxExtend, hard );
	}
	
	pBVHStatic.MarkDirty();
}

void deoglGIState::TouchDynamicArea( const decDVector &minExtend, const decDVector &maxExtend ){
// 		pRenderThread.GetLogger().LogInfoFormat("TouchDynamicArea (%g,%g,%g) (%g,%g,%g)",
// 			minExtend.x, minExtend.y, minExtend.z, maxExtend.x, maxExtend.y, maxExtend.z);
	if( ! ( maxExtend > minExtend ) ){
		return;
	}
	
	int i;
	for( i=0; i<pCascadeCount; i++ ){
		pCascades[ i ]->TouchDynamicArea( minExtend, maxExtend );
	}
	
	pBVHDynamic.MarkDirty();
}

void deoglGIState::ValidatedRayCaches(){
	#ifdef GISTATE_SPECIAL_DEBUG
	return;
	#endif
	
	GetActiveCascade().ValidatedRayCaches();
	pProbesExtendsChanged = true;
}

void deoglGIState::ComponentEnteredWorld( deoglRComponent *component ){
	if( ! pAreaTracker.ComponentTouches( *component ) || pAreaTracker.RejectComponent( *component ) ){
		return;
	}
	
	// this is unfortunately required and not cheap. but this happens rarely
	if( pInstances.GetInstanceWithComponent( component ) ){
		return;
	}
	
	// components entering game world can be anywhere and thus need to invalidate probes
// 		const decDVector p( component->GetMatrix().GetPosition() );
// 		pRenderThread.GetLogger().LogInfoFormat( "ComponentEnteredWorld: (%g,%g,%g) %s", p.x, p.y, p.z,
// 			component->GetModel()->GetFilename().GetString());
	pInstances.AddComponent( component, true );
}

void deoglGIState::ComponentChangedLayerMask( deoglRComponent *component ){
	if( ! pAreaTracker.ComponentTouches( *component ) || pAreaTracker.RejectComponent( *component ) ){
		return;
	}
	
	// this is unfortunately required and not cheap. but this happens rarely
	if( pInstances.GetInstanceWithComponent( component ) ){
		return;
	}
	
	// components changing layer mask can be anywhere and thus need to invalidate probes
// 		const decDVector p( component->GetMatrix().GetPosition() );
// 		pRenderThread.GetLogger().LogInfoFormat( "ComponentChangedLayerMask: (%g,%g,%g) %s", p.x, p.y, p.z,
// 			component->GetModel()->GetFilename().GetString());
	pInstances.AddComponent( component, true );
}

void deoglGIState::ComponentBecameVisible( deoglRComponent *component ){
	ComponentEnteredWorld( component );
}

void deoglGIState::StartReadBack(){
	const deoglGICascade &cascade = GetActiveCascade();
	INIT_SPECIAL_TIMING
	
	if( pProbesHaveMoved && cascade.GetUpdateProbeCount() > 0 ){
		pPBProbeOffsets->GPUReadToCPU( cascade.GetUpdateProbeCount() );
		SPECIAL_TIMER_PRINT("StartReadBack: > ProbeOffsets.TransferFrom")
	}
	
	if( pProbesExtendsChanged && cascade.GetRayCacheProbeCount() > 0 ){
		pPBProbeExtends->GPUReadToCPU( cascade.GetRayCacheProbeCount() );
		SPECIAL_TIMER_PRINT("StartReadBack: > ProbeExtends.TransferFrom")
	}
}



// Private Functions
//////////////////////

void deoglGIState::pCleanUp(){
	if( pCascades ){
		int i;
		for( i=0; i<pCascadeCount; i++ ){
			delete pCascades[ i ];
		}
		delete [] pCascades;
	}
	if( pCascaceUpdateCycle ){
		delete [] pCascaceUpdateCycle;
	}
}

void deoglGIState::pInitCascades(){
	pCascades = new deoglGICascade*[ 4 ];
	
	const float scaleFactor2nd = 2.0f;
	const float offsetFactor2nd = 0.0f; // 0.25f; <= when using 5th cascade offset by 0.5
	const float scaleFactor3rd = 1.0f / 3.0f;
	
	decVector largestSpacing( pSize.x / ( float )pGridCoordClamp.x,
		pSize.y / ( float )pGridCoordClamp.y, pSize.z / ( float )pGridCoordClamp.z );
	
	decVector smallestSpacing( decVector( 1.0f, 1.0f, 1.0f ).Smallest( largestSpacing / 8.0f ) );
	
	pCascades[ 0 ] = new deoglGICascade( *this, 0, smallestSpacing, decVector() );
	pCascades[ 0 ]->SetFillUpUpdatesWithExpensiveProbes( true );
	pCascadeCount = 1;
	
	pCascades[ 1 ] = new deoglGICascade( *this, 1, smallestSpacing * scaleFactor2nd,
		smallestSpacing * offsetFactor2nd );
	pCascades[ 1 ]->SetFillUpUpdatesWithExpensiveProbes( true );
	pCascadeCount = 2;
	
	pCascades[ 2 ] = new deoglGICascade( *this, 2, ( smallestSpacing * scaleFactor2nd ).
		Mix( largestSpacing, scaleFactor3rd ), decVector() );
	pCascades[ 2 ]->SetFillUpUpdatesWithExpensiveProbes( true );
	pCascadeCount = 3;
	
	pCascades[ 3 ] = new deoglGICascade( *this, 3, largestSpacing, decVector() );
	pCascades[ 3 ]->SetFillUpUpdatesWithExpensiveProbes( true );
	pCascadeCount = 4;
	
	// what cascade to use depends on how GI shadow maps are handled.
	// 
	// 
	// version 1
	// ---------
	// use the highest cascade for the entire GI state.
	// 
	// using this method reduces static shadow map updates to the minimum. the big problem
	// though is the resolution.
	// 
	// for the default camera view distance of 250m and a shadow map size of 1024 the
	// resolution is roughly 0.7m per pixel. this causes light to leak in causing the GI
	// result to be falsified. with higher shadow map size this is improved but with
	// larger camera view distance the problem comes back.
	// 
	// 
	// version 2
	// ---------
	// use one shadow map for two cascades.
	// 
	// the first and second cascade are scaled by factor 2 so the second cascade can be
	// used as shadow cascade for the first. the same is true for the other two cascaded.
	// 
	// using 1m probe spacing and 50m detection range the maximum AABB size of the second
	// cascade is 161 (= (31/2)*2 + 50). this is constant no matter how large the camera
	// view distance is. in the worst sky light orientation the shadow map area is of
	// size 322. in typical situations this is 240. using the worst situation and a shadow
	// map size of 1024 this results in a worst possible resolution of 0.31m or 0.23m for
	// the less worse case. for a typical outer house wall size of 0.4m this is good enough.
	// 
	// by increasing the shadow map size the resolution can be improved to 0.16m (for 2048)
	// and 0.08m (for 4096) for the worst case and 0.12m respectively 0.06m for the less
	// worse situation.
	// 
	// for the two other cascades the resolution depends on the camera view distance and is
	// as bad as in the first solution. but this is less of a problem since the larger
	// cascades are filler cascades for large distance where errors are less problematic
// 	pCascades[ 0 ]->SetSkyShadowCascade( 3 );
// 	pCascades[ 1 ]->SetSkyShadowCascade( 3 );
// 	pCascades[ 2 ]->SetSkyShadowCascade( 3 );
// 	pCascades[ 3 ]->SetSkyShadowCascade( 3 );
	
	pCascades[ 0 ]->SetSkyShadowCascade( 1 );
	pCascades[ 2 ]->SetSkyShadowCascade( 3 );
	
	// debug
	deoglRTLogger &logger = pRenderThread.GetLogger();
	logger.LogInfo( "GI Cascades:" );
	int i;
	for( i=0; i<pCascadeCount; i++ ){
		const decVector &size = pCascades[ i ]->GetFieldSize();
		const decVector &spacing = pCascades[ i ]->GetProbeSpacing();
		const decVector &detbox = pCascades[ i ]->GetDetectionBox();
		logger.LogInfoFormat( "- %d: size=(%g,%g,%g) spacing=(%g,%g,%g) detbox=(%g,%g,%g)", i,
			size.x, size.y, size.z, spacing.x, spacing.y, spacing.z, detbox.x, detbox.y, detbox.z );
	}
}

void deoglGIState::pInitCascadeUpdateCycle(){
	// normal processing of cascades. update the cascades using this pattern: 0, 1, 2, 0, 1, 3.
	// this pattern ensures the smallest two cascades are updated more frequently than the
	// largest two cascades.
	// 
	// an alternative pattern would be: 0, 1, 0, 2, 0, 1, 0, 3. this pattern updates the
	// cascades faster the smaller it is.
	// 
	// another possible pattern is: 0, 1, 2, 3. this pattern updates all cascades equally fast
	// 
	// another possible pattern is: 0, 1, 0, 2, 0, 3. this pattern updates the smallest cascade
	// fast and the other patterns equally slower. this ensures all cascades but the smallest
	// update on equal speed with the smallest cascade updating faster. this puts emphasis on
	// the area around the camera while keeping larger cascades at equal pacing
	
	pCascaceUpdateCycle = new int[ 6 ];
	pCascaceUpdateCycle[ 0 ] = 0;
	pCascaceUpdateCycle[ 1 ] = 1;
	pCascaceUpdateCycle[ 2 ] = 2;
	pCascaceUpdateCycle[ 3 ] = 0;
	pCascaceUpdateCycle[ 4 ] = 1;
	pCascaceUpdateCycle[ 5 ] = 3;
	pCascaceUpdateCycleCount = 6;
	
	// debug
// 	pCascaceUpdateCycle[ 0 ] = 0;
// 	pCascaceUpdateCycleCount = 1;
	#ifdef GISTATE_SPECIAL_DEBUG
	pCascaceUpdateCycle[ 0 ] = 3;
	pCascaceUpdateCycleCount = 1;
	#endif
}

void deoglGIState::pInitUBOClearProbes(){
	pUBOClearProbes.TakeOver( new deoglSPBlockUBO( pRenderThread ) );
	deoglSPBlockUBO &ubo = pUBOClearProbes;
	
	ubo.SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo.SetParameterCount( 1 );
	ubo.GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, ( pRealProbeCount / 32 ) / 4 ); // uvec4
	ubo.MapToStd140();
	ubo.SetBindingPoint( 0 );
}

void deoglGIState::pInvalidateAllRayCaches(){
	int i;
	for( i=0; i<pCascadeCount; i++ ){
		pCascades[ i ]->InvalidateAllRayCaches();
	}
}

void deoglGIState::pFindContent( const decDVector &position ){
	pAreaTracker.SetPosition( position );
	pAreaTracker.Update();
}

void deoglGIState::pTrackInstanceChanges(){
	/*{
		const int count = pCollideListFiltered.GetComponentCount();
		int i;
		for( i=0; i<count; i++ ){
			if( pCollideListFiltered.GetComponentAt(i)->GetComponent() ){
				if( pCollideListFiltered.GetComponentAt(i)->GetComponent()->GetModel() ){
					const decDVector p( pCollideListFiltered.GetComponentAt(i)->GetComponent()->GetMatrix().GetPosition() );
					pRenderThread.GetLogger().LogInfoFormat( "FILTERED: %d (%g,%g,%g) %s", i, p.x, p.y, p.z,
						pCollideListFiltered.GetComponentAt(i)->GetComponent()->GetModel()->GetFilename().GetString());
				}
			}
		}
	}*/
	
	if( pAreaTracker.GetAllLeaving() ){
		pInstances.Clear();
		
	}else{
		pInstances.ApplyChanges();
	}
	
	pInstances.RemoveComponents( pAreaTracker.GetLeaving() );
	
	// entering contains only components entering the GI area due to the GI area moving.
	// for these components we do not need to invalidate the probes since all rotated
	// out probes have been already invalidated. if all leaving flag is set then all
	// probes are invalidated too so we also do not need to invalidate here
	pInstances.AddComponents( pAreaTracker.GetEntering(), false );
	
	pAreaTracker.ClearChanges();
	
// 	pRenderThread.GetLogger().LogInfo( "pTrackInstanceChanges" );
// 	pInstances.DebugPrint();
}

void deoglGIState::pUpdateProbeOffsetFromShader( deoglGICascade &cascade ){
	if( ! pProbesHaveMoved || cascade.GetUpdateProbeCount() == 0 ){
		return;
	}
	
	INIT_SPECIAL_TIMING
	pProbesHaveMoved = false;
	
	const deoglSPBMapBufferRead mapped( pPBProbeOffsets, 0, cascade.GetUpdateProbeCount() );
	SPECIAL_TIMER_PRINT("UpdateProbeOffsetFromShader: > MapPBO")
	
	cascade.UpdateProbeOffsetFromShader( pPBProbeOffsets->GetMappedBuffer() );
	SPECIAL_TIMER_PRINT("UpdateProbeOffsetFromShader: > UpdateCascade")
}

void deoglGIState::pUpdateProbeExtendsFromShader( deoglGICascade &cascade ){
	if( ! pProbesExtendsChanged || cascade.GetRayCacheProbeCount() == 0 ){
		return;
	}
	
	INIT_SPECIAL_TIMING
	pProbesExtendsChanged = false;
	
	const deoglSPBMapBufferRead mapped( pPBProbeExtends, 0, cascade.GetRayCacheProbeCount() );
	SPECIAL_TIMER_PRINT("UpdateProbeExtendsFromShader: > MapPBO")
	
	cascade.UpdateProbeExtendsFromShader( pPBProbeExtends->GetMappedBuffer() );
	SPECIAL_TIMER_PRINT("UpdateProbeExtendsFromShader: > UpdateCascade")
}

void deoglGIState::pPrepareTraceProbes( deoglGICascade &cascade, const deoglDCollisionFrustum &frustum ){
	INIT_SPECIAL_TIMING
	pFindProbesToUpdate( cascade, frustum );
	SPECIAL_TIMER_PRINT("Update: > FindProbesToUpdate")
	pPrepareProbeTexturesAndFBO();
	SPECIAL_TIMER_PRINT("Update: > PrepareProbeTexturesAndFBO")
}

void deoglGIState::pFindProbesToUpdate( deoglGICascade &cascade, const deoglDCollisionFrustum &frustum ){
	cascade.FindProbesToUpdate( frustum );
	
	const deoglGITraceRays &traceRays = pRenderThread.GetGI().GetTraceRays();
	pSampleImageSize.x = traceRays.GetProbesPerLine() * traceRays.GetRaysPerProbe();
	pSampleImageSize.y = ( cascade.GetUpdateProbeCount() - 1 ) / traceRays.GetProbesPerLine() + 1;
// 		pRenderThread.GetLogger().LogInfoFormat( "GIFindProbesUpdate cheap=%d expensive=%d", specialCountCheap, specialCountExpensive );
}

void deoglGIState::pPrepareRayCacheProbes( deoglGICascade &cascade ){
	const int raysPerProbe = pRenderThread.GetGI().GetTraceRays().GetRaysPerProbe();
	if( raysPerProbe != pRayCache.GetRaysPerProbe() ){
		pInvalidateAllRayCaches();
		pRayCache.SetRaysPerProbe( raysPerProbe );
	}
	
	cascade.PrepareRayCacheProbes();
// 	pRenderThread.GetLogger().LogInfoFormat("PrepareRayCacheProbes: %d", pRayCacheProbeCount);
}

void deoglGIState::pPrepareProbeTexturesAndFBO(){
	if( pTexProbeIrradiance.GetTexture() && pTexProbeDistance.GetTexture()
	&& pTexProbeOffset.GetTexture() && ! pClearMaps ){
		return;
	}
	
	if( ! pTexProbeIrradiance.GetTexture() ){
		pTexProbeIrradiance.SetFBOFormat( 4, true ); // image load/store supports only 1, 2 and 4 not 3
		pTexProbeIrradiance.SetSize( ( pSizeTexIrradiance + 2 ) * pProbeCount.x * pProbeCount.y + 2,
			( pSizeTexIrradiance + 2 ) * pProbeCount.z + 2, pCascadeCount );
		pTexProbeIrradiance.CreateTexture();
	}
	
	if( ! pTexProbeDistance.GetTexture() ){
		pTexProbeDistance.SetFBOFormat( 2, true );
		pTexProbeDistance.SetSize( ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2,
			( pSizeTexDistance + 2 ) * pProbeCount.z + 2, pCascadeCount );
		pTexProbeDistance.CreateTexture();
	}
	
	if( ! pTexProbeOffset.GetTexture() ){
		pTexProbeOffset.SetFBOFormat( 4, true );
		pTexProbeOffset.SetSize( pProbeCount.x * pProbeCount.y, pProbeCount.z, pCascadeCount );
		pTexProbeOffset.CreateTexture();
	}
	
	if( pClearMaps ){
		deoglPixelBuffer::Ref pixbuf;
		pClearMaps = false;
		
		pixbuf.TakeOver( new deoglPixelBuffer( deoglPixelBuffer::epfFloat4,
			pTexProbeIrradiance.GetWidth(), pTexProbeIrradiance.GetHeight(),
			pTexProbeIrradiance.GetLayerCount() ) );
		pixbuf->SetToFloatColor( 0.0f, 0.0f, 0.0f, 0.0f );
		pTexProbeIrradiance.SetPixels( pixbuf );
		
		pixbuf.TakeOver( new deoglPixelBuffer( deoglPixelBuffer::epfFloat2,
			pTexProbeDistance.GetWidth(), pTexProbeDistance.GetHeight(),
			pTexProbeDistance.GetLayerCount() ) );
		const GLfloat maxProbeDistance = pCascades[ pCascadeCount - 1]->GetMaxProbeDistance();
		pixbuf->SetToFloatColor( maxProbeDistance, maxProbeDistance,
			maxProbeDistance, maxProbeDistance * maxProbeDistance );
		pTexProbeDistance.SetPixels( pixbuf );
		
		pixbuf.TakeOver( new deoglPixelBuffer( deoglPixelBuffer::epfFloat4,
			pTexProbeOffset.GetWidth(), pTexProbeOffset.GetHeight(),
			pTexProbeOffset.GetLayerCount() ) );
		pixbuf->SetToFloatColor( 0.0f, 0.0f, 0.0f, 0.0f );
		pTexProbeOffset.SetPixels( pixbuf );
	}
}

void deoglGIState::pPrepareProbeVBO(){
	const bool rowMajor = pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	// parameter block probe dynamic states
	pPBProbeDynamicStates.TakeOver( new deoglSPBlockSSBO( pRenderThread, deoglSPBlockSSBO::etGpu ) );
	pPBProbeDynamicStates->SetRowMajor( rowMajor );
	pPBProbeDynamicStates->SetParameterCount( 1 );
	pPBProbeDynamicStates->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint state
	pPBProbeDynamicStates->SetElementCount( GI_MAX_PROBE_COUNT );
	pPBProbeDynamicStates->MapToStd140();
	pPBProbeDynamicStates->EnsureBuffer();
	
	// parameter block probe offset
	pPBProbeOffsets.TakeOver( new deoglSPBlockSSBO( pRenderThread, deoglSPBlockSSBO::etRead ) );
	pPBProbeOffsets->SetRowMajor( rowMajor );
	pPBProbeOffsets->SetParameterCount( 2 );
	pPBProbeOffsets->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 offset
	pPBProbeOffsets->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // uint flags
	pPBProbeOffsets->SetElementCount( GI_MAX_PROBE_COUNT );
	pPBProbeOffsets->MapToStd140();
	pPBProbeOffsets->EnsureBuffer();
	
	// parameter block probe extends
	pPBProbeExtends.TakeOver( new deoglSPBlockSSBO( pRenderThread, deoglSPBlockSSBO::etRead ) );
	pPBProbeExtends->SetRowMajor( rowMajor );
	pPBProbeExtends->SetParameterCount( 2 );
	pPBProbeExtends->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 minExtend
	pPBProbeExtends->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 maxExtend
	pPBProbeExtends->SetElementCount( GI_MAX_PROBE_COUNT );
	pPBProbeExtends->MapToStd140();
	pPBProbeExtends->EnsureBuffer();
}

void deoglGIState::pPrepareUBORayDirections() const{
	const deoglGI &gi = pRenderThread.GetGI();
	const deoglGITraceRays &traceRays = gi.GetTraceRays();
	const int raysPerProbe = traceRays.GetRaysPerProbe();
	
	deoglSPBlockUBO &ubo = pRenderThread.GetGI().NextUBORayDirection();
	const deoglSPBMapBuffer mapped( ubo );
	
// #define GI_USE_RANDOM_DIRECTION 1
	#ifdef  GI_USE_RANDOM_DIRECTION
	const decMatrix randomOrientation( decMatrix::CreateRotation( decMath::random( -PI, PI ),
		decMath::random( -PI, PI ), decMath::random( -PI, PI ) ) );
	#endif
	
	const float sf_PHI = sqrtf( 5.0f ) * 0.5f + 0.5f;
	const float sf_n = ( float )raysPerProbe;
	#define madfrac(A, B) ((A)*(B)-floorf((A)*(B)))
	
	int i;
	for( i=0; i<raysPerProbe; i++ ){
		const float sf_i = ( float )i;
		const float phi = TWO_PI * madfrac( sf_i, sf_PHI - 1.0f );
		const float cosTheta = 1.0f - ( 2.0f * sf_i + 1.0f ) * ( 1.0f / sf_n );
		const float sinTheta = sqrtf( decMath::clamp( 1.0f - cosTheta * cosTheta, 0.0f, 1.0f ) );
		const decVector sf( cosf( phi ) * sinTheta, sinf( phi ) * sinTheta, cosTheta );
		
		// the paper uses random rotation matrix. this results though in huge flickering
		// even if smoothed using hystersis which is close to epiletic attack. disabling
		// the random rotation keeps the result stable. it is most probably not as smooth
		// as it could be with random rotation but avoids the unsupportable flickering
		//ubo.SetParameterDataArrayVec3( 0, i, randomOrientation * sf );
		#ifdef GI_USE_RANDOM_DIRECTION
		ubo.SetParameterDataArrayVec3( 0, i, randomOrientation * sf );
		#else
		ubo.SetParameterDataArrayVec3( 0, i, sf );
		#endif
	}
	
	#undef madfrac
	
	// DEBUG
	/*{
		for( i=0; i<pRaysPerProbe; i++ ){
			float pc = TWO_PI * i / pRaysPerProbe;
			decVector dir( sinf( pc ), 0.0f, cos( pc ) );
			ubo.SetParameterDataArrayVec3( eutpRayDirection, i, dir );
		}
		for( i=0; i<pUpdateProbeCount; i++ ){
			ubo.SetParameterDataArrayVec3( eutpProbePosition, i, (i%4)*3.0/4.0-1.5, 1, 0 );
		}
	}*/
	// DEBUG
}
