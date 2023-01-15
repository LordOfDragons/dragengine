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
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../vbo/deoglVBOLayout.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTest::deoglOcclusionTest( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pInputListeners( nullptr ),
pInputData( nullptr ),
pInputDataCount( 0 ),
pInputDataSize( 0 )
{
	try{
		pVBOInput.TakeOver( new deoglSPBlockSSBO( renderThread ) );
		pVBOInput->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		pVBOInput->SetParameterCount( 2 );
		pVBOInput->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 minExtend
		pVBOInput->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 maxExtend
		pVBOInput->MapToStd140();
		pVBOInput->SetBindingPoint( 0 );
		
		pVBOResult.TakeOver( new deoglSPBlockSSBO( renderThread ) );
		pVBOResult->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		pVBOResult->SetParameterCount( 1 );
		pVBOResult->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtBool, 1, 1, 1 ); // bool result
		pVBOResult->MapToStd140();
		pVBOResult->SetBindingPoint( 1 );
		
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
		pResizeInputData( pInputDataSize + 128 );
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
	pVBOInput->SetElementCount( pInputDataCount );
	
	if( pInputDataCount > 0 ){
		deoglSPBlockSSBO &vbo = pVBOInput;
		const deoglSPBMapBuffer mapped( vbo );
		int i;
		
		for( i=0; i<pInputDataCount; i++ ){
			const sInputData &inputData = pInputData[ i ];
			vbo.SetParameterDataVec3( 0, i, inputData.minExtend );
			vbo.SetParameterDataVec3( 1, i, inputData.maxExtend );
		}
	}
	
	pVBOResult->SetElementCount( pInputDataCount );
	pVBOResult->EnsureBuffer();
}

void deoglOcclusionTest::UpdateResults(){
	if( pInputDataCount == 0 ){
		return;
	}
	
	if( pVBOResult ){
		if( pInputDataCount > pVBOResultDataSize ){
			if( pVBOResultData ){
				delete [] pVBOResultData;
				pVBOResultData = nullptr;
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
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer( pVBOResult );
	
	if( pInputListeners ){
		delete [] pInputListeners;
	}
	if( pInputData ){
		delete [] pInputData;
	}
}

void deoglOcclusionTest::pResizeInputData( int size ){
	if( size <= pInputDataSize ){
		return;
	}
	
	sInputData * const newInputData = new sInputData[ size ];
	if( pInputData ){
		memcpy( newInputData, pInputData, sizeof( sInputData ) * pInputDataCount );
		delete [] pInputData;
	}
	pInputDataSize = size;
	pInputData = newInputData;
	
	deoglOcclusionTestListener ** const newListeners = new deoglOcclusionTestListener*[ size ];
	if( pInputListeners ){
		memcpy( newListeners, pInputListeners, sizeof( deoglOcclusionTestListener* ) * pInputDataCount );
		delete [] pInputListeners;
	}
	pInputListeners = newListeners;
}
