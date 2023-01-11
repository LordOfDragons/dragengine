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
#include "../configuration/deoglConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../utils/collision/deoglDCollisionFrustum.h"

#include <dragengine/common/exceptions.h>


// Class deoglGICascade
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglGICascade::deoglGICascade( deoglGIState &giState, int index,
	const decVector &probeSpacing, const decVector &offset ) :
pGIState( giState ),
pIndex( index ),

pOffset( offset ),
pProbeSpacing( probeSpacing ),
pProbeSpacingInv( 1.0f / probeSpacing.x, 1.0f / probeSpacing.y, 1.0f / probeSpacing.z ),
pFieldSize( probeSpacing.Multiply( decVector( giState.GetGridCoordClamp() ) ) ),
pFieldOrigin( pFieldSize * -0.5f ),
pPositionClamp( pFieldSize ),
pDynamicHalfEnlarge( probeSpacing * 1.9f * 0.5f ), // enlarge = spacing * (1 + 0.45 * 2)
pStaticHalfEnlarge( 0.05f, 0.05f, 0.05f ),
pFillUpUpdatesWithExpensiveProbes( false ),
pSkyShadowCascade( index ),

pMaxDetectionRange( 50.0f ),
pDetectionBox( pFieldSize * 0.5f + decVector( pMaxDetectionRange, pMaxDetectionRange, pMaxDetectionRange ) ),

// according to comment in source code this should be slightly larger than
// the length of the diagonal across cells
pMaxProbeDistance( probeSpacing.Length() * 1.5f ),

pPosition( offset ),
pLastRefPosition( offset ),

pProbes( NULL ),
pAgedProbes( NULL ),
pHasInvalidProbesInsideView( true ),
pRequiresFullUpdateInsideView( true ),

pClearProbes( NULL ),
pClearProbeCount( giState.GetRealProbeCount() / 32 ),
pHasClearProbes( false ),

pUpdateProbes( NULL ),
pUpdateProbeCount( 0 ),

pRayCacheProbes( NULL ),
pRayCacheProbeCount( 0 )
{
	try{
		pInitProbes();
		pClearProbes = new uint32_t[ pClearProbeCount ];
		pUpdateProbes = new uint16_t[ GI_MAX_PROBE_COUNT ];
		#ifdef GI_USE_RAY_CACHE
		pRayCacheProbes = new uint16_t[ GI_MAX_PROBE_COUNT ];
		#endif
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGICascade::~deoglGICascade(){
	pCleanUp();
}



// Management
///////////////

void deoglGICascade::SetFillUpUpdatesWithExpensiveProbes( bool fillUp ){
	pFillUpUpdatesWithExpensiveProbes = fillUp;
}

void deoglGICascade::SetSkyShadowCascade( int cascade ){
	pSkyShadowCascade = cascade;
}

decPoint3 deoglGICascade::ProbeIndex2GridCoord( int index ) const{
	decPoint3 coord;
	coord.y = index / pGIState.GetStrideProbeCount();
	index -= pGIState.GetStrideProbeCount() * coord.y;
	coord.z = index / pGIState.GetProbeCount().x;
	coord.x = index - pGIState.GetProbeCount().x * coord.z;
	return coord;
}

int deoglGICascade::GridCoord2ProbeIndex( const decPoint3 &coord ) const{
	return pGIState.GetStrideProbeCount() * coord.y + pGIState.GetProbeCount().x * coord.z + coord.x;
}

decVector deoglGICascade::Grid2Local( const decPoint3 &coord ) const{
	return pProbeSpacing.Multiply( decVector( coord ) ) + pFieldOrigin;
}

decPoint3 deoglGICascade::World2Grid( const decDVector &position ) const{
	return ( position - pPosition - decDVector( pFieldOrigin ) ).Multiply( pProbeSpacingInv ).Round();
}

decDVector deoglGICascade::Grid2World( const decPoint3 &grid ) const{
	return pPosition + decDVector( pFieldOrigin ) + decDVector( grid ).Multiply( pProbeSpacing );
}

decDVector deoglGICascade::WorldClosestGrid( const decDVector &position ) const{
	const decDVector pos( position - pOffset );
	decDVector result( ( double )pProbeSpacing.x * floor( pos.x * ( double )pProbeSpacingInv.x ),
		( double )pProbeSpacing.y * floor( pos.y * ( double )pProbeSpacingInv.y ),
		( double )pProbeSpacing.z * floor( pos.z * ( double )pProbeSpacingInv.z ) );
	
	const decDVector halfGrid( decDVector( pProbeSpacing ) * 0.5 );
	if( pos.x - result.x >= halfGrid.x ){
		result.x += ( double )pProbeSpacing.x;
	}
	if( pos.y - result.y >= halfGrid.y ){
		result.y += ( double )pProbeSpacing.y;
	}
	if( pos.z - result.z >= halfGrid.z ){
		result.z += ( double )pProbeSpacing.z;
	}
	
	return result + pOffset;
}

decPoint3 deoglGICascade::LocalGrid2ShiftedGrid( const decPoint3 &coord ) const{
	decPoint3 shifted( coord + pGridCoordShift );
	shifted.x %= pGIState.GetProbeCount().x;
	shifted.y %= pGIState.GetProbeCount().y;
	shifted.z %= pGIState.GetProbeCount().z;
	return shifted;
}

decPoint3 deoglGICascade::ShiftedGrid2LocalGrid( const decPoint3 &coord ) const{
	decPoint3 local( coord - pGridCoordShift + pGIState.GetProbeCount() );
	local.x %= pGIState.GetProbeCount().x;
	local.y %= pGIState.GetProbeCount().y;
	local.z %= pGIState.GetProbeCount().z;
	return local;
}



void deoglGICascade::SetRequiresFullUpdateInsideView( bool requiresUpdate ){
	pRequiresFullUpdateInsideView = requiresUpdate;
}



void deoglGICascade::ClearClearProbes(){
	memset( pClearProbes, 0, pClearProbeCount * sizeof( uint32_t ) );
	pHasClearProbes = false;
}

void deoglGICascade::PrepareUBOClearProbes( deoglSPBlockUBO &ubo ) const{
	const deoglSPBMapBuffer mapped( ubo );
	const int count = pClearProbeCount / 4;
	int i, j;
	
	for( i=0, j=0; i<count; i++, j+=4 ){
		ubo.SetParameterDataArrayUVec4( 0, i, pClearProbes[ j ],
			pClearProbes[ j + 1 ], pClearProbes[ j + 2 ], pClearProbes[ j + 3 ] );
	}
}



void deoglGICascade::Invalidate(){
	const int count = pGIState.GetRealProbeCount();
	uint16_t i;
	
	for( i=0; i<count; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		probe.minExtend = -pDetectionBox;
		probe.maxExtend = pDetectionBox;
	}
	
	pHasInvalidProbesInsideView = true;
	
	ClearClearProbes();
}

void deoglGICascade::InvalidateArea( const decDVector &minExtend, const decDVector &maxExtend, bool hard ){
	const decVector lminExtend( minExtend - pPosition - pStaticHalfEnlarge );
	const decVector lmaxExtend( maxExtend - pPosition + pStaticHalfEnlarge );
	if( ! ( lmaxExtend > -pDetectionBox && lminExtend < pDetectionBox ) ){
		return;
	}
	
	const int flagMask = hard ? 0 : epfDisabled;
	const int flagFilter = 0;
	const int count = pGIState.GetRealProbeCount();
	int i;
// 		decString debugText( "InvalidateArea: (%g,%g,%g) (%g,%g,%g);" );
	
	for( i=0; i<count; i++ ){
		sProbe &probe = pProbes[ i ];
		if( ( probe.flags & flagMask ) == flagFilter
		&& probe.maxExtend > lminExtend && probe.minExtend < lmaxExtend ){
// 				pGIState.GetRenderThread().GetLogger().LogInfoFormat("Cascade(%d) InvalidateArea Probe(%d,%d,%d) (%g,%g,%g) (%g,%g,%g)",
// 					pIndex, probe.shiftedCoord.x, probe.shiftedCoord.y, probe.shiftedCoord.z,
// 					probe.minExtend.x, probe.minExtend.y, probe.minExtend.z,
// 					probe.maxExtend.x, probe.maxExtend.y, probe.maxExtend.z);
			probe.flags &= ~( epfDisabled | epfNearGeometry | epfRayCacheValid | epfDynamicDisable );
			probe.offset.SetZero();
			probe.countOffsetMoved = 0;
// 				debugText.AppendFormat( " %d,", i );
		}
	}
		
// 		pRenderThread.GetLogger().LogInfoFormat( debugText, lminExtend.x, lminExtend.y, lminExtend.z, lmaxExtend.x, lmaxExtend.y, lmaxExtend.z );
}

void deoglGICascade::TouchDynamicArea( const decDVector &minExtend, const decDVector &maxExtend ){
	const decVector lminExtend( ( minExtend - pPosition ).ToVector() - pDynamicHalfEnlarge );
	const decVector lmaxExtend( ( maxExtend - pPosition ).ToVector() + pDynamicHalfEnlarge );
	if( ! ( lmaxExtend > pFieldOrigin && lminExtend < -pFieldOrigin ) ){
		return;
	}
	
	const decPoint3 from( ( ( lminExtend - pFieldOrigin ).Multiply( pProbeSpacingInv ) ).
		Clamped( decPoint3(), pGIState.GetGridCoordClamp() ) );
	const decPoint3 to( ( ( lmaxExtend - pFieldOrigin ).Multiply( pProbeSpacingInv ) ).
		Clamped( decPoint3(), pGIState.GetGridCoordClamp() ) );
	
	decPoint3 i;
	for( i.y=from.y; i.y<=to.y; i.y++ ){
		for( i.z=from.z; i.z<=to.z; i.z++ ){
			for( i.x=from.x; i.x<=to.x; i.x++ ){
				sProbe &probe = pProbes[ GridCoord2ProbeIndex( ShiftedGrid2LocalGrid( i ) ) ];
				if( probe.position > lminExtend && probe.position < lmaxExtend ){
					probe.flags &= ~epfDynamicDisable;
				}
			}
		}
	}
}

void deoglGICascade::ValidatedRayCaches(){
	int i;
	for( i=0; i<pRayCacheProbeCount; i++ ){
		pProbes[ pRayCacheProbes[ i ] ].flags |= epfRayCacheValid;
	}
}

void deoglGICascade::InvalidateAllRayCaches(){
	const int count = pGIState.GetRealProbeCount();
	int i;
	for( i=0; i<count; i++ ){
		pProbes[ i ].flags &= ~( epfRayCacheValid | epfDisabled | epfDynamicDisable );
	}
}



// #define DO_SPECIAL_TIMING 1
#ifdef DO_SPECIAL_TIMING
#include <dragengine/common/utils/decTimer.h>
#define INIT_SPECIAL_TIMING decTimer sttimer;
#define SPECIAL_TIMER_PRINT(w) pGIState.GetRenderThread().GetLogger().LogInfoFormat("GICascade." w "=%dys", (int)(sttimer.GetElapsedTime()*1e6f));
#else
#define INIT_SPECIAL_TIMING
#define SPECIAL_TIMER_PRINT(w)
#endif

void deoglGICascade::UpdatePosition( const decDVector &position ){
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
	const bool refPosKeepX = refPosDiff.x < ( double )pProbeSpacing.x * 0.8;
	const bool refPosKeepY = refPosDiff.y < ( double )pProbeSpacing.y * 0.8;
	const bool refPosKeepZ = refPosDiff.z < ( double )pProbeSpacing.z * 0.8;
	
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
	// NOTE extends offset moves in the opposite direction
	const decVector extendsOffset( closestPosition - pPosition );
	const decPoint3 gridOffset( extendsOffset.Multiply( pProbeSpacingInv ).Round() );
	const int realProbeCount = pGIState.GetRealProbeCount();
	const decPoint3 &probeCount = pGIState.GetProbeCount();
	int i;
	
	for( i=0; i<realProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		
		const decPoint3 coord( probe.shiftedCoord - gridOffset );
		if( coord >= decPoint3() && coord < probeCount ){
			// probe is still valid
			probe.minExtend -= extendsOffset;
			probe.maxExtend -= extendsOffset;
			continue;
		}
		
		// probe rotated out and becomes invalid
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		probe.minExtend = -pDetectionBox;
		probe.maxExtend = pDetectionBox;
		
		pHasInvalidProbesInsideView = true;
		
		pClearProbes[ i / 32 ] |= ( uint32_t )1 << ( i % 32 );
		pHasClearProbes = true;
	}
	
	// set the new tracing position
	pPosition = closestPosition;
	
	if( ! refPosKeepX ){
		pLastRefPosition.x = position.x;
	}
	if( ! refPosKeepY ){
		pLastRefPosition.y = position.y;
	}
	if( ! refPosKeepZ ){
		pLastRefPosition.z = position.z;
	}
	
	// update shift
	pGridCoordShift -= gridOffset;
	pGridCoordShift.x %= probeCount.x;
	pGridCoordShift.y %= probeCount.y;
	pGridCoordShift.z %= probeCount.z;
	pGridCoordShift += probeCount;
	pGridCoordShift.x %= probeCount.x;
	pGridCoordShift.y %= probeCount.y;
	pGridCoordShift.z %= probeCount.z;
	
	// update probe cordinates. has to come after adjusting the parameters
	for( i=0; i<realProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.shiftedCoord = LocalGrid2ShiftedGrid( probe.coord );
		probe.position = Grid2Local( probe.shiftedCoord );
	}
}

void deoglGICascade::FindProbesToUpdate( const deoglDCollisionFrustum &frustum ){
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
	
	const int realProbeCount = pGIState.GetRealProbeCount();
	int i;
	for( i=0; i<realProbeCount; i++ ){
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
	
	pUpdateProbeCount = 0;
	
	if( pRequiresFullUpdateInsideView ){
		pFindProbesToUpdateFullUpdateInsideView();
		pRequiresFullUpdateInsideView = false;
		
	}else{
		pFindProbesToUpdateRegular();
	}
	
	// update states of probes to update. has to be done after adding the probes to avoid
	// changing flags to affect the filtering
	for( i=0; i<pUpdateProbeCount; i++ ){
		sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
		
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
}

void deoglGICascade::PrepareRayCacheProbes(){
	pRayCacheProbeCount = 0;
	
	int i;
	for( i=0; i<pUpdateProbeCount; i++ ){
		const sProbe &probe = pProbes[ pUpdateProbes[ i ] ];
		if( ( probe.flags & epfRayCacheValid ) != epfRayCacheValid ){
			pRayCacheProbes[ pRayCacheProbeCount++ ] = pUpdateProbes[ i ];
		}
	}
}



float deoglGICascade::CalcUBOSelfShadowBias() const{
	return pGIState.GetSelfShadowBias() * decMath::min( pProbeSpacing.x, pProbeSpacing.y, pProbeSpacing.z );
}

void deoglGICascade::UpdateUBOParameters( deoglSPBlockUBO &ubo, int probeCount, const deoglGIBVH &bvh ) const{
	const deoglGI &gi = pGIState.GetRenderThread().GetGI();
	const deoglGITraceRays &traceRays = gi.GetTraceRays();
	const int raysPerProbe = traceRays.GetRaysPerProbe();
	const deoglSPBMapBuffer mapped( ubo );
	
	ubo.SetParameterDataVec2( deoglGI::eupSampleImageScale,
		1.0f / ( float )pGIState.GetSampleImageSize().x,
		1.0f / ( float )pGIState.GetSampleImageSize().y );
	ubo.SetParameterDataInt( deoglGI::eupProbeCount, probeCount );
	ubo.SetParameterDataInt( deoglGI::eupRaysPerProbe, raysPerProbe );
	ubo.SetParameterDataInt( deoglGI::eupProbesPerLine, traceRays.GetProbesPerLine() );
	ubo.SetParameterDataInt( deoglGI::eupIrradianceMapSize, pGIState.GetIrradianceMapSize() );
	ubo.SetParameterDataInt( deoglGI::eupDistanceMapSize, pGIState.GetDistanceMapSize() );
	ubo.SetParameterDataVec2( deoglGI::eupIrradianceMapScale, pGIState.GetIrradianceMapScale() );
	ubo.SetParameterDataVec2( deoglGI::eupDistanceMapScale, pGIState.GetDistanceMapScale() );
	ubo.SetParameterDataFloat( deoglGI::eupMaxProbeDistance, pMaxProbeDistance );
	ubo.SetParameterDataFloat( deoglGI::eupDepthSharpness, pGIState.GetDepthSharpness() );
	ubo.SetParameterDataVec3( deoglGI::eupGridOrigin, pFieldOrigin );
	ubo.SetParameterDataIVec3( deoglGI::eupGridCoordUnshift, pGridCoordShift );
	ubo.SetParameterDataVec3( deoglGI::eupFieldSize, pFieldSize );
	ubo.SetParameterDataFloat( deoglGI::eupBlendUpdateProbe, 1.0f - pGIState.GetHysteresis() );
	ubo.SetParameterDataInt( deoglGI::eupBVHInstanceRootNode, bvh.GetIndexRootNode() );
	ubo.SetParameterDataIVec3( deoglGI::eupGridProbeCount, pGIState.GetProbeCount() );
	ubo.SetParameterDataVec3( deoglGI::eupGridProbeSpacing, pProbeSpacing );
	ubo.SetParameterDataFloat( deoglGI::eupIrradianceGamma, pGIState.GetIrradianceGamma() );
	ubo.SetParameterDataFloat( deoglGI::eupInvIrradianceGamma, 1.0f / pGIState.GetIrradianceGamma() );
	ubo.SetParameterDataFloat( deoglGI::eupSelfShadowBias, CalcUBOSelfShadowBias() );
	ubo.SetParameterDataInt( deoglGI::eupCascade, pIndex );
	ubo.SetParameterDataVec3( deoglGI::eupDetectionBox, pDetectionBox );
	ubo.SetParameterDataVec3( deoglGI::eupBVHOffset, ( pPosition - bvh.GetPosition() ).ToVector() );
	
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
	ubo.SetParameterDataVec3( deoglGI::eupMoveMaxOffset, pProbeSpacing * 0.49f /*0.45f*/ );
	ubo.SetParameterDataFloat( deoglGI::eupMoveMinDistToSurface,
		decMath::min( pProbeSpacing.x, pProbeSpacing.y, pProbeSpacing.z ) * 0.25f );
	
	// rays
	ubo.SetParameterDataVec2( deoglGI::eupRayMapScale, pGIState.GetRayCache().GetRayMapScale() );
}

void deoglGICascade::UpdateUBOProbePosition( deoglSPBlockUBO &ubo ) const{
	pUpdateUBOProbePosition( ubo, pUpdateProbes, pUpdateProbeCount );
}

void deoglGICascade::UpdateUBOProbeIndices( deoglSPBlockUBO &ubo ) const{
	pUpdateUBOProbeIndices( ubo, pUpdateProbes, pUpdateProbeCount );
}

void deoglGICascade::UpdateUBOProbeIndicesRayCache( deoglSPBlockUBO &ubo ) const{
	pUpdateUBOProbeIndices( ubo, pRayCacheProbes, pRayCacheProbeCount );
}

void deoglGICascade::UpdateUBOProbePositionRayCache( deoglSPBlockUBO &ubo ) const{
	pUpdateUBOProbePosition( ubo, pRayCacheProbes, pRayCacheProbeCount );
}

void deoglGICascade::UpdateProbeOffsetFromShader( const float *data ){
	INIT_SPECIAL_TIMING
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
	SPECIAL_TIMER_PRINT("UpdateProbeOffsetFromShader: > > Offsets")
	
	// update all has probe flags. this is done here and nowhere else since this method is
	// called if one or more probes are updated. only in this situation these flags can
	// potentially change and have to be updated
	pUpdateHasProbeFlags();
	SPECIAL_TIMER_PRINT("UpdateProbeOffsetFromShader: > > Flags")
}

void deoglGICascade::UpdateProbeExtendsFromShader( const float *data ){
	INIT_SPECIAL_TIMING
	int i;
	for( i=0; i<pRayCacheProbeCount; i++, data+=6 ){
		sProbe &probe = pProbes[ pRayCacheProbes[ i ] ];
		probe.minExtend.Set( data[ 0 ], data[ 1 ], data[ 2 ] );
		probe.maxExtend.Set( data[ 3 ], data[ 4 ], data[ 5 ] );
	}
	SPECIAL_TIMER_PRINT("UpdateProbeExtendsFromShader: > > Extends")
	
	/*
	deoglRTLogger &l = pGIState.GetRenderThread().GetLogger();
	l.LogInfo("UpdateProbeExtendsFromShader:");
	for( i=0; i<pRayCacheProbeCount; i++ ){
		sProbe &probe = pProbes[ pRayCacheProbes[ i ] ];
		l.LogInfoFormat("- %d(%d,%d,%d) (%g,%g,%g) (%g,%g,%g)(%g,%g,%g)", probe.index,
			probe.shiftedCoord.x, probe.shiftedCoord.y, probe.shiftedCoord.z,
			probe.position.x, probe.position.y, probe.position.z,
			probe.minExtend.x, probe.minExtend.y, probe.minExtend.z,
			probe.maxExtend.x, probe.maxExtend.y, probe.maxExtend.z);
	}
	*/
}



// Private Functions
//////////////////////

void deoglGICascade::pCleanUp(){
	if( pProbes ){
		delete [] pProbes;
	}
	if( pAgedProbes ){
		delete [] pAgedProbes;
	}
	if( pClearProbes ){
		delete [] pClearProbes;
	}
	if( pRayCacheProbes ){
		delete [] pRayCacheProbes;
	}
	if( pUpdateProbes ){
		delete [] pUpdateProbes;
	}
}

void deoglGICascade::pInitProbes(){
	const int count = pGIState.GetRealProbeCount();
	uint16_t i;
	
	pProbes = new sProbe[ count ];
	pAgedProbes = new uint16_t[ count ];
	
	for( i=0; i<count; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.index = i;
		probe.flags = 0;
		probe.offset.SetZero();
		probe.countOffsetMoved = 0;
		probe.minExtend = -pDetectionBox;
		probe.maxExtend = pDetectionBox;
		probe.coord = ProbeIndex2GridCoord( i );
		probe.shiftedCoord = LocalGrid2ShiftedGrid( probe.coord );
		probe.position = Grid2Local( probe.shiftedCoord );
		
		pAgedProbes[ i ] = i;
	}
	
	pHasInvalidProbesInsideView = true;
}

void deoglGICascade::pFindProbesToUpdateFullUpdateInsideView(){
	// update all inside probes regardless of update count selected by the user
	const int realProbeCount = pGIState.GetRealProbeCount();
	const int maxUpdateCount = GI_MAX_PROBE_COUNT;
	int last = realProbeCount;
	
	int fillUpCount = maxUpdateCount;
	pAddUpdateProbes( epfInsideView, epfInsideView, last, fillUpCount, maxUpdateCount );
	
	// finish the aged probe list to make it valid again
	int i;
	for( i=0; i<pUpdateProbeCount; i++ ){
		pAgedProbes[ last++ ] = pUpdateProbes[ i ];
	}
}

void deoglGICascade::pFindProbesToUpdateRegular(){
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
	
	const int maxUpdateCount = pGIState.GetRenderThread().GetGI().GetTraceRays().GetProbeCount();
	const int realProbeCount = pGIState.GetRealProbeCount();
	
	// add probes by priority
	const int mask = epfValid | epfInsideView | epfDisabled | epfDynamicDisable | epfRayCacheValid;
	int last = realProbeCount;
	
	// invalid probes inside view. expensive updates but cause incomplete lighting.
	// add all such probes ignoring config settings up to the maximum supported count
	int maxUpdateCountExpensiveInside = GI_MAX_PROBE_COUNT;
	pAddUpdateProbes( mask, epfInsideView, last, maxUpdateCountExpensiveInside, GI_MAX_PROBE_COUNT );
	
	// - invalid probes inside view. expensive updates. at most 50% count
	// - valid requiring cache update probes inside view. expensive updates. at most 50% count
	const int maxUpdateCountExpensive = maxUpdateCount * 0.5f; // 50%
	int maxUpdateCountExpensiveOutside = maxUpdateCountExpensive * 0.2f; // 20%
	maxUpdateCountExpensiveInside = maxUpdateCountExpensive - maxUpdateCountExpensiveOutside // 80%
		- pUpdateProbeCount; // minus already used count
	
// 	pAddUpdateProbes( mask, epfInsideView, last, maxUpdateCountExpensiveInside, maxUpdateCount );
	pAddUpdateProbes( mask, epfValid | epfInsideView, last, maxUpdateCountExpensiveInside, maxUpdateCount );
	
	// - invalid probes outside view. expensive updates. at most 50% count
	// - valid requiring cache update probes outside view. expensive updates. at most 50% count
	pAddUpdateProbes( mask, 0, last, maxUpdateCountExpensiveOutside, maxUpdateCount );
	pAddUpdateProbes( mask, epfValid, last, maxUpdateCountExpensiveOutside, maxUpdateCount );
	
	// - valid requiring dynamic update probes inside view. cheap updates. at most 80% count
	const int maxUpdateCountCheap = maxUpdateCount - pUpdateProbeCount;
	int maxUpdateCountCheapOutside = maxUpdateCountCheap * 0.2f; // 20%
	int maxUpdateCountCheapInside = maxUpdateCountCheap - maxUpdateCountCheapOutside; // 80%
	
	pAddUpdateProbes( mask, epfValid | epfInsideView | epfRayCacheValid, last, maxUpdateCountCheapInside, maxUpdateCount );
	
	// - valid requiring dynamic update probes outside view. cheap updates. fill up to max count
	int fillUpCount = maxUpdateCount - pUpdateProbeCount;
	pAddUpdateProbes( mask, epfValid | epfRayCacheValid, last, fillUpCount, maxUpdateCount );
	
	// if not all update slots are used fill up with expensive updates if important enough.
	// this situation happens usually only if the cascade teleported to a new location where
	// there are many expensive probes and next to no cheap probes. in this situation filling
	// up with expensive probes potentially causes a 1-2 frame short hickup but prevents bad
	// lighting results.
	// 
	// enabling this can be adjusted on a per cascade basis. for large cascades doing this
	// helps to ensure smaller cascades can fall back to some kind of lighting. for small
	// cascades this can be disabled to speed up by using large cascade results
	if( pFillUpUpdatesWithExpensiveProbes ){
		// - invalid probes. expensive updates but important since it avoids lighting gaps
		const int maskGrouped = epfValid | epfDisabled | epfDynamicDisable | epfRayCacheValid;
		
		pAddUpdateProbes( maskGrouped, 0, last, fillUpCount, maxUpdateCount );
		
		// - valid requiring cache update probes. expensive updates but not important enough
// 		pAddUpdateProbes( maskGrouped, epfValid, last, fillUpCount, maxUpdateCount );
	}
	
	// finish the aged probe list to make it valid again
	int i;
	for( i=0; i<pUpdateProbeCount; i++ ){
		pAgedProbes[ last++ ] = pUpdateProbes[ i ];
	}
}

void deoglGICascade::pAddUpdateProbes( uint8_t mask, uint8_t flags, int &lastIndex,
int &remainingMatchCount, int maxUpdateCount ){
	if( remainingMatchCount <= 0 || pUpdateProbeCount >= maxUpdateCount ){
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

void deoglGICascade::pUpdateUBOProbePosition( deoglSPBlockUBO &ubo, const uint16_t *indices, int count ) const{
	const deoglSPBMapBuffer mapped( ubo );
	int i;
	for( i=0; i<count; i++ ){
		const sProbe &p = pProbes[ indices[ i ] ];
		ubo.SetParameterDataArrayVec4( 0, i, p.position + p.offset, ( float )p.flags );
	}
}

void deoglGICascade::pUpdateUBOProbeIndices( deoglSPBlockUBO &ubo, const uint16_t *indices, int count ) const{
	const deoglSPBMapBuffer mapped( ubo );
	const int realCount = ( count - 1 ) / 4 + 1;
	int i, j;
	
	for( i=0, j=0; i<realCount; i++, j+=4 ){
		ubo.SetParameterDataArrayIVec4( 0, i,
			j < count ? indices[ j ] : 0,
			j + 1 < count ? indices[ j + 1 ] : 0,
			j + 2 < count ? indices[ j + 2 ] : 0,
			j + 3 < count ? indices[ j + 3 ] : 0 );
	}
}

void deoglGICascade::pUpdateHasProbeFlags(){
	const int count = pGIState.GetRealProbeCount();
	int i;
	
	pHasInvalidProbesInsideView = false;
	
	for( i=0; i<count; i++ ){
		if( ( pProbes[ i ].flags & ( epfValid | epfInsideView ) ) == epfInsideView ){
			pHasInvalidProbesInsideView = true;
			break;
		}
	}
}
