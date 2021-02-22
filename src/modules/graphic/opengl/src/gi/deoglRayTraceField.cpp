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

#include "deoglRayTraceField.h"
#include "deoglGI.h"
#include "../capabilities/deoglCapabilities.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../rendering/deoglRenderGI.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRayTraceField::deoglRayTraceField( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pSpacing( 1.0f, 1.0f, 1.0f ),
pInverseSpacing( 1.0f, 1.0f, 1.0f ),
pProbeSize( 8 ), // equals 64 rays
pProbesPerLine( 128 ), // equals image width of 1024
pTexRays( renderThread ),
pFBORays( NULL ),
pUBO( NULL )
{
	try{
		pUBO.TakeOver( new deoglSPBlockUBO( renderThread ) );
		deoglSPBlockUBO &ubo = GetUBO();
		ubo.SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ubo.SetParameterCount( eupSize + 1 );
		ubo.GetParameterAt( eupProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eupProbesPerLine ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eupProbeSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eupProbeStride ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eupResolution ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
		ubo.GetParameterAt( eupClamp ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
		ubo.GetParameterAt( eupSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.GetParameterAt( eupOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.GetParameterAt( eupSize ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.MapToStd140();
		ubo.SetBindingPoint( 0 );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRayTraceField::~deoglRayTraceField(){
	pCleanUp();
}



// Management
///////////////

void deoglRayTraceField::Init( const decVector &minExtend, const decVector &maxExtend ){
	pOrigin = minExtend;
	pSize = maxExtend - minExtend;
	
	// choose the resolution to obtain roughly a 1m spacing between probes
	// 
	// NOTE we have to be careful to not exceed the maximum texture resolution
	//      supported by the opengl implementation. the maximum texture size
	//      should be used to determine the maximum resolution supported given
	//      a specific map size.
	//      
	//      another solution is using array textures to store the vertical
	//      resolution. typically this is a lot smaller than the horizontal
	//      resolution and allows to use larger textures.
	//      
	//      another solution is to split up the field into smaller fields.
	//      this is actually not a bad solution since large meshes are reduced
	//      into smaller fields which can be culled better.
	//      
	//      for the ground floor mesh (roughly 40x3x20 meters in size) a
	//      desiresSpacing spacing of 0.25 works while 0.125 already exceeds
	//      the texture size limits
	// 
	// NOTE a field of 32x16x32 probes requires a texture of size 1024x1024 (2MB).
	//      a field of 64x16*64 probes requires a texture of size 2048x2048 (8MB).
	//      a field of 128x16x128 probes requires a texture of size 4096x4096 (32MB).
	//      
	//      for the beginning using a limit of 32x16x32 keeps the texture size down
	//      to 1024 and the memory consumption to 2MB.
	//      
	//      this can be later on optimized by using an algorithm on the GPU which
	//      distributes a fixed set of probes on an uneven grid to optimize the
	//      coverage and quality. for this to work requires an additional R16UI
	//      texture which stores for an even grid the index of the probe to use. 
	const float desiresSpacing = 1.0f;
	const decPoint3 maxCellCount( decPoint3( 32, 16, 32 ) - decPoint3( 3, 3, 3 ) );
	const decPoint3 cellCount( decPoint3( 1, 1, 1 ).Largest( ( pSize / desiresSpacing ).Round() ).Smallest( maxCellCount ) );
	
	pSpacing.x = pSize.x / ( float )cellCount.x;
	pSpacing.y = pSize.y / ( float )cellCount.y;
	pSpacing.z = pSize.z / ( float )cellCount.z;
	pInverseSpacing.x = 1.0f / pSpacing.x;
	pInverseSpacing.y = 1.0f / pSpacing.y;
	pInverseSpacing.z = 1.0f / pSpacing.z;
	
	// to tightly fit the extends the resolution is 1 larger than the cell count.
	// add border around the object of cell size. this increases the resolution by 2
	pResolution = cellCount + decPoint3( 3, 3, 3 );
	pCoordClamp = pResolution - decPoint3( 1, 1, 1 );
	pProbeCount = pResolution.x * pResolution.y * pResolution.z;
	
	pOrigin -= pSpacing;
	pSize += pSpacing * 2.0f;
}



decPoint3 deoglRayTraceField::Position2Coord( const decVector &position ) const{
	return ( position - pOrigin ).Multiply( pInverseSpacing )
		.Round().Clamped( decPoint3( 0, 0, 0 ), pCoordClamp );
}

decVector deoglRayTraceField::Coord2Position( const decPoint3 &coord ) const{
	return pOrigin + decVector( coord ).Multiply( pSpacing );
}



void deoglRayTraceField::RenderField( deoglROcclusionMesh &occlusionMesh ){
	pRenderThread.GetGI().PrepareRayTracing( occlusionMesh );
	pPrepareRayTexFBO();
	pPrepareUBOState();
	pRenderThread.GetRenderers().GetGI().TraceRays( *this );
	DropFBO();
}

void deoglRayTraceField::DropFBO(){
	if( pFBORays ){
		delete pFBORays;
		pFBORays = NULL;
	}
}



// Private Functions
//////////////////////

void deoglRayTraceField::pCleanUp(){
	if( pFBORays ){
		delete pFBORays;
	}
}

void deoglRayTraceField::pPrepareUBOState(){
	deoglSPBlockUBO &ubo = GetUBO();
	
	ubo.MapBuffer();
	try{
		ubo.SetParameterDataInt( eupProbeCount, pProbeCount );
		ubo.SetParameterDataInt( eupProbesPerLine, pProbesPerLine );
		ubo.SetParameterDataInt( eupProbeSize, pProbeSize );
		ubo.SetParameterDataInt( eupProbeStride, pResolution.x * pResolution.z );
		ubo.SetParameterDataIVec3( eupResolution, pResolution );
		ubo.SetParameterDataIVec3( eupClamp, pResolution - decPoint3( 1, 1, 1 ) );
		ubo.SetParameterDataVec3( eupSpacing, pSpacing );
		ubo.SetParameterDataVec3( eupOrigin, pOrigin );
		ubo.SetParameterDataVec3( eupSize, pSize );
		
	}catch( const deException & ){
		ubo.UnmapBuffer();
		throw;
	}
	ubo.UnmapBuffer();
}

void deoglRayTraceField::pPrepareRayTexFBO(){
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	bool setupFbo = false;
	
	if( ! pFBORays ){
		pFBORays = new deoglFramebuffer( pRenderThread, false );
		setupFbo = true;
	}
	
	if( ! pTexRays.GetTexture() ){
		const int width = pProbesPerLine * pProbeSize;
		const int height = pProbeSize * ( ( ( pProbeCount - 1 ) / pProbesPerLine ) + 1 );
		
		pTexRays.SetFBOFormat( 1, true );
		pTexRays.SetSize( width, height );
		pTexRays.CreateTexture();
		
		setupFbo = true;
	}
	
	pRenderThread.GetFramebuffer().Activate( pFBORays );
	
	if( setupFbo ){
		pFBORays->DetachAllImages();
		pFBORays->AttachColorTexture( 0, &pTexRays );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBORays->Verify();
	}
	
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	const GLfloat clear[ 4 ] = { 10000.0f, 10000.0f, 10000.0f, 1.0f };
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clear[ 0 ] ) );
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
