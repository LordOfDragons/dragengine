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
#include "../model/deoglRModel.h"
#include "../model/deoglModelLOD.h"
#include "../rendering/deoglRenderOcclusion.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../world/deoglRWorld.h"
#include "../utils/collision/deoglCollisionDetection.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionFrustum.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIState
///////////////////////

// Constructor, destructor
////////////////////////////

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

pTexProbeIrradiance( renderThread ),
pTexProbeDistance( renderThread ),
pTexProbeOffset( renderThread ),
pTexProbeState( renderThread ),
pFBOProbeIrradiance( renderThread, false ),
pFBOProbeDistance( renderThread, false ),
pFBOProbeOffset( renderThread, false ),
pFBOProbeState( renderThread, false ),
pClearMaps( true ),
pVBOProbeOffsets( 0 ),
pVBOProbeOffsetsTransition( 0 ),
pVBOProbeOffsetsData( NULL ),
pProbesHaveMoved( false ),

pVBOProbeExtends( 0 ),
pVBOProbeExtendsData( NULL ),
pProbesExtendsChanged( false ),

pInstances( *this ),
pRayCache( renderThread, 64, pRealProbeCount, 4 )
{
	try{
		pInitCascades();
		pInitCascadeUpdateCycle();
		pInitUBOClearProbes();
		
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

deoglGICascade & deoglGIState::GetSkyShadowCascade() const{
	// what cascade to use depends on how GI shadow maps are handled. right now one shadow
	// map is used for the entire GI state. for this reason the highest cascade position
	// is used for tracking static changes.
	// 
	// another solution is to use two shadow maps. for the 3rd and 4th cascade use the 4th
	// cascade. for the 1st and 2nd cascade use the 2nd cascade. this reduces the shadow
	// maps to two but still allows to use the better matching one for each cascade
	//return *pCascades[ pActiveCascade + ( 1 - ( pActiveCascade % 2 ) ) ];
	return *pCascades[ pCascadeCount - 1 ];
}



void deoglGIState::PrepareUBOClearProbes() const{
	deoglGICascade &cascade = GetActiveCascade();
	cascade.UpdateUBOParameters( pRenderThread.GetGI().GetUBOParameter(), 0 );
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

void deoglGIState::Update( const decDVector &cameraPosition, const deoglDCollisionFrustum &frustum ){
// 		pRenderThread.GetLogger().LogInfoFormat( "Update GIState %p (%g,%g,%g)",
// 			this, cameraPosition.x, cameraPosition.y, cameraPosition.z );
	
	// updates from last frame
	pUpdateProbeOffsetFromShader( GetActiveCascade() );
	pUpdateProbeExtendsFromShader( GetActiveCascade() );
	
	INIT_SPECIAL_TIMING
	// monitor configuration changes
	pRenderThread.GetGI().GetTraceRays().UpdateFromConfig();
	
	// choose cascade to update this frame
	pActivateNextCascade();
	
	// update position
	deoglGICascade &cascade = GetActiveCascade();
	cascade.UpdatePosition( cameraPosition );
	SPECIAL_TIMER_PRINT("Update: UpdatePosition")
	
	pFindContent( cameraPosition );
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
	cascade.UpdateUBOParameters( pRenderThread.GetGI().GetUBOParameter(), count );
	pPrepareUBORayDirections();
	
	if( count > 0 ){
		cascade.UpdateUBOProbeIndices( pRenderThread.GetGI().GetUBOProbeIndex() );
		cascade.UpdateUBOProbePosition( pRenderThread.GetGI().GetUBOProbePosition() );
	}
}

void deoglGIState::PrepareUBOStateRayCache() const{
	deoglGICascade &cascade = GetActiveCascade();
	const int count = cascade.GetRayCacheProbeCount();
	cascade.UpdateUBOParameters( pRenderThread.GetGI().GetUBOParameter(), count );
	pPrepareUBORayDirections();
	
	if( count > 0 ){
		cascade.UpdateUBOProbeIndicesRayCache( pRenderThread.GetGI().GetUBOProbeIndex() );
		cascade.UpdateUBOProbePositionRayCache( pRenderThread.GetGI().GetUBOProbePosition() );
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

void deoglGIState::InvalidateArea( const decDVector &minExtend, const decDVector &maxExtend ){
// 		pRenderThread.GetLogger().LogInfoFormat("InvalidateArea (%g,%g,%g) (%g,%g,%g)",
// 			minExtend.x, minExtend.y, minExtend.z, maxExtend.x, maxExtend.y, maxExtend.z);
	if( ! ( maxExtend > minExtend ) ){
		return;
	}
	
	int i;
	for( i=0; i<pCascadeCount; i++ ){
		pCascades[ i ]->InvalidateArea( minExtend, maxExtend );
	}
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
}

void deoglGIState::ValidatedRayCaches(){
	GetActiveCascade().ValidatedRayCaches();
	pProbesExtendsChanged = true;
}

void deoglGIState::ComponentEnteredWorld( deoglRComponent *component ){
	if( ! pAreaTracker.ComponentTouches( *component ) || pAreaTracker.RejectComponent( *component ) ){
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



// Private Functions
//////////////////////

void deoglGIState::pCleanUp(){
	if( pVBOProbeOffsetsData ){
		delete [] pVBOProbeOffsetsData;
	}
	if( pVBOProbeOffsets ){
		pglDeleteBuffers( 1, &pVBOProbeOffsets );
	}
	if( pVBOProbeOffsetsTransition ){
		pglDeleteBuffers( 1, &pVBOProbeOffsetsTransition );
	}
	if( pVBOProbeExtendsData ){
		delete [] pVBOProbeExtendsData;
	}
	if( pVBOProbeExtends ){
		pglDeleteBuffers( 1, &pVBOProbeExtends );
	}
	
	if( pCascades ){
		int i;
		for( i=0; i<pCascadeCount; i++ ){
			delete pCascades[ i ];
		}
	}
	if( pCascaceUpdateCycle ){
		delete [] pCascaceUpdateCycle;
	}
}

void deoglGIState::pInitCascades(){
	pCascades = new deoglGICascade*[ 4 ];
	
	const decVector smallestSpacing( 1.0f, 1.0f, 1.0f );
	const decVector largestSpacing( pSize.x / ( float )pGridCoordClamp.x,
		pSize.y / ( float )pGridCoordClamp.y, pSize.z / ( float )pGridCoordClamp.z );
	const float scaleFactor2nd = 2.0f;
	const float offsetFactor2nd = 0.0f; // 0.25f; <= when using 5th cascade offset by 0.5
	const float scaleFactor3rd = 1.0f / 3.0f;
	
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
}

void deoglGIState::pInitUBOClearProbes(){
	pUBOClearProbes.TakeOver( new deoglSPBlockUBO( pRenderThread ) );
	deoglSPBlockUBO &ubo = GetUBOClearProbes();
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
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeOffsets ) );
	OGL_CHECK( pRenderThread, pglGetBufferSubData( GL_ARRAY_BUFFER,
		0, cascade.GetUpdateProbeCount() * 4 * sizeof( GLfloat ), pVBOProbeOffsetsData ) );
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	SPECIAL_TIMER_PRINT("UpdateProbeOffsetFromShader: > GetVBOData")
	
	cascade.UpdateProbeOffsetFromShader( pVBOProbeOffsetsData );
	SPECIAL_TIMER_PRINT("UpdateProbeOffsetFromShader: > UpdateCascade")
}

void deoglGIState::pUpdateProbeExtendsFromShader( deoglGICascade &cascade ){
	if( ! pProbesExtendsChanged || cascade.GetRayCacheProbeCount() == 0 ){
		return;
	}
	
	INIT_SPECIAL_TIMING
	pProbesExtendsChanged = false;
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeExtends ) );
	OGL_CHECK( pRenderThread, pglGetBufferSubData( GL_ARRAY_BUFFER,
		0, cascade.GetRayCacheProbeCount() * 6 * sizeof( GLfloat ), pVBOProbeExtendsData ) );
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	SPECIAL_TIMER_PRINT("UpdateProbeExtendsFromShader: > GetVBOData")
	
	cascade.UpdateProbeExtendsFromShader( pVBOProbeExtendsData );
	SPECIAL_TIMER_PRINT("UpdateProbeExtendsFromShader: > UpdateCascade")
}

void deoglGIState::pActivateNextCascade(){
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
	&& pTexProbeOffset.GetTexture() && pTexProbeState.GetTexture() && ! pClearMaps ){
		return;
	}
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	if( ! pTexProbeIrradiance.GetTexture() ){
		const int width = ( pSizeTexIrradiance + 2 ) * pProbeCount.x * pProbeCount.y + 2;
		const int height = ( pSizeTexIrradiance + 2 ) * pProbeCount.z + 2;
		
		pTexProbeIrradiance.SetFBOFormat( 3, true );
		pTexProbeIrradiance.SetSize( width, height, pCascadeCount );
		pTexProbeIrradiance.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeIrradiance );
		pFBOProbeIrradiance.DetachAllImages();
		pFBOProbeIrradiance.AttachColorArrayTexture( 0, &pTexProbeIrradiance );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeIrradiance.Verify();
	}
	
	if( ! pTexProbeDistance.GetTexture() ){
		const int width = ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2;
		const int height = ( pSizeTexDistance + 2 ) * pProbeCount.z + 2;
		
		pTexProbeDistance.SetFBOFormat( 2, true );
		pTexProbeDistance.SetSize( width, height, pCascadeCount );
		pTexProbeDistance.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeDistance );
		pFBOProbeDistance.DetachAllImages();
		pFBOProbeDistance.AttachColorArrayTexture( 0, &pTexProbeDistance );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeDistance.Verify();
	}
	
	if( ! pTexProbeOffset.GetTexture() ){
		const int width = pProbeCount.x * pProbeCount.y;
		const int height = pProbeCount.z;
		
		pTexProbeOffset.SetFBOFormat( 4, true );
		pTexProbeOffset.SetSize( width, height, pCascadeCount );
		pTexProbeOffset.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeOffset );
		pFBOProbeOffset.DetachAllImages();
		pFBOProbeOffset.AttachColorArrayTexture( 0, &pTexProbeOffset );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeOffset.Verify();
	}
	
	if( ! pTexProbeState.GetTexture() ){
		const int width = pProbeCount.x * pProbeCount.y;
		const int height = pProbeCount.z;
		
		pTexProbeState.SetFBOFormatIntegral( 1, 8, true );
		pTexProbeState.SetSize( width, height );
		pTexProbeState.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeState );
		pFBOProbeState.DetachAllImages();
		pFBOProbeState.AttachColorTexture( 0, &pTexProbeState );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeState.Verify();
	}
	
	if( pClearMaps ){
		OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
		OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeIrradiance );
		const GLfloat clearIrradiance[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearIrradiance[ 0 ] ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeDistance );
		const GLfloat maxProbeDistance = pCascades[ pCascadeCount - 1]->GetMaxProbeDistance();
		const GLfloat clearDistance[ 4 ] = { maxProbeDistance, maxProbeDistance,
			maxProbeDistance, maxProbeDistance * maxProbeDistance };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDistance[ 0 ] ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeOffset );
		const GLfloat clearOffset[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearOffset[ 0 ] ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeState );
		const GLuint clearState[ 4 ] = { 0, 0, 0, 0 };
		OGL_CHECK( pRenderThread, pglClearBufferuiv( GL_COLOR, 0, &clearState[ 0 ] ) );
	}
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
	pClearMaps = false;
}

void deoglGIState::pPrepareProbeVBO(){
	// data arrays
	pVBOProbeOffsetsData = new GLfloat[ GI_MAX_PROBE_COUNT * 4 ];
	pVBOProbeExtendsData = new GLfloat[ GI_MAX_PROBE_COUNT * 6 ];
	
	// VBO probe offset
	OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOProbeOffsets ) );
	if( ! pVBOProbeOffsets ){
		DETHROW( deeOutOfMemory );
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeOffsets ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER,
		GI_MAX_PROBE_COUNT * 4 * sizeof( GLfloat ), NULL, GL_STREAM_READ ) );
	
	// transition VBO probe offset
	OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOProbeOffsetsTransition ) );
	if( ! pVBOProbeOffsetsTransition ){
		DETHROW( deeOutOfMemory );
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeOffsetsTransition ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER,
		GI_MAX_PROBE_COUNT * 4 * sizeof( GLfloat ), NULL, GL_STREAM_DRAW ) );
	
	// VBO probe extends
	OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOProbeExtends ) );
	if( ! pVBOProbeExtends ){
		DETHROW( deeOutOfMemory );
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeExtends ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER,
		GI_MAX_PROBE_COUNT * 6 * sizeof( GLfloat ), NULL, GL_STREAM_READ ) );
}

void deoglGIState::pPrepareUBORayDirections() const{
	const deoglGI &gi = pRenderThread.GetGI();
	const deoglGITraceRays &traceRays = gi.GetTraceRays();
	const int raysPerProbe = traceRays.GetRaysPerProbe();
	
	deoglSPBlockUBO &ubo = pRenderThread.GetGI().GetUBORayDirection();
	ubo.MapBuffer();
	try{
// #define GI_USE_RANDOM_DIRECTION 1
		#ifdef  GI_USE_RANDOM_DIRECTION
		const decMatrix randomOrientation( decMatrix::CreateRotation( decMath::random( -PI, PI ),
			decMath::random( -PI, PI ), decMath::random( -PI, PI ) ) );
		#endif
		
		const float sf_PHI = sqrtf( 5.0f ) * 0.5f + 0.5f;
		const float sf_n = ( float )raysPerProbe;
		#define madfrac(A, B) ((A)*(B)-floor((A)*(B)))
		
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
		
	}catch( const deException & ){
		ubo.UnmapBuffer();
		throw;
	}
	ubo.UnmapBuffer();
}
