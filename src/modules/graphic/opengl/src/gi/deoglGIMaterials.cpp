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

#include "deoglGIMaterials.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIMaterials
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIMaterials::deoglGIMaterials( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pMaxMaterialMapSize( 64 ),
pMaxMaterialsPerRow( 32 ),
pMaxRowsPerImage( 32 ),
pMaterialMapSize( pMaxMaterialMapSize ),
pMaterialsPerRow( pMaxMaterialsPerRow ),
pRowsPerImage( pMaxRowsPerImage ),
pMaxMaterialCount( pMaxMaterialsPerRow * pMaxRowsPerImage ),
pMaterialCount( 0 ),
pTexDiffuse( renderThread ),
pTexReflectivity( renderThread ),
pTexEmissivity( renderThread ),
pFBOMaterial( renderThread, false )
{
	try{
		pCreateFBOMaterial();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIMaterials::~deoglGIMaterials(){
	pCleanUp();
}



// Management
///////////////

void deoglGIMaterials::SetMaterialCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pMaterialCount = decMath::min( count, 16383 );
	
	// by resetting the highest texture size is used if the count. if reset is not used
	// the size is reduced if the count grows too large and stays at this level.
	// theoretically this can lead to some fluctuations in calculated GI if the textures
	// have unfortunate properties
	pMaterialMapSize = pMaxMaterialMapSize;
	pMaterialsPerRow = pMaxMaterialsPerRow;
	pMaxMaterialCount = pMaxMaterialsPerRow * pMaxRowsPerImage;
	
	while( pMaterialCount > pMaxMaterialCount ){
		pMaterialMapSize /= 2;
		pMaterialsPerRow *= 2;
		pRowsPerImage *= 2;
		pMaxMaterialCount = pMaxMaterialsPerRow * pMaxRowsPerImage;
	}
}


// Private Functions
//////////////////////

void deoglGIMaterials::pCleanUp(){
}

void deoglGIMaterials::pCreateFBOMaterial(){
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	
	const int size = pMaterialMapSize * pRowsPerImage;
	
	pTexDiffuse.SetFBOFormat( 4, false );
	pTexDiffuse.SetSize( size, size );
	pTexDiffuse.CreateTexture();
	
	pTexReflectivity.SetFBOFormat( 4, false );
	pTexReflectivity.SetSize( size, size );
	pTexReflectivity.CreateTexture();
	
	pTexEmissivity.SetFBOFormat( 4, true );
	pTexEmissivity.SetSize( size, size );
	pTexEmissivity.CreateTexture();
	
	pRenderThread.GetFramebuffer().Activate( &pFBOMaterial );
	pFBOMaterial.AttachColorTexture( 0, &pTexDiffuse );
	pFBOMaterial.AttachColorTexture( 1, &pTexReflectivity );
	pFBOMaterial.AttachColorTexture( 2, &pTexEmissivity );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 3, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOMaterial.Verify();
	
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
	OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	
	const GLfloat clearDiffTintMask[ 4 ] = { 0.85f, 0.85f, 0.85f, 1.0f };
	const GLfloat clearReflRough[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.5f };
	const GLfloat clearEmiss[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDiffTintMask[ 0 ] ) );
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 1, &clearReflRough[ 1 ] ) );
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 2, &clearEmiss[ 2 ] ) );
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
