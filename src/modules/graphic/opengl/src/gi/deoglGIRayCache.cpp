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
#include "deoglGIRayCache.h"
#include "../capabilities/deoglCapabilities.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIRayCache
//////////////////////

// Constructor, destructor
////////////////////////////

deoglGIRayCache::deoglGIRayCache( deoglRenderThread &renderThread,
	int raysPerProbe, int probeCount, int layerCount ) :
pRenderThread( renderThread  ),
pRaysPerProbe( raysPerProbe ),
pProbesPerLine( 8 ),
pProbeCount( probeCount ),
pLayerCount( layerCount ),
pRayMapScale( 1.0f, 1.0f ),
pTexDistance( renderThread ),
pTexNormal( renderThread ),
pTexDiffuse( renderThread ),
pTexReflectivity( renderThread ),
pTexLight( renderThread ),
pFBOResult( renderThread, false )
{
	if( raysPerProbe < 16 || probeCount < 64 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pCreateFBO();
		
	}catch( const deException &e ){
		pCleanUp();
		throw;
	}
}

deoglGIRayCache::~deoglGIRayCache(){
	pCleanUp();
}



// Management
///////////////

void deoglGIRayCache::SetRaysPerProbe( int raysPerProbe ){
	if( raysPerProbe == pRaysPerProbe ){
		return;
	}
	if( raysPerProbe < 16 ){
		DETHROW( deeInvalidParam );
	}
	
	pRaysPerProbe = raysPerProbe;
	
	pCreateFBO();
}

void deoglGIRayCache::SetProbeCount( int count ){
	if( count == pProbeCount ){
		return;
	}
	if( count < 64 ){
		DETHROW( deeInvalidParam );
	}
	
	pProbeCount = count;
	
	pCreateFBO();
}

void deoglGIRayCache::SetLayerCount( int count ){
	if( count == pLayerCount ){
		return;
	}
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pLayerCount = count;
	
	pCreateFBO();
}



// Private Functions
//////////////////////

void deoglGIRayCache::pCleanUp(){
}

void deoglGIRayCache::pCreateFBO(){
	// memory consumption (assuming 8k probes):
	// case 1: 64 rays per probe => 512x1024
	// case 2: 256 rays per probe => 2048x1024
	// case 2: 512 rays per probe => 4096x1024
	// 
	// distance limit: (4MB, 1MB) [4194304, 1048576]
	// 
	// distance, normal and light: (23M, 6M) [23068672, 5767168]
	// diffuse and reflectivity: (15M, 4M) [14680064, 3670016]
	// total: (37M, 9M) [37748736, 9437184]
	// 
	const deoglRestoreFramebuffer restoreFbo( pRenderThread );
	#ifdef GI_USE_RAY_CACHE
	const GLenum buffers[ 5 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	#endif
	
	const int width = pProbesPerLine * pRaysPerProbe;
	const int height = pProbeCount / pProbesPerLine;
	
	pRayMapScale.x = 1.0f / ( float )width;
	pRayMapScale.y = 1.0f / ( float )height;
	
	// create/resize textures
	#ifdef GI_USE_RAY_CACHE
		if( ! pTexDistance.GetTexture() ){
			pTexDistance.SetFBOFormat( 1, true );
		}
		pTexDistance.SetSize( width, height, pLayerCount );
		pTexDistance.CreateTexture();
		
		if( ! pTexNormal.GetTexture() ){
			pTexNormal.SetFBOFormatSNorm( 3, 8 );
		}
		pTexNormal.SetSize( width, height, pLayerCount );
		pTexNormal.CreateTexture();
		
		if( ! pTexDiffuse.GetTexture() ){
			pTexDiffuse.SetFBOFormat( 3, false );
		}
		pTexDiffuse.SetSize( width, height, pLayerCount );
		pTexDiffuse.CreateTexture();
		
		if( ! pTexReflectivity.GetTexture() ){
			pTexReflectivity.SetFBOFormat( 4, false );
		}
		pTexReflectivity.SetSize( width, height, pLayerCount );
		pTexReflectivity.CreateTexture();
		
		if( ! pTexLight.GetTexture() ){
			pTexLight.SetFBOFormat( 3, true );
		}
		pTexLight.SetSize( width, height, pLayerCount );
		pTexLight.CreateTexture();
	#endif
	
	// update framebuffer if required and clear textures
	#ifdef GI_USE_RAY_CACHE
		pRenderThread.GetFramebuffer().Activate( &pFBOResult );
		
		pFBOResult.DetachAllImages();
		pFBOResult.AttachColorArrayTexture( 0, &pTexDistance );
		pFBOResult.AttachColorArrayTexture( 1, &pTexNormal );
		pFBOResult.AttachColorArrayTexture( 2, &pTexDiffuse );
		pFBOResult.AttachColorArrayTexture( 3, &pTexReflectivity );
		pFBOResult.AttachColorArrayTexture( 4, &pTexLight );
		OGL_CHECK( pRenderThread, pglDrawBuffers( 5, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		pFBOResult.Verify();
		
		const GLfloat clearDistance[ 4 ] = { 10000.0f, 10000.0f, 10000.0f, 10000.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDistance[ 0 ] ) );
		
		const GLfloat clearNormal[ 4 ] = { 0.0f, 0.0f, 1.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 1, &clearNormal[ 0 ] ) );
		
		const GLfloat clearDiffuse[ 4 ] = { 1.0f, 1.0f, 1.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 2, &clearDiffuse[ 0 ] ) );
		
		const GLfloat clearReflectivity[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 3, &clearReflectivity[ 0 ] ) );
		
		const GLfloat clearLight[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 4, &clearLight[ 0 ] ) );
	#endif
}
