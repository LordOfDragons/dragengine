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

#include "deoglCapCheckUBOIndirectMatrixAccess.h"
#include "../deoglCapabilities.h"
#include "../deoglCapsTextureFormat.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"

#include <dragengine/common/exceptions.h>



// Class deoglCapCheckUBOIndirectMatrixAccess
///////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapCheckUBOIndirectMatrixAccess::deoglCapCheckUBOIndirectMatrixAccess(
deoglCapabilities &capabilities ) :
pCapabilities( capabilities ),
pWorking( true ){
}



// Management
///////////////

void deoglCapCheckUBOIndirectMatrixAccess::Check( GLuint fbo ){
	// Summary:
	//    The UBO Indirect Matrix Access Bug if a nasty bug present in the shader compiler of broken
	//    graphic drivers. The first contact with this bug has been on the ATI graphic driver for
	//    the linux system ( 12.6_beta_pre897:legacy ). The presence of this bug prevents shaders
	//    from working if they try to access matrices indirectly.
	//    
	// Background:
	//    Uniform Buffer Objects provide a way to store uniform parameters in a GPU memory location
	//    similar to Vertex Buffer Objects. They can then be assigned to a shader directly without
	//    having to set all uniforms manually. One problem with this system is how matrices are
	//    accessed. They can be either used in row-major or colon-major order. OpenGL uses colon-major
	//    as default while the Drag[en]gine uses row-major as default. Row-major is the preferable
	//    way since due to the packing requirements of Std140 row-major matrices waste less memory.
	//    For this parameter blocks can be qualified with row-major or colon-major determining how
	//    matrices are laid out in the memory.
	//    
	// Bug Behavior:
	//    In a bugged driver this behavior is broken in certain situations. In this particular bug
	//    matrices are properly accessed directly but if accessed indirectly they are incorrectly
	//    read as colon-major causing shaders to fail. A direct access scenario looks like this:
	//    
	//       uniform mat4x3 testMatrix;
	//       myVectorTransformed = testMatrix * myVector
	//    
	//    This situation works in a broken driver. The following is an indirect access scenario:
	//    
	//       uniform mat4x3 testMatrix;
	//       mat4x3 modifiedMatrix = testMatrix; // modified for example with matrixA * matrixB
	//       myVectorTransformed = testMatrix * myVector
	//    
	//    In this situation the bugged driver reads the matrix data from the UBO not as row-major
	//    as it should but reads it as colon-major. The bugged driver thus ignores the order
	//    qualifier of the matrix for the case of indirect access whereas it honored the qualifier
	//    for the case of direct access. This results in the problem that data from the next
	//    matrix is incorrectly read which in the best case reads a broken matrix and in the worst
	//    case causes a crash since the shaders reads outside the buffer data. This graphic shows
	//    the problem in more detail. aXX is the first matrix (the one with the problem) and b**
	//    the second matrix. *** are undefined values due to the second matrix being a mat3x3:
	//    
	//       [ a11 ][ a12 ][ a13 ][ a14 ]
	//       [ a21 ][ a22 ][ a23 ][ a24 ]
	//       [ a31 ][ a32 ][ a33 ][ a34 ]
	//       [ b11 ][ b12 ][ b13 ][ *** ]
	//       [ b21 ][ b22 ][ b23 ][ *** ]
	//       [ b31 ][ b32 ][ b33 ][ *** ]
	//    
	//    In the direct case the matrix is correctly read as row-major and thus like this:
	//    
	//       [ a11 ][ a12 ][ a13 ][ a14 ]
	//       [ a21 ][ a22 ][ a23 ][ a24 ]
	//       [ a31 ][ a32 ][ a33 ][ a34 ]
	//    
	//    In the indirect case the matrix is incorrectly read as colon-major and thus like this:
	//    
	//       [ a11 ][ a21 ][ a31 ][ b11 ]
	//       [ a12 ][ a22 ][ a32 ][ b12 ]
	//       [ a13 ][ a23 ][ a33 ][ b13 ]
	//    
	// Test Procedure:
	//    A shader is tested where a matrix is accessed indirectly. The matrixes in the buffer
	//    are constructed in a way that all values are 0 except in the translation part where one
	//    value of 1 is placed. If the matrix is read in row-major the value of 1 is located
	//    in the Z coordinate. If the matrix is read in colon-major the value of 1 is located
	//    in the X coordinate. The bug is detected if the result of the test returns a value
	//    of 255 in the red color component (X coordinate) instead of the blue one (Z coordinate).
	
	if( ! pglUniformBlockBinding ){
		return;
	}
	
	deoglRenderThread &renderThread = pCapabilities.GetRenderThread();
	
	#ifdef ANDROID
		// a strange problem on ES 3.0 type hardware. some compilers fail linking with internal
		// compiler error like:
		//   Fragment shader compilation failed.
		//   Internal compiler error: Invalid source symbol
		// 
		// there is no way to check for this compiler bug. only remedy is to make this check
		// fail. most probably a bug with row_major/coumn_major handling
		if( renderThread.GetExtensions().GetGLESVersion() == deoglExtensions::evgles3p0 ){
			pWorking = false;
			renderThread.GetLogger().LogWarn( "Capabilities: UBO Indirect Matrix Access: "
				"Driver Bug (Android, ES 3.0 detected)!" );
			return;
		}
	#endif
	
	const deoglCapsTextureFormat &texformat = *pCapabilities.GetFormats()
		.GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfRGBA8 );
	deoglShaderManager &shaderManager = renderThread.GetShader().GetShaderManager();
	deoglSPBlockUBO *spb = NULL;
	const deoglShaderSources *sources;
	deoglShaderDefines defines;
	decMatrix matrix1, matrix2;
	GLubyte result[ 3 ];
	GLuint texture = 0;
	
	try{
		// load shader
		sources = shaderManager.GetSourcesNamed( "Test UBO Indirect Matrix Access Bug" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		const deoglShaderProgram * const shader = shaderManager.GetProgramWith( sources, defines );
		
		// generate shader parameter block
		spb = new deoglSPBlockUBO( renderThread );
		spb->SetParameterCount( 2 );
		spb->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3
		spb->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 3, 3, 1 ); // mat3
		spb->MapToStd140();
		spb->SetBindingPoint( 0 );
		
		matrix1.a11 = 0.0f; matrix1.a12 = 0.0f; matrix1.a13 = 0.0f; matrix1.a14 = 0.0f;
		matrix1.a21 = 0.0f; matrix1.a22 = 0.0f; matrix1.a23 = 0.0f; matrix1.a24 = 0.0f;
		matrix1.a31 = 0.0f; matrix1.a32 = 0.0f; matrix1.a33 = 0.0f; matrix1.a34 = 1.0f;
		matrix1.a41 = 0.0f; matrix1.a42 = 0.0f; matrix1.a43 = 0.0f; matrix1.a44 = 0.0f;
		
		matrix2.a11 = 1.0f; matrix2.a12 = 0.0f; matrix2.a13 = 0.0f; matrix2.a14 = 0.0f;
		matrix2.a21 = 0.0f; matrix2.a22 = 0.0f; matrix2.a23 = 0.0f; matrix2.a24 = 0.0f;
		matrix2.a31 = 0.0f; matrix2.a32 = 0.0f; matrix2.a33 = 0.0f; matrix2.a34 = 0.0f;
		matrix2.a41 = 0.0f; matrix2.a42 = 0.0f; matrix2.a43 = 0.0f; matrix2.a44 = 0.0f;
		
		spb->MapBuffer();
		try{
			spb->SetParameterDataMat4x3( 0, matrix1 );
			spb->SetParameterDataMat3x3( 1, matrix2 );
			
		}catch( const deException & ){
			spb->UnmapBuffer();
			throw;
		}
		spb->UnmapBuffer();
		
		// generate test texture
		OGL_CHECK( renderThread, glGenTextures( 1, &texture ) );
		if( ! texture ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE ) );
		OGL_CHECK( renderThread, glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE ) );
		
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 1 ) );
		OGL_CHECK( renderThread, glTexImage2D( GL_TEXTURE_2D, 0, texformat.GetFormat(),
			1, 1, 0, texformat.GetPixelFormat(), texformat.GetPixelType(), NULL ) );
		OGL_CHECK( renderThread, glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ) );
		
		// bind test texture to fbo and set render parameters
		OGL_CHECK( renderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0 ) );
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( renderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		OGL_CHECK( renderThread, glViewport( 0, 0, 1, 1 ) );
		OGL_CHECK( renderThread, glScissor( 0, 0, 1, 1 ) );
		OGL_CHECK( renderThread, glEnable( GL_SCISSOR_TEST ) );
		
		OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
		OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
		OGL_CHECK( renderThread, glDisable( GL_STENCIL_TEST ) );
		OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
		
		// set shader and parameters
		renderThread.GetShader().ActivateShader( shader );
		spb->Activate();
		
		// render and unbind from fbo
		OGL_CHECK( renderThread, pglBindVertexArray( pCapabilities.GetFSQuadVAO()  ) );
		OGL_CHECK( renderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		
		#ifdef ANDROID
			OGL_CHECK( renderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
			OGL_CHECK( renderThread, glReadPixels( 0, 0, 1, 1, texformat.GetPixelFormat(),
				texformat.GetPixelType(), ( GLvoid* )&result[ 0 ] ) );
			OGL_CHECK( renderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
			
			OGL_CHECK( renderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 ) );
			
		#else
			OGL_CHECK( renderThread, pglFramebufferTexture2D( GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 ) );
			
			// retrieve the results and clean up
			OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, texture ) );
			OGL_CHECK( renderThread, glPixelStorei( GL_PACK_ALIGNMENT, 1 ) );
			OGL_CHECK( renderThread, glGetTexImage( GL_TEXTURE_2D, 0, GL_RGB,
				GL_UNSIGNED_BYTE, ( GLvoid* )&result[ 0 ] ) );
			OGL_CHECK( renderThread, glPixelStorei( GL_PACK_ALIGNMENT, 4 ) );
			OGL_CHECK( renderThread, glBindTexture( GL_TEXTURE_2D, 0 ) );
		#endif
		
		OGL_CHECK( renderThread, glDeleteTextures( 1, &texture ) );
		texture = 0;
		
		spb->FreeReference();
		spb = NULL;
		
	}catch( const deException & ){
		if( texture ){
			glDeleteTextures( 1, &texture );
		}
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	// evaluate the results. ATI and nVidia return slightly different results for this test.
	// ATI returns 128 for a value of 0.5 but nVidia returns 127. the tesult is tested with
	// a little error margin to catch this.
	pWorking = 
		   result[ 0 ] >= 127 && result[ 0 ] <= 128
		&& result[ 1 ] >= 127 && result[ 1 ] <= 128
		&& result[ 2 ] >= 254;
// 	renderThread.GetLogger().LogInfoFormat( "%i %i %i => %i",
// 		result[ 0 ], result[ 1 ], result[ 2 ], pWorking );
	
	if( pWorking ){
		renderThread.GetLogger().LogInfo(
			"Capabilities: UBO Indirect Matrix Access: Working" );
		
	}else{
		renderThread.GetLogger().LogWarn(
			"Capabilities: UBO Indirect Matrix Access: Driver Bug!" );
	}
}
