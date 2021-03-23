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
pTexPosition( renderThread ),
pTexNormal( renderThread ),
pTexMaterial( renderThread ),
pTexTexCoord( renderThread ),
pTexDiffuse( renderThread ),
pTexReflectivity( renderThread ),
pTexLight( renderThread ),
pFBOResult( renderThread, false ),
pFBOMaterial( renderThread, false ),
pFBOLight( renderThread, false )
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
	// position, normal and light: (52M,13M) [52428800, 13107200]
	// diffuse and reflectivity: (15M, 4M) [14680064, 3670016]
	// material and texcoord: (12M, 3M) [12582912, 3145728]
	// 
	// total: (63M, 16M) [62914560, 15728640]
	// 
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 5 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = pProbeCount / pProbesPerLine;
	
	bool updateFBOResult = false;
	bool updateFBOMaterial = false;
	bool updateFBOLight = false;
	
	pRayMapScale.x = 1.0f / ( float )width;
	pRayMapScale.y = 1.0f / ( float )height;
	
	
	
	// create/resize textures
	if( ! pTexPosition.GetTexture() ){
		pTexPosition.SetFBOFormat( 4, true );
		updateFBOResult = true;
	}
	pTexPosition.SetSize( width, height );
	pTexPosition.CreateTexture();
	
	if( ! pTexNormal.GetTexture() ){
		pTexNormal.SetFBOFormatSNorm( 3, 8 );
		updateFBOResult = true;
	}
	pTexNormal.SetSize( width, height );
	pTexNormal.CreateTexture();
	
	if( ! pTexMaterial.GetTexture() ){
		pTexMaterial.SetFBOFormatIntegral( 1, 16, true );
		updateFBOResult = true;
	}
	pTexMaterial.SetSize( width, height );
	pTexMaterial.CreateTexture();
	
	if( ! pTexTexCoord.GetTexture() ){
		pTexTexCoord.SetFBOFormat( 2, true );
		updateFBOResult = true;
	}
	pTexTexCoord.SetSize( width, height );
	pTexTexCoord.CreateTexture();
	
	if( ! pTexDiffuse.GetTexture() ){
		pTexDiffuse.SetFBOFormat( 3, false );
		updateFBOMaterial = true;
	}
	pTexDiffuse.SetSize( width, height );
	pTexDiffuse.CreateTexture();
	
	if( ! pTexReflectivity.GetTexture() ){
		pTexReflectivity.SetFBOFormat( 4, false );
		updateFBOMaterial = true;
	}
	pTexReflectivity.SetSize( width, height );
	pTexReflectivity.CreateTexture();
	
	if( ! pTexLight.GetTexture() ){
		pTexLight.SetFBOFormat( 3, true );
		updateFBOLight = true;
	}
	pTexLight.SetSize( width, height );
	pTexLight.CreateTexture();
	
	
	
	// update framebuffer if required and clear textures
	pRenderThread.GetFramebuffer().Activate( &pFBOResult );
	
	if( updateFBOResult ){
		pFBOResult.AttachColorTexture( 0, &pTexPosition );
		pFBOResult.AttachColorTexture( 1, &pTexNormal );
		pFBOResult.AttachColorTexture( 2, &pTexMaterial );
		pFBOResult.AttachColorTexture( 3, &pTexTexCoord );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 4, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOResult.Verify();
	}
	
	const GLfloat clearPosition[ 4 ] = { 0.0f, 0.0f, 0.0f, 10000.0f };
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearPosition[ 0 ] ) );
	
	const GLint clearNormal[ 4 ] = { 0, 0, 0, 0 };
	OGL_CHECK( pRenderThread, pglClearBufferiv( GL_COLOR, 1, &clearNormal[ 0 ] ) );
	
	const GLuint clearMaterial[ 4 ] = { 0, 0, 0, 0 };
	OGL_CHECK( pRenderThread, pglClearBufferuiv( GL_COLOR, 2, &clearMaterial[ 0 ] ) );
	
	const GLfloat clearTexCoord[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 3, &clearTexCoord[ 0 ] ) );
	
	
	
	pRenderThread.GetFramebuffer().Activate( &pFBOMaterial );
	
	if( updateFBOMaterial ){
		pFBOMaterial.AttachColorTexture( 0, &pTexDiffuse );
		pFBOMaterial.AttachColorTexture( 1, &pTexReflectivity );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 2, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOMaterial.Verify();
	}
	
	const GLfloat clearDiffuse[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDiffuse[ 0 ] ) );
	
	const GLfloat clearReflectivity[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 1, &clearReflectivity[ 0 ] ) );
	
	
	
	pRenderThread.GetFramebuffer().Activate( &pFBOLight );
	
	if( updateFBOLight ){
		pFBOLight.AttachColorTexture( 0, &pTexLight );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOLight.Verify();
	}
	
	const GLfloat clearLight[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearLight[ 0 ] ) );
	
	
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
