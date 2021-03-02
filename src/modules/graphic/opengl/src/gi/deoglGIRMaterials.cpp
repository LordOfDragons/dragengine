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
//////////////////////

// Constructor, destructor
////////////////////////////

deoglGIMaterials::deoglGIMaterials( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pMaxMaterialMapSize( 64 ),
pMaxMaterialsPerRow( 32 ),
pMaterialMapSize( pMaxMaterialMapSize ),
pMaterialsPerRow( pMaxMaterialsPerRow ),
pMaxMaterialCount( pMaxMaterialsPerRow * pMaxMaterialsPerRow ),
pMaterialCount( 0 ),
pTexColorTransparency( renderThread ),
pTexReflectivityRoughness( renderThread ),
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

void deoglGIMaterials::AddMaterial(){
}


// Private Functions
//////////////////////

void deoglGIMaterials::pCleanUp(){
}

void deoglGIMaterials::pCreateFBOMaterial(){
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	
	const int size = pMaterialMapSize * pMaterialsPerRow;
	
	pTexColorTransparency.SetFBOFormat( 4, false );
	pTexColorTransparency.SetSize( size, size );
	pTexColorTransparency.CreateTexture();
	
	pTexReflectivityRoughness.SetFBOFormat( 4, false );
	pTexReflectivityRoughness.SetSize( size, size );
	pTexReflectivityRoughness.CreateTexture();
	
	pTexEmissivity.SetFBOFormat( 3, true );
	pTexEmissivity.SetSize( size, size );
	pTexEmissivity.CreateTexture();
	
	pRenderThread.GetFramebuffer().Activate( &pFBOMaterial );
	pFBOMaterial.AttachColorTexture( 0, &pTexColorTransparency );
	pFBOMaterial.AttachColorTexture( 1, &pTexReflectivityRoughness );
	pFBOMaterial.AttachColorTexture( 2, &pTexEmissivity );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 3, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOMaterial.Verify();
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
