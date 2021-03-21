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

#include "deoglGIRays.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIRays
//////////////////////

// Constructor, destructor
////////////////////////////

deoglGIRays::deoglGIRays( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pRaysPerProbe( 64 ),
pProbesPerLine( 8 ),
pProbeCount( 2048 ), // 2048 would be the maximum in UBO size
pRBufDepth( renderThread ),
pTexPosition( renderThread ),
pTexNormal( renderThread ),
pTexDiffuse( renderThread ),
pTexReflectivity( renderThread ),
pTexLight( renderThread ),
pFBOResult( renderThread, false ),
pFBOLight( renderThread, false )
{
	try{
		pCreateFBORay();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIRays::~deoglGIRays(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglGIRays::pCleanUp(){
}

void deoglGIRays::pCreateFBORay(){
	// memory consumption:
	// case 1: 64 rays per probe => 512x1024
	// case 2: 256 rays per probe => 2048x1024
	// 
	// depth buffer, position, normal and light: (52M,13M) [52428800, 13107200]
	// diffuse and reflectivity: (15M, 4M) [14680064, 3670016]
	// material and texcoord: (12M, 3M) [12582912, 3145728]
	// 
	// total 1: (67M, 17M) [67108864, 16777216]
	// total 2: (65M, 16M) [65011712, 16252928]
	// 
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 5 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = pProbeCount / pProbesPerLine;
	
	pRBufDepth.SetDepthFormat( false );
	pRBufDepth.SetSize( width, height );
	pRBufDepth.CreateBuffer();
	
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
	pFBOResult.AttachDepthRenderbuffer( &pRBufDepth );
	pFBOResult.AttachColorTexture( 0, &pTexPosition );
	pFBOResult.AttachColorTexture( 1, &pTexNormal );
	pFBOResult.AttachColorTexture( 2, &pTexDiffuse );
	pFBOResult.AttachColorTexture( 3, &pTexReflectivity );
	pFBOResult.AttachColorTexture( 4, &pTexLight );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 5, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOResult.Verify();
	
	pRenderThread.GetFramebuffer().Activate( &pFBOLight );
	pFBOLight.AttachColorTexture( 0, &pTexLight );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOLight.Verify();
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
