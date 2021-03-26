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
#include "../renderthread/deoglRTLogger.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"

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
pTexDiffuse( renderThread ),
pTexReflectivity( renderThread ),
pTexEmissivity( renderThread ),
pFBOMaterial( renderThread, false )
{
	try{
		pTUCs.Add( NULL ); // index 0 is fallback
		
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

void deoglGIMaterials::AddTUC( deoglTexUnitsConfig *tuc ){
	if( ! tuc || tuc->GetMaterialIndex() != -1 || tuc->GetUsageCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	int index = pFirstUnusedMaterial();
	if( index != -1 ){
		deoglTexUnitsConfig &unusedTuc = *( ( deoglTexUnitsConfig* )pTUCs.GetAt( index ) );
		unusedTuc.SetMaterialIndex( -1 );
		unusedTuc.RemoveUsage(); // potentially deleted now
		
		pTUCs.SetAt( index, tuc );
		tuc->SetMaterialIndex( index );
		tuc->AddUsage();
		return;
	}
	
	index = pTUCs.GetCount();
	if( index > 16383 ){
		// fallback. not nice but better than causing troubles
		tuc->SetMaterialIndex( 0 );
		return;
	}
	
	pTUCs.Add( tuc );
	tuc->SetMaterialIndex( index );
	tuc->AddUsage();
	
	if( ( pTUCs.GetCount() - 1 ) % 10 == 0 ){
		pRenderThread.GetLogger().LogInfoFormat( "GIMaterials: Reached %d materials", pTUCs.GetCount() - 1 );
	}
	
	if( pTUCs.GetCount() > pMaxMaterialCount ){
		pEnlarge();
	}
}

deoglTexUnitsConfig *deoglGIMaterials::GetTUC( int materialIndex ) const{
	if( materialIndex < 1 || materialIndex >= pTUCs.GetCount() ){
		return NULL;
	}
	return ( deoglTexUnitsConfig* )pTUCs.GetAt( materialIndex );
}



// Private Functions
//////////////////////

void deoglGIMaterials::pCleanUp(){
	const int count = pTUCs.GetCount();
	int i;
	for( i=1; i<count; i++ ){
		deoglTexUnitsConfig &tuc = *( ( deoglTexUnitsConfig* )pTUCs.GetAt( i ) );
		tuc.SetMaterialIndex( -1 );
		tuc.RemoveUsage(); // potentially deleted now
	}
}

void deoglGIMaterials::pCreateFBOMaterial(){
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const GLenum buffers[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	
	const int size = pMaterialMapSize * pRowsPerImage;
	
	// GPU RAM consumption (size 2048 squared):
	// - diffuse: 16M
	// - reflectivity: 16M
	// - emissivity: 32M
	// total: 67M
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

void deoglGIMaterials::pEnlarge(){
	if( pMaxMaterialCount >= 16383 ){
		DETHROW( deeInvalidParam ); // we should never end up here
	}
	
	pMaterialMapSize /= 2;
	pMaterialsPerRow *= 2;
	pRowsPerImage *= 2;
	pMaxMaterialCount = pMaxMaterialsPerRow * pMaxRowsPerImage;
	
	// scale images down
}

int deoglGIMaterials::pFirstUnusedMaterial() const{
	const int count = pTUCs.GetCount();
	int i;
	
	for( i=1; i<count; i++ ){
		if( ( ( deoglTexUnitsConfig* )pTUCs.GetAt( i ) )->GetUsageCount() == 1 ){
			return i;
		}
	}
	
	return -1;
}
