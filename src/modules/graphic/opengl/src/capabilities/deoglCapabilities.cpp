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
#include "deoglCapsTextureFormat.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class deoglCapabilities
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapabilities::deoglCapabilities( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pFormats( *this ),

pMaxTextureSize( 1024 ),
pMax3DTextureSize( 1024 ),
pMaxDrawBuffers( 4 ),
pUBOMaxSize( 0 ),
pTBOMaxSize( 0 ),
pSSBOMaxSize( 0 ),
pSSBOMaxBlocksVertex( 0 ),
pSSBOMaxBlocksFragment( 0 ),
pSSBOMaxBlocksGeometry( 0 ),
pSSBOMaxBlocksCompute( 0 ),
pUBOOffsetAlignment( 4 ),
pGeometryShaderMaxVertices( 0 ),
pGeometryShaderMaxComponents( 0 ),
pNumProgramBinaryFormats( 0 ),
pMaxComputeWorkGroupInvocations(0),
pMaxTextureImageUnits(32),

pATLUnbind( *this ),
pUBOIndirectMatrixAccess( *this ),
pClearEntireArrayTexture( *this ),
pUBODirectLinkDeadloop( *this ),
pFramebufferTextureSingle( *this ),
pStd430( *this ),
pRestrictedImageBufferFormats(false)
{
	const GLfloat fsquad[ 12 ] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f };
	
	pFSQuadVBO = 0;
	pFSQuadVAO = 0;
	
	// full screen quad VBO and VAO
	OGL_CHECK( renderThread, pglGenVertexArrays( 1, &pFSQuadVAO ) );
	if( ! pFSQuadVAO ){
		DETHROW( deeOutOfMemory );
	}
	OGL_CHECK( renderThread, pglBindVertexArray( pFSQuadVAO ) );
	
	OGL_CHECK( renderThread, pglGenBuffers( 1, &pFSQuadVBO ) );
	if( ! pFSQuadVBO ){
		DETHROW( deeOutOfMemory );
	}
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pFSQuadVBO ) );
	OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( fsquad ),
		( const GLvoid * )&fsquad, GL_STATIC_DRAW ) );
	
	OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) );
	OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT,
		GL_FALSE, 0, ( const GLvoid * )0 ) );
	
	OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
}

deoglCapabilities::~deoglCapabilities(){
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	if( pFSQuadVAO ){
		pglDeleteVertexArrays( 1, &pFSQuadVAO );
	}
	dops.DeleteOpenGLBuffer( pFSQuadVBO );
}



// Management
///////////////



void deoglCapabilities::DetectCapabilities(){
	// NOTE not using OGL_CHECK in some places on purpose to not produce misleading error logs
	// #ifdef OS_ANDROID
	// deoglFramebuffer *framebuffer = NULL;
	// #endif
	
	deoglExtensions &ext = pRenderThread.GetExtensions();
	deoglRTLogger &logger = pRenderThread.GetLogger();
	GLint64 resultsInt64[1];
	GLint resultsInt[3];
	GLuint fbo = 0;
	int i;
	
	OGL_CHECK( pRenderThread, pglActiveTexture( GL_TEXTURE0 ) );
	
	pRenderThread.GetDebug().SetEnableHwDebugOutput( false ); // otherwise we get bogus reports
	
	try{
		OGL_CHECK( pRenderThread, pglGenFramebuffers( 1, &fbo ) );
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_FRAMEBUFFER, fbo ) );
		
		pATLUnbind.Check( fbo );
		
		pFormats.DetectFormats( fbo );
		
		// test important hardware limits
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_TEXTURE_SIZE, &resultsInt[ 0 ] ) );
		pMaxTextureSize = ( int )resultsInt[ 0 ];
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &resultsInt[ 0 ] ) );
		pMax3DTextureSize = ( int )resultsInt[ 0 ];
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_DRAW_BUFFERS, &resultsInt[ 0 ] ) );
		pMaxDrawBuffers = ( int )resultsInt[ 0 ];
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_UNIFORM_BUFFER_BINDINGS, &resultsInt[ 0 ] ) );
		const int maxUBOBindings = resultsInt[ 0 ]; // at least 84 required by spec
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &resultsInt[ 0 ] ) );
		pUBOOffsetAlignment = ( int )resultsInt[ 0 ];
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_UNIFORM_BLOCK_SIZE, &resultsInt[ 0 ] ) );
		pUBOMaxSize = ( int )resultsInt[ 0 ];
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_TEXTURE_BUFFER_SIZE, &resultsInt[ 0 ] ) );
		pTBOMaxSize = ( int )resultsInt[ 0 ];
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &resultsInt[0]));
		pMaxTextureImageUnits = (int)resultsInt[0];
		
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_storage_buffer_object)
		|| ext.GetGLESVersion() >= deoglExtensions::evgles3p1){
			if( pglGetInteger64v ){
				OGL_CHECK( pRenderThread, pglGetInteger64v( GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &resultsInt64[ 0 ] ) );
				if( resultsInt64[ 0 ] > 0x7fffffff ){
					resultsInt64[ 0 ] = 0x7fffffff; // max 2GB is enough and does not overflow int32
				}
				pSSBOMaxSize = ( int )resultsInt64[ 0 ];
				
			}else{
				OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &resultsInt[ 0 ] ) );
				pSSBOMaxSize = ( int )resultsInt[ 0 ];
			}
			
			OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &resultsInt[ 0 ] ) );
			pSSBOMaxBlocksVertex = ( int )resultsInt[ 0 ];
			
			OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &resultsInt[ 0 ] ) );
			pSSBOMaxBlocksFragment = ( int )resultsInt[ 0 ];
			
			OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, &resultsInt[ 0 ] ) );
			pSSBOMaxBlocksGeometry = ( int )resultsInt[ 0 ];
			
			OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &resultsInt[ 0 ] ) );
			pSSBOMaxBlocksCompute = ( int )resultsInt[ 0 ];
		}
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_GEOMETRY_OUTPUT_VERTICES, &resultsInt[ 0 ] ) );
		pGeometryShaderMaxVertices = ( int )resultsInt[ 0 ];
		
		OGL_CHECK( pRenderThread, glGetIntegerv( GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS, &resultsInt[ 0 ] ) );
		pGeometryShaderMaxComponents = ( int )resultsInt[ 0 ];
		
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_get_program_binary)){
			OGL_CHECK(pRenderThread, glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &resultsInt[0]));
			pNumProgramBinaryFormats = (int)resultsInt[0];
		}
		
		for(i=0; i<3; i++){
			OGL_CHECK(pRenderThread, pglGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, i, &resultsInt[i]));
		}
		pMaxComputeWorkGroupSize.x = (int)resultsInt[0];
		pMaxComputeWorkGroupSize.y = (int)resultsInt[1];
		pMaxComputeWorkGroupSize.z = (int)resultsInt[2];
		
		OGL_CHECK(pRenderThread, glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &resultsInt[0]));
		pMaxComputeWorkGroupInvocations = (int)resultsInt[0];
		
		// report capabilities
		logger.LogInfo( "Capabilities:" );
		logger.LogInfoFormat( "- Maximum texture size = %d", pMaxTextureSize );
		logger.LogInfoFormat( "- Maximum 3D texture size = %d", pMax3DTextureSize );
		logger.LogInfoFormat( "- Maximum draw buffers = %d", pMaxDrawBuffers );
		logger.LogInfoFormat( "- UBO Maximum Bindings = %d", maxUBOBindings );
		logger.LogInfoFormat( "- UBO Maximum Block Size = %d", pUBOMaxSize );
		logger.LogInfoFormat( "- UBO Buffer Offset Alignment = %d", pUBOOffsetAlignment );
		logger.LogInfoFormat( "- TBO Maximum Size = %d", pTBOMaxSize );
		logger.LogInfoFormat( "- SSBO Maximum Size = %d", pSSBOMaxSize );
		logger.LogInfoFormat( "- Count program binary formats = %d", pNumProgramBinaryFormats );
		logger.LogInfoFormat("- Maximum texture image units = %d", pMaxTextureImageUnits);
		logger.LogInfo( "- SSBO Maximum Blocks Shader:" );
		logger.LogInfoFormat( "  - Vertex = %d", pSSBOMaxBlocksVertex );
		logger.LogInfoFormat( "  - Geometry = %d", pSSBOMaxBlocksGeometry );
		logger.LogInfoFormat( "  - Fragment = %d", pSSBOMaxBlocksFragment );
		logger.LogInfoFormat( "  - Compute = %d", pSSBOMaxBlocksCompute );
		logger.LogInfo( "- Geometry Shader:" );
		logger.LogInfoFormat( "  - Max Vertices = %d", pGeometryShaderMaxVertices );
		logger.LogInfoFormat( "  - Max Components = %d", pGeometryShaderMaxComponents );
		logger.LogInfo( "- Compute Shader:" );
		logger.LogInfoFormat("  - Max Work Group Size = x=%d y=%d z=%d",
			pMaxComputeWorkGroupSize.x, pMaxComputeWorkGroupSize.y, pMaxComputeWorkGroupSize.z);
		logger.LogInfoFormat("  - Max Work Group Invocations = %d", pMaxComputeWorkGroupInvocations);
		
		// hard fail if certain bugs are present we can not fix
		if( ( pUBOOffsetAlignment % 4 ) != 0 ){
			logger.LogError( "Capabilities: UBO Buffer Offset Alignment is not a multiple of 4!" );
			DETHROW( deeInvalidParam );
		}
		
		// disable extensions with troubles
		if(ext.GetHasExtension(deoglExtensions::ext_ARB_shader_storage_buffer_object)
		&& pSSBOMaxBlocksCompute < 4){
			logger.LogWarn( "Capabilities: SSBO does not support at least 4 compute SSBO blocks. Disable SSBO Support." );
			ext.DisableExtension( deoglExtensions::ext_ARB_shader_storage_buffer_object );
		}
		
		// verify driver is working correctly
		pFramebufferTextureSingle.Check( fbo );
		
		pUBOIndirectMatrixAccess.Check( fbo );
		pClearEntireArrayTexture.Check( fbo );
		pUBODirectLinkDeadloop.Check( fbo );
		
		pStd430.Check();
		
#ifdef OS_ANDROID
		pRestrictedImageBufferFormats = true;
#endif
		
		// #ifdef OS_ANDROID
		// framebuffer = new deoglFramebuffer( pRenderThread, false );
		// pAndroidTest( framebuffer );
		// pRenderThread.GetFramebuffer().Activate( NULL );
		// delete framebuffer;
		// framebuffer = NULL;
		// #endif
		
		// ensure the color/depth/stencil attachments are unbound
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 ) );
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 ) );
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0 ) );
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 ) );
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0 ) );
		OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_STENCIL_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 ) );
		
		// switch back to the primary fbo
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
		
		// release the fbo name
		OGL_CHECK( pRenderThread, pglDeleteFramebuffers( 1, &fbo ) );
		
		pRenderThread.GetDebug().SetEnableHwDebugOutput( true ); // revert back to normal operation
		
	}catch( const deException &e ){
		e.PrintError();
		
		if( fbo ){
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 ) );
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 ) );
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0 ) );
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 ) );
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0 ) );
			OGL_CHECK( pRenderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_STENCIL_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, 0 ) );
			
			OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_FRAMEBUFFER, 0 ) );
			
			OGL_CHECK( pRenderThread, pglDeleteFramebuffers( 1, &fbo ) );
		}
		
		pRenderThread.GetDebug().SetEnableHwDebugOutput( true ); // revert back to normal operation
		
		// #ifdef OS_ANDROID
		// pRenderThread.GetFramebuffer().Activate( NULL );
		// if( framebuffer ){
		// 	delete framebuffer;
		// }
		// #endif
		
		throw;
	}
}

bool deoglCapabilities::Verify() const{
	if( pStd430.Broken() ){
		pRenderThread.GetLogger().LogError( "Std430 Layout Not Supported" );
		return false;
	}
	
	if(pMaxComputeWorkGroupInvocations < 256){
		pRenderThread.GetLogger().LogError("Max compute work group invocations too low");
		return false;
	}
	
	return true;
}



// Private Functions
//////////////////////

#ifdef OS_ANDROID

#include "../renderthread/deoglRTShader.h"
#include "../shaders/deoglShaderCompiled.h"
#include "../shaders/deoglShaderDefines.h"
#include "../shaders/deoglShaderManager.h"
#include "../shaders/deoglShaderProgram.h"
#include "../shaders/deoglShaderSources.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"

void deoglCapabilities::pAndroidTest( deoglFramebuffer *framebuffer ){
	// testing packing stuff.
	// 
	// in opengl fp16 has these parameters:
	// 1-sign bit, 5-bit exponent, 10-bit mantisse
	// max=6.55x10^4, min=6.10x10^-5, digits precision 3.31
	// 
	// assuming a vec2 how could the bits be split up? one possible solution would be
	// to split the value up into 4 pieces: 2-bit exponent (2 times), 5-bit mantisse (2 times).
	// blending works with any number of additive components. using this setup 14 bits can
	// be used. each vec2 component would compose of one exponent and one mantisse part.
	// so how would this look like?
	// 
	// mantisse part 1 (0->31): value
	// mantisse part 2 (0->31): value * 32.0
	// exponent part 1 (0->3): pow( 2.0, value
	// 
	// strip 2 bits from mantisse to get 7(5+2) and 8(10-2)
	// value 1 maps to (0.25 -> ...) exp(-2) -> exp(
	// 
	// 
	// 
	// bias = pow(2, expbits - 1) - 1
	// (exponent = 0): value = (mantisse / pow(2.0, manbits)) * pow(2.0, bias - 1)
	// (exponent > 0): value = (1.0 + mantisse) * pow(2.0, exponent - bias)
	// (exponent = pow(2, expbits - 1)): inf/nan
	// 
	// bias = pow(2, 5 - 1) - 1 = 15
	// manscalar = pow(2, bias - 1) / pow(2, manbits - 1)
	//           = pow(2, 14) / pow(2, 9)
	//           = pow(2, 14-9) = pow(2, 5) = 32
	// (exponent = 0): value = mantisse * 32
	// (exponent > 0): value = (1 + mantisse) * pow(2, exponent - 15)
	// 
	// v = pow(2,e-15) + m*pow(2,e-15)
	// v = pow(2,e-15) + (m1+m2)*pow(2,e-15)   // m1=floor(m/256)*256, m2=fract(m/256)*256
	// v = pow(2,e-15) + m1*pow(2,e-15) + m2*pow(2,e-15)
	// 
	// 
	// 
	// with 2 bits swifted to the lower value the upper has a range of:
	//   exponent = 4..15 => value = (1+0)*pow(2,4-15) = 0.00048828125
	//   exponent = 4..15 => value = (1+32)*pow(2,4-15) 0.01611328125
	//   exponent = 3 => value = (1+1023)*pow(2,3-15) = 0.25
	//   exponent = 3 => value = (1+31)*pow(2,3-15) = 0.0078125
	//   center value 0.011962890625 ~0.012
	// 
	// 65'500 .. 0.000'061
	// 
	// 
	// there is also somebody on the net which does a strange thing I don't understand
	// https://uncommoncode.wordpress.com/2012/11/07/float-packing-in-shaders-encoding-multiple-components-in-one-float
	// 
	// 
	// 
	// on gamedev net there had been another idea
	// half PackHalf( float A/*one bit*/, float B/*5 bits*/, float C/*10 bits*/ )//all inputs in 0-1 range
	// {
	// float s = A*2-1;// -1/+1   => A ? -1.0 : 1.0   // if A is bool
	// float e = lerp( -13, 14, B );  => mix(-13.0, 14.0, B);
	// float f = lerp( 1024, 2047, C );   => mix(1024.0, 2047.0, C);
	// return s*f*pow(2,e);
	// }//N.B. most likely contains mistakes
	// 
	
	deoglShaderManager &shaderManager = pRenderThread.GetShader().GetShaderManager();
	const deoglPixelBuffer::Ref pixelBuffer( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfFloat4, 1, 1, 1 ) ) );
	deoglPixelBuffer::sFloat4 * const pixels = pixelBuffer->GetPointerFloat4();
	deoglTexture *texture = NULL;
	const deoglShaderSources *sources;
	deoglShaderDefines defines;
	
	try{
		// load shader
		sources = shaderManager.GetSourcesNamed( "Test Integer Packing" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		const deoglShaderProgram * const shader = shaderManager.GetProgramWith( sources, defines );
		
		// create test texture
		texture = new deoglTexture( pRenderThread );
		texture->SetSize( 1, 1 );
		texture->SetFBOFormat( 4, true );
		
		pixels->r = 0.2f;
		pixels->g = 0.0f;
		pixels->b = 0.0f;
		pixels->a = 1.0f;
		texture->SetPixels( pixelBuffer );
		
		// set states
		OGL_CHECK( pRenderThread, glViewport( 0, 0, 1, 1 ) );
		OGL_CHECK( pRenderThread, glScissor( 0, 0, 1, 1 ) );
		OGL_CHECK( pRenderThread, glEnable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( pRenderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( pRenderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( pRenderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( pRenderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( pRenderThread, glDepthMask( GL_FALSE ) );
		
		// prepare framebuffer with texture
		pRenderThread.GetFramebuffer().Activate( framebuffer );
		framebuffer->DetachAllImages();
		framebuffer->AttachColorTexture( 0, texture );
		framebuffer->UpdateReadWriteBuffers();
		framebuffer->Verify();
		
		// render
		pRenderThread.GetShader().ActivateShader( shader );
		
		OGL_CHECK( pRenderThread, pglBindVertexArray( pFSQuadVAO ) );
		OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
		
		// retrieve the results and clean up
		framebuffer->DetachAllImages();
		texture->GetPixels( pixelBuffer );
		
		delete texture;
		texture = NULL;
		
	}catch( const deException &e ){
		if( texture ){
			delete texture;
		}
		pRenderThread.GetLogger().LogException( e );
		return;
	}
	
	// evaluate the results
	pRenderThread.GetLogger().LogInfoFormat( "IntPacking: %g %g %g %g",
		pixels[0].r, pixels[0].g, pixels[0].b, pixels[0].a );
}
#endif
