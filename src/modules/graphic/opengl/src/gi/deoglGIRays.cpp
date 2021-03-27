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

deoglGIRays::deoglGIRays( deoglRenderThread &renderThread, int raysPerProbe, int probeCount ) :
pRenderThread( renderThread  ),
pRaysPerProbe( raysPerProbe ),
pProbesPerLine( 8 ),
pProbeCount( probeCount ),
pRayMapScale( 1.0f, 1.0f ),
pTexDistanceLimit( renderThread ),
pFBODistanceLimit( renderThread, false )
{
	if( raysPerProbe < 16 || probeCount < 64 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pCreateFBO();
		
	}catch( const deException &e ){
		pCleanUp();
		throw;
	}
}

deoglGIRays::~deoglGIRays(){
	pCleanUp();
}



// Management
///////////////

void deoglGIRays::SetRaysPerProbe( int raysPerProbe ){
	if( raysPerProbe == pRaysPerProbe ){
		return;
	}
	if( raysPerProbe < 16 ){
		DETHROW( deeInvalidParam );
	}
	
	pRaysPerProbe = raysPerProbe;
	
	pCreateFBO();
}

void deoglGIRays::SetProbeCount( int count ){
	if( count == pProbeCount ){
		return;
	}
	if( count < 64 ){
		DETHROW( deeInvalidParam );
	}
	
	pProbeCount = count;
	
	pCreateFBO();
}



// Private Functions
//////////////////////

void deoglGIRays::pCleanUp(){
}

void deoglGIRays::pCreateFBO(){
	// memory consumption (assuming 8k probes):
	// case 1: 64 rays per probe => 512x1024
	// case 2: 256 rays per probe => 2048x1024
	// 
	// distance limit: (4MB, 1MB) [4194304, 1048576]
	// 
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = pProbeCount / pProbesPerLine;
	
	bool updateFBODistanceLimit = false;
	
	pRayMapScale.x = 1.0f / ( float )width;
	pRayMapScale.y = 1.0f / ( float )height;
	
	// create/resize textures
	if( ! pTexDistanceLimit.GetTexture() ){
		pTexDistanceLimit.SetFBOFormat( 1, true );
		updateFBODistanceLimit = true;
	}
	pTexDistanceLimit.SetSize( width, height );
	pTexDistanceLimit.CreateTexture();
	
	// update framebuffer if required and clear textures
	pRenderThread.GetFramebuffer().Activate( &pFBODistanceLimit );
	
	if( updateFBODistanceLimit ){
		pFBODistanceLimit.AttachColorTexture( 0, &pTexDistanceLimit );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBODistanceLimit.Verify();
	}
	
	const GLfloat clearDistanceLimit[ 4 ] = { 10000.0f, 10000.0f, 10000.0f, 10000.0f };
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDistanceLimit[ 0 ] ) );
	
	// clean up
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
