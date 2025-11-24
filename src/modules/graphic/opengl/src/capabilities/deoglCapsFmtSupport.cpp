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

#include "deoglCapabilities.h"
#include "deoglCapsFmtSupport.h"
#include "deoglCapsTextureFormat.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../extensions/deoglExtensions.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBParameter.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>

#include "deoglCapsFmtSupportSetup.h"


// Class deoglCapsFmtSupport
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapsFmtSupport::deoglCapsFmtSupport( deoglCapabilities &capabilities ) :
pCapabilities( capabilities )
{
	int i;
	
	for( i=0; i<UseTextureFormatCount; i++ ){
		pUseTex2DFormats[ i ] = NULL;
		pUseTexCubeFormats[ i ] = NULL;
		pUseArrTexFormats[ i ] = NULL;
		pUseFBOTex2DFormats[ i ] = NULL;
		pUseFBOTexCubeFormats[ i ] = NULL;
		pUseFBOArrTexFormats[ i ] = NULL;
	}
}



// Management
///////////////

const deoglCapsTextureFormat *deoglCapsFmtSupport::GetUseTex2DFormatFor(
eUseTextureFormats type ) const{
	if( type < 0 || type >= UseTextureFormatCount ){
		DETHROW( deeInvalidParam );
	}
	return pUseTex2DFormats[ type ];
}

const deoglCapsTextureFormat *deoglCapsFmtSupport::GetUseTexCubeFormatFor(
eUseTextureFormats type ) const{
	if( type < 0 || type >= UseTextureFormatCount ){
		DETHROW( deeInvalidParam );
	}
	return pUseTexCubeFormats[ type ];
}

const deoglCapsTextureFormat* deoglCapsFmtSupport::GetUseArrayTexFormatFor(
eUseTextureFormats type ) const{
	if( type < 0 || type >= UseTextureFormatCount ){
		DETHROW( deeInvalidParam );
	}
	return pUseArrTexFormats[ type ];
}

const deoglCapsTextureFormat *deoglCapsFmtSupport::GetUseFBOTex2DFormatFor(
eUseTextureFormats type ) const{
	if( type < 0 || type >= UseTextureFormatCount ){
		DETHROW( deeInvalidParam );
	}
	return pUseFBOTex2DFormats[ type ];
}

const deoglCapsTextureFormat *deoglCapsFmtSupport::GetUseFBOTexCubeFormatFor(
eUseTextureFormats type ) const{
	if( type < 0 || type >= UseTextureFormatCount ){
		DETHROW( deeInvalidParam );
	}
	return pUseFBOTexCubeFormats[ type ];
}

const deoglCapsTextureFormat *deoglCapsFmtSupport::GetUseFBOArrayTexFormatFor(
eUseTextureFormats type ) const{
	if( type < 0 || type >= UseTextureFormatCount ){
		DETHROW( deeInvalidParam );
	}
	return pUseFBOArrTexFormats[ type ];
}



const deoglCapsTextureFormat &deoglCapsFmtSupport::RequireUseTex2DFormatFor(
eUseTextureFormats type) const{
	return pRequireFormat("Tex2D", type,
		(const deoglCapsTextureFormat **)&pUseTex2DFormats, pFoundTex2DFormats);
}

const deoglCapsTextureFormat &deoglCapsFmtSupport::RequireUseTexCubeFormatFor(
eUseTextureFormats type) const{
	return pRequireFormat("TexCube", type,
		(const deoglCapsTextureFormat **)&pUseTexCubeFormats, pFoundTexCubeFormats);
}

const deoglCapsTextureFormat &deoglCapsFmtSupport::RequireUseArrayTexFormatFor(
eUseTextureFormats type) const{
	return pRequireFormat("ArrTex", type,
		(const deoglCapsTextureFormat **)&pUseArrTexFormats, pFoundArrTexFormats);
}

const deoglCapsTextureFormat &deoglCapsFmtSupport::RequireUseFBOTex2DFormatFor(
eUseTextureFormats type) const{
	return pRequireFormat("FBOTex2D", type,
		(const deoglCapsTextureFormat **)&pUseFBOTex2DFormats, pFoundFBOTex2DFormats);
}

const deoglCapsTextureFormat &deoglCapsFmtSupport::RequireUseFBOTexCubeFormatFor(
eUseTextureFormats type) const{
	return pRequireFormat("FBOTexCube", type,
		(const deoglCapsTextureFormat **)&pUseFBOTexCubeFormats, pFoundFBOTexCubeFormats);
}

const deoglCapsTextureFormat &deoglCapsFmtSupport::RequireUseFBOArrayTexFormatFor(
eUseTextureFormats type) const{
	return pRequireFormat("FBOArrTex", type,
		(const deoglCapsTextureFormat **)&pUseFBOArrTexFormats, pFoundFBOArrTexFormats);
}


const char *deoglCapsFmtSupport::GetTextureFormatName(eUseTextureFormats type) const{
	return vTextureFormatNames[type];
}



void deoglCapsFmtSupport::DetectFormats( GLuint fbo ){
	OGL_IF_CHECK( deoglRenderThread &renderThread = pCapabilities.GetRenderThread(); )
	
	OGL_CHECK( renderThread, pglBindFramebuffer( GL_FRAMEBUFFER, fbo ) );
	
	// test 2d-texture and cube-texture for texture mapping
	pDetectTex2DFormats();
	pDetectTexCubeFormats();
	pDetectArrayTexFormats();
	
	// test 2d-texture and cube-texture as color/depth/stencil targets
	pDetectFBOTex2DFormats( fbo );
	pDetectFBOTexCubeFormats( fbo );
	pDetectFBOArrayTexFormats( fbo );
	
	#ifdef OS_ANDROID
	/*
	{
	const deoglCapsTextureFormatList* lists[6] = {
		&pFoundTex2DFormats, &pFoundTexCubeFormats, &pFoundArrTexFormats,
		&pFoundFBOTex2DFormats, &pFoundFBOTexCubeFormats, &pFoundFBOArrTexFormats};
	const char *listNames[6] = {
		"Tex2DFormats", "TexCubeFormats", "ArrTexFormats",
		"FBOTex2DFormats", "FBOTexCubeFormats", "FBOArrTexFormats"};
	int i, j;
	for(i=0; i<6; i++){
		renderThread.GetLogger().LogInfo(listNames[i]);
		const int count = lists[i]->GetFormatCount();
		for(j=0; j<count; j++){
			renderThread.GetLogger().LogInfoFormat("- %s", lists[i]->GetFormatAt(j)->GetName().GetString());
		}
	}
	
	const deoglCapsTextureFormat** lists2[6] = {
		&pUseTex2DFormats[0], &pUseTexCubeFormats[0], &pUseArrTexFormats[0],
		&pUseFBOTex2DFormats[0], &pUseFBOTexCubeFormats[0], &pUseFBOArrTexFormats[0]};
		
	for(i=0; i<6; i++){
		renderThread.GetLogger().LogInfoFormat("Use-%s", listNames[i]);
		for(j=0; j<UseTextureFormatCount; j++){
			renderThread.GetLogger().LogInfoFormat("- %s = %s", vTextureFormatNames[j],
				lists2[i][j] ? lists2[i][j]->GetName().GetString() : "-");
		}
	}
	}
	*/
	#endif
}

decString deoglCapsFmtSupport::SupportedFormatsTex2D() const{
	return pSupportedFormats(pFoundTex2DFormats);
}

decString deoglCapsFmtSupport::SupportedFormatsTexCube() const{
	return pSupportedFormats(pFoundTexCubeFormats);
}

decString deoglCapsFmtSupport::SupportedFormatsArrayTex() const{
	return pSupportedFormats(pFoundArrTexFormats);
}

decString deoglCapsFmtSupport::SupportedFormatsFBOTex2D() const{
	return pSupportedFormats(pFoundFBOTex2DFormats);
}

decString deoglCapsFmtSupport::SupportedFormatsFBOTexCube() const{
	return pSupportedFormats(pFoundFBOTexCubeFormats);
}

decString deoglCapsFmtSupport::SupportedFormatsFBOArrayTex() const{
	return pSupportedFormats(pFoundFBOArrTexFormats);
}


// Private Functions
//////////////////////

void deoglCapsFmtSupport::pDetectTex2DFormats(){
	int f, p;
	
	// test all formats
	for( f=0; f<ETTF_COUNT; f++ ){
		pTestTex2DFormat(vTestTextureFormats[f].format, vTestTextureFormats[f].pixelFormat,
			vTestTextureFormats[f].pixelType, vTestTextureFormats[f].bitsPerPixel,
			vTestTextureFormats[f].flags, vTestTextureFormats[f].name);
	}
	
	// find a format to use for the list of possible types
	for( p=0; p<TEST_PROGRAM_COUNT; p++ ){
		if( ! pUseTex2DFormats[ vTestProgram[ p ].target ] ){
			pUseTex2DFormats[vTestProgram[p].target] = pFoundTex2DFormats.GetFormatWith(
				vTestTextureFormats[vTestProgram[p].testFormat].format);
		}
	}
	
	for( p=0; p<TEST_FALLBACK_COUNT; p++ ){
		if( ! pUseTex2DFormats[ vTestFallback[ p ].target ] ){
			pUseTex2DFormats[vTestFallback[p].target] =
				pUseTex2DFormats[vTestFallback[p].fallbackTarget];
		}
	}
	
	// verify that all required formats are found
	const int required[ 11 ] = { eutfR8, eutfR16F, eutfRG8, eutfRG16F, eutfRGB8, eutfRGB16F,
		eutfRGBA8, eutfRGBA16F, eutfDepth, eutfDepth_Stencil, eutfDepth16 };
	
	for( p=0; p<11; p++ ){
		if( ! pUseTex2DFormats[ required[ p ] ] ){
			pCapabilities.GetRenderThread().GetLogger().LogErrorFormat(
				"Required format %s not found for 2D-Textures!",
				vTextureFormatNames[ required[ p ] ] );
			
			pCapabilities.GetRenderThread().GetLogger().LogError("Supported formats:");
			int i;
			for(i=0; i<pFoundTex2DFormats.GetFormatCount(); i++){
				pCapabilities.GetRenderThread().GetLogger().LogErrorFormat("- %s",
					pFoundTex2DFormats.GetFormatAt(i)->GetName().GetString());
			}
			
			DETHROW( deeInvalidParam );
		}
	}
}

void deoglCapsFmtSupport::pDetectTexCubeFormats(){
	int f, p;
	
	// test all formats
	for( f=0; f<ETTF_COUNT; f++ ){
		pTestTexCubeFormat(vTestTextureFormats[f].format, vTestTextureFormats[f].pixelFormat,
			vTestTextureFormats[f].pixelType, vTestTextureFormats[f].bitsPerPixel,
			vTestTextureFormats[f].flags, vTestTextureFormats[f].name);
	}
	
	// find a format to use for the list of possible types
	for( p=0; p<TEST_PROGRAM_COUNT; p++ ){
		if( ! pUseTexCubeFormats[ vTestProgram[ p ].target ] ){
			pUseTexCubeFormats[vTestProgram[p].target] = pFoundTexCubeFormats.GetFormatWith(
				vTestTextureFormats[vTestProgram[p].testFormat].format);
		}
	}
	
	for( p=0; p<TEST_FALLBACK_COUNT; p++ ){
		if( ! pUseTexCubeFormats[ vTestFallback[ p ].target ] ){
			pUseTexCubeFormats[vTestFallback[p].target] =
				pUseTexCubeFormats[vTestFallback[p].fallbackTarget];
		}
	}
	
	// verify that all required formats are found
	const int required[ 10 ] = { eutfR8, eutfR16F, eutfRG8, eutfRG16F, eutfRGB8, eutfRGB16F,
		eutfRGBA8, eutfRGBA16F, eutfDepth, eutfDepth16 };
	
	for( p=0; p<10; p++ ){
		if( ! pUseTexCubeFormats[ required[ p ] ] ){
			pCapabilities.GetRenderThread().GetLogger().LogErrorFormat(
				"Required format %s not found for Cube-Textures!",
				vTextureFormatNames[ required[ p ] ] );
			
			pCapabilities.GetRenderThread().GetLogger().LogError("Supported formats:");
			int i;
			for(i=0; i<pFoundTexCubeFormats.GetFormatCount(); i++){
				pCapabilities.GetRenderThread().GetLogger().LogErrorFormat("- %s",
					pFoundTexCubeFormats.GetFormatAt(i)->GetName().GetString());
			}
			
			DETHROW( deeInvalidParam );
		}
	}
}

void deoglCapsFmtSupport::pDetectArrayTexFormats(){
	int f, p;
	
	// test all formats
	for( f=0; f<ETTF_COUNT; f++ ){
		pTestArrayTexFormat(vTestTextureFormats[f].format, vTestTextureFormats[f].pixelFormat,
			vTestTextureFormats[f].pixelType, vTestTextureFormats[f].bitsPerPixel,
			vTestTextureFormats[f].flags, vTestTextureFormats[f].name);
	}
	
	// find a format to use for the list of possible types
	for( p=0; p<TEST_PROGRAM_COUNT; p++ ){
		if( ! pUseArrTexFormats[ vTestProgram[ p ].target ] ){
			pUseArrTexFormats[vTestProgram[p].target] = pFoundArrTexFormats.GetFormatWith(
				vTestTextureFormats[vTestProgram[p].testFormat].format);
		}
	}
	
	for( p=0; p<TEST_FALLBACK_COUNT; p++ ){
		if( ! pUseArrTexFormats[ vTestFallback[ p ].target ] ){
			pUseArrTexFormats[ vTestFallback[ p ].target ] =
				pUseArrTexFormats[ vTestFallback[ p ].fallbackTarget ];
		}
	}
	
	// verify that all required formats are found
	const int required[ 15 ] = { eutfR8, eutfR16F, eutfRG8, eutfRG16F, eutfRGB8, eutfRGB16F,
		eutfRGBA8, eutfRGBA16F, eutfR8_S, eutfRG8_S, eutfRGB8_S, eutfRGBA8_S,
		eutfDepth, eutfDepth_Stencil, eutfDepth16 };
	
	for( p=0; p<15; p++ ){
		if( ! pUseArrTexFormats[ required[ p ] ] ){
			pCapabilities.GetRenderThread().GetLogger().LogErrorFormat(
				"Required format %s not found for 2D-Array-Textures!",
				vTextureFormatNames[ required[ p ] ] );
			
			pCapabilities.GetRenderThread().GetLogger().LogError("Supported formats:");
			int i;
			for(i=0; i<pFoundArrTexFormats.GetFormatCount(); i++){
				pCapabilities.GetRenderThread().GetLogger().LogErrorFormat("- %s",
					pFoundArrTexFormats.GetFormatAt(i)->GetName().GetString());
			}
			
			DETHROW( deeInvalidParam );
		}
	}
}

void deoglCapsFmtSupport::pDetectFBOTex2DFormats( GLuint fbo ){
	int f, p;
	
	// test all formats
	for( f=0; f<ETTF_COUNT; f++ ){
		pTestFBOTex2DFormat(fbo, vTestTextureFormats[f].format,
			vTestTextureFormats[f].pixelFormat, vTestTextureFormats[f].pixelType,
			vTestTextureFormats[f].bitsPerPixel, vTestTextureFormats[f].flags,
			vTestTextureFormats[f].name, vTestTextureFormats[f].what);
	}
	
	// find a format to use for the list of possible types
	for( p=0; p<TEST_PROGRAM_COUNT; p++ ){
		if( ! pUseFBOTex2DFormats[ vTestProgram[ p ].target ] ){
			pUseFBOTex2DFormats[vTestProgram[p].target] = pFoundFBOTex2DFormats.GetFormatWith(
					vTestTextureFormats[vTestProgram[p].testFormat].format);
		}
	}
	
	for( p=0; p<TEST_FALLBACK_COUNT; p++ ){
		if( ! pUseFBOTex2DFormats[ vTestFallback[ p ].target ] ){
			pUseFBOTex2DFormats[ vTestFallback[ p ].target ] =
				pUseFBOTex2DFormats[ vTestFallback[ p ].fallbackTarget ];
		}
	}
	
	// verify that all required formats are found
	const int required[ 11 ] = { eutfR8, eutfR16F, eutfRG8, eutfRG16F, eutfRGB8, eutfRGB16F,
		eutfRGBA8, eutfRGBA16F, eutfDepth, eutfDepth_Stencil, eutfDepth16 };
	
	for( p=0; p<11; p++ ){
		if( ! pUseFBOTex2DFormats[ required[ p ] ] ){
			pCapabilities.GetRenderThread().GetLogger().LogErrorFormat(
				"Required format %s not found for FBO 2D-Textures!",
				vTextureFormatNames[ required[ p ] ] );
			
			pCapabilities.GetRenderThread().GetLogger().LogError("Supported formats:");
			int i;
			for(i=0; i<pFoundFBOTex2DFormats.GetFormatCount(); i++){
				pCapabilities.GetRenderThread().GetLogger().LogErrorFormat("- %s",
					pFoundFBOTex2DFormats.GetFormatAt(i)->GetName().GetString());
			}
			
			DETHROW( deeInvalidParam );
		}
	}
}

void deoglCapsFmtSupport::pDetectFBOTexCubeFormats( GLuint fbo ){
	int f, p;
	
	// test all formats
	for( f=0; f<ETTF_COUNT; f++ ){
		pTestFBOTexCubeFormat(fbo, vTestTextureFormats[f].format,
			vTestTextureFormats[f].pixelFormat, vTestTextureFormats[f].pixelType,
			vTestTextureFormats[f].bitsPerPixel, vTestTextureFormats[f].flags,
			vTestTextureFormats[f].name, vTestTextureFormats[f].what);
	}
	
	// find a format to use for the list of possible types
	for( p=0; p<TEST_PROGRAM_COUNT; p++ ){
		if( ! pUseFBOTexCubeFormats[ vTestProgram[ p ].target ] ){
			pUseFBOTexCubeFormats[vTestProgram[p].target] = pFoundFBOTexCubeFormats.GetFormatWith(
				vTestTextureFormats[vTestProgram[p].testFormat].format);
		}
	}
	
	for( p=0; p<TEST_FALLBACK_COUNT; p++ ){
		if( ! pUseFBOTexCubeFormats[ vTestFallback[ p ].target ] ){
			pUseFBOTexCubeFormats[vTestFallback[p].target] =
				pUseFBOTexCubeFormats[vTestFallback[p].fallbackTarget];
		}
	}
	
	// verify that all required formats are found
	const int required[ 10 ] = { eutfR8, eutfR16F, eutfRG8, eutfRG16F, eutfRGB8, eutfRGB16F,
		eutfRGBA8, eutfRGBA16F, eutfDepth, eutfDepth16 };
	
	for( p=0; p<10; p++ ){
		if( ! pUseFBOTexCubeFormats[ required[ p ] ] ){
			pCapabilities.GetRenderThread().GetLogger().LogErrorFormat(
				"Required format %s not found for FBO Cube-Textures!",
				vTextureFormatNames[ required[ p ] ] );
			
			pCapabilities.GetRenderThread().GetLogger().LogError("Supported formats:");
			int i;
			for(i=0; i<pFoundFBOTexCubeFormats.GetFormatCount(); i++){
				pCapabilities.GetRenderThread().GetLogger().LogErrorFormat("- %s",
					pFoundFBOTexCubeFormats.GetFormatAt(i)->GetName().GetString());
			}
			
			DETHROW( deeInvalidParam );
		}
	}
}

void deoglCapsFmtSupport::pDetectFBOArrayTexFormats( GLuint fbo ){
	int f, p;
	
	// test all formats
	for( f=0; f<ETTF_COUNT; f++ ){
		pTestFBOArrayTexFormat(fbo, vTestTextureFormats[f].format,
			vTestTextureFormats[f].pixelFormat, vTestTextureFormats[f].pixelType,
			vTestTextureFormats[f].bitsPerPixel, vTestTextureFormats[f].flags,
			vTestTextureFormats[f].name, vTestTextureFormats[f].what);
	}
	
	// find a format to use for the list of possible types
	for( p=0; p<TEST_PROGRAM_COUNT; p++ ){
		if( ! pUseFBOArrTexFormats[ vTestProgram[ p ].target ] ){
			pUseFBOArrTexFormats[vTestProgram[p].target] = pFoundFBOArrTexFormats.GetFormatWith(
				vTestTextureFormats[vTestProgram[p].testFormat].format);
		}
	}
	
	for( p=0; p<TEST_FALLBACK_COUNT; p++ ){
		if( ! pUseFBOArrTexFormats[ vTestFallback[ p ].target ] ){
			pUseFBOArrTexFormats[vTestFallback[p].target] =
				pUseFBOArrTexFormats[vTestFallback[p].fallbackTarget];
		}
	}
	
	// verify that all required formats are found
	#ifndef OS_ANDROID
	const int required[ 15 ] = { eutfR8, eutfR16F, eutfRG8, eutfRG16F, eutfRGB8, eutfRGB16F,
		eutfRGBA8, eutfRGBA16F, eutfR8_S, eutfRG8_S, eutfRGB8_S, eutfRGBA8_S,
		eutfDepth, eutfDepth_Stencil, eutfDepth16 };
	
	for( p=0; p<15; p++ ){
		if( ! pUseFBOArrTexFormats[ required[ p ] ] ){
			pCapabilities.GetRenderThread().GetLogger().LogErrorFormat(
				"Required format %s not found for FBO 2D-Array-Textures!",
				vTextureFormatNames[ required[ p ] ] );
			
			pCapabilities.GetRenderThread().GetLogger().LogError("Supported formats:");
			int i;
			for(i=0; i<pFoundFBOArrTexFormats.GetFormatCount(); i++){
				pCapabilities.GetRenderThread().GetLogger().LogErrorFormat("- %s",
					pFoundFBOArrTexFormats.GetFormatAt(i)->GetName().GetString());
			}
			
			DETHROW( deeInvalidParam );
		}
	}
	#endif
}



bool deoglCapsFmtSupport::pTestTex2DFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
int bitsPerPixel, int flags, const char *name ){
	OGL_IF_CHECK( deoglRenderThread &renderThread = pCapabilities.GetRenderThread(); )
	
	// HACK: Bug in Ati driver 8.54.3 ( these formats cause driver to crash if probed )
	if( ! ENABLE_COMPRESS_LATC1 ){
		if( format == GL_COMPRESSED_LUMINANCE_LATC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_LATC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_LATC2 ){
		if( format == GL_COMPRESSED_LUMINANCE_ALPHA_LATC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC1 ){
		if( format == GL_COMPRESSED_RED_RGTC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_RGTC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC2 ){
		if( format == GL_COMPRESSED_RED_GREEN_RGTC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2 ) return false;
	}
	// ENDHACK
	
	GLuint texture = 0;
	int errorCode;
	
	OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
	if( ! texture ) DETHROW( deeOutOfMemory );
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	
	glTexImage2D( GL_TEXTURE_2D, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	errorCode = glGetError();
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, 0 ) );
	
	OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
	
	if( errorCode == GL_NO_ERROR ){
		pFoundTex2DFormats.AddFormat( format, pixelFormat, pixelType, bitsPerPixel,
			HAS_FLAG_DEPTH( flags ), HAS_FLAG_DEPTH_FLOAT( flags ), HAS_FLAG_STENCIL( flags ),
			HAS_FLAG_COMPRESSED( flags ), name );
	}
		
	return errorCode == GL_NO_ERROR;
}

bool deoglCapsFmtSupport::pTestTexCubeFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
int bitsPerPixel, int flags, const char *name ){
	OGL_IF_CHECK( deoglRenderThread &renderThread = pCapabilities.GetRenderThread(); )
	
	// HACK: Bug in Ati driver 8.54.3 ( these formats cause driver to crash if probed )
	if( ! ENABLE_COMPRESS_LATC1 ){
		if( format == GL_COMPRESSED_LUMINANCE_LATC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_LATC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_LATC2 ){
		if( format == GL_COMPRESSED_LUMINANCE_ALPHA_LATC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC1 ){
		if( format == GL_COMPRESSED_RED_RGTC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_RGTC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC2 ){
		if( format == GL_COMPRESSED_RED_GREEN_RGTC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2 ) return false;
	}
	// ENDHACK
	
	GLuint texture = 0;
	int errorCode;
	
	OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
	if( ! texture ) DETHROW( deeOutOfMemory );
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, texture ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE ) );
	
	glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	errorCode = glGetError();
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, 0 ) );
	
	OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
	
	if( errorCode == GL_NO_ERROR ){
		pFoundTexCubeFormats.AddFormat( format, pixelFormat, pixelType, bitsPerPixel,
			HAS_FLAG_DEPTH( flags ), HAS_FLAG_DEPTH_FLOAT( flags ), HAS_FLAG_STENCIL( flags ),
			HAS_FLAG_COMPRESSED( flags ), name );
	}
	
	return errorCode == GL_NO_ERROR;
}

bool deoglCapsFmtSupport::pTestArrayTexFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
int bitsPerPixel, int flags, const char *name ){
	OGL_IF_CHECK( deoglRenderThread &renderThread = pCapabilities.GetRenderThread(); )
	
	// HACK: Bug in Ati driver 8.54.3 ( these formats cause driver to crash if probed )
	if( ! ENABLE_COMPRESS_LATC1 ){
		if( format == GL_COMPRESSED_LUMINANCE_LATC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_LATC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_LATC2 ){
		if( format == GL_COMPRESSED_LUMINANCE_ALPHA_LATC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC1 ){
		if( format == GL_COMPRESSED_RED_RGTC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_RGTC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC2 ){
		if( format == GL_COMPRESSED_RED_GREEN_RGTC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2 ) return false;
	}
	// ENDHACK
	
	GLuint texture = 0;
	int errorCode;
	
	OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
	if( ! texture ) DETHROW( deeOutOfMemory );
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D_ARRAY, texture ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	
	pglTexImage3D( GL_TEXTURE_2D_ARRAY, 0, format, 8, 8, 4, 0, pixelFormat, pixelType, NULL );
	errorCode = glGetError();
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D_ARRAY, 0 ) );
	
	OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
	
	if( errorCode == GL_NO_ERROR ){
		pFoundArrTexFormats.AddFormat( format, pixelFormat, pixelType, bitsPerPixel,
			HAS_FLAG_DEPTH( flags ), HAS_FLAG_DEPTH_FLOAT( flags ), HAS_FLAG_STENCIL( flags ),
			HAS_FLAG_COMPRESSED( flags ), name );
	}
	
	return errorCode == GL_NO_ERROR;
}

bool deoglCapsFmtSupport::pTestFBOTex2DFormat( GLuint fbo, GLint format, GLenum pixelFormat, GLenum pixelType,
int bitsPerPixel, int flags, const char *name, int what ){
	OGL_IF_CHECK( deoglRenderThread &renderThread = pCapabilities.GetRenderThread(); )
	
	// HACK: Bug in Ati driver 8.54.3 ( these formats cause driver to crash if probed )
	if( ! ENABLE_COMPRESS_LATC1 ){
		if( format == GL_COMPRESSED_LUMINANCE_LATC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_LATC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_LATC2 ){
		if( format == GL_COMPRESSED_LUMINANCE_ALPHA_LATC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC1 ){
		if( format == GL_COMPRESSED_RED_RGTC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_RGTC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC2 ){
		if( format == GL_COMPRESSED_RED_GREEN_RGTC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2 ) return false;
	}
	// ENDHACK
	
	GLuint texture = 0;
	int errorCode;
	
//	renderThread.GetLogger().LogInfoFormat( "Capabilities: Testing FBO Tex2D %s", name );
	OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
	if( ! texture ) DETHROW( deeOutOfMemory );
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	
	glTexImage2D( GL_TEXTURE_2D, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	errorCode = glGetError();
	
	if( errorCode == GL_NO_ERROR ){
		if( what == etwColor ){
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
			
		}else if( what == etwDepth ){
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
			
		}else if( what == etwStencil ){
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
			
		}else{ // etwDepthStencil
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0 );
			errorCode = glGetError();
			if( errorCode == GL_NO_ERROR ){
				pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0 );
				errorCode = glGetError();
			}
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
		}
		
		if( errorCode == GL_NO_ERROR ){
			errorCode = pglCheckFramebufferStatus( GL_FRAMEBUFFER );
			if( errorCode == GL_FRAMEBUFFER_COMPLETE ){
				errorCode = GL_NO_ERROR;
				pFoundFBOTex2DFormats.AddFormat( format, pixelFormat, pixelType, bitsPerPixel,
					HAS_FLAG_DEPTH( flags ), HAS_FLAG_DEPTH_FLOAT( flags ),
					HAS_FLAG_STENCIL( flags ), HAS_FLAG_COMPRESSED( flags ), name );
			}
		}
		
		pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );
		pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0 );
		pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0 );
	}
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, 0 ) );
	
	OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
	
	return errorCode == GL_NO_ERROR;
}

bool deoglCapsFmtSupport::pTestFBOTexCubeFormat( GLuint fbo, GLint format, GLenum pixelFormat,
GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what ){
	OGL_IF_CHECK( deoglRenderThread &renderThread = pCapabilities.GetRenderThread(); )
	
	// HACK: Bug in Ati driver 8.54.3 ( these formats cause driver to crash if probed )
	if( ! ENABLE_COMPRESS_LATC1 ){
		if( format == GL_COMPRESSED_LUMINANCE_LATC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_LATC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_LATC2 ){
		if( format == GL_COMPRESSED_LUMINANCE_ALPHA_LATC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC1 ){
		if( format == GL_COMPRESSED_RED_RGTC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_RGTC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC2 ){
		if( format == GL_COMPRESSED_RED_GREEN_RGTC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2 ) return false;
	}
	// ENDHACK
	
	GLuint texture = 0;
	int errorCode;
	
//	renderThread.GetLogger().LogInfoFormat( "Capabilities Testing FBO Tex2D %s", name );
	OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
	if( ! texture ) DETHROW( deeOutOfMemory );
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, texture ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE ) );
	
	glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	glTexImage2D( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, 8, 8, 0, pixelFormat, pixelType, NULL );
	errorCode = glGetError();
	
	if( errorCode == GL_NO_ERROR ){
		if( what == etwColor ){
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
			
		}else if( what == etwDepth ){
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
			
		}else if( what == etwStencil ){
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
			
		}else{ // etwDepthStencil
			pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture, 0 );
			errorCode = glGetError();
			if( errorCode == GL_NO_ERROR ){
				pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture, 0 );
				errorCode = glGetError();
			}
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
		}
		
		if( errorCode == GL_NO_ERROR ){
			errorCode = pglCheckFramebufferStatus( GL_FRAMEBUFFER );
			if( errorCode == GL_FRAMEBUFFER_COMPLETE ){
				errorCode = GL_NO_ERROR;
				pFoundFBOTexCubeFormats.AddFormat( format, pixelFormat, pixelType, bitsPerPixel,
					HAS_FLAG_DEPTH( flags ), HAS_FLAG_DEPTH_FLOAT( flags ),
					HAS_FLAG_STENCIL( flags ), HAS_FLAG_COMPRESSED( flags ), name );
			}
		}
		
		pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 );
		pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 );
		pglFramebufferTexture2D( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 );
	}
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_CUBE_MAP, 0 ) );
	
	OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
	
	return errorCode == GL_NO_ERROR;
}

bool deoglCapsFmtSupport::pTestFBOArrayTexFormat( GLuint fbo, GLint format, GLenum pixelFormat,
GLenum pixelType, int bitsPerPixel, int flags, const char *name, int what ){
	OGL_IF_CHECK( deoglRenderThread &renderThread = pCapabilities.GetRenderThread(); )
	
	// HACK: Bug in Ati driver 8.54.3 ( these formats cause driver to crash if probed )
	if( ! ENABLE_COMPRESS_LATC1 ){
		if( format == GL_COMPRESSED_LUMINANCE_LATC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_LATC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_LATC2 ){
		if( format == GL_COMPRESSED_LUMINANCE_ALPHA_LATC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC1 ){
		if( format == GL_COMPRESSED_RED_RGTC1 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_RGTC1 ) return false;
	}
	if( ! ENABLE_COMPRESS_RGTC2 ){
		if( format == GL_COMPRESSED_RED_GREEN_RGTC2 ) return false;
		if( format == GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2 ) return false;
	}
	// ENDHACK
	
	GLuint texture = 0;
	int errorCode;
	
//	renderThread.GetLogg().LogInfoFormat( "Capabilities: Testing FBO Tex2D %s", name );
	OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
	if( ! texture ) DETHROW( deeOutOfMemory );
	
	OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D_ARRAY, texture ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
	OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
	
	pglTexImage3D( GL_TEXTURE_2D_ARRAY, 0, format, 8, 8, 4, 0, pixelFormat, pixelType, NULL );
	errorCode = glGetError();
	
	if( errorCode == GL_NO_ERROR ){
		if( what == etwColor ){
			pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
			
		}else if( what == etwDepth ){
			pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
			
		}else if( what == etwStencil ){
			pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, texture, 0, 0 );
			errorCode = glGetError();
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
			
		}else{ // etwDepthStencil
			pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0, 0 );
			errorCode = glGetError();
			if( errorCode == GL_NO_ERROR ){
				pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, texture, 0, 0 );
				errorCode = glGetError();
			}
			
			const GLenum buffers[ 1 ] = { GL_NONE };
			OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
			OGL_CHECK( renderThread, glReadBuffer( GL_NONE ) );
		}
		
		if( errorCode == GL_NO_ERROR ){
			errorCode = pglCheckFramebufferStatus( GL_FRAMEBUFFER );
			if( errorCode == GL_FRAMEBUFFER_COMPLETE ){
				errorCode = GL_NO_ERROR;
				pFoundFBOArrTexFormats.AddFormat( format, pixelFormat, pixelType, bitsPerPixel,
					HAS_FLAG_DEPTH( flags ), HAS_FLAG_DEPTH_FLOAT( flags ),
					HAS_FLAG_STENCIL( flags ), HAS_FLAG_COMPRESSED( flags ), name );
			}
		}
		
		pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0, 0 );
		if( pCapabilities.ATLUnbind().GetResult() == 1 ){
			//pglFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0 ); // GL_INVALID_OPERATION on nvidia
			
		}else{
			pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0, 0 ); // GL_INVALID_OPERATION on ati
		}
		pglFramebufferTextureLayer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, 0, 0, 0 );
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D_ARRAY, 0 ) );
		OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
	}
	
	return errorCode == GL_NO_ERROR;
}

const deoglCapsTextureFormat &deoglCapsFmtSupport::pRequireFormat(const char *categoryName,
eUseTextureFormats type, const deoglCapsTextureFormat **listUse,
const deoglCapsTextureFormatList &listSupported) const{
	const deoglCapsTextureFormat * const fmt = listUse[type];
	if(!fmt){
		const decString names(pSupportedFormats(listSupported));
		
		deoglRTLogger &logger = pCapabilities.GetRenderThread().GetLogger();
		
		decString message;
		message.Format("%s format not found %s", categoryName, vTextureFormatNames[type]);
		logger.LogError(message);
		
		logger.LogError(decString("Supported: ") + names);
		
		DETHROW_INFO(deeInvalidParam, message);
	}
	return *fmt;
}

decString deoglCapsFmtSupport::pSupportedFormats(
const deoglCapsTextureFormatList &listSupported) const{
	decStringList supnames;
	const int count = listSupported.GetFormatCount();
	int i;
	for(i=0; i<count; i++){
		supnames.Add(listSupported.GetFormatAt(i)->GetName());
	}
	return supnames.Join(", ");
}
