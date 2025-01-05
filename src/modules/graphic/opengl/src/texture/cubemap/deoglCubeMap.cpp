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

#include "deoglCubeMap.h"
#include "../deoglTextureStageManager.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../memory/deoglMemoryManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTDebug.h"

#ifdef OS_ANDROID
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#endif

#include <dragengine/common/exceptions.h>



// Class deoglCubeMap
///////////////////////

// Constructor, destructor
////////////////////////////

deoglCubeMap::deoglCubeMap( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pMemUse( pRenderThread.GetMemoryManager().GetConsumption().textureCube )
{
	pTexture = 0;
	
	pSize = 1;
	pFormat = &renderThread.GetCapabilities().GetFormats().
		RequireUseTexCubeFormatFor(deoglCapsFmtSupport::eutfRGB8);
	pMipMapLevelCount = 0;
	pRealMipMapLevelCount = 0;
	pMipMapped = false;
}

deoglCubeMap::~deoglCubeMap(){
	DestroyCubeMap();
}



// Management
///////////////

void deoglCubeMap::SetSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size != pSize ){
		DestroyCubeMap();
		pSize = size;
	}
}

void deoglCubeMap::SetFormat( const deoglCapsTextureFormat *format ){
	if( ! format ){
		DETHROW( deeInvalidParam );
	}
	
	if( format != pFormat ){
		DestroyCubeMap();
		pFormat = format;
	}
}

void deoglCubeMap::SetFormatMappingByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber ){
	SetFormat(&pRenderThread.GetCapabilities().GetFormats().RequireUseTexCubeFormatFor(formatNumber));
}

void deoglCubeMap::SetFormatFBOByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber ){
	SetFormat(&pRenderThread.GetCapabilities().GetFormats().RequireUseFBOTexCubeFormatFor(formatNumber));
}

void deoglCubeMap::SetMipMapped( bool mipmapped ){
	if( mipmapped != pMipMapped ){
		DestroyCubeMap();
		pMipMapped = mipmapped;
	}
}

void deoglCubeMap::SetMipMapLevelCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pMipMapLevelCount ){
		DestroyCubeMap();
		pMipMapLevelCount = count;
	}
}

void deoglCubeMap::CreateCubeMap(){
	if( pTexture ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const GLenum glpixelformat = pFormat->GetPixelFormat();
	const GLenum glpixeltype = pFormat->GetPixelType();
	const GLenum glformat = pFormat->GetFormat();
	GLenum t;
	
	OGL_CHECK( pRenderThread, glGenTextures( 1, &pTexture ) );
	
	if( ! pTexture ){
		OGL_CHECK( pRenderThread, glGenTextures( 1, &pTexture ) );
		
		if( ! pTexture ){
			DETHROW( deeInvalidParam );
		}
	}
	
	tsmgr.EnableBareCubeMap( 0, *this );
	
	for( t=GL_TEXTURE_CUBE_MAP_POSITIVE_X; t<=GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; t++ ){
		OGL_CHECK( pRenderThread, glTexImage2D( t, 0, glformat, pSize, pSize,
			0, glpixelformat, glpixeltype, NULL ) );
	}
	
	if( pMipMapped ){
		int count = pMipMapLevelCount;
		int size = pSize;
		int i;
		
		if( count == 0 ){
			count = ( int )( floorf( log2f( ( float )pSize ) ) );
		}
		
		for( i=0; i<count; i++ ){
			size >>= 1;
			if( size < 1 ){
				size = 1;
			}
			
			for( t=GL_TEXTURE_CUBE_MAP_POSITIVE_X; t<=GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; t++ ){
				OGL_CHECK( pRenderThread, glTexImage2D( t, i + 1, glformat, size, size,
					0, glpixelformat, glpixeltype, NULL ) );
			}
		}
		
		pRealMipMapLevelCount = count;
	}
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, pRealMipMapLevelCount ) );
	
	if( pRenderThread.GetConfiguration().GetDisableCubeMapLinearFiltering() ){ // true for bugged nvidia drivers
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	}else{
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
	}
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_NONE ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL ) );
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE ) );
	
	switch( glpixelformat ){
	case GL_RED:
	case GL_LUMINANCE:
	case GL_RED_INTEGER:
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, GL_ONE ) );
		break;
		
	case GL_RG:
	case GL_LUMINANCE_ALPHA:
	case GL_RG_INTEGER:
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_R, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_G, GL_GREEN ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_B, GL_ZERO ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_SWIZZLE_A, GL_ONE ) );
		break;
		
	default:
		break;
	}
	
	tsmgr.DisableStage( 0 );
	
	UpdateMemoryUsage();
	pUpdateDebugObjectLabel();
}

void deoglCubeMap::DestroyCubeMap(){
	if( pTexture ){
		pRenderThread.GetDelayedOperations().DeleteOpenGLTexture( pTexture );
		pTexture = 0;
		
		UpdateMemoryUsage();
	}
}

void deoglCubeMap::SetPixels( const deoglPixelBuffer &pixelBuffer ){
	SetPixelsLevel( 0, pixelBuffer );
}

void deoglCubeMap::SetPixelsLevel( int level, const deoglPixelBuffer &pixelBuffer ){
	const int size = GetLevelSize( level );
	if( pixelBuffer.GetWidth() != size || pixelBuffer.GetHeight() != size || pixelBuffer.GetDepth() != 6 ){
		DETHROW( deeInvalidParam );
	}
	
	const GLubyte * const pixelsPtr = ( const GLubyte * )pixelBuffer.GetPointer();
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const int faceStride = pixelBuffer.GetLayerStride();
	GLenum t;
	
	CreateCubeMap();
	tsmgr.EnableBareCubeMap( 0, *this );
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
	
	for( t=0; t<6; t++ ){
		if( pixelBuffer.GetCompressed() ){
			OGL_CHECK( pRenderThread, pglCompressedTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + t, level, pFormat->GetFormat(),
				size, size, 0, faceStride, ( const GLvoid * )( pixelsPtr + faceStride * t ) ) );
			//OGL_CHECK( pRenderThread, pglCompressedTexSubImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + t, level, 0, 0, size, size,
			//	pFormat->GetFormat(), faceStride, ( const GLvoid * )( pixelsPtr + faceStride * t ) ) );
			
		}else{
#ifdef OS_ANDROID
			oglClearError();
			pglTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + t, level, 0, 0, size, size,
				pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType(),
				(const GLvoid *)(pixelsPtr + faceStride * t));
			if(glGetError() == GL_INVALID_OPERATION){
				pRenderThread.GetLogger().LogInfoFormat(
					"deoglCubeMap::SetPixelsLevelLayer Failed: size=%d level=%d f=%s pf=0x%x pt=0x%x",
					size, level, pFormat->GetName().GetString(),
					pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType());
				DETHROW(deeInvalidParam);
			}
#else
			OGL_CHECK( pRenderThread, pglTexSubImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + t, level, 0, 0, size, size,
				pixelBuffer.GetGLPixelFormat(), pixelBuffer.GetGLPixelType(), ( const GLvoid * )( pixelsPtr + faceStride * t ) ) );
#endif
		}
	}
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
	
	tsmgr.DisableStage( 0 );
}

void deoglCubeMap::GetPixels( deoglPixelBuffer &pixelBuffer ) const{
	GetPixelsLevel( 0, pixelBuffer );
}

void deoglCubeMap::GetPixelsLevel( int level, deoglPixelBuffer &pixelBuffer ) const{
	const int size = GetLevelSize( level );
	
	if( pixelBuffer.GetWidth() != size || pixelBuffer.GetHeight() != size || pixelBuffer.GetDepth() != 6 ){
		DETHROW( deeInvalidParam );
	}
	if( ! pTexture ){
		return;
	}
	
	#ifdef OS_ANDROID
	// glReadPixels under OpenGL ES does only support GL_RGBA and GL_RGBA_INTEGRAL.
	// if something else is required (for exampel GL_RGB, GL_RG or GL_RED) we have
	// to use a temporary texture containing 4 components and copy over from there.
	// inefficient but the only solution possible
	switch( pixelBuffer.GetFormat() ){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfByte3:{
		const deoglPixelBuffer::Ref tempPixBuf( deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer( deoglPixelBuffer::epfByte4, size, size, 6 ) ) );
		const int count = size * size;
		int i, j;
		
		GetPixelsLevel( level, tempPixBuf ); // get data using temporary 4-channels buffer
		
		switch( pixelBuffer.GetFormat() ){
		case deoglPixelBuffer::epfByte1:{
			for( j=0; j<6; j++ ){
				deoglPixelBuffer::sByte1 *dataDest = pixelBuffer.GetPointerByte1() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf->GetPointerByte4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfByte2:{
			for( j=0; j<6; j++ ){
				deoglPixelBuffer::sByte2 *dataDest = pixelBuffer.GetPointerByte2() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf->GetPointerByte4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfByte3:{
			for( j=0; j<6; j++ ){
				deoglPixelBuffer::sByte3 *dataDest = pixelBuffer.GetPointerByte3() + count;
				const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf->GetPointerByte4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
					dataDest[ i ].b = dataSrc[ i ].b;
				}
			}
			}return;
			
		default:
			return;
		}
		}return;
		
	case deoglPixelBuffer::epfFloat1:
	case deoglPixelBuffer::epfFloat2:
	case deoglPixelBuffer::epfFloat3:{
		const deoglPixelBuffer::Ref tempPixBuf( deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer( deoglPixelBuffer::epfFloat4, size, size, 6 ) ) );
		const int count = size * size;
		int i, j;
		
		GetPixelsLevel( level, tempPixBuf ); // get data using temporary 4-channels buffer
		
		switch( pixelBuffer.GetFormat() ){
		case deoglPixelBuffer::epfFloat1:{
			for( j=0; j<6; j++ ){
				deoglPixelBuffer::sFloat1 *dataDest = pixelBuffer.GetPointerFloat1() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf->GetPointerFloat4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfFloat2:{
			for( j=0; j<6; j++ ){
				deoglPixelBuffer::sFloat2 *dataDest = pixelBuffer.GetPointerFloat2() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf->GetPointerFloat4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
				}
			}
			}return;
			
		case deoglPixelBuffer::epfFloat3:{
			for( j=0; j<6; j++ ){
				deoglPixelBuffer::sFloat3 *dataDest = pixelBuffer.GetPointerFloat3() + count;
				const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf->GetPointerFloat4() + count;
				for( i=0; i<count; i++ ){
					dataDest[ i ].r = dataSrc[ i ].r;
					dataDest[ i ].g = dataSrc[ i ].g;
					dataDest[ i ].b = dataSrc[ i ].b;
				}
			}
			}return;
			
		default:
			return;
		}
		}return;
		
	default:
		break;
	}
	
	// this is called if the format is 4 channels
	const GLubyte * const pixelsPtr = ( const GLubyte * )pixelBuffer.GetPointer();
	const int faceStride = pixelBuffer.GetLayerStride();
	deoglRTFramebuffer &rtframebuffer = pRenderThread.GetFramebuffer();
	deoglFramebuffer * const oldFbo = rtframebuffer.GetActive();
	deoglFramebuffer *fbo = NULL;
	int i;
	
	try{
		fbo = rtframebuffer.GetManager().GetFBOWithResolution( size, size );
		//fbo = new deoglFramebuffer( pRenderThread, false );
		rtframebuffer.Activate( fbo );
		fbo->DetachAllImages();
		
		const GLenum buffers[ 1 ] = { GL_NONE };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		if( pFormat->GetIsDepth() ){
			OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
		}else{
			OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		}
		
		OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
		
		for( i=0; i<6; i++ ){
			if( pFormat->GetIsDepth() ){
				fbo->AttachDepthCubeMapFaceLevel( ( deoglCubeMap* )this, i, level );
			}else{
				fbo->AttachColorCubeMapFaceLevel( 0, ( deoglCubeMap* )this, i, level );
			}
			
			OGL_CHECK( pRenderThread, glReadPixels( 0, 0, size, size, pixelBuffer.GetGLPixelFormat(),
				pixelBuffer.GetGLPixelType(), ( GLvoid* )( pixelsPtr + faceStride * i ) ) );
		}
		
		OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
		
		fbo->DetachColorImage( 0 );
		fbo->DetachDepthImage();
		
		rtframebuffer.Activate( oldFbo );
		
		fbo->DecreaseUsageCount();
		//delete fbo;
		fbo = NULL;
		
	}catch( const deException & ){
		rtframebuffer.Activate( oldFbo );
		if( fbo ){
			fbo->DecreaseUsageCount();
			//delete fbo;
		}
		throw;
	}
	
	#else
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const GLubyte * const pixelsPtr = ( const GLubyte * )pixelBuffer.GetPointer();
	const GLenum glpixelformat = pixelBuffer.GetGLPixelFormat();
	const GLenum glpixeltype = pixelBuffer.GetGLPixelType();
	const int faceStride = pixelBuffer.GetLayerStride();
	int i;
	
	const GLenum faceTargets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };
	
	tsmgr.EnableBareCubeMap( 0, *this );
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
	
	for( i=0; i<6; i++ ){
		OGL_CHECK( pRenderThread, glGetTexImage( faceTargets[ i ], level, glpixelformat,
			glpixeltype, ( GLvoid* )( pixelsPtr + faceStride * i ) ) );
	}
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
	tsmgr.DisableStage( 0 );
	#endif
}



int deoglCubeMap::GetLevelSize( int level ) const{
	if( level < 0 || level > pRealMipMapLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i, size = pSize;
	
	for( i=0; i<level; i++ ){
		size >>= 1;
		if( size < 1 ){
			size = 1;
		}
	}
	
	return size;
}

void deoglCubeMap::CreateMipMaps(){
	if( pMipMapped && pMipMapLevelCount == 0 ){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		
		tsmgr.EnableBareCubeMap( 0, *this );
		pglGenerateMipmap( GL_TEXTURE_CUBE_MAP );
		tsmgr.DisableStage( 0 );
	}
}



void deoglCubeMap::CopyFrom( const deoglCubeMap &cubemap, bool withMipMaps ){
	CopyFrom( cubemap, withMipMaps, pSize, 0, 0, 0, 0 );
}

void deoglCubeMap::CopyFrom( const deoglCubeMap &cubemap, bool withMipMaps, int size, int srcX, int srcY, int destX, int destY ){
	if( destX < 0 || destY < 0 || srcX < 0 || srcY < 0 || destX + size > pSize || destY + size > pSize ){
		DETHROW( deeInvalidParam );
	}
	
	const int srcSize = cubemap.GetSize();
	
	if( srcX + size > srcSize || srcY + size > srcSize ){
		DETHROW( deeInvalidParam );
	}
	
	CreateCubeMap();
	
	if( withMipMaps && cubemap.GetMipMapped() && pMipMapped ){
		const int srcMipMapLevelCount = cubemap.GetRealMipMapLevelCount();
		int i, mipMapLevelCount;
		
		mipMapLevelCount = pRealMipMapLevelCount;
		if( srcMipMapLevelCount < mipMapLevelCount ){
			mipMapLevelCount = srcMipMapLevelCount;
		}
		
		for( i=0; i<mipMapLevelCount; i++ ){
			if( pglCopyImageSubData ){
				pglCopyImageSubData( cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_CUBE_MAP, i, destX, destY, 0, size, size, 6 );
				
			}else if( pglCopyImageSubDataNV ){
				pglCopyImageSubDataNV( cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, i, srcX, srcY, 0,
					pTexture, GL_TEXTURE_CUBE_MAP, i, destX, destY, 0, size, size, 6 );
				
			}else{
				DETHROW( deeInvalidParam );
			}
			
			srcX >>= 1;
			srcY >>= 1;
			destX >>= 1;
			destY >>= 1;
			
			size >>= 1;
			if( size < 1 ){
				size = 1;
			}
		}
		
	}else{
		if( pglCopyImageSubData ){
			pglCopyImageSubData( cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_CUBE_MAP, 0, destX, destY, 0, size, size, 6 );
			
		}else if( pglCopyImageSubDataNV ){
			pglCopyImageSubDataNV( cubemap.GetTexture(), GL_TEXTURE_CUBE_MAP, 0, srcX, srcY, 0,
				pTexture, GL_TEXTURE_CUBE_MAP, 0, destX, destY, 0, size, size, 6 );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
}



void deoglCubeMap::UpdateMemoryUsage(){
	pMemUse.Clear();
	
	if( ! pTexture || ! pFormat ){
		return;
	}
	
	#ifdef OS_ANDROID
	pMemUse.SetUncompressed( *pFormat, pSize, pSize, 6, pRealMipMapLevelCount );
	
	#else
	if( pFormat->GetIsCompressed() ){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareCubeMap( 0, *this );
		
		GLint isReallyCompressed = 0;
		OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			0, GL_TEXTURE_COMPRESSED, &isReallyCompressed ) );
		
		if( isReallyCompressed ){
			unsigned long consumption = 0ull;
			GLint t, l, compressedSize;
			
			for( l=0; l<pRealMipMapLevelCount; l++ ){
				for( t=GL_TEXTURE_CUBE_MAP_POSITIVE_X; t<=GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; t++ ){
					OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( t, l,
						GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize ) );
					consumption += ( unsigned long long )compressedSize;
				}
			}
			
			pMemUse.SetCompressed( consumption, *pFormat );
			
		}else{
			pMemUse.SetUncompressed( *pFormat, pSize, pSize, 6, pMipMapped ? pRealMipMapLevelCount : 0 );
		}
		
		tsmgr.DisableStage( 0 );
		
	}else{
		pMemUse.SetUncompressed( *pFormat, pSize, pSize, 6, pMipMapped ? pRealMipMapLevelCount : 0 );
	}
	#endif
}



// Helper Functions
/////////////////////

void deoglCubeMap::SetMapingFormat( int channels, bool useFloat, bool compressed ){
	if( channels == 1 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfR8 );
			}
		}
		
	}else if( channels == 2 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG8 );
			}
		}
		
	}else if( channels == 3 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGB16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGB8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGB8 );
			}
		}
		
	}else if( channels == 4 ){
		if( useFloat ){
			SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA16F );
			
		}else{
			if( compressed ){
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA8_C );
				
			}else{
				SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA8 );
			}
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglCubeMap::SetFBOFormat( int channels, bool useFloat ){
	if( channels == 1 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR8 );
		}
		
	}else if( channels == 2 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG8 );
		}
		
	}else if( channels == 3 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB8 );
		}
		
	}else if( channels == 4 ){
		if( useFloat ){
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA16F );
			
		}else{
			SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA8 );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglCubeMap::SetDepthFormat( bool useFloat ){
	if( useFloat ){
		SetFormatFBOByNumber( deoglCapsFmtSupport::eutfDepthF );
		
	}else{
		SetFormatFBOByNumber( deoglCapsFmtSupport::eutfDepth );
	}
}



void deoglCubeMap::CreateMatrixForFace( decMatrix &matrix, const decVector &position, int face ){
	switch( face ){
	case deoglCubeMap::efPositiveX:
		//matrix.SetCamera( position, decVector( 1.0f, 0.0f, 0.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 =  0.0f; matrix.a12 =  0.0f; matrix.a13 = -1.0f; matrix.a14 =  position.z;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f; matrix.a24 = -position.y;
		matrix.a31 =  1.0f; matrix.a32 =  0.0f; matrix.a33 =  0.0f; matrix.a34 = -position.x;
		break;
		
	case deoglCubeMap::efNegativeX:
		//matrix.SetCamera( position, decVector( -1.0f, 0.0f, 0.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 =  0.0f; matrix.a12 =  0.0f; matrix.a13 =  1.0f; matrix.a14 = -position.z;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f; matrix.a24 = -position.y;
		matrix.a31 = -1.0f; matrix.a32 =  0.0f; matrix.a33 =  0.0f; matrix.a34 =  position.x;
		break;
		
	case deoglCubeMap::efPositiveY:
		//matrix.SetCamera( position, decVector( 0.0f, 1.0f, 0.0f ), decVector( 0.0f, 0.0f, -1.0f ) );
		matrix.a11 =  1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f; matrix.a14 = -position.x;
		matrix.a21 =  0.0f; matrix.a22 =  0.0f; matrix.a23 = -1.0f; matrix.a24 =  position.z;
		matrix.a31 =  0.0f; matrix.a32 =  1.0f; matrix.a33 =  0.0f; matrix.a34 = -position.y;
		break;
		
	case deoglCubeMap::efNegativeY:
		//matrix.SetCamera( position, decVector( 0.0f, -1.0f, 0.0f ), decVector( 0.0f, 0.0f, 1.0f ) );
		matrix.a11 =  1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f; matrix.a14 = -position.x;
		matrix.a21 =  0.0f; matrix.a22 =  0.0f; matrix.a23 =  1.0f; matrix.a24 = -position.z;
		matrix.a31 =  0.0f; matrix.a32 = -1.0f; matrix.a33 =  0.0f; matrix.a34 =  position.y;
		break;
		
	case deoglCubeMap::efPositiveZ:
		//matrix.SetCamera( position, decVector( 0.0f, 0.0f, 1.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 =  1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f; matrix.a14 = -position.x;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f; matrix.a24 = -position.y;
		matrix.a31 =  0.0f; matrix.a32 =  0.0f; matrix.a33 =  1.0f; matrix.a34 = -position.z;
		break;
		
	case deoglCubeMap::efNegativeZ:
		//matrix.SetCamera( position, decVector( 0.0f, 0.0f, -1.0f ), decVector( 0.0f, 1.0f, 0.0f ) );
		matrix.a11 = -1.0f; matrix.a12 =  0.0f; matrix.a13 =  0.0f; matrix.a14 =  position.x;
		matrix.a21 =  0.0f; matrix.a22 =  1.0f; matrix.a23 =  0.0f; matrix.a24 = -position.y;
		matrix.a31 =  0.0f; matrix.a32 =  0.0f; matrix.a33 = -1.0f; matrix.a34 =  position.z;
		break;
	}
	
	matrix.a41 = 0.0f; matrix.a42 = 0.0f; matrix.a43 = 0.0f; matrix.a44 = 1.0f;
}

void deoglCubeMap::CreateMatrixForFace( decDMatrix &matrix, const decDVector &position, int face ){
	//matrix.SetCamera( lightPosition, decDVector( 1.0, 0.0, 0.0 ), decDVector( 0.0, 1.0, 0.0 ) );
	// right = up % view;
	// [ right.x | right.y | right.z | -(position * right) ]
	// [    up.x |    up.y |    up.z |    -(position * up) ]
	// [  view.x |  view.y |  view.z |  -(position * view) ]
	// [    0    |    0    |    0    |          1          ]
	
	if( face == deoglCubeMap::efPositiveX ){
		//matrix.SetCamera( position, decDVector( 1.0, 0.0, 0.0 ), decDVector( 0.0, 1.0, 0.0 ) );
		matrix.a11 =  0.0; matrix.a12 =  0.0; matrix.a13 = -1.0; matrix.a14 =  position.z;
		matrix.a21 =  0.0; matrix.a22 =  1.0; matrix.a23 =  0.0; matrix.a24 = -position.y;
		matrix.a31 =  1.0; matrix.a32 =  0.0; matrix.a33 =  0.0; matrix.a34 = -position.x;
		
	}else if( face == deoglCubeMap::efNegativeX ){
		//matrix.SetCamera( position, decDVector( -1.0, 0.0, 0.0 ), decDVector( 0.0, 1.0, 0.0 ) );
		matrix.a11 =  0.0; matrix.a12 =  0.0; matrix.a13 =  1.0; matrix.a14 = -position.z;
		matrix.a21 =  0.0; matrix.a22 =  1.0; matrix.a23 =  0.0; matrix.a24 = -position.y;
		matrix.a31 = -1.0; matrix.a32 =  0.0; matrix.a33 =  0.0; matrix.a34 =  position.x;
		
	}else if( face == deoglCubeMap::efPositiveY ){
		//matrix.SetCamera( position, decDVector( 0.0, 1.0, 0.0 ), decDVector( 0.0, 0.0, -1.0 ) );
		matrix.a11 =  1.0; matrix.a12 =  0.0; matrix.a13 =  0.0; matrix.a14 = -position.x;
		matrix.a21 =  0.0; matrix.a22 =  0.0; matrix.a23 = -1.0; matrix.a24 =  position.z;
		matrix.a31 =  0.0; matrix.a32 =  1.0; matrix.a33 =  0.0; matrix.a34 = -position.y;
		
	}else if( face == deoglCubeMap::efNegativeY ){
		//matrix.SetCamera( position, decDVector( 0.0, -1.0, 0.0 ), decDVector( 0.0, 0.0, 1.0 ) );
		matrix.a11 =  1.0; matrix.a12 =  0.0; matrix.a13 =  0.0; matrix.a14 = -position.x;
		matrix.a21 =  0.0; matrix.a22 =  0.0; matrix.a23 =  1.0; matrix.a24 = -position.z;
		matrix.a31 =  0.0; matrix.a32 = -1.0; matrix.a33 =  0.0; matrix.a34 =  position.y;
		
	}else if( face == deoglCubeMap::efPositiveZ ){
		//matrix.SetCamera( position, decDVector( 0.0, 0.0, 1.0 ), decDVector( 0.0, 1.0, 0.0 ) );
		matrix.a11 =  1.0; matrix.a12 =  0.0; matrix.a13 =  0.0; matrix.a14 = -position.x;
		matrix.a21 =  0.0; matrix.a22 =  1.0; matrix.a23 =  0.0; matrix.a24 = -position.y;
		matrix.a31 =  0.0; matrix.a32 =  0.0; matrix.a33 =  1.0; matrix.a34 = -position.z;
		
	}else if( face == deoglCubeMap::efNegativeZ ){
		//matrix.SetCamera( position, decDVector( 0.0, 0.0, -1.0 ), decDVector( 0.0, 1.0, 0.0 ) );
		matrix.a11 = -1.0; matrix.a12 =  0.0; matrix.a13 =  0.0; matrix.a14 =  position.x;
		matrix.a21 =  0.0; matrix.a22 =  1.0; matrix.a23 =  0.0; matrix.a24 = -position.y;
		matrix.a31 =  0.0; matrix.a32 =  0.0; matrix.a33 = -1.0; matrix.a34 =  position.z;
	}
	
	matrix.a41 = 0.0; matrix.a42 = 0.0; matrix.a43 = 0.0; matrix.a44 = 1.0;
}

void deoglCubeMap::SetDebugObjectLabel( const char *name ){
	pDebugObjectLabel.Format( "Cube: %s", name );
	if( pTexture ){
		pUpdateDebugObjectLabel();
	}
}

void deoglCubeMap::pUpdateDebugObjectLabel(){
	pRenderThread.GetDebug().SetDebugObjectLabel( GL_TEXTURE, pTexture, pDebugObjectLabel );
}
