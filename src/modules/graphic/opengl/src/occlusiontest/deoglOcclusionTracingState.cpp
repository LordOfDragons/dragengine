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

#include "deoglOcclusionTracing.h"
#include "deoglOcclusionTracingState.h"
#include "../capabilities/deoglCapabilities.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../rendering/deoglRenderOcclusion.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTracingState::deoglOcclusionTracingState( deoglOcclusionTracing &tracing ) :
pTracing( tracing ),
pProbes( NULL ),
pElapsedUpdateProbe( 0.0f ),
pUpdateProbeInterval( 0.1f ),
pUpdateProbes( NULL ),
pUpdateProbeCount( 0 ),
pWeightedProbes( NULL ),
pWeightedProbeBinSize( tracing.GetRealProbeCount() / 20 ),
pWeightedProbeBinCount( 20 ),
pWeightedProbeBinClamp( pWeightedProbeBinCount - 1 ),
pWeightedProbeBinProbeCounts( NULL ),
pTexProbeOcclusion( tracing.GetRenderThread() ),
pTexProbeDistance( tracing.GetRenderThread() ),
pFBOProbeOcclusion( tracing.GetRenderThread(), false ),
pFBOProbeDistance( tracing.GetRenderThread(), false ),
pClearMaps( true )
{
	try{
		pInitProbes();
		pUpdateProbes = new sProbe*[ tracing.GetMaxUpdateProbeCount() ];
		pWeightedProbes = new sProbe*[ tracing.GetRealProbeCount() ];
		pWeightedProbeBinProbeCounts = new int[ pWeightedProbeBinCount ];
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglOcclusionTracingState::~deoglOcclusionTracingState(){
	pCleanUp();
}



// Management
///////////////

// #define DO_TIMING 1

decPoint3 deoglOcclusionTracingState::World2Grid( const decDVector &position ) const{
	return ( position - pPosition - decDVector( pTracing.GetProbeOrigin() ) )
		.Multiply( pTracing.GetProbeSpacingInverse() ).Round();
}

decDVector deoglOcclusionTracingState::Grid2World( const decPoint3 &grid ) const{
	return pPosition + decDVector( pTracing.GetProbeOrigin() )
		+ decDVector( grid ).Multiply( pTracing.GetProbeSpacing() );
}

decDVector deoglOcclusionTracingState::WorldClosestGrid( const decDVector &position ) const{
	const decVector &probeSpacing = pTracing.GetProbeSpacing();
	const decVector &probeSpacingInv = pTracing.GetProbeSpacingInverse();
	decDVector result( ( double )probeSpacing.x * floor( position.x * ( double )probeSpacingInv.x ),
		( double )probeSpacing.y * floor( position.y * ( double )probeSpacingInv.y ),
		( double )probeSpacing.z * floor( position.z * ( double )probeSpacingInv.z ) );
	
	const decDVector halfGrid( decDVector( probeSpacing ) * 0.5 );
	if( position.x - result.x >= halfGrid.x ){
		result.x += ( double )probeSpacing.x;
	}
	if( position.y - result.y >= halfGrid.y ){
		result.y += ( double )probeSpacing.y;
	}
	if( position.z - result.z >= halfGrid.z ){
		result.z += ( double )probeSpacing.z;
	}
	
	return result;
}

decPoint3 deoglOcclusionTracingState::LocalGrid2ShiftedGrid( const decPoint3 &coord ) const{
	const decPoint3 &probeCount = pTracing.GetProbeCount();
	decPoint3 shifted( coord + pGridCoordShift );
	shifted.x %= probeCount.x;
	shifted.y %= probeCount.y;
	shifted.z %= probeCount.z;
	return shifted;
}

decPoint3 deoglOcclusionTracingState::ShiftedGrid2LocalGrid( const decPoint3 &coord ) const{
	const decPoint3 &probeCount = pTracing.GetProbeCount();
	decPoint3 local( coord - pGridCoordShift + probeCount );
	local.x %= probeCount.x;
	local.y %= probeCount.y;
	local.z %= probeCount.z;
	return local;
}



void deoglOcclusionTracingState::Update( deoglRWorld &world, const decDVector &cameraPosition,
const decDMatrix &cameraMatrix, float fovX, float fovY ){
	#ifdef DO_TIMING
	decTimer timer;
	#endif
	
	pUpdateProbeCount = 0;
	
	pUpdatePosition( cameraPosition );
	
	pTracing.PrepareRayTracing( world, pPosition );
	
	const decMatrix matrixView( decDMatrix::CreateTranslation( pPosition ) * cameraMatrix );
	pTraceProbes( matrixView, fovX, fovY );
	
	#ifdef DO_TIMING
	pRenderThread.GetLogger().LogInfoFormat("OcclusionTracing: Elapsed %.1fys", timer.GetElapsedTime() * 1e6f);
	#endif
}

void deoglOcclusionTracingState::Invalidate(){
	const int probeCount = pTracing.GetRealProbeCount();
	int i;
	
	for( i=0; i<probeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.age = 0;
		probe.blendFactor = 1.0f;
		probe.valid = false;
	}
	
	pClearMaps = true;
}



// Private Functions
//////////////////////

void deoglOcclusionTracingState::pCleanUp(){
	if( pUpdateProbes ){
		delete [] pUpdateProbes;
	}
	if( pWeightedProbes ){
		delete [] pWeightedProbes;
	}
	if( pWeightedProbeBinProbeCounts ){
		delete [] pWeightedProbeBinProbeCounts;
	}
}

void deoglOcclusionTracingState::pInitProbes(){
	const int probeCount = pTracing.GetRealProbeCount();
	pProbes = new sProbe[ probeCount ];
	
	int i;
	for( i=0; i<probeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.index = i;
		probe.blendFactor = 1.0f;
		probe.age = 0;
		probe.valid = false;
		probe.coord = pTracing.ProbeIndex2GridCoord( i );
	}
}

void deoglOcclusionTracingState::pUpdatePosition( const decDVector &position ){
	// find world position closest to the next grid position. if the position is
	// the same no updating is required
	const decDVector closestPosition( WorldClosestGrid( position ) );
	if( closestPosition.IsEqualTo( pPosition ) ){
		return;
	}
	
	// invalidate probes shifted out
	const decPoint3 gridOffset( ( closestPosition - pPosition )
		.Multiply( pTracing.GetProbeSpacingInverse() ).Round() );
	
	const int realProbeCount = pTracing.GetRealProbeCount();
	const decPoint3 &probeCount = pTracing.GetProbeCount();
	int i;
	
	for( i=0; i<realProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		const decPoint3 coord( LocalGrid2ShiftedGrid( probe.coord ) - gridOffset );
		if( coord >= decPoint3() && coord < probeCount ){
			continue;
		}
		
		probe.age = 0;
		probe.blendFactor = 1.0f;
		probe.valid = false;
	}
	
	// set the new tracing position
	pPosition = closestPosition;
	
	// update shift
	pGridCoordShift -= gridOffset;
	pGridCoordShift.x %= probeCount.x;
	pGridCoordShift.y %= probeCount.y;
	pGridCoordShift.z %= probeCount.z;
	pGridCoordShift += probeCount;
	pGridCoordShift.x %= probeCount.x;
	pGridCoordShift.y %= probeCount.y;
	pGridCoordShift.z %= probeCount.z;
}

void deoglOcclusionTracingState::pTraceProbes( const decMatrix &matrixView, float fovX, float fovY ){
	pElapsedUpdateProbe += pTimerUpdateProbe.GetElapsedTime();
	if( pElapsedUpdateProbe < pUpdateProbeInterval ){
// 		return; // how to use this properly? we track snapped position already
	}
	
	// keep only the remainder. this avoids updates to pile up if framerate drops or staggers
	pElapsedUpdateProbe = fmodf( pElapsedUpdateProbe, pUpdateProbeInterval );
	
	pAgeProbes();
	pFindProbesToUpdate( matrixView, fovX, fovY );
	pPrepareProbeTexturesAndFBO();
	pPrepareUBOState();
	pTracing.GetRenderThread().GetRenderers().GetOcclusion().RenderOcclusionTraceProbes( *this );
}

void deoglOcclusionTracingState::pAgeProbes(){
	const int probeCount = pTracing.GetRealProbeCount();
	int i;
	
	for( i=0; i<probeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.age++;
		probe.shiftedCoord = LocalGrid2ShiftedGrid( probe.coord );
		probe.position = pTracing.Grid2Local( probe.shiftedCoord );
	}
}

void deoglOcclusionTracingState::pFindProbesToUpdate( const decMatrix &matrixView, float fovX, float fovY ){
	// prepare weighting probes
	const float weightDistanceLimit = pTracing.GetProbeSpacing().z * 8.0f;
	const decVector2 viewAngleBorder( decVector2( fovX, fovY ) * 0.5f );
	const decVector2 weightViewAngleLimit( viewAngleBorder * 0.25f ); // 25% from center
	const int realProbeCount = pTracing.GetRealProbeCount();
	const float weightAgeLimit = 30.0f;
	int i = 0;
	
	for( i=0; i<realProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		
		// geometric weight considers distance from field center and view orientation
		const decVector view( matrixView * probe.position );
		const float length = view.Length();
		
		if( length > FLOAT_SAFE_EPSILON ){
			const decVector2 angle( acosf( view.z / length ), asinf( view.y / length ) );
			
			probe.weightDistance = decMath::linearStep( length, 0.0f, weightDistanceLimit, 1.0f, 0.25f );
			probe.weightViewAngle.x = decMath::linearStep( angle.x, weightViewAngleLimit.x, PI, 1.0f, 0.25f );
			probe.weightViewAngle.y = decMath::linearStep( angle.y, weightViewAngleLimit.y, PI, 1.0f, 0.25f );
			probe.insideView = angle < viewAngleBorder;
			
		}else{
			probe.weightDistance = 1.0f;
			probe.weightViewAngle.Set( 1.0f, 1.0f );
			probe.insideView = true;
		}
		
		// age weight
		probe.weightAge = decMath::linearStep( ( float )probe.age, 0.0f, weightAgeLimit );
// 			probe.weightDistance = 1.0f;
// 			probe.weightViewAngle.Set( 1.0f, 1.0f );
	}
	
	// add invalid probes inside the view. they are always added up to the maximum number of
	// allowed update probes. invalid probes outside the view and valid probes are binned
	for( i=0; i<pWeightedProbeBinCount; i++ ){
		pWeightedProbeBinProbeCounts[ i ] = 0;
	}
	pUpdateProbeCount = 0;
	
	for( i=0; i<realProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		if( probe.valid ){
			pBinWeightedProbe( &probe, probe.weightDistance * probe.weightViewAngle.x
				* probe.weightViewAngle.y * probe.weightAge );
			
		}else{
			if( probe.insideView ){
				pAddUpdateProbe( &probe );
				if( pUpdateProbeCount >= pTracing.GetMaxUpdateProbeCount() ){
					break;
				}
				
			}else{
				pBinWeightedProbe( &probe, probe.weightDistance
					* probe.weightViewAngle.x * probe.weightViewAngle.y );
			}
		}
	}
	
	// add top rated probes as update probe up to the desired update probe count.
	// this only fills up the reminaing counts unless already above the limit
	const int maxUpdateCount = 256; //pTracing.GetMaxUpdateProbeCount();
	
	if( pUpdateProbeCount < maxUpdateCount ){
		int j;
		for( i=0; i<pWeightedProbeBinCount; i++ ){
			sProbe ** const binProbes = pWeightedProbes + pWeightedProbeBinSize * i;
			
			for( j=0; j<pWeightedProbeBinProbeCounts[ i ]; j++ ){
				pAddUpdateProbe( binProbes[ j ] );
				if( pUpdateProbeCount >= maxUpdateCount ){
					break;
				}
			}
		}
	}
	
// 	/*
	// TODO update probe grid. for the time being 8x4x8
	pUpdateProbeCount = 256; //pMaxUpdateProbeCount;
	
	//const decPoint3 spread( 16, 4, 16 );
	const decPoint3 spread( 8, 4, 8 );
	const decPoint3 gis( ( pTracing.GetProbeCount() - spread ) / 2 );
	const decPoint3 gie( gis + spread );
	
	decPoint3 gi;
	i = 0;
	for( gi.y=gis.y; gi.y<gie.y; gi.y++ ){
		for( gi.z=gis.z; gi.z<gie.z; gi.z++ ){
			for( gi.x=gis.x; gi.x<gie.x; gi.x++ ){
				sProbe &probe = pProbes[ pTracing.GridCoord2ProbeIndex( ShiftedGrid2LocalGrid( gi ) ) ];
				
				if( probe.valid ){
					probe.blendFactor = 1.0f - pTracing.GetHysteresis();
					
				}else{
					probe.blendFactor = 1.0f; // force full update
				}
				
				probe.age = 0;
				probe.valid = true;
				
				pUpdateProbes[ i++ ] = &probe;
			}
		}
	}
// 	*/
	
	// determine the required sample image size
	pSampleImageSize.x = pTracing.GetProbesPerLine() * pTracing.GetRaysPerProbe();
	pSampleImageSize.y = ( pUpdateProbeCount - 1 ) / pTracing.GetProbesPerLine() + 1;
}

void deoglOcclusionTracingState::pBinWeightedProbe( sProbe *probe, float weight ){
	int index = pWeightedProbeBinClamp - decMath::clamp( ( int )( weight * pWeightedProbeBinClamp ), 0, pWeightedProbeBinClamp );
	for( ; index<pWeightedProbeBinCount; index++ ){
		if( pWeightedProbeBinProbeCounts[ index ] < pWeightedProbeBinSize ){
			pWeightedProbes[ pWeightedProbeBinSize * index + pWeightedProbeBinProbeCounts[ index ]++ ] = probe;
			return;
		}
	}
}

void deoglOcclusionTracingState::pAddUpdateProbe( sProbe *probe ){
	if( probe->valid ){
		probe->blendFactor = 1.0f - pTracing.GetHysteresis();
		
	}else{
		probe->blendFactor = 1.0f; // force full update
		probe->valid = true;
	}
	probe->age = 0;
	
	pUpdateProbes[ pUpdateProbeCount++ ] = probe;
}

void deoglOcclusionTracingState::pPrepareUBOState(){
	deoglSPBlockUBO &ubo = pTracing.GetUBOTracing();
	int i, j, count;
	
	ubo.MapBuffer();
	try{
		const int raysPerProbe = pTracing.GetRaysPerProbe();
		
		ubo.SetParameterDataVec2( deoglOcclusionTracing::eutpSampleImageScale,
			1.0f / ( float )pSampleImageSize.x, 1.0f / ( float )pSampleImageSize.y );
		ubo.SetParameterDataInt( deoglOcclusionTracing::eutpProbeCount, pUpdateProbeCount );
		ubo.SetParameterDataInt( deoglOcclusionTracing::eutpRaysPerProbe, raysPerProbe );
		ubo.SetParameterDataInt( deoglOcclusionTracing::eutpProbesPerLine, pTracing.GetProbesPerLine() );
		ubo.SetParameterDataInt( deoglOcclusionTracing::eutpOcclusionMapSize, pTracing.GetOcclusionMapSize() );
		ubo.SetParameterDataInt( deoglOcclusionTracing::eutpDistanceMapSize, pTracing.GetDistanceMapSize() );
		ubo.SetParameterDataVec2( deoglOcclusionTracing::eutpOcclusionMapScale, pTracing.GetOcclusionMapScale() );
		ubo.SetParameterDataVec2( deoglOcclusionTracing::eutpDistanceMapScale, pTracing.GetDistanceMapScale() );
		ubo.SetParameterDataFloat( deoglOcclusionTracing::eutpMaxProbeDistance, pTracing.GetMaxProbeDistance() );
		ubo.SetParameterDataFloat( deoglOcclusionTracing::eutpDepthSharpness, pTracing.GetDepthSharpness() );
		ubo.SetParameterDataVec3( deoglOcclusionTracing::eutpFieldOrigin, 0.0f, 0.0f, 0.0f );
		ubo.SetParameterDataInt( deoglOcclusionTracing::eutpBVHInstanceRootNode, pTracing.GetBVHInstanceRootNode() );
		ubo.SetParameterDataIVec3( deoglOcclusionTracing::eutpGridProbeCount, pTracing.GetProbeCount() );
		ubo.SetParameterDataVec3( deoglOcclusionTracing::eutpGridProbeSpacing, pTracing.GetProbeSpacing() );
		
		if( pUpdateProbeCount > 0 ){
			count = ( pUpdateProbeCount - 1 ) / 4 + 1;
			for( i=0, j=0; i<count; i++, j+=4 ){
				ubo.SetParameterDataArrayIVec4( deoglOcclusionTracing::eutpProbeIndex, i,
					j < pUpdateProbeCount ? pUpdateProbes[ j ]->index : 0,
					j + 1 < pUpdateProbeCount ? pUpdateProbes[ j + 1 ]->index : 0,
					j + 2 < pUpdateProbeCount ? pUpdateProbes[ j + 2 ]->index : 0,
					j + 3 < pUpdateProbeCount ? pUpdateProbes[ j + 3 ]->index : 0 );
			}
			
			for( i=0; i<pUpdateProbeCount; i++ ){
				ubo.SetParameterDataArrayVec4( deoglOcclusionTracing::eutpProbePosition, i,
					pUpdateProbes[ i ]->position, pUpdateProbes[ i ]->blendFactor );
			}
		}
		
// 		const decMatrix randomOrientation( decMatrix::CreateRotation( decMath::random( -PI, PI ),
// 			decMath::random( -PI, PI ), decMath::random( -PI, PI ) ) );
		//const decVector * const sphericalFibonnaci = pTracing.GetSphericalFibonacci();
		const float sf_PHI = sqrtf( 5.0f ) * 0.5f + 0.5f;
		const float sf_n = ( float )pTracing.GetRaysPerProbe();
		#define madfrac(A, B) ((A)*(B)-floor((A)*(B)))
		
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
			//ubo.SetParameterDataArrayVec3( deoglOcclusionTracing::eutpRayDirection, i, randomOrientation * sf );
			ubo.SetParameterDataArrayVec3( deoglOcclusionTracing::eutpRayDirection, i, sf );
			
			/*ubo.SetParameterDataArrayVec3( deoglOcclusionTracing::eutpRayDirection,
				i, randomOrientation * sphericalFibonnaci[ i ] );*/
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

void deoglOcclusionTracingState::pPrepareProbeTexturesAndFBO(){
	if( pTexProbeOcclusion.GetTexture() && pTexProbeDistance.GetTexture() && ! pClearMaps ){
		return;
	}
	
	deoglRenderThread &renderThread = pTracing.GetRenderThread();
	const decPoint3 &probeCount = pTracing.GetProbeCount();
	
	deoglFramebuffer * const oldfbo = renderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	if( ! pTexProbeOcclusion.GetTexture() ){
		const int width = ( pTracing.GetSizeTexOcclusion() + 2 ) * probeCount.x * probeCount.y + 2;
		const int height = ( pTracing.GetSizeTexOcclusion() + 2 ) * probeCount.z + 2;
		
		pTexProbeOcclusion.SetFBOFormat( 1, true );
		pTexProbeOcclusion.SetSize( width, height );
		pTexProbeOcclusion.CreateTexture();
		
		renderThread.GetFramebuffer().Activate( &pFBOProbeOcclusion );
		pFBOProbeOcclusion.DetachAllImages();
		pFBOProbeOcclusion.AttachColorTexture( 0, &pTexProbeOcclusion );
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeOcclusion.Verify();
	}
	
	if( ! pTexProbeDistance.GetTexture() ){
		const int width = ( pTracing.GetSizeTexDistance() + 2 ) * probeCount.x * probeCount.y + 2;
		const int height = ( pTracing.GetSizeTexDistance() + 2 ) * probeCount.z + 2;
		
		pTexProbeDistance.SetFBOFormat( 2, true );
		pTexProbeDistance.SetSize( width, height );
		pTexProbeDistance.CreateTexture();
		
		renderThread.GetFramebuffer().Activate( &pFBOProbeDistance );
		pFBOProbeDistance.DetachAllImages();
		pFBOProbeDistance.AttachColorTexture( 0, &pTexProbeDistance );
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOProbeDistance.Verify();
	}
	
	if( pClearMaps ){
		OGL_CHECK( renderThread, glDisable( GL_SCISSOR_TEST ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		renderThread.GetFramebuffer().Activate( &pFBOProbeOcclusion );
		const GLfloat clearOcclusion[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearOcclusion[ 0 ] ) );
		
		renderThread.GetFramebuffer().Activate( &pFBOProbeDistance );
		const GLfloat clearDistance[ 4 ] = { 4.0f, 4.0f, 4.0f, 16.0f };
		OGL_CHECK( renderThread, pglClearBufferfv( GL_COLOR, 0, &clearDistance[ 0 ] ) );
	}
	
	renderThread.GetFramebuffer().Activate( oldfbo );
	pClearMaps = false;
}
