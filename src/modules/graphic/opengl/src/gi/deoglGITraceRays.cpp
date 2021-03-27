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

#include "deoglGITraceRays.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"

#include <dragengine/common/exceptions.h>


// Class deoglGITraceRays
//////////////////////

// Constructor, destructor
////////////////////////////

deoglGITraceRays::deoglGITraceRays( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pRaysPerProbe( 256 ),
pProbesPerLine( 8 ),
pProbeCount( 2048 ), // 2048 would be the maximum in UBO size
pTexPosition( renderThread ),
pTexNormal( renderThread ),
pTexDiffuse( renderThread ),
pTexReflectivity( renderThread ),
pTexLight( renderThread ),
pFBOResult( renderThread, false ),
pFBODistance( renderThread, false ),
pFBOLight( renderThread, false )
{
	try{
		pCreateFBORay();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGITraceRays::~deoglGITraceRays(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglGITraceRays::pCleanUp(){
}

void deoglGITraceRays::pCreateFBORay(){
	// memory consumption:
	// case 1: 64 rays per probe => 512x1024
	// case 2: 256 rays per probe => 2048x1024
	// 
	// position, normal and light: (36M, 9M) [35651584, 8912896]
	// diffuse and reflectivity: (15M, 4M) [14680064, 3670016]
	// 
	// total: (63M, 16M) [62914560, 15728640]
	// 
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 5 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = pProbeCount / pProbesPerLine;
	
	pTexPosition.SetFBOFormat( 4, true );
	pTexPosition.SetSize( width, height );
	pTexPosition.CreateTexture();
	
	pTexNormal.SetFBOFormatSNorm( 3, 8 );
	pTexNormal.SetSize( width, height );
	pTexNormal.CreateTexture();
	
	pTexDiffuse.SetFBOFormat( 3, false );
	pTexDiffuse.SetSize( width, height );
	pTexDiffuse.CreateTexture();
	
	pTexReflectivity.SetFBOFormat( 4, false );
	pTexReflectivity.SetSize( width, height );
	pTexReflectivity.CreateTexture();
	
	pTexLight.SetFBOFormat( 3, true );
	pTexLight.SetSize( width, height );
	pTexLight.CreateTexture();
	
	pRenderThread.GetFramebuffer().Activate( &pFBOResult );
	pFBOResult.AttachColorTexture( 0, &pTexPosition );
	pFBOResult.AttachColorTexture( 1, &pTexNormal );
	pFBOResult.AttachColorTexture( 2, &pTexDiffuse );
	pFBOResult.AttachColorTexture( 3, &pTexReflectivity );
	pFBOResult.AttachColorTexture( 4, &pTexLight );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 5, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOResult.Verify();
	
	pRenderThread.GetFramebuffer().Activate( &pFBODistance );
	pFBODistance.AttachColorTexture( 0, &pTexPosition );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBODistance.Verify();
	
	pRenderThread.GetFramebuffer().Activate( &pFBOLight );
	pFBOLight.AttachColorTexture( 0, &pTexLight );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOLight.Verify();
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
