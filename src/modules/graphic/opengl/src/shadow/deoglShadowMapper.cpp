/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
#include <string.h>
#include <stdlib.h>
#include "deoglShadowMapper.h"

#include "../configuration/deoglConfiguration.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/arraytexture/deoglRenderableColorArrayTexture.h"
#include "../texture/arraytexture/deoglRenderableColorArrayTextureManager.h"
#include "../texture/arraytexture/deoglRenderableDepthArrayTexture.h"
#include "../texture/arraytexture/deoglRenderableDepthArrayTextureManager.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/cubemap/deoglRenderableColorCubeMap.h"
#include "../texture/cubemap/deoglRenderableColorCubeMapManager.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMap.h"
#include "../texture/cubemap/deoglRenderableDepthCubeMapManager.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglRenderableColorTexture.h"
#include "../texture/texture2d/deoglRenderableColorTextureManager.h"
#include "../texture/texture2d/deoglRenderableDepthTexture.h"
#include "../texture/texture2d/deoglRenderableDepthTextureManager.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../capabilities/deoglCapsTextureFormat.h"

#include <dragengine/common/exceptions.h>



// #define DO_DEBUG_CALLS 1

#ifdef DO_DEBUG_CALLS
#define DBGCALL(m,t,f,u) pRenderThread.GetLogger().LogInfoFormat("ShadowMapper.%s: t=%p f=%p u=%p", m, t, f, u);
#define DBGCALL2(m,t,f,u,p) pRenderThread.GetLogger().LogInfoFormat("ShadowMapper.%s: t=%p f=%p u=%p p=%p", m, t, f, u, p);
#define DBGCALL3(m,f,u) pRenderThread.GetLogger().LogInfoFormat("ShadowMapper.%s: f=%p u=%p", m, f, u);
#else
#define DBGCALL(m,t,f,u)
#define DBGCALL2(m,t,f,u,p)
#define DBGCALL3(m,f,u)
#endif


// Class deoglShadowMapper
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShadowMapper::deoglShadowMapper( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pFBOTextureSolid( NULL ),
pFBOTextureTransp( NULL ),
pTextureDepthSolid( NULL ),
pTextureDepthTransp( NULL ),
pTextureColorTransp( NULL ),
pForeignTexDepthSolid( NULL ),
pForeignTexDepthTransp( NULL ),
pForeignTexColorTransp( NULL ),
pUseTexDepthSolid( NULL ),
pUseTexDepthTransp( NULL ),
pUseTexColorTransp( NULL ),

pFBOCube( NULL ),
pCubeMapDepthSolid( NULL ),
pCubeMapDepthTransp( NULL ),
pCubeMapColorTransp( NULL ),
pForeignCubeMapDepthSolid( NULL ),
pForeignCubeMapDepthTransp( NULL ),
pForeignCubeMapColorTransp( NULL ),
pUseCubeMapDepthSolid( NULL ),
pUseCubeMapDepthTransp( NULL ),
pUseCubeMapColorTransp( NULL ),

pFBOArrTex( NULL ),
pArrTexSolidDepth( NULL ),
pArrTexTranspDepth( NULL ),
pArrTexTranspColor( NULL ),
pForeignArrTexSolidDepth( NULL ),
pForeignArrTexTranspDepth( NULL ),
pForeignArrTexTranspColor( NULL ),
pUseArrTexSolidDepth( NULL ),
pUseArrTexTranspDepth( NULL ),
pUseArrTexTranspColor( NULL ),

pFBOOcclusion( NULL ),
pTextureOcclusion( NULL ),

pFBOAmbient( NULL ),
pTextureAmbient( NULL ),
pForeignTexAmbient( NULL ),
pUseTexAmbient( NULL ),

pFBOCubeAmbient( NULL ),
pCubeMapAmbient( NULL ),
pForeignCubeMapAmbient( NULL ),
pUseCubeMapAmbient( NULL ){
}

deoglShadowMapper::~deoglShadowMapper(){
	pCleanUp();
}



// Management
///////////////

deoglTexture *deoglShadowMapper::GetSolidDepthTexture() const{
	if( ! pTextureDepthSolid ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureDepthSolid->GetTexture();
}

void deoglShadowMapper::SetForeignSolidDepthTexture( deoglTexture *texture ){
	if( pTextureDepthSolid ){
		pTextureDepthSolid->SetInUse( false );
		pTextureDepthSolid = NULL;
	}
	
	pForeignTexDepthSolid = texture;
	pUseTexDepthSolid = texture;
}

deoglTexture *deoglShadowMapper::GetTransparentDepthTexture() const{
	if( ! pTextureDepthTransp ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureDepthTransp->GetTexture();
}

void deoglShadowMapper::SetForeignTransparentDepthTexture( deoglTexture *texture ){
	if( pTextureDepthTransp ){
		pTextureDepthTransp->SetInUse( false );
		pTextureDepthTransp = NULL;
	}
	
	pForeignTexDepthTransp = texture;
	pUseTexDepthTransp = texture;
}

deoglTexture *deoglShadowMapper::GetTransparentColorTexture() const{
	if( ! pTextureColorTransp ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureColorTransp->GetTexture();
}

void deoglShadowMapper::SetForeignTransparentColorTexture( deoglTexture *texture ){
	if( pTextureColorTransp ){
		pTextureColorTransp->SetInUse( false );
		pTextureColorTransp = NULL;
	}
	
	pForeignTexColorTransp = texture;
	pUseTexColorTransp = texture;
}

void deoglShadowMapper::DropTextures(){
	DropTexturesSolid();
	DropTexturesTransparent();
}

void deoglShadowMapper::DropTexturesSolid(){
	if( pFBOTextureSolid ){
		pFBOTextureSolid->DecreaseUsageCount();
		pFBOTextureSolid = NULL;
	}
	
	pUseTexDepthSolid = NULL;
	pForeignTexDepthSolid = NULL;
	if( pTextureDepthSolid ){
		pTextureDepthSolid->SetInUse( false );
		pTextureDepthSolid = NULL;
	}
}

void deoglShadowMapper::DropTexturesTransparent(){
	if( pFBOTextureTransp ){
		pFBOTextureTransp->DecreaseUsageCount();
		pFBOTextureTransp = NULL;
	}
	
	pUseTexDepthTransp = NULL;
	pForeignTexDepthTransp = NULL;
	if( pTextureDepthTransp ){
		pTextureDepthTransp->SetInUse( false );
		pTextureDepthTransp = NULL;
	}
	
	pUseTexColorTransp = NULL;
	pForeignTexColorTransp = NULL;
	if( pTextureColorTransp ){
		pTextureColorTransp->SetInUse( false );
		pTextureColorTransp = NULL;
	}
}

void deoglShadowMapper::DropForeignTextures(){
	if( pForeignTexDepthSolid ){
		pForeignTexDepthSolid = NULL;
		pUseTexDepthSolid = NULL;
	}
	
	if( pForeignTexDepthTransp ){
		pForeignTexDepthTransp = NULL;
		pUseTexDepthTransp = NULL;
	}
	
	if( pForeignTexColorTransp ){
		pForeignTexColorTransp = NULL;
		pUseTexColorTransp = NULL;
	}
}

void deoglShadowMapper::ActivateSolidTexture( int size, bool useFloatDepth, bool withStencil ){
	// drop the textures including the fbo if the size differs
	if( pForeignTexDepthSolid ){
		if( pForeignTexDepthSolid->GetWidth() != size
		|| pForeignTexDepthSolid->GetFormat()->GetIsDepthFloat() != useFloatDepth ){
			DropTexturesSolid();
		}
	}
	if( pTextureDepthSolid ){
		if( pTextureDepthSolid->GetWidth() != size 
		|| pTextureDepthSolid->GetUseFloat() != useFloatDepth ){
			DropTexturesSolid();
		}
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOTextureSolid ){
		pFBOTextureSolid = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain solid depth texture if not existing already
	if( ! pTextureDepthSolid && ! pForeignTexDepthSolid ){
		pTextureDepthSolid = pRenderThread.GetTexture().GetRenderableDepthTexture()
			.GetTextureWith( size, size, false, useFloatDepth );
		pUseTexDepthSolid = pTextureDepthSolid->GetTexture();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOTextureSolid );
	
	pFBOTextureSolid->DetachAllImages();
	pFBOTextureSolid->AttachDepthTexture( pUseTexDepthSolid );
	if( withStencil ){
		pFBOTextureSolid->AttachStencilTexture( pUseTexDepthSolid );
	}
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOTextureSolid->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}

void deoglShadowMapper::ActivateTransparentTexture( int size, bool useFloatDepth ){
	// drop the textures including the fbo if the size differs
	if( pForeignTexDepthTransp ){
		if( pForeignTexDepthTransp->GetWidth() != size
		|| pForeignTexDepthTransp->GetFormat()->GetIsDepthFloat() != useFloatDepth ){
			DropTexturesTransparent();
		}
	}
	if( pTextureDepthTransp ){
		if( pTextureDepthTransp->GetWidth() != size
		|| pTextureDepthTransp->GetUseFloat() != useFloatDepth ){
			DropTexturesTransparent();
		}
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOTextureTransp ){
		pFBOTextureTransp = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain transparent depth texture if not existing already
	if( ! pTextureDepthTransp && ! pForeignTexDepthTransp ){
		pTextureDepthTransp = pRenderThread.GetTexture().GetRenderableDepthTexture()
			.GetTextureWith( size, size, false, false );
		pUseTexDepthTransp = pTextureDepthTransp->GetTexture();
	}
	
	// obtain transparent color texture if not existing already
	if( ! pTextureColorTransp && ! pForeignTexColorTransp ){
		pTextureColorTransp = pRenderThread.GetTexture().GetRenderableColorTexture()
			.GetTextureWith( size, size, 4, false );
		pUseTexColorTransp = pTextureColorTransp->GetTexture();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOTextureTransp );
	
	pFBOTextureTransp->DetachAllImages();
	pFBOTextureTransp->AttachDepthTexture( pUseTexDepthTransp );
	pFBOTextureTransp->AttachColorTexture( 0, pUseTexColorTransp );
	
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBOTextureTransp->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}



deoglCubeMap *deoglShadowMapper::GetSolidDepthCubeMap() const{
	if( ! pCubeMapDepthSolid ){
		DETHROW( deeInvalidParam );
	}
	
	return pCubeMapDepthSolid->GetCubeMap();
}

void deoglShadowMapper::SetForeignSolidDepthCubeMap( deoglCubeMap *cubemap ){
	if( pCubeMapDepthSolid ){
		pCubeMapDepthSolid->SetInUse( false );
		pCubeMapDepthSolid = NULL;
	}
	
	pForeignCubeMapDepthSolid = cubemap;
	pUseCubeMapDepthSolid = cubemap;
	DBGCALL2("SetForeignSolidDepthCubeMap", pCubeMapDepthSolid, pForeignCubeMapDepthSolid, pUseCubeMapDepthSolid, cubemap)
}

deoglCubeMap *deoglShadowMapper::GetTransparentDepthCubeMap() const{
	if( ! pCubeMapDepthTransp ){
		DETHROW( deeInvalidParam );
	}
	
	return pCubeMapDepthTransp->GetCubeMap();
}

void deoglShadowMapper::SetForeignTransparentDepthCubeMap( deoglCubeMap *cubemap ){
	if( pCubeMapDepthTransp ){
		pCubeMapDepthTransp->SetInUse( false );
		pCubeMapDepthTransp = NULL;
	}
	
	pForeignCubeMapDepthTransp = cubemap;
	pUseCubeMapDepthTransp = cubemap;
	DBGCALL2("SetForeignTransparentDepthCubeMap", pCubeMapDepthTransp, pForeignCubeMapDepthTransp, pUseCubeMapDepthTransp, cubemap)
}

deoglCubeMap *deoglShadowMapper::GetTransparentColorCubeMap() const{
	if( ! pCubeMapColorTransp ){
		DETHROW( deeInvalidParam );
	}
	
	return pCubeMapColorTransp->GetCubeMap();
}

void deoglShadowMapper::SetForeignTransparentColorCubeMap( deoglCubeMap *cubemap ){
	if( pCubeMapColorTransp ){
		pCubeMapColorTransp->SetInUse( false );
		pCubeMapColorTransp = NULL;
	}
	
	pForeignCubeMapColorTransp = cubemap;
	pUseCubeMapColorTransp = cubemap;
	DBGCALL2("SetForeignTransparentColorCubeMap", pCubeMapColorTransp, pForeignCubeMapColorTransp, pUseCubeMapColorTransp, cubemap)
}

void deoglShadowMapper::DropCubeMaps(){
	DropCubeMapsSolid();
	DropCubeMapsTransparent();
}

void deoglShadowMapper::DropCubeMapsSolid(){
	if( pFBOCube ){
		pFBOCube->DecreaseUsageCount();
		pFBOCube = NULL;
	}
	
	pUseCubeMapDepthSolid = NULL;
	pForeignCubeMapDepthSolid = NULL;
	if( pCubeMapDepthSolid ){
		pCubeMapDepthSolid->SetInUse( false );
		pCubeMapDepthSolid = NULL;
	}
	DBGCALL("DropSolidCubeMaps", pCubeMapDepthSolid, pForeignCubeMapDepthSolid, pUseCubeMapDepthSolid)
}

void deoglShadowMapper::DropCubeMapsTransparent(){
	if( pFBOCube ){
		pFBOCube->DecreaseUsageCount();
		pFBOCube = NULL;
	}
	
	pUseCubeMapDepthTransp = NULL;
	pForeignCubeMapDepthTransp = NULL;
	if( pCubeMapDepthTransp ){
		pCubeMapDepthTransp->SetInUse( false );
		pCubeMapDepthTransp = NULL;
	}
	DBGCALL("DropTransparentCubeMaps1", pUseCubeMapDepthTransp, pForeignCubeMapDepthTransp, 0)
	
	pUseCubeMapColorTransp = NULL;
	pForeignCubeMapColorTransp = NULL;
	if( pCubeMapColorTransp ){
		pCubeMapColorTransp->SetInUse( false );
		pCubeMapColorTransp = NULL;
	}
	DBGCALL("DropTransparentCubeMaps2", pUseCubeMapColorTransp, pForeignCubeMapColorTransp, pCubeMapColorTransp)
}

void deoglShadowMapper::DropForeignCubeMaps(){
	if( pForeignCubeMapDepthSolid ){
		pForeignCubeMapDepthSolid = NULL;
		pUseCubeMapDepthSolid = NULL;
	}
	DBGCALL3("DropForeignCubeMaps1", pForeignCubeMapDepthSolid, pUseCubeMapDepthSolid)
	
	if( pForeignCubeMapDepthTransp ){
		pForeignCubeMapDepthTransp = NULL;
		pUseCubeMapDepthTransp = NULL;
	}
	DBGCALL3("DropForeignCubeMaps2", pForeignCubeMapDepthTransp, pUseCubeMapDepthTransp)
	
	if( pForeignCubeMapColorTransp ){
		pForeignCubeMapColorTransp = NULL;
		pUseCubeMapColorTransp = NULL;
	}
	DBGCALL3("DropForeignCubeMaps3", pForeignCubeMapColorTransp, pUseCubeMapColorTransp)
}

void deoglShadowMapper::ActivateSolidCubeMap( int size ){
	DBGCALL("ActivateSolidCubeMap", pCubeMapDepthSolid, pForeignCubeMapDepthSolid, pUseCubeMapDepthSolid)
	// drop the cubemaps including the fbo if the size differs
	if( pForeignCubeMapDepthSolid && pForeignCubeMapDepthSolid->GetSize() != size ){
		DropCubeMapsSolid();
	}
	if( pCubeMapDepthSolid && pCubeMapDepthSolid->GetSize() != size ){
		DropCubeMapsSolid();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOCube ){
		pFBOCube = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain solid depth cubemap if not existing already
	if( ! pCubeMapDepthSolid && ! pForeignCubeMapDepthSolid ){
		pCubeMapDepthSolid = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
		pUseCubeMapDepthSolid = pCubeMapDepthSolid->GetCubeMap();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOCube );
	
	pFBOCube->DetachAllImages();
	pFBOCube->AttachDepthCubeMap( pUseCubeMapDepthSolid );
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOCube->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}

void deoglShadowMapper::ActivateSolidCubeMapFace( int size, int face ){
	DBGCALL("ActivateSolidCubeMapFace", pCubeMapDepthSolid, pForeignCubeMapDepthSolid, pUseCubeMapDepthSolid)
	// drop the cubemaps including the fbo if the size differs
	if( pForeignCubeMapDepthSolid && pForeignCubeMapDepthSolid->GetSize() != size ){
		DropCubeMapsSolid();
	}
	if( pCubeMapDepthSolid && pCubeMapDepthSolid->GetSize() != size ){
		DropCubeMapsSolid();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOCube ){
		pFBOCube = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain solid depth cubemap if not existing already
	if( ! pCubeMapDepthSolid && ! pForeignCubeMapDepthSolid ){
		pCubeMapDepthSolid = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
		pUseCubeMapDepthSolid = pCubeMapDepthSolid->GetCubeMap();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOCube );
	
	pFBOCube->DetachAllImages();
	pFBOCube->AttachDepthCubeMapFace( pUseCubeMapDepthSolid, face );
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOCube->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}

void deoglShadowMapper::ActivateTransparentCubeMap( int size ){
	DBGCALL("ActivateTransparentCubeMap", pCubeMapDepthTransp, pForeignCubeMapDepthTransp, pUseCubeMapDepthTransp)
	// drop the cubemaps including the fbo if the size differs
	if( pForeignCubeMapDepthTransp && pForeignCubeMapDepthTransp->GetSize() != size ){
		DropCubeMapsTransparent();
	}
	if( pCubeMapDepthTransp && pCubeMapDepthTransp->GetSize() != size ){
		DropCubeMapsTransparent();
	}
	
	// obtain transparent depth cubemap if not existing already
	if( ! pCubeMapDepthTransp && ! pForeignCubeMapDepthTransp ){
		pCubeMapDepthTransp = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
		pUseCubeMapDepthTransp = pCubeMapDepthTransp->GetCubeMap();
	}
	
	// obtain transparent color cubemap if not existing already
	if( ! pCubeMapColorTransp && ! pForeignCubeMapColorTransp ){
		pCubeMapColorTransp = pRenderThread.GetTexture().GetRenderableColorCubeMap().GetCubeMapWith( size, 4, false );
		pUseCubeMapColorTransp = pCubeMapColorTransp->GetCubeMap();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOCube ){
		pFBOCube = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOCube );
	
	pFBOCube->DetachAllImages();
	pFBOCube->AttachDepthCubeMap( pUseCubeMapDepthTransp );
	pFBOCube->AttachColorCubeMap( 0, pUseCubeMapColorTransp );
	
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBOCube->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}

void deoglShadowMapper::ActivateTransparentCubeMapFace( int size, int face ){
	DBGCALL("ActivateTransparentCubeMapFace", pCubeMapDepthTransp, pForeignCubeMapDepthTransp, pUseCubeMapDepthTransp)
	// drop the cubemaps including the fbo if the size differs
	if( pForeignCubeMapDepthTransp && pForeignCubeMapDepthTransp->GetSize() != size ){
		DropCubeMapsTransparent();
	}
	if( pCubeMapDepthTransp && pCubeMapDepthTransp->GetSize() != size ){
		DropCubeMapsTransparent();
	}
	
	// obtain transparent depth cubemap if not existing already
	if( ! pCubeMapDepthTransp && ! pForeignCubeMapDepthTransp ){
		pCubeMapDepthTransp = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
		pUseCubeMapDepthTransp = pCubeMapDepthTransp->GetCubeMap();
	}
	
	// obtain transparent color cubemap if not existing already
	if( ! pCubeMapColorTransp && ! pForeignCubeMapColorTransp ){
		pCubeMapColorTransp = pRenderThread.GetTexture().GetRenderableColorCubeMap().GetCubeMapWith( size, 4, false );
		pUseCubeMapColorTransp = pCubeMapColorTransp->GetCubeMap();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOCube ){
		pFBOCube = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOCube );
	
	pFBOCube->DetachAllImages();
	pFBOCube->AttachDepthCubeMapFace( pUseCubeMapDepthTransp, face );
	pFBOCube->AttachColorCubeMapFace( 0, pUseCubeMapColorTransp, face );
	
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBOCube->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}



void deoglShadowMapper::CopyShadowMapToShadowCube( int size, int face ){
	/*
	sShadowCube &shadowCube = pShadowCubes[ pGetShadowCubeWith( size ) ];
	//sShadowMap &shadowMap = pShadowMaps[ pGetShadowMapWith( size, true ) ];
	deoglTextureStageManager *tsMgr = pRenderThread.GetTexture().GetStages();
	
	// get the target
	GLenum target = pGetCubeFaceTarget( face );
	
	// copy the depth cube map
	tsMgr->EnableCubeMap( 0, shadowCube.cube, deoglTextureStageManager::etfNearest );
	OGL_CHECK( pRenderThread, glCopyTexSubImage2D( target, 0, 0, 0, 0, 0, size, size ) );
	
	// copy the color cube map
//	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
//	texStageMgr->EnableCubeMap( 0, pShadowCubeMapColor, true );
//	OGL_CHECK( pRenderThread, glCopyTexImage2D( target, 0, GL_RGB, 0, 0, pShadowCubeMapColor->GetWidth(), pShadowCubeMapColor->GetHeight(), 0 ) );
*/
}



deoglArrayTexture *deoglShadowMapper::GetSolidDepthArrayTexture() const{
	if( ! pArrTexSolidDepth ){
		DETHROW( deeInvalidParam );
	}
	
	return pArrTexSolidDepth->GetArrayTexture();
}

void deoglShadowMapper::SetForeignSolidDepthArrayTexture( deoglArrayTexture *texture ){
	if( pArrTexSolidDepth ){
		pArrTexSolidDepth->SetInUse( false );
		pArrTexSolidDepth = NULL;
	}
	
	pForeignArrTexSolidDepth = texture;
	pUseArrTexSolidDepth = texture;
}

deoglArrayTexture *deoglShadowMapper::GetTransparentDepthArrayTexture() const{
	if( ! pArrTexTranspDepth ){
		DETHROW( deeInvalidParam );
	}
	
	return pArrTexTranspDepth->GetArrayTexture();
}

void deoglShadowMapper::SetForeignTransparentDepthArrayTexture( deoglArrayTexture *texture ){
	if( pArrTexTranspDepth ){
		pArrTexTranspDepth->SetInUse( false );
		pArrTexTranspDepth = NULL;
	}
	
	pForeignArrTexTranspDepth = texture;
	pUseArrTexTranspDepth = texture;
}

deoglArrayTexture *deoglShadowMapper::GetTransparentColorArrayTexture() const{
	if( ! pArrTexTranspColor ){
		DETHROW( deeInvalidParam );
	}
	
	return pArrTexTranspColor->GetArrayTexture();
}

void deoglShadowMapper::SetForeignTransparentColorArrayTexture( deoglArrayTexture *texture ){
	if( pArrTexTranspColor ){
		pArrTexTranspColor->SetInUse( false );
		pArrTexTranspColor = NULL;
	}
	
	pForeignArrTexTranspColor = texture;
	pUseArrTexTranspColor = texture;
}

void deoglShadowMapper::DropArrayTextures(){
	if( pFBOArrTex ){
		pFBOArrTex->DecreaseUsageCount();
		pFBOArrTex = NULL;
	}
	
	pUseArrTexSolidDepth = NULL;
	pForeignArrTexSolidDepth = NULL;
	if( pArrTexSolidDepth ){
		pArrTexSolidDepth->SetInUse( false );
		pArrTexSolidDepth = NULL;
	}
	
	pUseArrTexTranspDepth = NULL;
	pForeignArrTexTranspDepth = NULL;
	if( pArrTexTranspDepth ){
		pArrTexTranspDepth->SetInUse( false );
		pArrTexTranspDepth = NULL;
	}
	
	pUseArrTexTranspColor = NULL;
	pForeignArrTexTranspColor = NULL;
	if( pArrTexTranspColor ){
		pArrTexTranspColor->SetInUse( false );
		pArrTexTranspColor = NULL;
	}
}

void deoglShadowMapper::DropForeignArrayTextures(){
	if( pForeignArrTexSolidDepth ){
		pForeignArrTexSolidDepth = NULL;
		pUseArrTexSolidDepth = NULL;
	}
	
	if( pForeignArrTexTranspDepth ){
		pForeignArrTexTranspDepth = NULL;
		pUseArrTexTranspDepth = NULL;
	}
	
	if( pForeignArrTexTranspColor ){
		pForeignArrTexTranspColor = NULL;
		pUseArrTexTranspColor = NULL;
	}
}

void deoglShadowMapper::ActivateSolidArrayTexture( int size, int layerCount, bool withStencil ){
	// drop the textures including the fbo if the size differs
	if( pForeignArrTexSolidDepth && ( pForeignArrTexSolidDepth->GetWidth() != size
	|| pForeignArrTexSolidDepth->GetLayerCount() != layerCount ) ){
		DropArrayTextures();
	}
	if( pArrTexSolidDepth && ( pArrTexSolidDepth->GetWidth() != size
	|| pArrTexSolidDepth->GetLayerCount() != layerCount ) ){
		DropArrayTextures();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOArrTex ){
		pFBOArrTex = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain solid depth texture if not existing already
	if( ! pArrTexSolidDepth && ! pForeignArrTexSolidDepth ){
		pArrTexSolidDepth = pRenderThread.GetTexture().GetRenderableDepthArrayTexture()
			.GetWith( size, size, layerCount, withStencil, false );
		pUseArrTexSolidDepth = pArrTexSolidDepth->GetArrayTexture();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOArrTex );
	
	pFBOArrTex->DetachAllImages();
	pFBOArrTex->AttachDepthArrayTexture( pUseArrTexSolidDepth );
	if( withStencil ){
		pFBOArrTex->AttachStencilArrayTexture( pUseArrTexSolidDepth );
	}
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOArrTex->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}

void deoglShadowMapper::ActivateSolidArrayTextureLayer( int size, int layerCount, int layer, bool withStencil ){
	// drop the textures including the fbo if the size differs
	if( pForeignArrTexSolidDepth && ( pForeignArrTexSolidDepth->GetWidth() != size
	|| pForeignArrTexSolidDepth->GetLayerCount() != layerCount ) ){
		DropArrayTextures();
	}
	if( pArrTexSolidDepth && ( pArrTexSolidDepth->GetWidth() != size
	|| pArrTexSolidDepth->GetLayerCount() != layerCount ) ){
		DropArrayTextures();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOArrTex ){
		pFBOArrTex = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain solid depth texture if not existing already
	if( ! pArrTexSolidDepth && ! pForeignArrTexSolidDepth ){
		pArrTexSolidDepth = pRenderThread.GetTexture().GetRenderableDepthArrayTexture()
			.GetWith( size, size, layerCount, withStencil, false );
		pUseArrTexSolidDepth = pArrTexSolidDepth->GetArrayTexture();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOArrTex );
	
	pFBOArrTex->DetachAllImages();
	pFBOArrTex->AttachDepthArrayTextureLayer( pUseArrTexSolidDepth, layer );
	if( withStencil ){
		pFBOArrTex->AttachStencilArrayTextureLayer( pUseArrTexSolidDepth, layer );
	}
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOArrTex->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}

void deoglShadowMapper::ActivateTransparentArrayTexture( int size, int layerCount ){
	// drop the textures including the fbo if the size differs
	if( pForeignArrTexTranspDepth && ( pForeignArrTexTranspDepth->GetWidth() != size || pForeignArrTexTranspDepth->GetLayerCount() != layerCount ) ){
		DropArrayTextures();
	}
	if( pArrTexTranspDepth && ( pArrTexTranspDepth->GetWidth() != size || pArrTexTranspDepth->GetLayerCount() != layerCount ) ){
		DropArrayTextures();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOArrTex ){
		pFBOArrTex = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain transparent depth texture if not existing already
	if( ! pArrTexTranspDepth && ! pForeignArrTexTranspDepth ){
		pArrTexTranspDepth = pRenderThread.GetTexture().GetRenderableDepthArrayTexture().
			GetWith( size, size, layerCount, false, false );
		pUseArrTexTranspDepth = pArrTexTranspDepth->GetArrayTexture();
	}
	
	// obtain transparent color texture if not existing already
	if( ! pArrTexTranspColor && ! pForeignArrTexTranspColor ){
		pArrTexTranspColor = pRenderThread.GetTexture().GetRenderableColorArrayTexture().
			GetWith( size, size, layerCount, 4, false );
		pUseArrTexTranspColor = pArrTexTranspColor->GetArrayTexture();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOArrTex );
	
	pFBOArrTex->DetachAllImages();
	pFBOArrTex->AttachDepthArrayTexture( pUseArrTexTranspDepth );
	pFBOArrTex->AttachColorArrayTexture( 0, pForeignArrTexTranspColor );
	
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBOArrTex->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}



deoglTexture *deoglShadowMapper::GetOcclusionTexture() const{
	if( ! pTextureOcclusion ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureOcclusion->GetTexture();
}

void deoglShadowMapper::DropOcclusionTextures(){
	if( pFBOOcclusion ){
		pFBOOcclusion->DecreaseUsageCount();
		pFBOOcclusion = NULL;
	}
	
	if( pTextureOcclusion ){
		pTextureOcclusion->SetInUse( false );
		pTextureOcclusion = NULL;
	}
}

void deoglShadowMapper::ActivateOcclusionTexture( int width, int height ){
	// drop the textures including the fbo if the size differs
	if( pTextureOcclusion && ( pTextureOcclusion->GetWidth() != width || pTextureOcclusion->GetHeight() != height ) ){
		DropOcclusionTextures();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOOcclusion ){
		pFBOOcclusion = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( width, height );
	}
	
	// obtain occlusion texture if not existing already
	if( ! pTextureOcclusion ){
		pTextureOcclusion = pRenderThread.GetTexture().GetRenderableColorTexture().GetTextureWith( width, height, 4, false );
	}
	
	// switch to the framebuffer required by this occlusion map
	pRenderThread.GetFramebuffer().Activate( pFBOOcclusion );
	
	pFBOOcclusion->DetachAllImages();
	pFBOOcclusion->AttachColorTexture( 0, pTextureOcclusion->GetTexture() );
	
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBOOcclusion->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, width, height ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}



deoglTexture *deoglShadowMapper::GetAmbientTexture() const{
	if( ! pTextureAmbient ){
		DETHROW( deeInvalidParam );
	}
	
	return pTextureAmbient->GetTexture();
}

void deoglShadowMapper::SetForeignAmbientTexture( deoglTexture *texture ){
	if( pTextureAmbient ){
		pTextureAmbient->SetInUse( false );
		pTextureAmbient = NULL;
	}
	
	pForeignTexAmbient = texture;
	pUseTexAmbient = texture;
}

void deoglShadowMapper::DropAmbientTextures(){
	if( pFBOAmbient ){
		pFBOAmbient->DecreaseUsageCount();
		pFBOAmbient = NULL;
	}
	
	pUseTexAmbient = NULL;
	pForeignTexAmbient = NULL;
	if( pTextureAmbient ){
		pTextureAmbient->SetInUse( false );
		pTextureAmbient = NULL;
	}
}

void deoglShadowMapper::DropForeignAmbientTextures(){
	if( pForeignTexAmbient ){
		pForeignTexAmbient = NULL;
		pUseTexAmbient = NULL;
	}
}

void deoglShadowMapper::ActivateAmbientTexture( int size, bool useFloatDepth ){
	// drop the textures including the fbo if the size differs
	if( pForeignTexAmbient ){
		if( pForeignTexAmbient->GetWidth() != size
		|| pForeignTexAmbient->GetFormat()->GetIsDepthFloat() != useFloatDepth ){
			DropAmbientTextures();
		}
	}
	if( pTextureAmbient ){
		if( pTextureAmbient->GetWidth() != size || pTextureAmbient->GetUseFloat() != useFloatDepth ){
			DropAmbientTextures();
		}
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOAmbient ){
		pFBOAmbient = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain ambient texture if not existing already
	if( ! pTextureAmbient && ! pForeignTexAmbient ){
		pTextureAmbient = pRenderThread.GetTexture().GetRenderableDepthTexture()
			.GetTextureWith( size, size, false, useFloatDepth );
		pUseTexAmbient = pTextureAmbient->GetTexture();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOAmbient );
	
	pFBOAmbient->DetachAllImages();
	pFBOAmbient->AttachDepthTexture( pUseTexAmbient );
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOAmbient->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}



deoglCubeMap *deoglShadowMapper::GetAmbientCubeMap() const{
	if( ! pCubeMapAmbient ){
		DETHROW( deeInvalidParam );
	}
	return pCubeMapAmbient->GetCubeMap();
}

void deoglShadowMapper::SetForeignAmbientCubeMap( deoglCubeMap *cubemap ){
	if( pCubeMapAmbient ){
		pCubeMapAmbient->SetInUse( false );
		pCubeMapAmbient = NULL;
	}
	
	pForeignCubeMapAmbient = cubemap;
	pUseCubeMapAmbient = cubemap;
	DBGCALL2("SetForeignAmbientCubeMap", pCubeMapAmbient, pForeignCubeMapAmbient, pUseCubeMapAmbient, cubemap)
}

void deoglShadowMapper::DropAmbientCubeMaps(){
	if( pFBOCubeAmbient ){
		pFBOCubeAmbient->DecreaseUsageCount();
		pFBOCubeAmbient = NULL;
	}
	
	pUseCubeMapAmbient = NULL;
	pForeignCubeMapAmbient = NULL;
	if( pCubeMapAmbient ){
		pCubeMapAmbient->SetInUse( false );
		pCubeMapAmbient = NULL;
	}
	DBGCALL("DropAmbientCubeMaps", pCubeMapAmbient, pForeignCubeMapAmbient, pUseCubeMapAmbient)
}

void deoglShadowMapper::DropForeignAmbientCubeMaps(){
	if( pForeignCubeMapAmbient ){
		pForeignCubeMapAmbient = NULL;
		pUseCubeMapAmbient = NULL;
	}
	DBGCALL3("DropForeignAmbientCubeMaps", pForeignCubeMapAmbient, pUseCubeMapAmbient)
}

void deoglShadowMapper::ActivateAmbientCubeMap( int size ){
	DBGCALL("ActivateAmbientCubeMap", pCubeMapAmbient, pForeignCubeMapAmbient, pUseCubeMapAmbient)
	// drop the cubemaps including the fbo if the size differs
	if( pForeignCubeMapAmbient && pForeignCubeMapAmbient->GetSize() != size ){
		DropAmbientCubeMaps();
	}
	if( pCubeMapAmbient && pCubeMapAmbient->GetSize() != size ){
		DropAmbientCubeMaps();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOCubeAmbient ){
		pFBOCubeAmbient = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain solid depth cubemap if not existing already
	if( ! pCubeMapAmbient && ! pForeignCubeMapAmbient ){
		pCubeMapAmbient = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
		pUseCubeMapAmbient = pCubeMapAmbient->GetCubeMap();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOCubeAmbient );
	
	pFBOCubeAmbient->DetachAllImages();
	pFBOCubeAmbient->AttachDepthCubeMap( pUseCubeMapAmbient );
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOCubeAmbient->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}

void deoglShadowMapper::ActivateAmbientCubeMapFace( int size, int face ){
	DBGCALL("ActivateAmbientCubeMapFace", pCubeMapAmbient, pForeignCubeMapAmbient, pUseCubeMapAmbient)
	// drop the cubemaps including the fbo if the size differs
	if( pForeignCubeMapAmbient && pForeignCubeMapAmbient->GetSize() != size ){
		DropAmbientCubeMaps();
	}
	if( pCubeMapAmbient && pCubeMapAmbient->GetSize() != size ){
		DropAmbientCubeMaps();
	}
	
	// obtain a framebuffer for this size if not existing already
	if( ! pFBOCubeAmbient ){
		pFBOCubeAmbient = pRenderThread.GetFramebuffer().GetManager().GetFBOWithResolution( size, size );
	}
	
	// obtain depth cubemap if not existing already
	if( ! pCubeMapAmbient && ! pForeignCubeMapAmbient ){
		pCubeMapAmbient = pRenderThread.GetTexture().GetRenderableDepthCubeMap().GetCubeMapWith( size );
		pUseCubeMapAmbient = pCubeMapAmbient->GetCubeMap();
	}
	
	// switch to the framebuffer required by this shadow map
	pRenderThread.GetFramebuffer().Activate( pFBOCubeAmbient );
	
	pFBOCubeAmbient->DetachAllImages();
	pFBOCubeAmbient->AttachDepthCubeMapFace( pUseCubeMapAmbient, face );
	
	const GLenum buffers[ 1 ] = { GL_NONE };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	
	pFBOCubeAmbient->Verify();
	
	OGL_CHECK( pRenderThread, glViewport( 0, 0, size, size ) );
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
}



int deoglShadowMapper::ShadowMapSize ( const deoglConfiguration &config ){
	switch( config.GetShadowQuality() ){
	case deoglConfiguration::esqVeryHigh:
		return 4096;
		
	case deoglConfiguration::esqHigh:
	default:
		return 2048;
		
	case deoglConfiguration::esqMedium:
		return 1024;
		
	case deoglConfiguration::esqLow:
		return 512;
		
	case deoglConfiguration::esqVeryLow:
		return 256;
	}
}

int deoglShadowMapper::ShadowCubeSize( const deoglConfiguration& config ){
	// cube map sizes above 1024 are brutal on the memory consumption especially if many
	// point lights are close to the cameras. later on this can be modified to use higher
	// resolution if the count of active point light shadow cubes is low
	// 
	// the memory consumption of a cube map is 6x the memory consumption of a shadow map
	// of the same size. if the cube map is half the size of the shadow map the memory
	// consumption of the cube map is 1.5x the memory consumption of the shadow map
	// 
	// for the time being half the shadow map size is used.
	switch( config.GetShadowQuality() ){
	case deoglConfiguration::esqVeryHigh:
		return 2048; //4096;
		
	case deoglConfiguration::esqHigh:
	default:
		return 1024; //2048;
		
	case deoglConfiguration::esqMedium:
		return 512; //1024;
		
	case deoglConfiguration::esqLow:
		return 256; //512;
		
	case deoglConfiguration::esqVeryLow:
		return 128; //256;
	}
}



// Private Functions
//////////////////////

void deoglShadowMapper::pCleanUp(){
	DropAmbientCubeMaps();
	DropAmbientTextures();
	DropOcclusionTextures();
	DropArrayTextures();
	DropCubeMaps();
	DropTextures();
}
/*
GLenum deoglShadowMapper::pGetCubeFaceTarget( int face ){
	if( face == deoglCubeMap::efPositiveX ){
		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		
	}else if( face == deoglCubeMap::efNegativeX ){
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		
	}else if( face == deoglCubeMap::efPositiveY ){
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		
	}else if( face == deoglCubeMap::efNegativeY ){
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		
	}else if( face == deoglCubeMap::efPositiveZ ){
		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		
	}else if( face == deoglCubeMap::efNegativeZ ){
		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
	}
	
	DETHROW( deeInvalidParam );
}
*/
