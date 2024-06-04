/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglGIMaterials.h"
#include "../capabilities/deoglCapabilities.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../rendering/light/deoglRenderLight.h"
#include "../rendering/light/deoglRenderGI.h"
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
pTexDiffuse( NULL ),
pTexReflectivity( NULL ),
pTexEmissivity( NULL ),
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
	
	if( pTexDiffuse ){
		delete pTexDiffuse;
	}
	if( pTexReflectivity ){
		delete pTexReflectivity;
	}
	if( pTexEmissivity ){
		delete pTexEmissivity;
	}
}

void deoglGIMaterials::pCreateFBOMaterial(){
	const deoglRestoreFramebuffer restoreFbo( pRenderThread );
	const GLenum buffers[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	
	const int size = pMaterialMapSize * pRowsPerImage;
	
	// GPU RAM consumption (size 2048 squared):
	// - diffuse: 16M
	// - reflectivity: 16M
	// - emissivity: 32M
	// total: 67M
	if( ! pTexDiffuse ){
		pTexDiffuse = new deoglTexture( pRenderThread );
	}
	pTexDiffuse->SetFBOFormat( 4, false );
	pTexDiffuse->SetSize( size, size );
	pTexDiffuse->CreateTexture();
	
	if( ! pTexReflectivity ){
		pTexReflectivity = new deoglTexture( pRenderThread );
	}
	pTexReflectivity->SetFBOFormat( 4, false );
	pTexReflectivity->SetSize( size, size );
	pTexReflectivity->CreateTexture();
	
	if( ! pTexEmissivity ){
		pTexEmissivity = new deoglTexture( pRenderThread );
	}
	pTexEmissivity->SetFBOFormat( 4, true );
	pTexEmissivity->SetSize( size, size );
	pTexEmissivity->CreateTexture();
	
	pRenderThread.GetRenderers().GetLight().GetRenderGI().GetPipelineClearBuffers()->Activate();
	pRenderThread.GetFramebuffer().Activate( &pFBOMaterial );
	pFBOMaterial.DetachAllImages();
	pFBOMaterial.AttachColorTexture( 0, pTexDiffuse );
	pFBOMaterial.AttachColorTexture( 1, pTexReflectivity );
	pFBOMaterial.AttachColorTexture( 2, pTexEmissivity );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 3, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOMaterial.Verify();
	
	const GLfloat clearDiffTintMask[ 4 ] = { 0.85f, 0.85f, 0.85f, 1.0f };
	const GLfloat clearReflRough[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.5f };
	const GLfloat clearEmiss[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearDiffTintMask[ 0 ] ) );
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 1, &clearReflRough[ 0 ] ) );
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 2, &clearEmiss[ 0 ] ) );
}

void deoglGIMaterials::pEnlarge(){
	if( pMaxMaterialCount >= 16383 ){
		DETHROW( deeInvalidParam ); // we should never end up here
	}
	
	const int mapsPerRows = pMaterialsPerRow;
	const int rowsPerImage = pRowsPerImage;
	
	pMaterialMapSize /= 2;
	pMaterialsPerRow *= 2;
	pRowsPerImage *= 2;
	pMaxMaterialCount = pMaxMaterialsPerRow * pMaxRowsPerImage;
	
	// scale image
	deoglTexture * const texDiffuse = pTexDiffuse;
	deoglTexture * const texReflectivity = pTexReflectivity;
	deoglTexture * const texEmissivity = pTexEmissivity;
	
	pTexDiffuse = NULL;
	pTexReflectivity = NULL;
	pTexEmissivity = NULL;
	
	try{
		pCreateFBOMaterial();
		pRenderThread.GetRenderers().GetLight().GetRenderGI().ResizeMaterials(
			*texDiffuse, *texReflectivity, *texEmissivity, mapsPerRows, rowsPerImage );
		
	}catch( const deException & ){
		delete texDiffuse;
		delete texReflectivity;
		delete texEmissivity;
		throw;
	}
	
	delete texDiffuse;
	delete texReflectivity;
	delete texEmissivity;
}

int deoglGIMaterials::pFirstUnusedMaterial() const{
	// if a TUC is found with usage count of 1 it is not used by anymore and has to be
	// deleted. such TUCs are always returned as first choice to improve memory usage.
	// 
	// as second choice TUCs with 0 material usage are returned but only if no empty
	// slots are present. this allows to reused TUCs if they are still in use by world
	// content but not in use by GI instances. by doing this only if no free slots are
	// available instances moving in and out of GI states quickly do not cause TUCs to
	// be re-added quickly. this favors filling up slots first before reusing such TUCs
	const int count = pTUCs.GetCount();
	int i;
	
	for( i=1; i<count; i++ ){
		if( ( ( deoglTexUnitsConfig* )pTUCs.GetAt( i ) )->GetUsageCount() == 1 ){
			return i;
		}
	}
	
	// as second choice TUCs with 0 material usage are returned but only if no empty
	// slots are present. this allows to reused TUCs if they are still in use by world
	// content but not in use by GI instances. by doing this only if no free slots are
	// available instances moving in and out of GI states quickly do not cause TUCs to
	// be re-added quickly. this favors filling up slots first before reusing such TUCs
	if( pTUCs.GetCount() < pMaxMaterialCount ){
		for( i=1; i<count; i++ ){
			if( ( ( deoglTexUnitsConfig* )pTUCs.GetAt( i ) )->GetMaterialUsageCount() == 0 ){
				return i;
			}
		}
	}
	
	return -1;
}

// void deoglGIMaterials::DEBUG(){
// 	pEnlarge();
// 	pMaterialMapSize *= 2;
// 	pMaterialsPerRow /= 2;
// 	pRowsPerImage /= 2;
// 	pMaxMaterialCount = pMaxMaterialsPerRow * pMaxRowsPerImage;
// 	pCreateFBOMaterial();
// }
