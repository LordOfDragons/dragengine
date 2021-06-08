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
#include "../utils/collision/deoglCollisionDetection.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionFrustum.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIState
///////////////////////

// Constructor, destructor
////////////////////////////

deoglGIState::deoglGIState( deoglRenderThread &renderThread, const decVector &size, int cascadeCount ) :
pRenderThread( renderThread ),

pSize( size ),
pCascadeCount( cascadeCount ),

pProbeSpacing( 1.0f, 1.0f, 1.0f ),
// pProbeSpacing( 0.5f, 0.5f, 0.5f ),
pProbeSpacingInv( 1.0f / pProbeSpacing.x, 1.0f / pProbeSpacing.y, 1.0f / pProbeSpacing.z ),
pProbeCount( 32, 8, 32 ),
pGridCoordClamp( pProbeCount - decPoint3( 1, 1, 1 ) ),
pFieldSize( pProbeSpacing.Multiply( decVector( pGridCoordClamp ) ) ),
pFieldOrigin( pFieldSize * -0.5f ),
pPositionClamp( pFieldSize ),
pDynamicHalfEnlarge( pProbeSpacing * 1.9f * 0.5f ), // enlarge = spacing * (1 + 0.45 * 2)
pStrideProbeCount( pProbeCount.x * pProbeCount.z ),
pRealProbeCount( pStrideProbeCount * pProbeCount.y ),

pMaxDetectionRange( 50.0f ),
// pDetectionBox( pFieldSize * 0.5f + decVector( pMaxDetectionRange, pMaxDetectionRange, pMaxDetectionRange ) ),
pDetectionBox( size * 0.5f + decVector( pMaxDetectionRange, pMaxDetectionRange, pMaxDetectionRange ) ),

pIrradianceMapSize( 8 ),
pDistanceMapSize( 16 ),
//pMaxProbeDistance( 4.0f ),

// according to comment in source code this should be slightly larger than the length
// of the diagonal across cells
pMaxProbeDistance( pProbeSpacing.Length() * 1.5f ),

pDepthSharpness( 50.0f ),
pHysteresis( 0.9f ), // 0.98 (paper)
pNormalBias( 0.25f ), // 0.25 or 0.2 in examples
pEnergyPreservation( 0.85f ),
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

pProbes( NULL ),
pAgedProbes( NULL ),
pUpdateProbes( NULL ),
pUpdateProbeCount( 0 ),
pRayCacheProbes( NULL ),
pRayCacheProbeCount( 0 ),

pClearProbes( NULL ),
pClearProbeCount( pRealProbeCount / 32 ),
pHasClearProbes( false ),

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
pVBOProbeOffsetsData( NULL ),
pProbesHaveMoved( false ),

pVBOProbeExtends( 0 ),
pVBOProbeExtendsData( NULL ),
pProbesExtendsChanged( false ),

pInstances( *this ),
pRayCache( renderThread, 64, pRealProbeCount, cascadeCount )
{
	try{
		pInitProbes();
		pClearProbes = new uint32_t[ pClearProbeCount ];
		pInitUBOClearProbes();
		pUpdateProbes = new uint16_t[ GI_MAX_PROBE_COUNT ];
		#ifdef GI_USE_RAY_CACHE
		pRayCacheProbes = new uint16_t[ GI_MAX_PROBE_COUNT ];
		#endif
		
		pAreaTracker.SetHalfExtends( pDetectionBox );
		
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

/*
void deoglGIState::SetSize( const decVector &size ){
	if( size.IsEqualTo( pSize ) ){
		return;
	}
	
	pSize = size;
	
	// TODO adjust pDetectionBox
	// TODO resize largest two volumes and invalidate all probes
	// NOTE smallest two volumes stay intact
}
*/

void deoglGIState::FindContentOld( deoglRWorld &world ){
	deoglDCollisionBox colbox( pPosition, pDetectionBox );
	pCollideList.Clear();
	pCollideList.AddComponentsColliding( world.GetOctree(), &colbox );
}

void deoglGIState::FindContent( deoglRWorld &world, const decLayerMask &layerMask ){
	pAreaTracker.SetWorld( &world );
	pAreaTracker.SetLayerMask( layerMask );
	pAreaTracker.SetPosition( pPosition );
	pAreaTracker.Update();
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
	return pProbeSpacing.Multiply( decVector( coord ) ) + pFieldOrigin;
}

decPoint3 deoglGIState::World2Grid( const decDVector &position ) const{
	return ( position - pPosition - decDVector( pFieldOrigin ) ).Multiply( pProbeSpacingInv ).Round();
}

decDVector deoglGIState::Grid2World( const decPoint3 &grid ) const{
	return pPosition + decDVector( pFieldOrigin ) + decDVector( grid ).Multiply( pProbeSpacing );
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

float deoglGIState::CalcUBOSelfShadowBias() const{
	return pSelfShadowBias * decMath::min( pProbeSpacing.x, pProbeSpacing.y, pProbeSpacing.z );
}



void deoglGIState::ClearClearProbes(){
	memset( pClearProbes, 0, pClearProbeCount * sizeof( uint32_t ) );
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
const decLayerMask &layerMask, const deoglDCollisionFrustum &frustum ){
// 		pRenderThread.GetLogger().LogInfoFormat( "Update GIState %p (%g,%g,%g)",
// 			this, cameraPosition.x, cameraPosition.y, cameraPosition.z );
	INIT_SPECIAL_TIMING
	
	if( pProbesHaveMoved ){
		pProbesHaveMoved = false;
		      UpdateProbeOffsetFromShader();
		SPECIAL_TIMER_PRINT("UpdateProbeOffsetFromTexture")
	}
	if( pProbesExtendsChanged ){
		pProbesExtendsChanged = false;
		      UpdateProbeExtendsFromShader();
		SPECIAL_TIMER_PRINT("UpdateProbeExtendsFromVBO")
	}
	
	// monitor configuration changes
	pRenderThread.GetGI().GetTraceRays().UpdateFromConfig();
	
	// update position
	pUpdatePosition( cameraPosition );
	SPECIAL_TIMER_PRINT("UpdatePosition")
	
	// find content to track. only static and dynamic content is tracked
	FindContentOld( world );
	SPECIAL_TIMER_PRINT("FindContentOld")
	
	FilterComponents();
	SPECIAL_TIMER_PRINT("FilterContent")
	
	FindContent( world, layerMask );
	SPECIAL_TIMER_PRINT("FindContent")
	
	// track changes in static instances has to be done first
	pTrackInstanceChanges();
	SPECIAL_TIMER_PRINT("TrackInstanceChanges")
	
	// prepare probes for tracing
	pUpdateProbeCount = 0;
	pPrepareTraceProbes( frustum );
	SPECIAL_TIMER_PRINT("PrepareTraceProbes")
	
	#ifdef GI_USE_RAY_CACHE
		pPrepareRayCacheProbes();
	#endif
	SPECIAL_TIMER_PRINT("PrepareRayCacheProbes")
	
	// synchronize all tracked instances using new position
	pSyncTrackedInstances();
	SPECIAL_TIMER_PRINT("SyncTrackedInstances")
}

void deoglGIState::PrepareUBOState() const{
	pPrepareUBOParameters( pUpdateProbeCount );
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

void deoglGIState::PrepareUBOStateRayCache() const{
	pPrepareUBOParameters( pRayCacheProbeCount );
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
	pInstances.Clear();
	pAreaTracker.SetWorld( NULL );
	
	uint16_t i;
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		probe.minExtend = -pFieldSize;
		probe.maxExtend = pFieldSize;
	}
	
	pClearMaps = true;
	pProbesHaveMoved = false;
	pProbesExtendsChanged = false;
	ClearClearProbes();
}

void deoglGIState::ProbesMoved(){
	pProbesHaveMoved = true;
}

void deoglGIState::UpdateProbeOffsetFromShader(){
	if( ! pVBOProbeOffsetsData ){
		pVBOProbeOffsetsData = new GLfloat[ GI_MAX_PROBE_COUNT * 4 ];
	}
	
// 	decTimer timer;
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeOffsets ) );
	OGL_CHECK( pRenderThread, pglGetBufferSubData( GL_ARRAY_BUFFER,
		0, pUpdateProbeCount * 4 * sizeof( GLfloat ), pVBOProbeOffsetsData ) );
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
// 	printf( "UpdateProbeOffsetFromTexture: GetVBOData %d\n", ( int )( timer.GetElapsedTime() * 1e6f ) );
	
	const GLfloat *data = pVBOProbeOffsetsData;
	int i;
	
	for( i=0; i<pUpdateProbeCount; i++, data+=4 ){
		sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
		
		probe.flags = ( uint8_t )data[ 3 ];
		
		// PROBLEM some probes flicker between two positions and can not make up their mind.
		//         this causes GI flickering and endless ray cache invalidating.
		//         
		//         as a quick fix a counter is used. each time the offset changes the counter
		//         is incremented. after a certain amount the probe is not allowed to move
		//         offset anymore. invalidating a probe resets the counter
		//         
		//         the root cause is unknown. would be better to find and fix it
		if( probe.countOffsetMoved < 5 ){
			if( ( probe.flags & epfDisabled ) != epfDisabled ){
				probe.countOffsetMoved++;
				
			}else{
				probe.countOffsetMoved = 5;
			}
			
			const decVector offset( data[ 0 ], data[ 1 ], data[ 2 ] );
			
			if( ! offset.IsEqualTo( probe.offset, 0.05f ) ){
				// update offset only if it moved far enough to justify an expensive update
				probe.offset = offset;
				probe.flags &= ~( epfRayCacheValid | epfDynamicDisable );
			}
		}
// 			pRenderThread.GetLogger().LogInfoFormat("UpdateProbeOffsetFromTexture: RayCacheInvalidate %d", pUpdateProbes[i]);
	}
}

void deoglGIState::UpdateProbeExtendsFromShader(){
	if( pRayCacheProbeCount == 0 ){
		return;
	}
	
	if( ! pVBOProbeExtendsData ){
		pVBOProbeExtendsData = new GLfloat[ GI_MAX_PROBE_COUNT * 6 ];
	}
	
// 	decTimer timer;
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeExtends ) );
	OGL_CHECK( pRenderThread, pglGetBufferSubData( GL_ARRAY_BUFFER, 0,
		pRayCacheProbeCount * 6 * sizeof( GLfloat ), pVBOProbeExtendsData ) );
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
// 	printf( "GetVBOResult: %d\n", ( int )( timer.GetElapsedTime() * 1e6f ) );
	
	/*
	printf( "Results:" );
	int i;
	for( i=0; i<pRayCacheProbeCount; i++ ){
		sProbe &probe = pProbes[ pRayCacheProbes[ i ] ];
		const GLfloat * const data = pVBOProbeExtendsData + 6 * i;
		probe.minExtend.Set( data[ 0 ], data[ 1 ], data[ 2 ] );
		probe.maxExtend.Set( data[ 3 ], data[ 4 ], data[ 5 ] );
		printf("%d (%g,%g,%g)(%g,%g,%g)  ", i, probe.minExtend.x, probe.minExtend.y, probe.minExtend.z,
			probe.maxExtend.x, probe.maxExtend.y, probe.maxExtend.z);
	}
	printf("\n");
	*/
}

void deoglGIState::InvalidateArea( const decDVector &minExtend, const decDVector &maxExtend ){
	const decVector lminExtend( minExtend - pPosition );
	const decVector lmaxExtend( maxExtend - pPosition );
// 		decString debugText( "InvalidateArea: (%g,%g,%g) (%g,%g,%g);" );
		
	int i;
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		if( ( probe.flags & epfRayCacheValid ) == epfRayCacheValid
		&& deoglCollisionDetection::AABoxHitsAABox( probe.minExtend, probe.maxExtend, lminExtend, lmaxExtend ) ){
			probe.flags &= ~( epfRayCacheValid | epfDisabled | epfDynamicDisable );
// 				debugText.AppendFormat( " %d,", i );
		}
	}
		
// 		pRenderThread.GetLogger().LogInfoFormat( debugText, lminExtend.x, lminExtend.y, lminExtend.z, lmaxExtend.x, lmaxExtend.y, lmaxExtend.z );
}

void deoglGIState::TouchDynamicArea( const decDVector &minExtend, const decDVector &maxExtend ){
	const decVector lminExtend( ( minExtend - pPosition ).ToVector() - pDynamicHalfEnlarge );
	const decVector lmaxExtend( ( maxExtend - pPosition ).ToVector() + pDynamicHalfEnlarge );
	
	const decPoint3 from( ( ( lminExtend - pFieldOrigin ).Multiply( pProbeSpacingInv ) ).Clamped( decPoint3(), pGridCoordClamp ) );
	const decPoint3 to( ( ( lmaxExtend - pFieldOrigin ).Multiply( pProbeSpacingInv ) ).Clamped( decPoint3(), pGridCoordClamp ) );
	decPoint3 i;
	
	for( i.y=from.y; i.y<=to.y; i.y++ ){
		for( i.z=from.z; i.z<=to.z; i.z++ ){
			for( i.x=from.x; i.x<=to.x; i.x++ ){
				sProbe &probe = pProbes[ GridCoord2ProbeIndex( ShiftedGrid2LocalGrid( i ) ) ];
				if( probe.position >= lminExtend && probe.position <= lmaxExtend ){
					probe.flags &= ~epfDynamicDisable;
				}
			}
		}
	}
}

void deoglGIState::ValidatedRayCaches(){
	int i;
	for( i=0; i<pRayCacheProbeCount; i++ ){
		pProbes[ pRayCacheProbes[ i ] ].flags |= epfRayCacheValid;
	}
	pProbesExtendsChanged = true;
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
	if( pVBOProbeExtendsData ){
		delete [] pVBOProbeExtendsData;
	}
	if( pVBOProbeExtends ){
		pglDeleteBuffers( 1, &pVBOProbeExtends );
	}
	if( pRayCacheProbes ){
		delete [] pRayCacheProbes;
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
		probe.minExtend = -pFieldSize;
		probe.maxExtend = pFieldSize;
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

void deoglGIState::pInvalidateAllRayCaches(){
	int i;
	for( i=0; i<pRealProbeCount; i++ ){
		pProbes[ i ].flags &= ~( epfRayCacheValid | epfDisabled | epfDynamicDisable );
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
	
	pInstances.AnyChanged();
	pInstances.RemoveComponents( pCollideListFiltered );
	pInstances.AddComponents( pCollideListFiltered );
	
// 	pRenderThread.GetLogger().LogInfo( "pTrackInstanceChanges" );
// 	pInstances.DebugPrint();
}

void deoglGIState::pSyncTrackedInstances(){
	pInstances.RemoveComponents( pCollideListFiltered );
	pInstances.AddComponents( pCollideListFiltered );
	
	pInstances.ClearAllChanged();
}

void deoglGIState::pUpdatePosition( const decDVector &position ){
	// find world position closest to the next grid position. if the position is
	// the same no updating is required
	decDVector closestPosition( WorldClosestGrid( position ) );
	if( closestPosition.IsEqualTo( pPosition ) ){
		return;
	}
	
	// skip update if position is too close to the last reference position. this avoids
	// excessive updates if the position oscillates around grid boundaries. this can
	// happen with player cameras affected by view bobbing
	const decDVector refPosDiff( ( position - pLastRefPosition ).Absolute() );
	const bool refPosKeepX = refPosDiff.x < pProbeSpacing.x * 0.8;
	const bool refPosKeepY = refPosDiff.y < pProbeSpacing.y * 0.8;
	const bool refPosKeepZ = refPosDiff.z < pProbeSpacing.z * 0.8;
	
	if( refPosKeepX && refPosKeepY && refPosKeepZ ){
		return;
	}
	
	// modify the closest position to change along each axis only if really necessary
	if( refPosKeepX ){
		closestPosition.x = pPosition.x;
	}
	if( refPosKeepY ){
		closestPosition.y = pPosition.y;
	}
	if( refPosKeepZ ){
		closestPosition.z = pPosition.z;
	}
	
	// invalidate probes shifted out
	const decVector extendsOffset( closestPosition - pPosition );
	const decPoint3 gridOffset( extendsOffset.Multiply( pProbeSpacingInv ).Round() );
	int i;
	
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		
		const decPoint3 coord( LocalGrid2ShiftedGrid( probe.coord ) - gridOffset );
		if( coord >= decPoint3() && coord < pProbeCount ){
			// probe is still valid
			probe.minExtend += extendsOffset;
			probe.maxExtend += extendsOffset;
			continue;
		}
		
		// probe rotated out and becomes invalid
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		probe.minExtend = -pFieldSize;
		probe.maxExtend = pFieldSize;
		
		pClearProbes[ i / 32 ] |= ( uint32_t )1 << ( i % 32 );
		pHasClearProbes = true;
	}
	
	// set the new tracing position
	pPosition = closestPosition;
	pLastRefPosition = position;
	
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

void deoglGIState::pPrepareTraceProbes( const deoglDCollisionFrustum &frustum ){
	INIT_SPECIAL_TIMING
	pFindProbesToUpdate( frustum );
	SPECIAL_TIMER_PRINT(">FindProbesToUpdate")
	pPrepareProbeTexturesAndFBO();
	SPECIAL_TIMER_PRINT(">PrepareProbeTexturesAndFBO")
}

void deoglGIState::pFindProbesToUpdate( const deoglDCollisionFrustum &frustum ){
	const deoglGITraceRays &traceRays = pRenderThread.GetGI().GetTraceRays();
	const int maxUpdateCount = traceRays.GetProbeCount();
	int i;
	
	pUpdateProbeCount = 0;
	
	// performance notes:
	// 
	// using 32*8*32 grid this yields a total of 8192 probes. inside view covers roughly 1/5
	// probes (view single face 1/6, view entire quart 1/4 and thus somewhere in the middle).
	// hence inside probes count is roughly 1600.
	// 
	// for non-RTX/compute supporting hardware this giUpdateSpeed mapping can be used:
	// veryHigh(2048), high(1024), medium(512), low(256), veryLow(128).
	// 
	// using veryHigh(2048) this can update roughly 400 probes outside view and 1600 probes
	// inside view (20%/80% ratio). this setting allows to update visible probes every frame
	// update (or every 2 using multi-volume). this refreshes 20 times per second.
	// 
	// using high(1024) this can update roughly 200 probes outside view and 800 probes inside
	// view. this setting allows to update visible probes every 2 frame updates (or every 4
	// using multi-volume). on 40 FPS this is 0.1s (or 0.2s on 20 FPS). this refreshes
	// 10 times per second.
	// 
	// using medium(512) this can update roughly 100 probes outside view and 400 probes inside
	// view. this setting allows to update visible probes every 4 frame updates (or every 8
	// using multi-volume). on 40 FPS this is 0.2s (or 0.4s on 20 FPS). this refreshes 5 times
	// per second. for medium update speed this is rather okay.
	// 
	// using low(256) this can update roughly 50 probes outside view and 200 probes inside
	// view. this setting allows to update visible probes every 8 frame updates (or every 16
	// using multi-volume). on 40 FPS this is 0.4s (or 0.8s on 20 FPS). this refreshes 2.5 times
	// per second. for low update speed the delay becomes already noticeable if frame rate drops.
	// still 0.5ms to get full screen update is still okay.
	// 
	// using veryLow(128) this can update roughly 25 probes outside view and 100 probes inside
	// view. this setting allows to update visible probes every 16 frame updates (or every 32
	// using multi-volume). on 40 FPS this is 0.8s (or 1.6s on 20 FPS). this refreshes 1.25
	// times per second. for veryLow update speed the delay is noticeable on good FPS and more
	// noticeable on bad FPS. still this is acceptable for certain games especially if
	// performance is improved with it.
	// 
	// using smaller grid this can be speed up but requires more distribution across volumes.
	// using fewer volumes helps in general.
	// 
	// now about the expensive/cheap probe update ratio. using medium update speed of 512
	// probes this can be 256 maximum (factor 2) or 128 maxiimum (factor 4). using factor
	// 2 doubles the inside view probe refresh time after teleporting and thus requires
	// 0.4s on 40 FPS (or 0.8s on 20 FPS). using factor 4 this quadruples the time hence
	// 0.8s on 40 FPS (or 1.6s on 20 FPS). this delay is certainly noticeable.
	// 
	// now expensive probes depends on the game content. with good LODing and not too many
	// objects on screen it should be possible to even update using a ratio of 1. now while
	// moving this invalidates roughly 256 probes per shift. at worst this is 768 if 3 planes
	// shift at the same time. this is though unlikely. it is enough to assume one plane
	// updates per frame. using a factor of 2 would allow to update all invalidated probes
	// at once in the medium scenario without adding a delay. this seems thus a good middle
	// ground.
	
	
	
	// classify probes into inside view and outside view. for this we test the probe position
	// against the frustum planes. this classifies though probes at the broder of the frustum
	// as outside if their position is outside but their extends protrude into the frustum.
	// as a quick fix for this the frustum planes are pushed backwards by half the maximum
	// possible box extend to be on the safe side. since we have now a modified frustum  the
	// near plane has to be checked too. plane normals point inside the frustum
	// 
	// the frustum handed to us is a camera frustum. we would have to translate all probe
	// positions into world space to use it. this is much more expesnvie than moving the
	// plane normals into GI space. the plane normals we can keep since GI space is not
	// rotated. the distance though we need to adjust. we need this kind of calculation:
	//   
	//   distance = planeNormal * planePosition
	//   distance' = planeNormal * (planePosition - giPosition)
	//   distance' = planeNormal * planePosition - planeNormal * giPosition
	//   distance' = distance - planeNormal * giPosition
	// 
	// at the same time determine if probes that are neither disabled nor near static geometry
	// are near dynamic geometry
	const decVector frustumNormalLeft( frustum.GetLeftNormal() );
	const decVector frustumNormalTop( frustum.GetTopNormal() );
	const decVector frustumNormalRight( frustum.GetRightNormal() );
	const decVector frustumNormalBottom( frustum.GetBottomNormal() );
	const decVector frustumNormalNear( frustum.GetNearNormal() );
	
	const float frustumPlaneShift = pProbeSpacing.Length() * 0.5f;
	
	const float frustumDistanceLeft = ( float )( frustum.GetLeftDistance()
		- frustum.GetLeftNormal() * pPosition ) - frustumPlaneShift;
	const float frustumDistanceTop = ( float )( frustum.GetTopDistance()
		- frustum.GetTopNormal() * pPosition ) - frustumPlaneShift;
	const float frustumDistanceRight = ( float )( frustum.GetRightDistance()
		- frustum.GetRightNormal() * pPosition ) - frustumPlaneShift;
	const float frustumDistanceBottom = ( float )( frustum.GetBottomDistance()
		- frustum.GetBottomNormal() * pPosition ) - frustumPlaneShift;
	const float frustumDistanceNear = ( float )( frustum.GetNearDistance()
		- frustum.GetNearNormal() * pPosition ) - frustumPlaneShift;
	
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		
		// inside view
		if( ( frustumNormalNear * probe.position >= frustumDistanceNear )
		&& ( frustumNormalLeft * probe.position >= frustumDistanceLeft )
		&& ( frustumNormalRight * probe.position >= frustumDistanceRight )
		&& ( frustumNormalTop * probe.position >= frustumDistanceTop )
		&& ( frustumNormalBottom * probe.position >= frustumDistanceBottom ) ){
			probe.flags |= epfInsideView;
			
		}else{
			probe.flags &= ~epfInsideView;
		}
	}
	
	// add probes by priority:
	// - invalid probes inside view. expensive updates. at most 1/2 count
	// - invalid probes outside view. expensive updates. at most 1/2 count
	// - valid requiring cache update probes inside view. expensive updates. at most 1/2 count
	// - valid requiring cache update probes outside view. expensive updates. at most 1/2 count
	// - valid requiring dynamic update probes inside view. cheap updates. at most 80% count
	// - valid requiring dynamic update probes outside view. cheap updates. fill up to max count
	const int mask = epfValid | epfInsideView | epfDisabled | epfDynamicDisable | epfRayCacheValid;
	int last = pRealProbeCount;
	
	const int maxUpdateCountExpensive = maxUpdateCount / 2;
	int maxUpdateCountExpensiveOutside = maxUpdateCountExpensive / 5; // 20%
	int maxUpdateCountExpensiveInside = maxUpdateCountExpensive - maxUpdateCountExpensiveOutside; // 80%
	
	pAddUpdateProbes( mask, epfInsideView, last, maxUpdateCountExpensiveInside, maxUpdateCount );
	pAddUpdateProbes( mask, epfValid | epfInsideView, last, maxUpdateCountExpensiveInside, maxUpdateCount );
	
	pAddUpdateProbes( mask, 0, last, maxUpdateCountExpensiveOutside, maxUpdateCount );
	pAddUpdateProbes( mask, epfValid, last, maxUpdateCountExpensiveOutside, maxUpdateCount );
	
	const int maxUpdateCountCheap = maxUpdateCount - pUpdateProbeCount;
	int maxUpdateCountCheapOutside = maxUpdateCountCheap / 5; // 20%
	int maxUpdateCountCheapInside = maxUpdateCountCheap - maxUpdateCountCheapOutside; // 80%
	
	pAddUpdateProbes( mask, epfValid | epfInsideView | epfRayCacheValid, last, maxUpdateCountCheapInside, maxUpdateCount );
	
	maxUpdateCountCheapOutside = maxUpdateCount - pUpdateProbeCount;
	
	pAddUpdateProbes( mask, epfValid | epfRayCacheValid, last, maxUpdateCountCheapOutside, maxUpdateCount );
	
	// finish the aged probe list to make it valid again
	for( i=0; i<pUpdateProbeCount; i++ ){
		pAgedProbes[ last++ ] = pUpdateProbes[ i ];
	}
	
	// update states of probes to update. has to be done after adding the probes to avoid
	// changing flags to affect the filtering
		int specialCountExpensive = 0, specialCountCheap = 0;
	
	for( i=0; i<pUpdateProbeCount; i++ ){
		sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
			if( ( probe.flags & epfRayCacheValid ) == epfRayCacheValid ){
				specialCountCheap++;
			}else{
				specialCountExpensive++;
			}
		
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
		
		probe.flags &= ~( epfDisabled | epfDynamicDisable );
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
#if 0
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
#endif
	
	// determine the required sample image size
	pSampleImageSize.x = traceRays.GetProbesPerLine() * traceRays.GetRaysPerProbe();
	pSampleImageSize.y = ( pUpdateProbeCount - 1 ) / traceRays.GetProbesPerLine() + 1;
// 		pRenderThread.GetLogger().LogInfoFormat( "GIFindProbesUpdate invalid=%d valid=%d", specialCountInvalid, specialCountValid );
}

void deoglGIState::pAddUpdateProbes( uint8_t mask, uint8_t flags, int& lastIndex,
int &remainingMatchCount, int maxUpdateCount ){
	if( remainingMatchCount == 0 || pUpdateProbeCount >= maxUpdateCount ){
		return;
	}
	
	const int endIndex = lastIndex;
	int i;
	
	for( i=0, lastIndex=0; i<endIndex; i++ ){
		sProbe &probe = pProbes[ pAgedProbes[ i ] ];
		
		if( ( probe.flags & mask ) != flags ){
			pAgedProbes[ lastIndex++ ] = pAgedProbes[ i ];
			continue;
		}
		
		// add probe if not exceeding the limits. the aged probes list is adjusted in a way
		// all not added probes are packaged at the beginning. then the added probes are
		// appended to the end of the list to obtain a full list again
		remainingMatchCount--;
		pUpdateProbes[ pUpdateProbeCount++ ] = probe.index;
		
		if( pUpdateProbeCount == maxUpdateCount || remainingMatchCount == 0 ){
			i++;
			break;
		}
	}
	
	while( i < endIndex ){
		pAgedProbes[ lastIndex++ ] = pAgedProbes[ i++ ];
	}
}

void deoglGIState::pPrepareRayCacheProbes(){
	const int raysPerProbe = pRenderThread.GetGI().GetTraceRays().GetRaysPerProbe();
	if( raysPerProbe != pRayCache.GetRaysPerProbe() ){
		pInvalidateAllRayCaches();
		pRayCache.SetRaysPerProbe( raysPerProbe );
	}
	
	pRayCacheProbeCount = 0;
	
	int i;
	for( i=0; i<pUpdateProbeCount; i++ ){
		const sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
		if( ( probe.flags & epfRayCacheValid ) != epfRayCacheValid ){
			pRayCacheProbes[ pRayCacheProbeCount++ ] = pUpdateProbes[ i ];
		}
	}
	
	if( pRayCacheProbeCount > 0 ){
		pPrepareProbeVBO();
	}
	
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
		
		pTexProbeOffset.SetFBOFormat( 4, true );
		pTexProbeOffset.SetSize( width, height );
		pTexProbeOffset.CreateTexture();
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeOffset );
		pFBOProbeOffset.DetachAllImages();
		pFBOProbeOffset.AttachColorTexture( 0, &pTexProbeOffset );
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
		const GLfloat clearDistance[ 4 ] = { 4.0f, 4.0f, 4.0f, 16.0f };
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
	if( ! pVBOProbeOffsets ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOProbeOffsets ) );
		if( ! pVBOProbeOffsets ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeOffsets ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER,
			GI_MAX_PROBE_COUNT * 4 * sizeof( GLfloat ), NULL, GL_STREAM_READ ) );
	}
	
	if( ! pVBOProbeExtends ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOProbeExtends ) );
		if( ! pVBOProbeExtends ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOProbeExtends ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER,
			GI_MAX_PROBE_COUNT * 6 * sizeof( GLfloat ), NULL, GL_STREAM_READ ) );
	}
}

void deoglGIState::pPrepareUBOParameters( int probeCount ) const{
	const deoglGI &gi = pRenderThread.GetGI();
	const deoglGITraceRays &traceRays = gi.GetTraceRays();
	const int raysPerProbe = traceRays.GetRaysPerProbe();
	
	deoglSPBlockUBO &ubo = pRenderThread.GetGI().GetUBOParameter();
	ubo.MapBuffer();
	try{
		ubo.SetParameterDataVec2( deoglGI::eupSampleImageScale,
			1.0f / ( float )pSampleImageSize.x, 1.0f / ( float )pSampleImageSize.y );
		ubo.SetParameterDataInt( deoglGI::eupProbeCount, probeCount );
		ubo.SetParameterDataInt( deoglGI::eupRaysPerProbe, raysPerProbe );
		ubo.SetParameterDataInt( deoglGI::eupProbesPerLine, traceRays.GetProbesPerLine() );
		ubo.SetParameterDataInt( deoglGI::eupIrradianceMapSize, pIrradianceMapSize );
		ubo.SetParameterDataInt( deoglGI::eupDistanceMapSize, pDistanceMapSize );
		ubo.SetParameterDataVec2( deoglGI::eupIrradianceMapScale, pIrradianceMapScale );
		ubo.SetParameterDataVec2( deoglGI::eupDistanceMapScale, pDistanceMapScale );
		ubo.SetParameterDataFloat( deoglGI::eupMaxProbeDistance, pMaxProbeDistance );
		ubo.SetParameterDataFloat( deoglGI::eupDepthSharpness, pDepthSharpness );
		ubo.SetParameterDataVec3( deoglGI::eupGridOrigin, pFieldOrigin );
		ubo.SetParameterDataIVec3( deoglGI::eupGridCoordUnshift, pGridCoordShift );
		ubo.SetParameterDataVec3( deoglGI::eupFieldSize, pFieldSize );
		ubo.SetParameterDataFloat( deoglGI::eupBlendUpdateProbe, 1.0f - pHysteresis );
		ubo.SetParameterDataInt( deoglGI::eupBVHInstanceRootNode, gi.GetBVH().GetIndexRootNode() );
		ubo.SetParameterDataIVec3( deoglGI::eupGridProbeCount, pProbeCount );
		ubo.SetParameterDataVec3( deoglGI::eupGridProbeSpacing, pProbeSpacing );
		ubo.SetParameterDataFloat( deoglGI::eupIrradianceGamma, pIrradianceGamma );
		ubo.SetParameterDataFloat( deoglGI::eupInvIrradianceGamma, 1.0f / pIrradianceGamma );
		ubo.SetParameterDataFloat( deoglGI::eupSelfShadowBias, CalcUBOSelfShadowBias() );
		ubo.SetParameterDataInt( deoglGI::eupCascade, 0 );
		
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
		// to be on the safe side use 45%. the minimum distance to the surface is set to
		// 25% of the smallest spacing. this is rather random value with the aim to
		// increase the visible surface captured by probes but without approaching the
		// maximum offset too much.
		ubo.SetParameterDataVec3( deoglGI::eupMoveMaxOffset, pProbeSpacing * 0.45f );
		ubo.SetParameterDataFloat( deoglGI::eupMoveMinDistToSurface,
			decMath::min( pProbeSpacing.x, pProbeSpacing.y, pProbeSpacing.z ) * 0.25f );
		
		// rays
		ubo.SetParameterDataVec2( deoglGI::eupRayMapScale, pRayCache.GetRayMapScale() );
		
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
