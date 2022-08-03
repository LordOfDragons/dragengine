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

#include "deoglOcclusionTest.h"
#include "deoglOcclusionTestListener.h"
#include "../capabilities/deoglCapabilities.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../extensions/deoglExtensions.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../vbo/deoglVBOLayout.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTest::deoglOcclusionTest( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pTextureResult( NULL ),
pFBOResult( NULL ),

pVBO( 0 ),
pVAO( 0 ),
pVBOLayout( NULL ),

pVBOResult( 0 ),
pVBOResultData( NULL ),
pVBOResultDataSize( 0 ),

pInputListeners( NULL ),
pInputData( NULL ),
pInputDataCount( 0 ),
pInputDataSize( 0 ),

pPBResults( NULL ),
pResults( NULL )
{
	try{
		pCreateVBOLayout();
		pResizeResult( decPoint( 128, 32 ) );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglOcclusionTest::~deoglOcclusionTest(){
	pCleanUp();
}



// Management
///////////////

int deoglOcclusionTest::AddInputData( const decVector &minExtend, const decVector &maxExtend,
deoglOcclusionTestListener *listener ){
	if( pInputDataCount == pInputDataSize ){
		decPoint size( pResultSize );
		
		if( size.y == size.x ) {
			size.x <<= 1;
			size.y >>= 1;
		}
		size.y++;
		
		pResizeResult( size );
	}
	
	sInputData &inputData = pInputData[ pInputDataCount ];
	
	inputData.minExtend.x = ( GLfloat )minExtend.x;
	inputData.minExtend.y = ( GLfloat )minExtend.y;
	inputData.minExtend.z = ( GLfloat )minExtend.z;
	inputData.maxExtend.x = ( GLfloat )maxExtend.x;
	inputData.maxExtend.y = ( GLfloat )maxExtend.y;
	inputData.maxExtend.z = ( GLfloat )maxExtend.z;
	
	pInputListeners[ pInputDataCount ] = listener;
	
	return pInputDataCount++;
}

void deoglOcclusionTest::RemoveAllInputData(){
	pInputDataCount = 0;
}



void deoglOcclusionTest::UpdateVBO(){
	const int dataSize = sizeof( sInputData ) * pInputDataCount;
	
	// input vbo
	if( ! pVBO ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	if( pInputDataCount > 0 ){
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		
		//OGL_CHECK( pRenderThread, oglBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, pVBOData ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, NULL, GL_STREAM_DRAW ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, pInputData, GL_STREAM_DRAW ) );
	}
	
	// result vbo if required
	if( pRenderThread.GetConfiguration().GetOcclusionTestMode() == deoglConfiguration::eoctmTransformFeedback ){
		if( ! pVBOResult ){
			OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOResult ) );
			if( ! pVBOResult ){
				DETHROW( deeOutOfMemory );
			}
		}
		
		if( pInputDataCount > 0 ){
			OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOResult ) );
			OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, pInputDataCount * 4, NULL, GL_STREAM_READ ) );
		}
		
	}else{
		if( pVBOResultData ){
			delete [] pVBOResultData;
			pVBOResultData = NULL;
			pVBOResultDataSize = 0;
		}
		pRenderThread.GetDelayedOperations().DeleteOpenGLBuffer( pVBOResult );
		pVBOResult = 0;
	}
	
	// textures and FBO
	pUpdateTexturesFBO();
}

GLuint deoglOcclusionTest::GetVAO(){
	if( pVAO == 0 && pVBO ){
		OGL_CHECK( pRenderThread, pglGenVertexArrays( 1, &pVAO ) );
		if( ! pVAO ){
			DETHROW( deeOutOfMemory );
		}
		
		OGL_CHECK( pRenderThread, pglBindVertexArray( pVAO ) );
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		pVBOLayout->SetVAOAttributeAt( pRenderThread, 0, 0 ); // position(0) => vao(0)
		pVBOLayout->SetVAOAttributeAt( pRenderThread, 1, 1 ); // extends(1) => vao(1)
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
	}
	
	return pVAO;
}

void deoglOcclusionTest::UpdateResults(){
	if( pInputDataCount == 0 ){
		return;
	}
	
	if( pVBOResult ){
		if( pInputDataCount > pVBOResultDataSize ){
			if( pVBOResultData ){
				delete [] pVBOResultData;
				pVBOResultData = NULL;
			}
			
			pVBOResultData = new GLfloat[ pInputDataCount ];
			pVBOResultDataSize = pInputDataCount;
		}
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOResult ) );
		OGL_CHECK( pRenderThread, pglGetBufferSubData( GL_ARRAY_BUFFER, 0, pInputDataCount * 4, pVBOResultData ) );
		
		// evaluate the results
		int i;
		for( i=0; i<pInputDataCount; i++ ){
			if( pVBOResultData[ i ] < 0.5f && pInputListeners[ i ] ){
				pInputListeners[ i ]->OcclusionTestInvisible();
			}
		}
		
	}else if( pTextureResult ){
		pTextureResult->GetPixels( *pPBResults );
		
		// evaluate the results
		int i;
		for( i=0; i<pInputDataCount; i++ ){
			if( pResults[ i ] == 0 && pInputListeners[ i ] ){
				pInputListeners[ i ]->OcclusionTestInvisible();
			}
		}
		
	}else{
		memset( pResults, 255, pInputDataCount );
		// no listener checks since all tests are considered visible
	}
}



bool deoglOcclusionTest::GetResultAt( int index ) const{
	if( index < 0 || index >= pInputDataCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( pVBOResultData ){
		return pVBOResultData[ index ] > 0.5f;
		
	}else{
		return pResults[ index ] != 0;
	}
}



// Private Functions
//////////////////////

void deoglOcclusionTest::pCleanUp(){
	if( pFBOResult ){
		delete pFBOResult;
	}
	
	if( pTextureResult ){
		delete pTextureResult;
	}
	if( pPBResults ){
		delete pPBResults;
	}
	
	if( pVBOResultData ){
		delete [] pVBOResultData;
	}
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer( pVBOResult );
	dops.DeleteOpenGLVertexArray( pVAO );
	dops.DeleteOpenGLBuffer( pVBO );
	
	if( pVBOLayout ){
		delete pVBOLayout;
	}
	
	if( pInputListeners ){
		delete [] pInputListeners;
	}
	if( pInputData ){
		delete [] pInputData;
	}
}

void deoglOcclusionTest::pCreateVBOLayout(){
	// name       | offset | type  | components
	// -----------+--------+-------+------------
	// minExtend  |      0 | float | x, y, z
	// maxExtend  |     12 | float | x, y, z
	pVBOLayout = new deoglVBOLayout;
	
	pVBOLayout->SetAttributeCount( 2 );
	pVBOLayout->SetStride( 24 );
	pVBOLayout->SetSize( pVBOLayout->GetStride() * pInputDataSize );
	pVBOLayout->SetIndexType( deoglVBOLayout::eitNone );
	
	deoglVBOAttribute &attributePosition = pVBOLayout->GetAttributeAt( 0 );
	attributePosition.SetComponentCount( 3 );
	attributePosition.SetDataType( deoglVBOAttribute::edtFloat );
	attributePosition.SetOffset( 0 );
	
	deoglVBOAttribute &attributeExtends = pVBOLayout->GetAttributeAt( 1 );
	attributeExtends.SetComponentCount( 3 );
	attributeExtends.SetDataType( deoglVBOAttribute::edtFloat );
	attributeExtends.SetOffset( 12 );
}

void deoglOcclusionTest::pResizeResult( const decPoint &size ){
	// resize input data array
	sInputData * const newInputData = new sInputData[ size.x * size.y ];
	if( pInputData ){
		memcpy( newInputData, pInputData, sizeof( sInputData ) * pInputDataSize );
		delete [] pInputData;
	}
	pInputDataSize = size.x * size.y;
	pInputData = newInputData;
	
	// resize listener array
	deoglOcclusionTestListener ** const newListeners = new deoglOcclusionTestListener*[ pInputDataSize ];
	if( pInputListeners ){
		memcpy( newListeners, pInputListeners, sizeof( deoglOcclusionTestListener* ) * pInputDataCount );
		delete [] pInputListeners;
	}
	pInputListeners = newListeners;
	
	pResultSize = size;
}

void deoglOcclusionTest::pUpdateTexturesFBO(){
	if( pTextureResult && pTextureResult->GetSize() == pResultSize ){
		return;
	}
	
	// result pixel buffer
	if( pPBResults ){
		delete pPBResults;
		pPBResults = NULL;
		pResults = NULL;
	}
	pPBResults = new deoglPixelBuffer( deoglPixelBuffer::epfByte1, pResultSize.x, pResultSize.y, 1 );
	pResults = ( GLubyte* )pPBResults->GetPointerByte1();
	
	// texture
	if( ! pTextureResult ){
		pTextureResult = new deoglTexture( pRenderThread );
		pTextureResult->SetMipMapped( false );
		pTextureResult->SetFBOFormat( 1, false );
	}
	
	pTextureResult->SetSize( pResultSize );
	pTextureResult->CreateTexture();
	
	// fbo
	const deoglRestoreFramebuffer restoreFbo( pRenderThread );
	
	if( ! pFBOResult ){
		pFBOResult = new deoglFramebuffer( pRenderThread, false );
	}
	
	pRenderThread.GetFramebuffer().Activate( pFBOResult );
	pFBOResult->DetachAllImages();
	
	pFBOResult->AttachColorTexture( 0, pTextureResult );
	
	const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBOResult->Verify();
}
