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
#include "../utils/collision/deoglDCollisionBox.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIState
///////////////////////

// Constructor, destructor
////////////////////////////

deoglGIState::deoglGIState( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pProbeSpacing( 1.0f, 1.0f, 1.0f ),
pProbeSpacingInv( 1.0f / pProbeSpacing.x, 1.0f / pProbeSpacing.y, 1.0f / pProbeSpacing.z ),
pProbeCount( 32, 8, 32 ),
pGridCoordClamp( pProbeCount - decPoint3( 1, 1, 1 ) ),
pProbeOrigin( pProbeSpacing.Multiply( decVector( pGridCoordClamp ) * -0.5f ) ),
pPositionClamp( pProbeSpacing.Multiply( decVector( pGridCoordClamp ) ) ),
pStrideProbeCount( pProbeCount.x * pProbeCount.z ),
pRealProbeCount( pStrideProbeCount * pProbeCount.y ),

pMaxDetectionRange( 10.0f ),
pDetectionBox( pProbeSpacing.Multiply( decVector( pGridCoordClamp ) * 0.5f )
	+ decVector( pMaxDetectionRange, pMaxDetectionRange, pMaxDetectionRange ) ),

pIrradianceMapSize( 8 ),
pDistanceMapSize( 16 ),
pMaxProbeDistance( 4.0f ),
pDepthSharpness( 50.0f ),
pHysteresis( 0.9f ), // 0.98 (paper)
pNormalBias( 0.05f ), //0.25f ),
pEnergyPreservation( 0.85f ),

pSizeTexIrradiance( pIrradianceMapSize ),
pSizeTexDistance( pDistanceMapSize ),

pIrradianceMapScale( 1.0f / ( ( pSizeTexIrradiance + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexIrradiance + 2 ) * pProbeCount.z + 2 ) ),
pDistanceMapScale( 1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.z + 2 ) ),

pProbes( NULL ),
pAgedProbes( NULL ),
pUpdateProbes( NULL ),
pUpdateProbeCount( 0 ),
pRayLimitProbes( NULL ),
pRayLimitProbeCount( 0 ),
pRayCacheProbes( NULL ),
pRayCacheProbeCount( 0 ),

pClearProbes( NULL ),
pClearProbeCount( pRealProbeCount / 32 ),
pHasClearProbes( false ),

pTexProbeIrradiance( renderThread ),
pTexProbeDistance( renderThread ),
pTexProbeOffset( renderThread ),
pFBOProbeIrradiance( renderThread, false ),
pFBOProbeDistance( renderThread, false ),
pFBOProbeOffset( renderThread, false ),
pPixBufProbeOffset( NULL ),
pClearMaps( true ),
pProbesHaveMoved( false ),

pInstances( renderThread ),
pRays( renderThread, 64, pRealProbeCount )
{
	try{
		pInitProbes();
		pClearProbes = new uint16_t[ pClearProbeCount ];
		pInitUBOClearProbes();
		pUpdateProbes = new uint16_t[ renderThread.GetGI().GetTraceRays().GetProbeCount() ];
		#ifdef GI_USE_RAY_LIMIT
			pRayLimitProbes = new uint16_t[ renderThread.GetGI().GetTraceRays().GetProbeCount() ];
		#endif
		#ifdef GI_USE_RAY_CACHE
			pRayCacheProbes = new uint16_t[ renderThread.GetGI().GetTraceRays().GetProbeCount() ];
		#endif
		
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

void deoglGIState::FindContent( deoglRWorld &world ){
	deoglDCollisionBox colbox( pPosition, pDetectionBox );
	pCollideList.Clear();
	pCollideList.AddComponentsColliding( world.GetOctree(), &colbox );
}

void deoglGIState::FilterOcclusionMeshes(){
	const int count = pCollideList.GetComponentCount();
	int i;
	
	pCollideListFiltered.Clear();
	
	for( i=0; i<count; i++ ){
		deoglRComponent * const component = pCollideList.GetComponentAt( i )->GetComponent();
		
		if( ! component->GetOcclusionMesh() ){
			continue;
		}
		
		pCollideListFiltered.AddComponent( component );
	}
}

void deoglGIState::FilterComponents(){
	const int count = pCollideList.GetComponentCount();
	int i;
	
	pCollideListFiltered.Clear();
	
	for( i=0; i<count; i++ ){
		deoglRComponent * const component = pCollideList.GetComponentAt( i )->GetComponent();
		
		if( ! component->GetModel() || component->GetLODCount() == 0 ){
			continue;
		}
		
		const deoglRModel::sExtends &extends = component->GetModel()->GetExtends();
		if( ( extends.maximum - extends.minimum ) < decVector( 0.5f, 0.5f, 0.5f ) ){
			continue; // skip small models to improve performance
		}
		
		pCollideListFiltered.AddComponent( component );
	}
}



decPoint3 deoglGIState::ProbeIndex2GridCoord( int index ) const{
	decPoint3 coord;
	coord.y = index / pStrideProbeCount;
	index -= pStrideProbeCount * coord.y;
	coord.z = index / pProbeCount.x;
	coord.x = index - pProbeCount.x * coord.z;
	return coord;
}

int deoglGIState::GridCoord2ProbeIndex( const decPoint3 &coord ) const{
	return pStrideProbeCount * coord.y + pProbeCount.x * coord.z + coord.x;
}

decVector deoglGIState::Grid2Local( const decPoint3 &coord ) const{
	return pProbeSpacing.Multiply( decVector( coord ) ) + pProbeOrigin;
}

decPoint3 deoglGIState::World2Grid( const decDVector &position ) const{
	return ( position - pPosition - decDVector( pProbeOrigin ) ).Multiply( pProbeSpacingInv ).Round();
}

decDVector deoglGIState::Grid2World( const decPoint3 &grid ) const{
	return pPosition + decDVector( pProbeOrigin ) + decDVector( grid ).Multiply( pProbeSpacing );
}

decDVector deoglGIState::WorldClosestGrid( const decDVector &position ) const{
	decDVector result( ( double )pProbeSpacing.x * floor( position.x * ( double )pProbeSpacingInv.x ),
		( double )pProbeSpacing.y * floor( position.y * ( double )pProbeSpacingInv.y ),
		( double )pProbeSpacing.z * floor( position.z * ( double )pProbeSpacingInv.z ) );
	
	const decDVector halfGrid( decDVector( pProbeSpacing ) * 0.5 );
	if( position.x - result.x >= halfGrid.x ){
		result.x += ( double )pProbeSpacing.x;
	}
	if( position.y - result.y >= halfGrid.y ){
		result.y += ( double )pProbeSpacing.y;
	}
	if( position.z - result.z >= halfGrid.z ){
		result.z += ( double )pProbeSpacing.z;
	}
	
	return result;
}

decPoint3 deoglGIState::LocalGrid2ShiftedGrid( const decPoint3 &coord ) const{
	decPoint3 shifted( coord + pGridCoordShift );
	shifted.x %= pProbeCount.x;
	shifted.y %= pProbeCount.y;
	shifted.z %= pProbeCount.z;
	return shifted;
}

decPoint3 deoglGIState::ShiftedGrid2LocalGrid( const decPoint3 &coord ) const{
	decPoint3 local( coord - pGridCoordShift + pProbeCount );
	local.x %= pProbeCount.x;
	local.y %= pProbeCount.y;
	local.z %= pProbeCount.z;
	return local;
}



void deoglGIState::ClearClearProbes(){
	memset( pClearProbes, 0, pClearProbeCount * sizeof( uint16_t ) );
	pHasClearProbes = false;
}

void deoglGIState::PrepareUBOClearProbes() const{
	deoglSPBlockUBO &ubo = GetUBOClearProbes();
	
	ubo.MapBuffer();
	try{
		const int count = pClearProbeCount / 4;
		int i, j;
		
		for( i=0, j=0; i<count; i++, j+=4 ){
			ubo.SetParameterDataArrayUVec4( 0, i, pClearProbes[ j ],
				pClearProbes[ j + 1 ], pClearProbes[ j + 2 ], pClearProbes[ j + 3 ] );
		}
		
	}catch( const deException & ){
		ubo.UnmapBuffer();
		throw;
	}
	ubo.UnmapBuffer();
}



// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) pRenderThread.GetLogger().LogInfoFormat("GIState.Update: " w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglGIState::Update( deoglRWorld &world, const decDVector &cameraPosition,
const decDMatrix &cameraMatrix, float fovX, float fovY ){
// 		pRenderThread.GetLogger().LogInfoFormat( "Update GIState %p (%g,%g,%g)",
// 			this, cameraPosition.x, cameraPosition.y, cameraPosition.z );
	if( pProbesHaveMoved ){
		pProbesHaveMoved = false;
		UpdateProbeOffsetFromTexture();
	}
	
	// monitor configuration changes
	pRenderThread.GetGI().GetTraceRays().UpdateFromConfig();
	
	// find content to track. only static and dynamic content is tracked
	INIT_SPECIAL_TIMING
	FindContent( world );
	SPECIAL_TIMER_PRINT("FindContent")
	
	#ifdef GI_USE_RAY_LIMIT
		FilterOcclusionMeshes();
	#else
		FilterComponents();
	#endif
	SPECIAL_TIMER_PRINT("FilterContent")
	
	// track changes in static instances has to be done first
	pTrackInstanceChanges();
	SPECIAL_TIMER_PRINT("TrackInstanceChanges")
	
	// prepare probes for tracing
	pUpdateProbeCount = 0;
	pUpdatePosition( cameraPosition );
	SPECIAL_TIMER_PRINT("UpdatePosition")
	pPrepareTraceProbes( decDMatrix::CreateTranslation( pPosition ) * cameraMatrix, fovX, fovY );
	SPECIAL_TIMER_PRINT("PrepareTraceProbes")
	
	#ifdef GI_USE_RAY_LIMIT
		pPrepareRayLimitProbes();
	#endif
	#ifdef GI_USE_RAY_CACHE
		pPrepareRayCacheProbes();
	#endif
	SPECIAL_TIMER_PRINT("PrepareRayCacheProbes")
	
	// synchronize all tracked instances using new position
	pSyncTrackedInstances();
	SPECIAL_TIMER_PRINT("SyncTrackedInstances")
}

void deoglGIState::PrepareUBOState() const{
	pPrepareUBOParameters();
	pPrepareUBORayDirections();
	
	if( pUpdateProbeCount == 0 ){
		return;
	}
	
	// probe indices
	deoglSPBlockUBO &uboIndices = pRenderThread.GetGI().GetUBOProbeIndex();
	uboIndices.MapBuffer();
	try{
		const int count = ( pUpdateProbeCount - 1 ) / 4 + 1;
		int i, j;
		
		for( i=0, j=0; i<count; i++, j+=4 ){
			uboIndices.SetParameterDataArrayIVec4( 0, i,
				j < pUpdateProbeCount ? pUpdateProbes[ j ] : 0,
				j + 1 < pUpdateProbeCount ? pUpdateProbes[ j + 1 ] : 0,
				j + 2 < pUpdateProbeCount ? pUpdateProbes[ j + 2 ] : 0,
				j + 3 < pUpdateProbeCount ? pUpdateProbes[ j + 3 ] : 0 );
		}
		
	}catch( const deException & ){
		uboIndices.UnmapBuffer();
		throw;
	}
	uboIndices.UnmapBuffer();
	
	// probe positions
	deoglSPBlockUBO &uboPositions = pRenderThread.GetGI().GetUBOProbePosition();
	uboPositions.MapBuffer();
	try{
		int i;
		for( i=0; i<pUpdateProbeCount; i++ ){
			const sProbe &p = pProbes[ pUpdateProbes[ i ] ];
			uboPositions.SetParameterDataArrayVec4( 0, i, p.position + p.offset, ( float )p.flags );
		}
		
	}catch( const deException & ){
		uboPositions.UnmapBuffer();
		throw;
	}
	uboPositions.UnmapBuffer();
}

void deoglGIState::PrepareUBOStateRayLimit() const{
	pPrepareUBOParameters();
	pPrepareUBORayDirections();
	
	if( pRayLimitProbeCount == 0 ){
		return;
	}
	
	// probe indices
	deoglSPBlockUBO &uboIndices = pRenderThread.GetGI().GetUBOProbeIndex();
	uboIndices.MapBuffer();
	try{
		const int count = ( pRayLimitProbeCount - 1 ) / 4 + 1;
		int i, j;
		for( i=0, j=0; i<count; i++, j+=4 ){
			uboIndices.SetParameterDataArrayIVec4( 0, i,
				j < pRayLimitProbeCount ? pRayLimitProbes[ j ] : 0,
				j + 1 < pRayLimitProbeCount ? pRayLimitProbes[ j + 1 ] : 0,
				j + 2 < pRayLimitProbeCount ? pRayLimitProbes[ j + 2 ] : 0,
				j + 3 < pRayLimitProbeCount ? pRayLimitProbes[ j + 3 ] : 0 );
		}
		
	}catch( const deException & ){
		uboIndices.UnmapBuffer();
		throw;
	}
	uboIndices.UnmapBuffer();
	
	// probe positions
	deoglSPBlockUBO &uboPositions = pRenderThread.GetGI().GetUBOProbePosition();
	uboPositions.MapBuffer();
	try{
		int i;
		for( i=0; i<pRayLimitProbeCount; i++ ){
			const sProbe &p = pProbes[ pRayLimitProbes[ i ] ];
			uboPositions.SetParameterDataArrayVec4( 0, i, p.position + p.offset, ( float )p.flags );
		}
		
	}catch( const deException & ){
		uboPositions.UnmapBuffer();
		throw;
	}
	uboPositions.UnmapBuffer();
}

void deoglGIState::PrepareUBOStateRayCache() const{
	pPrepareUBOParameters();
	pPrepareUBORayDirections();
	
	if( pRayCacheProbeCount == 0 ){
		return;
	}
	
	// probe indices
	deoglSPBlockUBO &uboIndices = pRenderThread.GetGI().GetUBOProbeIndex();
	uboIndices.MapBuffer();
	try{
		const int count = ( pRayCacheProbeCount - 1 ) / 4 + 1;
		int i, j;
		for( i=0, j=0; i<count; i++, j+=4 ){
			uboIndices.SetParameterDataArrayIVec4( 0, i,
				j < pRayCacheProbeCount ? pRayCacheProbes[ j ] : 0,
				j + 1 < pRayCacheProbeCount ? pRayCacheProbes[ j + 1 ] : 0,
				j + 2 < pRayCacheProbeCount ? pRayCacheProbes[ j + 2 ] : 0,
				j + 3 < pRayCacheProbeCount ? pRayCacheProbes[ j + 3 ] : 0 );
		}
		
	}catch( const deException & ){
		uboIndices.UnmapBuffer();
		throw;
	}
	uboIndices.UnmapBuffer();
	
	// probe positions
	deoglSPBlockUBO &uboPositions = pRenderThread.GetGI().GetUBOProbePosition();
	uboPositions.MapBuffer();
	try{
		int i;
		for( i=0; i<pRayCacheProbeCount; i++ ){
			const sProbe &probe = pProbes[ pRayCacheProbes[ i ] ];
			uboPositions.SetParameterDataArrayVec4( 0, i,
				probe.position + probe.offset, ( float )probe.flags );
		}
		
	}catch( const deException & ){
		uboPositions.UnmapBuffer();
		throw;
	}
	uboPositions.UnmapBuffer();
}

void deoglGIState::Invalidate(){
	uint16_t i;
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
	}
	
	pClearMaps = true;
	pProbesHaveMoved = false;
	ClearClearProbes();
}

void deoglGIState::ProbesMoved(){
	pProbesHaveMoved = true;
}

void deoglGIState::UpdateProbeOffsetFromTexture(){
	if( ! pPixBufProbeOffset ){
		pPixBufProbeOffset = new deoglPixelBuffer( deoglPixelBuffer::epfFloat3,
			pTexProbeOffset.GetWidth(), pTexProbeOffset.GetHeight(), 1 );
	}
	
	// this call is slow (>5ms) if used right after rendering the probe offset update
	// shader. by delaying the read-back to the next GI update cycle reduces the
	// time consumption to <300ys, which is acceptable
// 	decTimer timer;
	pTexProbeOffset.GetPixels( *pPixBufProbeOffset );
// 	printf( "GetPixels: %d\n", ( int )( timer.GetElapsedTime() * 1e6f ) );
	
	const deoglPixelBuffer::sFloat3 * const pixels = pPixBufProbeOffset->GetPointerFloat3();
	const int stride = pTexProbeOffset.GetWidth();
	int i;
	
	for( i=0; i<pUpdateProbeCount; i++ ){
		sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
		
		// PROBLEM some probes flicker between two positions and can not make up their mind.
		//         this causes GI flickering and endless ray cache invalidating.
		//         
		//         as a quick fix a counter is used. each time the offset changes the counter
		//         is incremented. after a certain amount the probe is not allowed to move
		//         offset anymore. invalidating a probe resets the counter
		//         
		//         the root cause is unknown. would be better to find and fix it
		if( probe.countOffsetMoved >= 5 ){
			continue;
		}
		
		const decPoint tc( pProbeCount.x * probe.coord.y + probe.coord.x, probe.coord.z );
		
		const deoglPixelBuffer::sFloat3 &pixel = pixels[ stride * tc.y + tc.x ];
		const decVector offset( pixel.r, pixel.g, pixel.b );
		
		if( offset.IsEqualTo( probe.offset, 0.05f ) ){
			continue; // update offset only if it moved far enough to justify an expensive update
		}
		
		probe.offset = offset;
		probe.countOffsetMoved++;
		probe.flags &= ~( epfRayLimitsValid | epfRayCacheValid );
	}
}

void deoglGIState::ValidatedRayCaches(){
	int i;
	for( i=0; i<pRayCacheProbeCount; i++ ){
		pProbes[ pRayCacheProbes[ i ] ].flags |= epfRayCacheValid;
	}
}



// Private Functions
//////////////////////

void deoglGIState::pCleanUp(){
	if( pPixBufProbeOffset ){
		delete pPixBufProbeOffset;
	}
	if( pRayCacheProbes ){
		delete [] pRayCacheProbes;
	}
	if( pRayLimitProbes ){
		delete [] pRayLimitProbes;
	}
	if( pUpdateProbes ){
		delete [] pUpdateProbes;
	}
	if( pClearProbes ){
		delete [] pClearProbes;
	}
	if( pAgedProbes ){
		delete [] pAgedProbes;
	}
	if( pProbes ){
		delete [] pProbes;
	}
}

void deoglGIState::pInitProbes(){
	uint16_t  i;
	
	pProbes = new sProbe[ pRealProbeCount ];
	pAgedProbes = new uint16_t[ pRealProbeCount ];
	
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.index = i;
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		probe.coord = ProbeIndex2GridCoord( i );
		
		pAgedProbes[ i ] = i;
	}
}

void deoglGIState::pInitUBOClearProbes(){
	pUBOClearProbes.TakeOver( new deoglSPBlockUBO( pRenderThread ) );
	deoglSPBlockUBO &ubo = GetUBOClearProbes();
	ubo.SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
	ubo.SetParameterCount( 1 );
	ubo.GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtInt, 4, 1, pClearProbeCount / 4 ); // uvec4
	ubo.MapToStd140();
	ubo.SetBindingPoint( 0 );
}

void deoglGIState::pInvalidateAllRayLimits(){
	int i;
	for( i=0; i<pRealProbeCount; i++ ){
		pProbes[ i ].flags &= ~epfRayLimitsValid;
	}
}

void deoglGIState::pInvalidateAllRayCaches(){
	int i;
	for( i=0; i<pRealProbeCount; i++ ){
		pProbes[ i ].flags &= ~epfRayCacheValid;
	}
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
	
	bool invalidateCaches = pInstances.AnyChanged();
	#ifdef GI_USE_RAY_LIMIT
		invalidateCaches |= pInstances.RemoveOcclusionMeshes( pCollideListFiltered );
		invalidateCaches |= pInstances.AddOcclusionMeshes( pCollideListFiltered );
	#else
		invalidateCaches |= pInstances.RemoveComponents( pCollideListFiltered );
		invalidateCaches |= pInstances.AddComponents( pCollideListFiltered );
	#endif
	
	if( invalidateCaches ){
// 		pRenderThread.GetLogger().LogInfo( "GIState.TrackInstanceChanges: invalidate all ray limits" );
		#ifdef GI_USE_RAY_LIMIT
			pInvalidateAllRayLimits();
		#else
			pInvalidateAllRayCaches();
		#endif
	}
	
// 	pRenderThread.GetLogger().LogInfo( "pTrackInstanceChanges" );
// 	pInstances.DebugPrint();
}

void deoglGIState::pSyncTrackedInstances(){
	#ifdef GI_USE_RAY_LIMIT
		pInstances.RemoveOcclusionMeshes( pCollideListFiltered );
		pInstances.AddOcclusionMeshes( pCollideListFiltered );
		
	#else
		pInstances.RemoveComponents( pCollideListFiltered );
		pInstances.AddComponents( pCollideListFiltered );
	#endif
	
	pInstances.ClearAllChanged();
}

void deoglGIState::pUpdatePosition( const decDVector &position ){
	// find world position closest to the next grid position. if the position is
	// the same no updating is required
	const decDVector closestPosition( WorldClosestGrid( position ) );
	if( closestPosition.IsEqualTo( pPosition ) ){
		return;
	}
	
	// invalidate probes shifted out
	const decPoint3 gridOffset( ( closestPosition - pPosition ).Multiply( pProbeSpacingInv ).Round() );
	int i;
	
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		
		const decPoint3 coord( LocalGrid2ShiftedGrid( probe.coord ) - gridOffset );
		if( coord >= decPoint3() && coord < pProbeCount ){
			continue; // probe is still valid
		}
		
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		
		pClearProbes[ i / 32 ] |= 1 << ( i % 32 );
		pHasClearProbes = true;
	}
	
	// set the new tracing position
	pPosition = closestPosition;
	
	// update shift
	pGridCoordShift -= gridOffset;
	pGridCoordShift.x %= pProbeCount.x;
	pGridCoordShift.y %= pProbeCount.y;
	pGridCoordShift.z %= pProbeCount.z;
	pGridCoordShift += pProbeCount;
	pGridCoordShift.x %= pProbeCount.x;
	pGridCoordShift.y %= pProbeCount.y;
	pGridCoordShift.z %= pProbeCount.z;
	
	// update probe cordinates. has to come after adjusting the parameters
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.shiftedCoord = LocalGrid2ShiftedGrid( probe.coord );
		probe.position = Grid2Local( probe.shiftedCoord );
	}
}

void deoglGIState::pPrepareTraceProbes( const decMatrix &matrixView, float fovX, float fovY ){
	INIT_SPECIAL_TIMING
	pFindProbesToUpdate( matrixView, fovX, fovY );
	SPECIAL_TIMER_PRINT(">FindProbesToUpdate")
	pPrepareProbeTexturesAndFBO();
	SPECIAL_TIMER_PRINT(">PrepareProbeTexturesAndFBO")
}

void deoglGIState::pFindProbesToUpdate( const decMatrix &matrixView, float fovX, float fovY ){
	//const int maxUpdateCount = pRenderThread.GetGI().GetTraceRays().GetProbeCount();
	const int maxUpdateCount = 1024; // 256
	const int maxUpdateCountOutsideView = maxUpdateCount / 5; // 20%
	const int maxUpdateCountInsideView = maxUpdateCount - maxUpdateCountOutsideView; // 80%
	int updateCountOutsideView = 0;
	int updateCountInsideView = 0;
	
	const deoglGITraceRays &traceRays = pRenderThread.GetGI().GetTraceRays();
	const float viewAngleX = fovX * 0.5f;
	const float viewAngleY = fovY * 0.5f;
	int i, last;
	
	pUpdateProbeCount = 0;
	
	// classify probes into inside view and outside view
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ pAgedProbes[ i ] ];
		
		const decVector view( matrixView * probe.position );
		const float length = view.Length();
		
		if( length < FLOAT_SAFE_EPSILON
		|| ( acosf( view.z / length ) < viewAngleX && asinf( view.y / length ) < viewAngleY ) ){
			probe.flags |= epfInsideView;
			
		}else{
			probe.flags &= ~epfInsideView;
		}
	}
	
	// add all invalid probes up to the limits
	for( i=0, last=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ pAgedProbes[ i ] ];
		
		// determine if probe has to be added (hence not exceeding the limits)
		bool addProbe = false;
		
		if( ( probe.flags & epfValid ) != epfValid ){
			if( ( probe.flags & epfInsideView ) == epfInsideView ){
				if( updateCountInsideView < maxUpdateCountInsideView ){
					addProbe = true;
					updateCountInsideView++;
				}
				
			}else{
				if( updateCountOutsideView < maxUpdateCountOutsideView ){
					addProbe = true;
					updateCountOutsideView++;
				}
			}
		}
		
		// add probe if not exceeding the limits. the aged probes list is adjusted in a way
		// all not added probes are packaged at the beginning. then the added probes are
		// appended to the end of the list to obtain a full list again
		if( addProbe ){
			pAddUpdateProbe( probe );
			
			if( pUpdateProbeCount == maxUpdateCount ){
				i++;
				break;
			}
			
		}else{
			pAgedProbes[ last++ ] = pAgedProbes[ i ];
		}
	}
	
	while( i < pRealProbeCount ){
		pAgedProbes[ last++ ] = pAgedProbes[ i++ ];
	}
	
	// add all regular probes up to the limits
	if( pUpdateProbeCount < maxUpdateCount ){
		const int endIndex = last;
		
		for( i=0, last=0; i<endIndex; i++ ){
			sProbe &probe = pProbes[ pAgedProbes[ i ] ];
			
			// determine if probe has to be added (hence not exceeding the limits)
			bool addProbe = false;
			
			if( ( probe.flags & epfInsideView ) == epfInsideView ){
				if( updateCountInsideView < maxUpdateCountInsideView ){
					addProbe = true;
					updateCountInsideView++;
				}
				
			}else{
				if( updateCountOutsideView < maxUpdateCountOutsideView ){
					addProbe = true;
					updateCountOutsideView++;
				}
			}
			
			// add probe if not exceeding the limits. the aged probes list is adjusted in a way
			// all not added probes are packaged at the beginning. then the added probes are
			// appended to the end of the list to obtain a full list again
			if( addProbe ){
				pAddUpdateProbe( probe );
				
				if( pUpdateProbeCount == maxUpdateCount ){
					i++;
					break;
				}
				
			}else{
				pAgedProbes[ last++ ] = pAgedProbes[ i ];
			}
		}
		
		while( i < endIndex ){
			pAgedProbes[ last++ ] = pAgedProbes[ i++ ];
		}
	}
	
	for( i=0; i<pUpdateProbeCount; i++ ){
		pAgedProbes[ last++ ] = pUpdateProbes[ i ];
	}
	
		/*{
		pRenderThread.GetLogger().LogInfoFormat("DEBUG: pUpdateProbeCount=%d", pUpdateProbeCount);
		decString s("pUpdateProbes:");
		for(i=0; i<pUpdateProbeCount; i++){
			s.AppendFormat(" % 4d,", pUpdateProbes[i]);
		}
		pRenderThread.GetLogger().LogInfo(s);
		s = "pAgedProbes";
		for(i=0; i<pRealProbeCount; i++){
			s.AppendFormat(" % 4d,", pAgedProbes[i]);
		}
		pRenderThread.GetLogger().LogInfo(s);
		}*/
	
	// TODO update probe grid. for the time being 8x4x8
// 	const decPoint3 spread( 16, 4, 32 ); // 2048 probes
// 	const decPoint3 spread( 16, 4, 16 ); // 1024 probes
	const decPoint3 spread( 8, 4, 8 ); // 256 probes
	const decPoint3 gis( ( pProbeCount - spread ) / 2 );
	const decPoint3 gie( gis + spread );
	pUpdateProbeCount = 0;
	
	decPoint3 gi;
	for( gi.y=gis.y; gi.y<gie.y; gi.y++ ){
		for( gi.z=gis.z; gi.z<gie.z; gi.z++ ){
			for( gi.x=gis.x; gi.x<gie.x; gi.x++ ){
				pAddUpdateProbe( pProbes[ GridCoord2ProbeIndex( ShiftedGrid2LocalGrid( gi ) ) ] );
			}
		}
	}
	
	
	// determine the required sample image size
	pSampleImageSize.x = traceRays.GetProbesPerLine() * traceRays.GetRaysPerProbe();
	pSampleImageSize.y = ( pUpdateProbeCount - 1 ) / traceRays.GetProbesPerLine() + 1;
}

void deoglGIState::pAddUpdateProbe( sProbe &probe ){
	if( ( probe.flags & epfValid ) == epfValid ){
		probe.flags |= epfSmoothUpdate;
		
		if( probe.countOffsetMoved == 1 ){
			// probe moved for the first time. this is usually a large jump from an invalid
			// or unfortunate position to the first potentially good position. for this
			// reason an update has to be forced for this first move but not following ones
			probe.flags &= ~epfSmoothUpdate;
		}
		
	}else{
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		probe.flags &= ~epfSmoothUpdate;
		probe.flags |= epfValid;
	}
	
	pUpdateProbes[ pUpdateProbeCount++ ] = probe.index;
}

void deoglGIState::pPrepareRayLimitProbes(){
	pRayLimitProbeCount = 0;
	
	int i;
	for( i=0; i<pUpdateProbeCount; i++ ){
		const sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
		if( ( probe.flags & epfRayLimitsValid ) != epfRayLimitsValid ){
			pRayLimitProbes[ pRayLimitProbeCount++ ] = pUpdateProbes[ i ];
		}
	}
}

void deoglGIState::pPrepareRayCacheProbes(){
	const int raysPerProbe = pRenderThread.GetGI().GetTraceRays().GetRaysPerProbe();
	if( raysPerProbe != pRays.GetRaysPerProbe() ){
		pInvalidateAllRayCaches();
		pRays.SetRaysPerProbe( raysPerProbe );
	}
	
	pRayCacheProbeCount = 0;
	
	int i;
	for( i=0; i<pUpdateProbeCount; i++ ){
		const sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
		if( ( probe.flags & epfRayCacheValid ) != epfRayCacheValid ){
			pRayCacheProbes[ pRayCacheProbeCount++ ] = pUpdateProbes[ i ];
		}
	}
}

void deoglGIState::pPrepareProbeTexturesAndFBO(){
	if( pTexProbeIrradiance.GetTexture() && pTexProbeDistance.GetTexture()
	&& pTexProbeOffset.GetTexture() && ! pClearMaps && pPixBufProbeOffset ){
		return;
	}
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	if( ! pTexProbeIrradiance.GetTexture() ){
		const int width = ( pSizeTexIrradiance + 2 ) * pProbeCount.x * pProbeCount.y + 2;
		const int height = ( pSizeTexIrradiance + 2 ) * pProbeCount.z + 2;
		
		pTexProbeIrradiance.SetFBOFormat( 3, true );
		pTexProbeIrradiance.SetSize( width, height );
		pTexProbeIrradiance.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeIrradiance );
		pFBOProbeIrradiance.DetachAllImages();
		pFBOProbeIrradiance.AttachColorTexture( 0, &pTexProbeIrradiance );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeIrradiance.Verify();
	}
	
	if( ! pTexProbeDistance.GetTexture() ){
		const int width = ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2;
		const int height = ( pSizeTexDistance + 2 ) * pProbeCount.z + 2;
		
		pTexProbeDistance.SetFBOFormat( 2, true );
		pTexProbeDistance.SetSize( width, height );
		pTexProbeDistance.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeDistance );
		pFBOProbeDistance.DetachAllImages();
		pFBOProbeDistance.AttachColorTexture( 0, &pTexProbeDistance );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeDistance.Verify();
	}
	
	if( ! pTexProbeOffset.GetTexture() ){
		const int width = pProbeCount.x * pProbeCount.y;
		const int height = pProbeCount.z;
		
		pTexProbeOffset.SetFBOFormat( 3, true );
		pTexProbeOffset.SetSize( width, height );
		pTexProbeOffset.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeOffset );
		pFBOProbeOffset.DetachAllImages();
		pFBOProbeOffset.AttachColorTexture( 0, &pTexProbeOffset );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeOffset.Verify();
	}
	
	if( pClearMaps ){
		OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
		OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeIrradiance );
		const GLfloat clearIrradiance[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearIrradiance[ 0 ] ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeDistance );
		const GLfloat clearDistance[ 4 ] = { 4.0f, 4.0f, 4.0f, 16.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDistance[ 0 ] ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeOffset );
		const GLfloat clearOffset[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearOffset[ 0 ] ) );
	}
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
	pClearMaps = false;
}

void deoglGIState::pPrepareUBOParameters() const{
	const deoglGI &gi = pRenderThread.GetGI();
	const deoglGITraceRays &traceRays = gi.GetTraceRays();
	const int raysPerProbe = traceRays.GetRaysPerProbe();
	
	deoglSPBlockUBO &ubo = pRenderThread.GetGI().GetUBOParameter();
	ubo.MapBuffer();
	try{
		ubo.SetParameterDataVec2( deoglGI::eupSampleImageScale,
			1.0f / ( float )pSampleImageSize.x, 1.0f / ( float )pSampleImageSize.y );
		ubo.SetParameterDataInt( deoglGI::eupProbeCount, pUpdateProbeCount );
		ubo.SetParameterDataInt( deoglGI::eupRaysPerProbe, raysPerProbe );
		ubo.SetParameterDataInt( deoglGI::eupProbesPerLine, traceRays.GetProbesPerLine() );
		ubo.SetParameterDataInt( deoglGI::eupIrradianceMapSize, pIrradianceMapSize );
		ubo.SetParameterDataInt( deoglGI::eupDistanceMapSize, pDistanceMapSize );
		ubo.SetParameterDataVec2( deoglGI::eupIrradianceMapScale, pIrradianceMapScale );
		ubo.SetParameterDataVec2( deoglGI::eupDistanceMapScale, pDistanceMapScale );
		ubo.SetParameterDataFloat( deoglGI::eupMaxProbeDistance, pMaxProbeDistance );
		ubo.SetParameterDataFloat( deoglGI::eupDepthSharpness, pDepthSharpness );
		ubo.SetParameterDataVec3( deoglGI::eupGridOrigin, pProbeOrigin );
		ubo.SetParameterDataIVec3( deoglGI::eupGridCoordUnshift, pGridCoordShift );
		ubo.SetParameterDataVec3( deoglGI::eupFieldOrigin, 0.0f, 0.0f, 0.0f );
		ubo.SetParameterDataFloat( deoglGI::eupBlendUpdateProbe, 1.0f - pHysteresis );
		ubo.SetParameterDataInt( deoglGI::eupBVHInstanceRootNode, gi.GetBVH().GetIndexRootNode() );
		ubo.SetParameterDataIVec3( deoglGI::eupGridProbeCount, pProbeCount );
		ubo.SetParameterDataVec3( deoglGI::eupGridProbeSpacing, pProbeSpacing );
		
		// material
		/*
		const int materialMapSize = gi.GetMaterials().GetMaterialMapSize();
		const int materialTexWidth = gi.GetMaterials().GetTextureDiffuseTintMask().GetWidth();
		const int materialTexHeight = gi.GetMaterials().GetTextureDiffuseTintMask().GetHeight();
		const float materialScaleU = ( float )materialMapSize / ( float )materialTexWidth;
		const float materialScaleV = ( float )materialMapSize / ( float )materialTexHeight;
		//const float materialClamp = ( 1.0f / ( float )materialMapSize ) * 0.5f;
		ubo.SetParameterDataVec2( deoglGI::eupMaterialMapTCScale, materialScaleU, materialScaleV );
		*/
		ubo.SetParameterDataInt( deoglGI::eupMaterialMapsPerRow, gi.GetMaterials().GetMaterialsPerRow() );
		ubo.SetParameterDataInt( deoglGI::eupMaterialMapSize, gi.GetMaterials().GetMaterialMapSize() );
		
		// move probes. probe can move at most 50% inside the grid acording to the paper.
		// to be on the safe side use 49%. the minimum distance to the surface is set to
		// 35% of the smallest spacing. this is rather random value with the aim to
		// increase the visible surface captured by probes but without approaching the
		// maximum offset too much.
		ubo.SetParameterDataVec3( deoglGI::eupMoveMaxOffset, pProbeSpacing * 0.49f );
		ubo.SetParameterDataFloat( deoglGI::eupMoveMinDistToSurface,
			decMath::min( pProbeSpacing.x, pProbeSpacing.y, pProbeSpacing.z ) * 0.35f );
		
		// rays
		ubo.SetParameterDataVec2( deoglGI::eupRayMapScale, pRays.GetRayMapScale() );
		
	}catch( const deException & ){
		ubo.UnmapBuffer();
		throw;
	}
	ubo.UnmapBuffer();
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
