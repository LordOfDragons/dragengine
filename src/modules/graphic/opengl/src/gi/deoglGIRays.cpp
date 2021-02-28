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
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 4 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = pProbeCount / pProbesPerLine;
	
	pRBufDepth.SetDepthFormat( false );
	pRBufDepth.SetSize( width, height );
	pRBufDepth.CreateBuffer();
	
	pTexPosition.SetFBOFormat( 4, true );
	pTexPosition.SetSize( width, height );
	pTexPosition.CreateTexture();
	
	pTexNormal.SetFBOFormat( 4, true );
	pTexNormal.SetSize( width, height );
	pTexNormal.CreateTexture();
	
	pTexDiffuse.SetFBOFormat( 4, false );
	pTexDiffuse.SetSize( width, height );
	pTexDiffuse.CreateTexture();
	
	pTexLight.SetFBOFormat( 3, true );
	pTexLight.SetSize( width, height );
	pTexLight.CreateTexture();
	
	pRenderThread.GetFramebuffer().Activate( &pFBOResult );
	pFBOResult.AttachDepthRenderbuffer( &pRBufDepth );
	pFBOResult.AttachColorTexture( 0, &pTexPosition );
	pFBOResult.AttachColorTexture( 1, &pTexNormal );
	pFBOResult.AttachColorTexture( 2, &pTexDiffuse );
	pFBOResult.AttachColorTexture( 3, &pTexLight );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 4, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOResult.Verify();
	
	pRenderThread.GetFramebuffer().Activate( &pFBOLight );
	pFBOLight.AttachColorTexture( 0, &pTexLight );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOLight.Verify();
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
