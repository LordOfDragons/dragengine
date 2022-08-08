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
#include <stdlib.h>
#include <string.h>

#include "deoglTexture1D.h"
#include "../deoglTextureStageManager.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../memory/deoglMemoryManager.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTDebug.h"

#ifdef ANDROID
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglFramebufferManager.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#endif

#include <dragengine/common/exceptions.h>



// Class deoglTexture1D
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglTexture1D::deoglTexture1D( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pMemUse( pRenderThread.GetMemoryManager().GetConsumption().texture1D )
{
	pTexture = 0;
	
	pSize = 1;
	pFormat = renderThread.GetCapabilities().GetFormats().GetUseTex2DFormatFor( deoglCapsFmtSupport::eutfRGB8 );
	pMipMapLevelCount = 0;
	pRealMipMapLevelCount = 0;
	pMipMapped = false;
}

deoglTexture1D::~deoglTexture1D(){
	DestroyTexture();
}



// Management
///////////////

void deoglTexture1D::SetSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size != pSize ){
		DestroyTexture();
		pSize = size;
	}
}

void deoglTexture1D::SetFormat( const deoglCapsTextureFormat *format ){
	if( ! format ){
		DETHROW( deeInvalidParam );
	}
	
	if( format != pFormat ){
		DestroyTexture();
		pFormat = format;
	}
}

void deoglTexture1D::SetFormatMappingByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber ){
	SetFormat( pRenderThread.GetCapabilities().GetFormats().GetUseTex2DFormatFor( formatNumber ) );
}

void deoglTexture1D::SetFormatFBOByNumber( deoglCapsFmtSupport::eUseTextureFormats formatNumber ){
	SetFormat( pRenderThread.GetCapabilities().GetFormats().GetUseFBOTex2DFormatFor( formatNumber ) );
}

void deoglTexture1D::SetMipMapped( bool mipmapped ){
	if( mipmapped != pMipMapped ){
		DestroyTexture();
		pMipMapped = mipmapped;
	}
}

void deoglTexture1D::SetMipMapLevelCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pMipMapLevelCount ){
		DestroyTexture();
		pMipMapLevelCount = count;
	}
}



void deoglTexture1D::CreateTexture(){
	if( pTexture ){
		return;
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	const GLenum glpixelformat = pFormat->GetPixelFormat();
	const GLenum glpixeltype = pFormat->GetPixelType();
	const GLenum glformat = pFormat->GetFormat();
	
	OGL_CHECK( pRenderThread, glGenTextures( 1, &pTexture ) );
	
	if( ! pTexture ){
		OGL_CHECK( pRenderThread, glGenTextures( 1, &pTexture ) );
		
		if( ! pTexture ){
			DETHROW( deeInvalidParam );
		}
	}
	
	tsmgr.EnableBareTexture1D( 0, *this );
	
	OGL_CHECK( pRenderThread, glTexImage1D( GL_TEXTURE_1D, 0, glformat, pSize, 0, glpixelformat, glpixeltype, NULL ) );
	
	if( pMipMapped ){
		int count = pMipMapLevelCount;
		int size = pSize;
		int i;
		
		if( count == 0 ){
			count = ( int )( floorf( log2f( size ) ) );
		}
		
		for( i=0; i<count; i++ ){
			size >>= 1;
			if( size < 1 ){
				size = 1;
			}
			
			OGL_CHECK( pRenderThread, glTexImage1D( GL_TEXTURE_1D, i + 1, glformat, size, 0, glpixelformat, glpixeltype, NULL ) );
		}
		
		pRealMipMapLevelCount = count;
	}
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, pRealMipMapLevelCount ) );
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ) );
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_COMPARE_MODE, GL_NONE ) );
	OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL ) );
	
	switch( glpixelformat ){
	case GL_RED:
	case GL_LUMINANCE:
	case GL_RED_INTEGER:
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_R, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_G, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_B, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_A, GL_ONE ) );
		break;
		
	case GL_RG:
	case GL_LUMINANCE_ALPHA:
	case GL_RG_INTEGER:
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_R, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_G, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_B, GL_RED ) );
		OGL_CHECK( pRenderThread, glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_SWIZZLE_A, GL_GREEN ) );
		break;
		
	default:
		break;
	}
	
	//OGL_CHECK( pRenderThread, glHint( GL_TEXTURE_COMPRESSION_HINT, GL_NICEST ) ); // GL_FATEST // GL_DONT_CARE
	
	/*
	if( pIsDepth ){
		GLint depth;
		OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( GL_TEXTURE_1D, 0, GL_TEXTURE_DEPTH_SIZE, &depth ) );
		printf( "a depth texture (%u) has been created with size %i and depth %i\n", pTexture, pSize, depth );
	}
	*/
	tsmgr.DisableStage( 0 );
	
	/*
	int result;
	OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( target, 0, GL_TEXTURE_COMPRESSED, &result ) );
	pRenderThread.GetLogger().LogInfoFormat("Texture %p: compressed=%i", this, result );
	*/
	
	UpdateMemoryUsage();
	pUpdateDebugObjectLabel();
}

void deoglTexture1D::DestroyTexture(){
	if( pTexture ){
		pRenderThread.GetDelayedOperations().DeleteOpenGLTexture( pTexture );
		pTexture = 0;
		
		UpdateMemoryUsage();
	}
}

void deoglTexture1D::SetPixels( const deoglPixelBuffer &pixelBuffer ){
	SetPixelsLevel( 0, pixelBuffer );
}

void deoglTexture1D::SetPixelsLevel( int level, const deoglPixelBuffer &pixelBuffer ){
	const int size = GetLevelSize( level );
	
	if( pixelBuffer.GetWidth() != size || pixelBuffer.GetHeight() != 1 || pixelBuffer.GetDepth() != 1 ){
		DETHROW( deeInvalidParam );
	}
	
	deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
	
	CreateTexture();
	tsmgr.EnableBareTexture1D( 0, *this );
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
	
	if( pixelBuffer.GetCompressed() ){
		OGL_CHECK( pRenderThread, pglCompressedTexImage1D( GL_TEXTURE_1D, level, pFormat->GetFormat(),
			       size, 0, pixelBuffer.GetImageSize(), ( const GLvoid * )pixelBuffer.GetPointer() ) );
		//OGL_CHECK( pRenderThread, pglCompressedTexSubImage1D( GL_TEXTURE_2D, level, 0, size,
		//	pFormat->GetFormat(), pixels.GetImageSize(), ( const GLvoid * )pixels.GetPointer() ) );
		
	}else{
		OGL_CHECK( pRenderThread, pglTexSubImage1D( GL_TEXTURE_1D, level, 0, size, pixelBuffer.GetGLPixelFormat(),
			pixelBuffer.GetGLPixelType(), ( const GLvoid * )pixelBuffer.GetPointer() ) );
	}
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
	
	tsmgr.DisableStage( 0 );
}

void deoglTexture1D::GetPixels( deoglPixelBuffer &pixelBuffer ) const{
	GetPixelsLevel( 0, pixelBuffer );
}

void deoglTexture1D::GetPixelsLevel( int level, deoglPixelBuffer &pixelBuffer ) const{
	const int size = GetLevelSize( level );
	
	if( pixelBuffer.GetWidth() != size || pixelBuffer.GetHeight() != 1 || pixelBuffer.GetDepth() != 1 ){
		DETHROW( deeInvalidParam );
	}
	if( ! pTexture ){
		return;
	}
	
	#ifdef ANDROID
	// glReadPixels under OpenGL ES does only support GL_RGBA and GL_RGBA_INTEGRAL.
	// if something else is required (for exampel GL_RGB, GL_RG or GL_RED) we have
	// to use a temporary texture containing 4 components and copy over from there.
	// inefficient but the only solution possible
	switch( pixelBuffer.GetFormat() ){
	case deoglPixelBuffer::epfByte1:
	case deoglPixelBuffer::epfByte2:
	case deoglPixelBuffer::epfByte3:{
		deoglPixelBuffer tempPixBuf( deoglPixelBuffer::epfByte4, size, 1, 1 );
		const deoglPixelBuffer::sByte4 *dataSrc = tempPixBuf.GetPointerByte4();
		int i;
		
		GetPixelsLevel( level, tempPixBuf ); // get data using temporary 4-channels buffer
		
		switch( pixelBuffer.GetFormat() ){
		case deoglPixelBuffer::epfByte1:{
			deoglPixelBuffer::sByte1 *dataDest = pixelBuffer.GetPointerByte1();
			for( i=0; i<size; i++ ){
				dataDest[ i ].r = dataSrc[ i ].r;
			}
			}return;
			
		case deoglPixelBuffer::epfByte2:{
			deoglPixelBuffer::sByte2 *dataDest = pixelBuffer.GetPointerByte2();
			for( i=0; i<size; i++ ){
				dataDest[ i ].r = dataSrc[ i ].r;
				dataDest[ i ].g = dataSrc[ i ].g;
			}
			}return;
			
		case deoglPixelBuffer::epfByte3:{
			deoglPixelBuffer::sByte3 *dataDest = pixelBuffer.GetPointerByte3();
			for( i=0; i<size; i++ ){
				dataDest[ i ].r = dataSrc[ i ].r;
				dataDest[ i ].g = dataSrc[ i ].g;
				dataDest[ i ].b = dataSrc[ i ].b;
			}
			}return;
			
		default:
			return;
		}
		}return;
		
	case deoglPixelBuffer::epfFloat1:
	case deoglPixelBuffer::epfFloat2:
	case deoglPixelBuffer::epfFloat3:{
		deoglPixelBuffer tempPixBuf( deoglPixelBuffer::epfFloat4, size, 1, 1 );
		const deoglPixelBuffer::sFloat4 *dataSrc = tempPixBuf.GetPointerFloat4();
		int i;
		
		GetPixelsLevel( level, tempPixBuf ); // get data using temporary 4-channels buffer
		
		switch( pixelBuffer.GetFormat() ){
		case deoglPixelBuffer::epfFloat1:{
			deoglPixelBuffer::sFloat1 *dataDest = pixelBuffer.GetPointerFloat1();
			for( i=0; i<size; i++ ){
				dataDest[ i ].r = dataSrc[ i ].r;
			}
			}return;
			
		case deoglPixelBuffer::epfFloat2:{
			deoglPixelBuffer::sFloat2 *dataDest = pixelBuffer.GetPointerFloat2();
			for( i=0; i<size; i++ ){
				dataDest[ i ].r = dataSrc[ i ].r;
				dataDest[ i ].g = dataSrc[ i ].g;
			}
			}return;
			
		case deoglPixelBuffer::epfFloat3:{
			deoglPixelBuffer::sFloat3 *dataDest = pixelBuffer.GetPointerFloat3();
			for( i=0; i<size; i++ ){
				dataDest[ i ].r = dataSrc[ i ].r;
				dataDest[ i ].g = dataSrc[ i ].g;
				dataDest[ i ].b = dataSrc[ i ].b;
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
	deoglRTFramebuffer &rtframebuffer = pRenderThread.GetFramebuffer();
	deoglFramebuffer * const oldFbo = rtframebuffer.GetActive();
	deoglFramebuffer *fbo = NULL;
	try{
		fbo = rtframebuffer.GetManager().GetFBOWithResolution( size, 1 );
		//fbo = new deoglFramebuffer( pRenderThread, false );
		rtframebuffer.Activate( fbo );
		fbo->DetachAllImages();
		
		const GLenum buffers[ 1 ] = { GL_NONE };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		if( pFormat->GetIsDepth() ){
			fbo->AttachDepthTexture1DLevel( ( deoglTexture1D* )this, level );
			OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
			
		}else{
			fbo->AttachColorTexture1DLevel( 0, ( deoglTexture1D* )this, level );
			OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		}
		
		OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
		OGL_CHECK( pRenderThread, glReadPixels( 0, 0, size, 1, pixelBuffer.GetGLPixelFormat(),
			pixelBuffer.GetGLPixelType(), ( GLvoid* )pixelBuffer.GetPointer() ) );
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
	
	tsmgr.EnableBareTexture1D( 0, *this );
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
	
	OGL_CHECK( pRenderThread, glGetTexImage( GL_TEXTURE_1D, level, pixelBuffer.GetGLPixelFormat(),
		pixelBuffer.GetGLPixelType(), ( GLvoid* )pixelBuffer.GetPointer() ) );
	
	OGL_CHECK( pRenderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
	tsmgr.DisableStage( 0 );
	#endif
}



int deoglTexture1D::GetLevelSize( int level ) const{
	if( level < 0 || level > pRealMipMapLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	int size = pSize;
	int i;
	
	for( i=0; i<level; i++ ){
		size >>= 1;
		if( size < 1 ){
			size = 1;
		}
	}
	
	return size;
}

void deoglTexture1D::CreateMipMaps(){
	if( pMipMapped && pMipMapLevelCount == 0 ){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		
		tsmgr.EnableBareTexture1D( 0, *this );
		pglGenerateMipmap( GL_TEXTURE_1D );
		tsmgr.DisableStage( 0 );
	}
}



void deoglTexture1D::CopyFrom( const deoglTexture1D &texture, bool withMipMaps ){
	CopyFrom( texture, withMipMaps, pSize, 0, 0 );
}

void deoglTexture1D::CopyFrom( const deoglTexture1D &texture, bool withMipMaps, int size, int srcX, int destX ){
	if( destX < 0 || srcX < 0 || destX + size > pSize ){
		DETHROW( deeInvalidParam );
	}
	
	const int srcSize = texture.GetSize();
	
	if( srcX + size > srcSize ){
		DETHROW( deeInvalidParam );
	}
	
	CreateTexture();
	
	if( withMipMaps && texture.GetMipMapped() && pMipMapped ){
		const int srcMipMapLevelCount = texture.GetRealMipMapLevelCount();
		int i, mipMapLevelCount;
		
		mipMapLevelCount = pRealMipMapLevelCount;
		if( srcMipMapLevelCount < mipMapLevelCount ){
			mipMapLevelCount = srcMipMapLevelCount;
		}
		
		for( i=0; i<mipMapLevelCount; i++ ){
			if( pglCopyImageSubData ){
				pglCopyImageSubData( texture.GetTexture(), GL_TEXTURE_1D, i, srcX, 0, 0,
					pTexture, GL_TEXTURE_1D, i, destX, 0, 0, size, 1, 1 );
				
			}else if( pglCopyImageSubDataNV ){
				pglCopyImageSubDataNV( texture.GetTexture(), GL_TEXTURE_1D, i, srcX, 0, 0,
					pTexture, GL_TEXTURE_1D, i, destX, 0, 0, size, 1, 1 );
				
			}else{
				DETHROW( deeInvalidParam );
			}
			
			srcX >>= 1;
			destX >>= 1;
			
			size >>= 1;
			if( size < 1 ){
				size = 1;
			}
		}
		
	}else{
		if( pglCopyImageSubData ){
			pglCopyImageSubData( texture.GetTexture(), GL_TEXTURE_1D, 0, srcX, 0, 0,
				pTexture, GL_TEXTURE_1D, 0, destX, 0, 0, size, 1, 1 );
			
		}else if( pglCopyImageSubDataNV ){
			pglCopyImageSubDataNV( texture.GetTexture(), GL_TEXTURE_1D, 0, srcX, 0, 0,
				pTexture, GL_TEXTURE_1D, 0, destX, 0, 0, size, 1, 1 );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
}



void deoglTexture1D::UpdateMemoryUsage(){
	pMemUse.Clear();
	
	if( ! pTexture || ! pFormat ){
		return;
	}
	
	#ifdef ANDROID
	pMemUse.SetUncompressed( *pFormat, pSize, 1, 1, pRealMipMapLevelCount );
	
	#else
	if( pFormat->GetIsCompressed() ){
		deoglTextureStageManager &tsmgr = pRenderThread.GetTexture().GetStages();
		tsmgr.EnableBareTexture1D( 0, *this );
		
		GLint isReallyCompressed = 0;
		OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( GL_TEXTURE_1D,
			0, GL_TEXTURE_COMPRESSED, &isReallyCompressed ) );
		
		if( isReallyCompressed ){
			unsigned long consumption = 0ull;
			GLint l, compressedSize;
			
			for( l=0; l<pRealMipMapLevelCount; l++ ){
				OGL_CHECK( pRenderThread, glGetTexLevelParameteriv( GL_TEXTURE_1D,
					l, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressedSize ) );
				consumption += ( unsigned long long )compressedSize;
			}
			
			pMemUse.SetCompressed( consumption, *pFormat );
			
		}else{
			pMemUse.SetUncompressed( *pFormat, pSize, 1, 1, pRealMipMapLevelCount );
		}
		
		tsmgr.DisableStage( 0 );
		
	}else{
		pMemUse.SetUncompressed( *pFormat, pSize, 1, 1, pRealMipMapLevelCount );
	}
	#endif
}



// Helper Functions
/////////////////////

void deoglTexture1D::SetMapingFormat( int channels, bool useFloat, bool compressed ){
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

void deoglTexture1D::SetFBOFormat( int channels, bool useFloat ){
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

void deoglTexture1D::SetFBOFormatIntegral( int channels, int bpp, bool useUnsigned ){
	if( bpp == 8 ){
		if( channels == 1 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR8UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR8I );
			}
			
		}else if( channels == 2 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG8UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG8I );
			}
			
		}else if( channels == 3 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB8UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB8I );
			}
			
		}else if( channels == 4 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA8UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA8I );
			}
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
	}else if( bpp == 16 ){
		if( channels == 1 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR16UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfR16I );
			}
			
		}else if( channels == 2 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG16UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG16I );
			}
			
		}else if( channels == 3 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB16UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGB16I );
			}
			
		}else if( channels == 4 ){
			if( useUnsigned ){
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA16UI );
				
			}else{
				SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRGBA16I );
			}
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglTexture1D::SetDepthFormat( bool packedStencil ){
	if( packedStencil ){
		SetFormatFBOByNumber( deoglCapsFmtSupport::eutfDepth_Stencil );
		
	}else{
		SetFormatFBOByNumber( deoglCapsFmtSupport::eutfDepth );
	}
}

void deoglTexture1D::SetDebugObjectLabel( const char *name ){
	pDebugObjectLabel = name;
	if( pTexture ){
		pUpdateDebugObjectLabel();
	}
}

void deoglTexture1D::pUpdateDebugObjectLabel(){
	pRenderThread.GetDebug().SetDebugObjectLabel( GL_TEXTURE, pTexture, pDebugObjectLabel );
}
