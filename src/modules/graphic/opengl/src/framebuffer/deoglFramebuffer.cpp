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

#include "deoglFramebuffer.h"
#include "../capabilities/deoglCapabilities.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTDebug.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>



// Definitions
////////////////

static GLenum targetMap[] = {
	0, // eatNone
	GL_TEXTURE_2D, // eatTexture
	GL_TEXTURE_CUBE_MAP, // eatCubeMap
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, // eatCubeMapPosX
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // eatCubeMapNegX
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // eatCubeMapPosY
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // eatCubeMapNegY
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // eatCubeMapPosZ
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // eatCubeMapNegZ
	0, // eatArrayTexture
	0 // eatArrayTextureLayer
};

// define to make Verify check the framebuffer for errors. comment this
// out in release builds as it sucks a bit of performance
#define VERIFY_FRAMEBUFFER 1

static const GLenum colorAttachments[ FBO_MAX_ATTACHMENT_COUNT ] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2,
	GL_COLOR_ATTACHMENT3,
	GL_COLOR_ATTACHMENT4,
	GL_COLOR_ATTACHMENT5,
	GL_COLOR_ATTACHMENT6,
	GL_COLOR_ATTACHMENT7
};

static const GLenum depthAttachment[ 1 ] = { GL_DEPTH_ATTACHMENT };
static const GLenum stencilAttachment[ 1 ] = { GL_STENCIL_ATTACHMENT };
static const GLenum depthStencilAttachment[ 1 ] = { GL_DEPTH_STENCIL_ATTACHMENT };
static const GLenum noAttachment[ 1 ] = { GL_NONE };



// Class deoglFramebuffer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglFramebuffer::deoglFramebuffer( deoglRenderThread &renderThread, bool primary ) :
pRenderThread( renderThread ),
pFBO( 0 ),
pPrimary( primary ),
pUsageWidth( 1 ),
pUsageHeight( 1 ),
pUsageCount( 0 )
{
	if( primary ){
		return;
	}
	
	try{
		OGL_CHECK( pRenderThread, pglGenFramebuffers( 1, &pFBO ) );
		if( pFBO == 0 ){
			DETHROW( deeOutOfMemory );
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglFramebuffer::~deoglFramebuffer(){
	pCleanUp();
}



// Management
///////////////

void deoglFramebuffer::SetUsageResolution( int width, int height ){
	if( width < 1 || height < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pUsageWidth = width;
	pUsageHeight = height;
}

void deoglFramebuffer::IncreaseUsageCount(){
	pUsageCount++;
}

void deoglFramebuffer::DecreaseUsageCount(){
	pUsageCount--;
}



void deoglFramebuffer::SetAsCurrent() const{
	// if primary, pFBO is 0 which is what we need
	OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_FRAMEBUFFER, pFBO ) );
}

void deoglFramebuffer::Verify(){
#ifdef VERIFY_FRAMEBUFFER
	if( pPrimary ){
		return; // primary is always complete
	}
	
	const GLenum status = pglCheckFramebufferStatus( GL_FRAMEBUFFER );
	
	if( status != GL_FRAMEBUFFER_COMPLETE ){
		// no access to exception trace from within render thread. requires storing away for later use in logger
// 		deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglFramebuffer::Verify", __LINE__ );
		
		if( status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" );
// 			tracePoint.AddValue( "description", "One or more framebuffer attachment points are not 'framebuffer attachement complete'" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" );
// 			tracePoint.AddValue( "description", "No attachement points contains an image" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS" );
// 			tracePoint.AddValue( "description", "Not all attached images have the same dimensions" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_FORMATS ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_FORMATS" );
// 			tracePoint.AddValue( "description", "Not all color attachments have the same internal format" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_FORMATS" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" );
// 			tracePoint.AddValue( "description", "Not all color attachments set for writing using glDrawBuffers have an image assigned" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" );
// 			tracePoint.AddValue( "description", "Not all color attachments set for reading using glReadBuffers have an image assigned" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" );
// 			tracePoint.AddValue( "description", "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" );
// 			tracePoint.AddValue( "description", "Not all attachments have the same layer count and are of the same layered type" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" );
			
		}else if( status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB" );
// 			tracePoint.AddValue( "description", "Not all attachments have the same layer count and are of the same layered type" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB" );
			
		}else if( status == GL_FRAMEBUFFER_UNSUPPORTED ){
// 			tracePoint.AddValue( "error", "GL_FRAMEBUFFER_UNSUPPORTED" );
// 			tracePoint.AddValue( "description", "The combination of formats used by the attachment points is not supported by the hardware" );
			pRenderThread.GetLogger().LogError( "FBO: GL_FRAMEBUFFER_UNSUPPORTED" );
			
		}else{
// 			tracePoint.AddValueInt( "error", status );
// 			tracePoint.AddValue( "description", "An unknown error has occurred that is not listed in the OpenGL specifications ( broken driver ? )" );
			pRenderThread.GetLogger().LogErrorFormat( "FBO: Unknown Error Code %d!", status );
		}
		
		DebugPrint( "FBO Configuration:" );
		
// 		AddConfigToTrace( tracePoint );
		DETHROW( deeInvalidParam );
	}
#endif
}



bool deoglFramebuffer::IsColorAttached( int index ) const{
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pAttColor[ index ].type != eatNone;
}

bool deoglFramebuffer::IsColorDetached( int index ) const{
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pAttColor[ index ].type == eatNone;
}

void deoglFramebuffer::AttachColorTexture( int index, deoglTexture *texture ){
	AttachColorTextureLevel( index, texture, 0 );
}

void deoglFramebuffer::AttachColorTextureLevel( int index, deoglTexture *texture, int level ){
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT || ! texture ){
		DETHROW( deeInvalidParam );
	}
	AttachColorTextureLevel( index, texture->GetTexture(), level );
}

void deoglFramebuffer::AttachColorCubeMap( int index, deoglCubeMap* texture ){
	AttachColorCubeMapLevel( index, texture, 0 );
}

void deoglFramebuffer::AttachColorCubeMapLevel( int index, deoglCubeMap* texture, int level ){
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttColor[ index ].DoesNotMatch( image, eatCubeMap, level ) ){
		DetachColorImage( index );
		
		if( pglFramebufferTexture ){
			OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + index, image, level ) );
			
		}else{
			DETHROW( deeInvalidAction );
		}
		
		pAttColor[ index ].Set( image, eatCubeMap, level );
	}
}

void deoglFramebuffer::AttachColorCubeMapFace( int index, deoglCubeMap *texture, int face ){
	AttachColorCubeMapFaceLevel( index, texture, face, 0 );
}

void deoglFramebuffer::AttachColorCubeMapFaceLevel( int index, deoglCubeMap *texture, int face, int level ){
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT || ! texture || face < 0 || face > 5 ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	int type;
	
	if( face == deoglCubeMap::efPositiveX ){
		type = eatCubeMapPosX;
		
	}else if( face == deoglCubeMap::efNegativeX ){
		type = eatCubeMapNegX;
		
	}else if( face == deoglCubeMap::efPositiveY ){
		type = eatCubeMapPosY;
		
	}else if( face == deoglCubeMap::efNegativeY ){
		type = eatCubeMapNegY;
		
	}else if( face == deoglCubeMap::efPositiveZ ){
		type = eatCubeMapPosZ;
		
	}else{// if( face == deoglCubeMap::efNegativeZ ){
		type = eatCubeMapNegZ;
	}
	
	if( pAttColor[ index ].DoesNotMatch( image, type, level, face ) ){
		DetachColorImage( index );
		
		// if( pglFramebufferTexture2D ){
			// this is the core 4.6 recommended one. use *Face as fallback
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + index, targetMap[ type ], image, level ) );
			
		/*}else{
			OGL_CHECK( pRenderThread, pglFramebufferTextureFace( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + index, image, level, targetMap[ type ] ) );
		}*/
		
		pAttColor[ index ].Set( image, type, level, face );
	}
}

void deoglFramebuffer::AttachColorArrayTexture( int index, deoglArrayTexture *texture ){
	AttachColorArrayTextureLevel( index, texture, 0 );
}

void deoglFramebuffer::AttachColorArrayTextureLevel( int index, deoglArrayTexture *texture, int level ){
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if(texture->GetLayerCount() == 1){
		AttachColorArrayTextureLayerLevel(index, texture, 0, level);
		return;
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttColor[ index ].DoesNotMatch( image, eatArrayTexture, level ) ){
		DetachColorImage( index );
		
#ifdef WITH_OPENGLES
		if(pglFramebufferTextureMultiviewOVR){
			OGL_CHECK(pRenderThread, pglFramebufferTextureMultiviewOVR(GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + index, image, level, 0, texture->GetLayerCount()));
		}else
#endif
		if( pglFramebufferTexture ){
			OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + index, image, level ) );
			
		}else{
			DETHROW( deeInvalidAction );
		}
		
		pAttColor[ index ].Set( image, eatArrayTexture, level );
	}
}

void deoglFramebuffer::AttachColorArrayTextureLayer( int index,
deoglArrayTexture *texture, int layer ){
	AttachColorArrayTextureLayerLevel( index, texture, layer, 0 );
}

void deoglFramebuffer::AttachColorArrayTextureLayerLevel( int index,
deoglArrayTexture *texture, int layer, int level ){
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttColor[ index ].DoesNotMatch( image, eatArrayTextureLayer, level, layer ) ){
		DetachColorImage( index );
		
		OGL_CHECK( pRenderThread, pglFramebufferTextureLayer( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + index, image, level, layer ) );
		
		pAttColor[ index ].Set( image, eatArrayTextureLayer, level, layer );
	}
}

void deoglFramebuffer::AttachColorTextureLevel( int index, GLuint texture, int level ){
	if( pPrimary || index < 0 || index >= FBO_MAX_ATTACHMENT_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	if( pAttColor[ index ].DoesNotMatch( texture, eatTexture, level ) ){
		DetachColorImage( index );
		
		if( pglFramebufferTexture
		&& pRenderThread.GetCapabilities().GetFramebufferTextureSingle().Working() ){
			OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + index, texture, level ) );
			
		}else{
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture, level ) );
		}
		
		pAttColor[ index ].Set( texture, eatTexture, level );
	}
}

void deoglFramebuffer::DetachColorImage( int index ){
	if( pAttColor[ index ].type == eatNone ){
		return;
	}
	
	if( pglFramebufferTexture && pRenderThread.GetCapabilities().GetFramebufferTextureSingle().Working() ){
		OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + index, 0, 0 ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + index, targetMap[ pAttColor[ index ].type ], 0, 0 ) );
	}
	
	pAttColor[ index ].Reset();
}

void deoglFramebuffer::DetachColorImages( int startIndex ){
	while( startIndex < FBO_MAX_ATTACHMENT_COUNT ){
		DetachColorImage( startIndex++ );
	}
}

void deoglFramebuffer::InvalidateColor( int index ){
	if( ! pglInvalidateFramebuffer ){
		return;
	}
	if( pAttColor[ index ].type == eatNone ){
		return;
	}
	
	OGL_CHECK( pRenderThread, pglInvalidateFramebuffer( GL_FRAMEBUFFER, 1, &colorAttachments[ index ] ) );
}



bool deoglFramebuffer::IsDepthAttached() const{
	return pAttDepth.type != eatNone;
}

bool deoglFramebuffer::IsDepthDetached() const{
	return pAttDepth.type == eatNone;
}

void deoglFramebuffer::AttachDepthTexture( deoglTexture *texture ){
	AttachDepthTextureLevel( texture, 0 );
}

void deoglFramebuffer::AttachDepthTextureLevel( deoglTexture *texture, int level ){
	if( pPrimary || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttDepth.DoesNotMatch( image, eatTexture, level ) ){
		DetachDepthImage();
		
		if( pglFramebufferTexture
		&& pRenderThread.GetCapabilities().GetFramebufferTextureSingle().Working() ){
			OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, image, level ) );
			
		}else{
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, image, level ) );
		}
		
		pAttDepth.Set( image, eatTexture, level );
	}
}

void deoglFramebuffer::AttachDepthCubeMap( deoglCubeMap *texture ){
	AttachDepthCubeMapLevel( texture, 0 );
}

void deoglFramebuffer::AttachDepthCubeMapLevel( deoglCubeMap *texture, int level ){
	if( pPrimary || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttDepth.DoesNotMatch( image, eatCubeMap, level ) ){
		DetachDepthImage();
		
		if( pglFramebufferTexture ){
			OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, image, level ) );
			
		}else{
			DETHROW( deeInvalidAction );
		}
		
		pAttDepth.Set( image, eatCubeMap, level );
	}
}

void deoglFramebuffer::AttachDepthCubeMapFace( deoglCubeMap *texture, int face ){
	AttachDepthCubeMapFaceLevel( texture, face, 0 );
}

void deoglFramebuffer::AttachDepthCubeMapFaceLevel( deoglCubeMap *texture, int face, int level ){
	if( pPrimary || ! texture || face < 0 || face > 5 ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	int type;
	
	if( face == deoglCubeMap::efPositiveX ){
		type = eatCubeMapPosX;
		
	}else if( face == deoglCubeMap::efNegativeX ){
		type = eatCubeMapNegX;
		
	}else if( face == deoglCubeMap::efPositiveY ){
		type = eatCubeMapPosY;
		
	}else if( face == deoglCubeMap::efNegativeY ){
		type = eatCubeMapNegY;
		
	}else if( face == deoglCubeMap::efPositiveZ ){
		type = eatCubeMapPosZ;
		
	}else{// if( face == deoglCubeMap::efNegativeZ ){
		type = eatCubeMapNegZ;
	}
	
	if( pAttDepth.DoesNotMatch( image, type, level, face ) ){
		DetachDepthImage();
		
		// if( pglFramebufferTexture2D ){
			// this is the core 4.6 recommended one. use *Face as fallback
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, targetMap[ type ], image, level ) );
			
		/*}else{
			OGL_CHECK( pRenderThread, pglFramebufferTextureFace( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, image, level, targetMap[ type ] ) );
		} */
		
		pAttDepth.Set( image, type, level, face );
	}
}

void deoglFramebuffer::AttachDepthArrayTexture( deoglArrayTexture *texture ){
	AttachDepthArrayTextureLevel( texture, 0 );
}

void deoglFramebuffer::AttachDepthArrayTextureLevel( deoglArrayTexture *texture, int level ){
	if( pPrimary || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if(texture->GetLayerCount() == 1){
		AttachDepthArrayTextureLayerLevel(texture, 0, level);
		return;
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttDepth.DoesNotMatch( image, eatArrayTexture, level ) ){
		DetachDepthImage();
		
#ifdef WITH_OPENGLES
		if(pglFramebufferTextureMultiviewOVR){
			OGL_CHECK(pRenderThread, pglFramebufferTextureMultiviewOVR(GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, image, level, 0, texture->GetLayerCount()));
		}else
#endif
		if(pglFramebufferTexture){
			OGL_CHECK(pRenderThread, pglFramebufferTexture(GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, image, level));
			
		}else{
			DETHROW(deeInvalidAction);
		}
		
		pAttDepth.Set( image, eatArrayTexture, level );
	}
}

void deoglFramebuffer::AttachDepthArrayTextureLayer( deoglArrayTexture *texture, int layer ){
	AttachDepthArrayTextureLayerLevel( texture, layer, 0 );
}

void deoglFramebuffer::AttachDepthArrayTextureLayerLevel( deoglArrayTexture *texture, int layer, int level ){
	if( pPrimary || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttDepth.DoesNotMatch( image, eatArrayTextureLayer, level, layer ) ){
		DetachDepthImage();
		
		OGL_CHECK( pRenderThread, pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, image, level, layer ) );
		
		pAttDepth.Set( image, eatArrayTextureLayer, level, layer );
	}
}

void deoglFramebuffer::DetachDepthImage(){
	if( pAttDepth.type == eatNone ){
		return;
	}
	
	if( pglFramebufferTexture && pRenderThread.GetCapabilities().GetFramebufferTextureSingle().Working() ){
		OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, 0, 0 ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, targetMap[ pAttDepth.type ], 0, 0 ) );
	}
	
	pAttDepth.Reset();
}

void deoglFramebuffer::InvalidateDepth(){
	if( ! pglInvalidateFramebuffer ){
		return;
	}
	if( pAttDepth.type == eatNone ){
		return;
	}
	
	OGL_CHECK( pRenderThread, pglInvalidateFramebuffer( GL_FRAMEBUFFER, 1, &depthAttachment[ 0 ] ) );
}



bool deoglFramebuffer::IsStencilAttached() const{
	return pAttStencil.type != eatNone;
}

bool deoglFramebuffer::IsStencilDetached() const{
	return pAttStencil.type == eatNone;
}

void deoglFramebuffer::AttachStencilTexture( deoglTexture *texture ){
	AttachStencilTextureLevel( texture, 0 );
}

void deoglFramebuffer::AttachStencilTextureLevel( deoglTexture *texture, int level ){
	if( pPrimary || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	if( ! pAttStencil.DoesNotMatch( image, eatTexture, level ) ){
		return;
	}
	
	DetachStencilImage();
	
	if( pglFramebufferTexture
	&& pRenderThread.GetCapabilities().GetFramebufferTextureSingle().Working() ){
		OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT, image, level ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, image, level ) );
	}
	
	pAttStencil.Set( image, eatTexture, level );
}

void deoglFramebuffer::AttachStencilArrayTexture( deoglArrayTexture *texture ){
	AttachStencilArrayTextureLevel( texture, 0 );
}

void deoglFramebuffer::AttachStencilArrayTextureLevel( deoglArrayTexture *texture, int level ){
	if( pPrimary || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if(texture->GetLayerCount() == 1){
		AttachStencilArrayTextureLayerLevel(texture, 0, level);
		return;
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttStencil.DoesNotMatch( image, eatArrayTexture, level ) ){
		DetachStencilImage();
		
#ifdef WITH_OPENGLES
		if(pglFramebufferTextureMultiviewOVR){
			OGL_CHECK(pRenderThread, pglFramebufferTextureMultiviewOVR(GL_FRAMEBUFFER,
				GL_STENCIL_ATTACHMENT, image, level, 0, texture->GetLayerCount()));
		}else
#endif
		if(pglFramebufferTexture){
			OGL_CHECK(pRenderThread, pglFramebufferTexture(GL_FRAMEBUFFER,
				GL_STENCIL_ATTACHMENT, image, level));
			
		}else{
			DETHROW(deeInvalidAction);
		}
		
		pAttStencil.Set( image, eatArrayTexture, level );
	}
}

void deoglFramebuffer::AttachStencilArrayTextureLayer( deoglArrayTexture *texture, int layer ){
	AttachStencilArrayTextureLayerLevel( texture, layer, 0 );
}

void deoglFramebuffer::AttachStencilArrayTextureLayerLevel( deoglArrayTexture *texture, int layer, int level ){
	if( pPrimary || ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	const GLuint image = texture->GetTexture();
	
	if( pAttStencil.DoesNotMatch( image, eatArrayTextureLayer, level, pAttStencil.layer ) ){
		DetachStencilImage();
		
		OGL_CHECK( pRenderThread, pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, image, level, layer ) );
		
		pAttStencil.Set( image, eatArrayTextureLayer, level, layer );
	}
}

void deoglFramebuffer::DetachStencilImage(){
	if( pAttStencil.type == eatNone ){
		return;
	}
	
	if( pglFramebufferTexture && pRenderThread.GetCapabilities().GetFramebufferTextureSingle().Working() ){
		OGL_CHECK( pRenderThread, pglFramebufferTexture( GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT, 0, 0 ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT, targetMap[ pAttStencil.type ], 0, 0 ) );
	}
	
	pAttStencil.Reset();
}

void deoglFramebuffer::InvalidateStencil(){
	if( ! pglInvalidateFramebuffer ){
		return;
	}
	if( pAttStencil.type == eatNone ){
		return;
	}
	
	OGL_CHECK( pRenderThread, pglInvalidateFramebuffer( GL_FRAMEBUFFER, 1, &stencilAttachment[ 0 ] ) );
}

void deoglFramebuffer::InvalidateDepthStencil(){
	if( ! pglInvalidateFramebuffer ){
		return;
	}
	
	if( pAttDepth.type != eatNone && pAttStencil.type != eatNone ){
		OGL_CHECK( pRenderThread, pglInvalidateFramebuffer( GL_FRAMEBUFFER, 1, &depthStencilAttachment[ 0 ] ) );
		
	}else if( pAttDepth.type != eatNone ){
		OGL_CHECK( pRenderThread, pglInvalidateFramebuffer( GL_FRAMEBUFFER, 1, &depthAttachment[ 0 ] ) );
		
	}else if( pAttStencil.type != eatNone ){
		OGL_CHECK( pRenderThread, pglInvalidateFramebuffer( GL_FRAMEBUFFER, 1, &stencilAttachment[ 0 ] ) );
	}
}



void deoglFramebuffer::DetachAllImages(){
	DetachColorImages( 0 );
	DetachDepthImage();
	DetachStencilImage();
}

void deoglFramebuffer::InvalidateAllImages(){
	if( ! pglInvalidateFramebuffer ){
		return;
	}
	
	GLenum buffers[ FBO_MAX_ATTACHMENT_COUNT + 1 ];
	int i, count = 0;
	
	for( i=0; i<FBO_MAX_ATTACHMENT_COUNT; i++ ){
		if( pAttColor[ i ].type != eatNone ){
			buffers[ count++ ] = colorAttachments[ i ];
		}
	}
	
	if( pAttDepth.type != eatNone && pAttStencil.type != eatNone ){
		buffers[ count++ ] = depthStencilAttachment[ 0 ];
		
	}else if( pAttDepth.type != eatNone ){
		buffers[ count++ ] = depthAttachment[ 0 ];
		
	}else if( pAttStencil.type != eatNone ){
		buffers[ count++ ] = stencilAttachment[ 0 ];
	}
	
	if( count > 0 ){
		OGL_CHECK( pRenderThread, pglInvalidateFramebuffer( GL_FRAMEBUFFER, count, &buffers[ 0 ] ) );
	}
}



void deoglFramebuffer::UpdateReadWriteBuffers(){
	int bufferCount;
	for( bufferCount=0; bufferCount<FBO_MAX_ATTACHMENT_COUNT; bufferCount++ ){
		if( pAttColor[ bufferCount ].type == eatNone ){
			break;
		}
	}
	
	if( bufferCount > 0 ){
		OGL_CHECK( pRenderThread, pglDrawBuffers( bufferCount, colorAttachments ) );
		OGL_CHECK( pRenderThread, glReadBuffer( colorAttachments[ 0 ] ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, noAttachment ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	}
}



// Debugging
//////////////

static const char * const vAttTypeName[] = {
	"none",
	"texture", "cubeMap",
	"cubeMapPosX", "cubeMapNegX",
	"cubeMapPosY", "cubeMapNegY",
	"cubeMapPosZ", "cubeMapNegZ",
	"arrayTexture", "arrayTextureLayer"
};

void deoglFramebuffer::DebugPrint(const char *prefix){
	decString text;
	int i;
	
	pRenderThread.GetLogger().LogInfoFormat("%s primary=%s size=%dx%d usage=%d",
		prefix, pPrimary ? "yes" : "no", pUsageWidth, pUsageHeight, pUsageCount);
	
	for(i=0; i<FBO_MAX_ATTACHMENT_COUNT; i++){
		pRenderThread.GetLogger().LogInfoFormat(
			"- AttachmentColor%d: type=%s image=%d layer=%d level=%d", i + 1,
			vAttTypeName[pAttColor[i].type], pAttColor[i].image,
			pAttColor[i].layer, pAttColor[i].level);
	}
	
	pRenderThread.GetLogger().LogInfoFormat(
		"- AttachmentDepth: type=%s image=%d layer=%d level=%d",
		vAttTypeName[pAttDepth.type], pAttDepth.image, pAttDepth.layer, pAttDepth.level);
	
	pRenderThread.GetLogger().LogInfoFormat(
		"- AttachmentStencil: type=%s image=%d layer=%d level=%d",
		vAttTypeName[pAttStencil.type], pAttStencil.image, pAttStencil.layer, pAttStencil.level);
}

void deoglFramebuffer::AddConfigToTrace( deErrorTracePoint &tracePoint ){
	decString text;
	int i;
	
	deErrorTraceValue &valueConfig = *tracePoint.AddValue( "configuration", "Configuration of the Framebuffer" );
	
	valueConfig.AddSubValueInt( "fbo", pFBO );
	valueConfig.AddSubValue( "primary", pPrimary ? "yes" : "no" );
	valueConfig.AddSubValueInt( "width", pUsageWidth );
	valueConfig.AddSubValueInt( "height", pUsageHeight );
	valueConfig.AddSubValueInt( "usageCount", pUsageCount );
	
	for( i=0; i<FBO_MAX_ATTACHMENT_COUNT; i++ ){
		text.Format( "attachmentColor%i", i + 1 );
		
		deErrorTraceValue &valueColor = *valueConfig.AddSubValue( text.GetString(), "<sAttachement>" );
		
		valueColor.AddSubValue( "type", vAttTypeName[ pAttColor[ i ].type ] );
		valueColor.AddSubValueInt( "image", pAttColor[ i ].image );
		valueColor.AddSubValueInt( "layer", pAttColor[ i ].layer );
		valueColor.AddSubValueInt( "level", pAttColor[ i ].level );
	}
	
	deErrorTraceValue &valueDepth = *valueConfig.AddSubValue( "attachmentDepth", "<sAttachement>" );
	valueDepth.AddSubValue( "type", vAttTypeName[ pAttDepth.type ] );
	valueDepth.AddSubValueInt( "image", pAttDepth.image );
	valueDepth.AddSubValueInt( "layer", pAttDepth.layer );
	valueDepth.AddSubValueInt( "level", pAttDepth.level );
	
	deErrorTraceValue &valueStencil = *valueConfig.AddSubValue( "attachmentStencil", "<sAttachement>" );
	valueStencil.AddSubValue( "type", vAttTypeName[ pAttStencil.type ] );
	valueStencil.AddSubValueInt( "image", pAttStencil.image );
	valueStencil.AddSubValueInt( "layer", pAttStencil.layer );
	valueStencil.AddSubValueInt( "level", pAttStencil.level );
}

void deoglFramebuffer::SetDebugObjectLabel( const char *name ){
	if( pFBO ){
		pRenderThread.GetDebug().SetDebugObjectLabel( GL_FRAMEBUFFER, pFBO, decString( "FB: " ) + name );
	}
}



// private functions
//////////////////////

void deoglFramebuffer::pCleanUp(){
	if(pUsageCount > 0){
		pRenderThread.GetLogger().LogWarnFormat(
			"Framebuffer (%d x %d) has %d usage count on deletion!",
			pUsageWidth, pUsageHeight, pUsageCount);
	}
	
	if(!pPrimary){
		if(pRenderThread.HasFramebuffer() && pRenderThread.GetFramebuffer().GetActive() == this){
			pRenderThread.GetFramebuffer().Activate(nullptr);
		}
		
		pRenderThread.GetDelayedOperations().DeleteOpenGLFramebuffer(pFBO);
	}
}
