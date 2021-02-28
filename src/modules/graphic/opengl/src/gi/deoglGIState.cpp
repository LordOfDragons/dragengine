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
#include "deoglGIRays.h"
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
pNormalBias( 0.25f ),
pEnergyPreservation( 0.85f ),

pSizeTexIrradiance( pIrradianceMapSize ),
pSizeTexDistance( pDistanceMapSize ),

pIrradianceMapScale( 1.0f / ( ( pSizeTexIrradiance + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexIrradiance + 2 ) * pProbeCount.z + 2 ) ),
pDistanceMapScale( 1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.x * pProbeCount.y + 2 ),
	1.0f / ( ( pSizeTexDistance + 2 ) * pProbeCount.z + 2 ) ),

pProbes( NULL ),
pElapsedUpdateProbe( 0.0f ),
pUpdateProbeInterval( 0.1f ),
pUpdateProbes( NULL ),
pUpdateProbeCount( 0 ),
pWeightedProbes( NULL ),
pWeightedProbeBinSize( pRealProbeCount / 20 ),
pWeightedProbeBinCount( 20 ),
pWeightedProbeBinClamp( pWeightedProbeBinCount - 1 ),
pWeightedProbeBinProbeCounts( NULL ),
pTexProbeIrradiance( renderThread ),
pTexProbeDistance( renderThread ),
pFBOProbeIrradiance( renderThread, false ),
pFBOProbeDistance( renderThread, false ),
pClearMaps( true )
{
	try{
		pInitProbes();
		pUpdateProbes = new sProbe*[ renderThread.GetGI().GetRays().GetProbeCount() ];
		pWeightedProbes = new sProbe*[ pRealProbeCount ];
		pWeightedProbeBinProbeCounts = new int[ pWeightedProbeBinCount ];
		
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

// #define DO_TIMING 1

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



void deoglGIState::Update( deoglRWorld &world, const decDVector &cameraPosition,
const decDMatrix &cameraMatrix, float fovX, float fovY ){
	pUpdateProbeCount = 0;
	pUpdatePosition( cameraPosition );
	pPrepareTraceProbes( decDMatrix::CreateTranslation( pPosition ) * cameraMatrix, fovX, fovY );
}

void deoglGIState::PrepareUBOState() const{
	const deoglGI &gi = pRenderThread.GetGI();
	const deoglGIRays &rays = gi.GetRays();
	deoglSPBlockUBO &ubo = gi.GetUBO();
	int i, j, count;
	
	ubo.MapBuffer();
	try{
		const int raysPerProbe = rays.GetRaysPerProbe();
		
		ubo.SetParameterDataVec2( deoglGI::eupSampleImageScale,
			1.0f / ( float )pSampleImageSize.x, 1.0f / ( float )pSampleImageSize.y );
		ubo.SetParameterDataInt( deoglGI::eupProbeCount, pUpdateProbeCount );
		ubo.SetParameterDataInt( deoglGI::eupRaysPerProbe, raysPerProbe );
		ubo.SetParameterDataInt( deoglGI::eupProbesPerLine, rays.GetProbesPerLine() );
		ubo.SetParameterDataInt( deoglGI::eupOcclusionMapSize, pIrradianceMapSize );
		ubo.SetParameterDataInt( deoglGI::eupDistanceMapSize, pDistanceMapSize );
		ubo.SetParameterDataVec2( deoglGI::eupOcclusionMapScale, pIrradianceMapScale );
		ubo.SetParameterDataVec2( deoglGI::eupDistanceMapScale, pDistanceMapScale );
		ubo.SetParameterDataFloat( deoglGI::eupMaxProbeDistance, pMaxProbeDistance );
		ubo.SetParameterDataFloat( deoglGI::eupDepthSharpness, pDepthSharpness );
		ubo.SetParameterDataVec3( deoglGI::eupFieldOrigin, 0.0f, 0.0f, 0.0f );
		ubo.SetParameterDataInt( deoglGI::eupBVHInstanceRootNode, gi.GetBVH().GetIndexRootNode() );
		ubo.SetParameterDataIVec3( deoglGI::eupGridProbeCount, pProbeCount );
		ubo.SetParameterDataVec3( deoglGI::eupGridProbeSpacing, pProbeSpacing );
		
		if( pUpdateProbeCount > 0 ){
			count = ( pUpdateProbeCount - 1 ) / 4 + 1;
			for( i=0, j=0; i<count; i++, j+=4 ){
				ubo.SetParameterDataArrayIVec4( deoglGI::eupProbeIndex, i,
					j < pUpdateProbeCount ? pUpdateProbes[ j ]->index : 0,
					j + 1 < pUpdateProbeCount ? pUpdateProbes[ j + 1 ]->index : 0,
					j + 2 < pUpdateProbeCount ? pUpdateProbes[ j + 2 ]->index : 0,
					j + 3 < pUpdateProbeCount ? pUpdateProbes[ j + 3 ]->index : 0 );
			}
			
			for( i=0; i<pUpdateProbeCount; i++ ){
				ubo.SetParameterDataArrayVec4( deoglGI::eupProbePosition, i,
					pUpdateProbes[ i ]->position, pUpdateProbes[ i ]->blendFactor );
			}
		}
		
// 		const decMatrix randomOrientation( decMatrix::CreateRotation( decMath::random( -PI, PI ),
// 			decMath::random( -PI, PI ), decMath::random( -PI, PI ) ) );
		//const decVector * const sphericalFibonnaci = pTracing.GetSphericalFibonacci();
		const float sf_PHI = sqrtf( 5.0f ) * 0.5f + 0.5f;
		const float sf_n = ( float )raysPerProbe;
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
			//ubo.SetParameterDataArrayVec3( deoglGI::eupRayDirection, i, randomOrientation * sf );
			ubo.SetParameterDataArrayVec3( deoglGI::eupRayDirection, i, sf );
			
			/*ubo.SetParameterDataArrayVec3( deoglGI::eupRayDirection,
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

void deoglGIState::Invalidate(){
	int i;
	
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.age = 0;
		probe.blendFactor = 1.0f;
		probe.valid = false;
	}
	
	pClearMaps = true;
}



// Private Functions
//////////////////////

void deoglGIState::pCleanUp(){
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

void deoglGIState::pInitProbes(){
	pProbes = new sProbe[ pRealProbeCount ];
	
	int i;
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.index = i;
		probe.blendFactor = 1.0f;
		probe.age = 0;
		probe.valid = false;
		probe.coord = ProbeIndex2GridCoord( i );
	}
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
	pGridCoordShift.x %= pProbeCount.x;
	pGridCoordShift.y %= pProbeCount.y;
	pGridCoordShift.z %= pProbeCount.z;
	pGridCoordShift += pProbeCount;
	pGridCoordShift.x %= pProbeCount.x;
	pGridCoordShift.y %= pProbeCount.y;
	pGridCoordShift.z %= pProbeCount.z;
}

void deoglGIState::pPrepareTraceProbes( const decMatrix &matrixView, float fovX, float fovY ){
	pElapsedUpdateProbe += pTimerUpdateProbe.GetElapsedTime();
	if( pElapsedUpdateProbe < pUpdateProbeInterval ){
// 		return; // how to use this properly? we track snapped position already
	}
	
	// keep only the remainder. this avoids updates to pile up if framerate drops or staggers
	pElapsedUpdateProbe = fmodf( pElapsedUpdateProbe, pUpdateProbeInterval );
	
	pAgeProbes();
	pFindProbesToUpdate( matrixView, fovX, fovY );
	pPrepareProbeTexturesAndFBO();
}

void deoglGIState::pAgeProbes(){
	int i;
	
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		probe.age++;
		probe.shiftedCoord = LocalGrid2ShiftedGrid( probe.coord );
		probe.position = Grid2Local( probe.shiftedCoord );
	}
}

void deoglGIState::pFindProbesToUpdate( const decMatrix &matrixView, float fovX, float fovY ){
	// prepare weighting probes
	const deoglGIRays &rays = pRenderThread.GetGI().GetRays();
	const float weightDistanceLimit = pProbeSpacing.z * 8.0f;
	const decVector2 viewAngleBorder( decVector2( fovX, fovY ) * 0.5f );
	const decVector2 weightViewAngleLimit( viewAngleBorder * 0.25f ); // 25% from center
	const float weightAgeLimit = 30.0f;
	int i = 0;
	
	for( i=0; i<pRealProbeCount; i++ ){
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
	
	for( i=0; i<pRealProbeCount; i++ ){
		sProbe &probe = pProbes[ i ];
		if( probe.valid ){
			pBinWeightedProbe( &probe, probe.weightDistance * probe.weightViewAngle.x
				* probe.weightViewAngle.y * probe.weightAge );
			
		}else{
			if( probe.insideView ){
				pAddUpdateProbe( &probe );
				if( pUpdateProbeCount >= rays.GetProbeCount() ){
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
	const int maxUpdateCount = 256; //rays.GetProbeCount();
	
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
	const decPoint3 gis( ( pProbeCount - spread ) / 2 );
	const decPoint3 gie( gis + spread );
	
	decPoint3 gi;
	i = 0;
	for( gi.y=gis.y; gi.y<gie.y; gi.y++ ){
		for( gi.z=gis.z; gi.z<gie.z; gi.z++ ){
			for( gi.x=gis.x; gi.x<gie.x; gi.x++ ){
				sProbe &probe = pProbes[ GridCoord2ProbeIndex( ShiftedGrid2LocalGrid( gi ) ) ];
				
				if( probe.valid ){
					probe.blendFactor = 1.0f - pHysteresis;
					
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
	pSampleImageSize.x = rays.GetProbesPerLine() * rays.GetRaysPerProbe();
	pSampleImageSize.y = ( pUpdateProbeCount - 1 ) / rays.GetProbesPerLine() + 1;
}

void deoglGIState::pBinWeightedProbe( sProbe *probe, float weight ){
	int index = pWeightedProbeBinClamp - decMath::clamp( ( int )( weight * pWeightedProbeBinClamp ), 0, pWeightedProbeBinClamp );
	for( ; index<pWeightedProbeBinCount; index++ ){
		if( pWeightedProbeBinProbeCounts[ index ] < pWeightedProbeBinSize ){
			pWeightedProbes[ pWeightedProbeBinSize * index + pWeightedProbeBinProbeCounts[ index ]++ ] = probe;
			return;
		}
	}
}

void deoglGIState::pAddUpdateProbe( sProbe *probe ){
	if( probe->valid ){
		probe->blendFactor = 1.0f - pHysteresis;
		
	}else{
		probe->blendFactor = 1.0f; // force full update
		probe->valid = true;
	}
	probe->age = 0;
	
	pUpdateProbes[ pUpdateProbeCount++ ] = probe;
}

void deoglGIState::pPrepareProbeTexturesAndFBO(){
	if( pTexProbeIrradiance.GetTexture() && pTexProbeDistance.GetTexture() && ! pClearMaps ){
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
	
	if( pClearMaps ){
		OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
		OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeIrradiance );
		const GLfloat clearIrradiance[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearIrradiance[ 0 ] ) );
		
		pRenderThread.GetFramebuffer().Activate( &pFBOProbeDistance );
		const GLfloat clearDistance[ 4 ] = { 4.0f, 4.0f, 4.0f, 16.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDistance[ 0 ] ) );
	}
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
	pClearMaps = false;
}
