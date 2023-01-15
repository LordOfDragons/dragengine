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
#include "../extensions/deoglExtensions.h"
#include "../renderthread/deoglRenderThread.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"

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
		pSSBOInput.TakeOver( new deoglSPBlockSSBO( renderThread ) );
		pSSBOInput->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		pSSBOInput->SetParameterCount( 2 );
		pSSBOInput->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 minExtend
		pSSBOInput->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3 maxExtend
		pSSBOInput->MapToStd140();
		pSSBOInput->SetBindingPoint( 0 );
		
		pSSBOResult.TakeOver( new deoglSPBlockSSBO( renderThread ) );
		pSSBOResult->SetRowMajor( renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		pSSBOResult->SetParameterCount( 1 );
		pSSBOResult->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtBool, 4, 1, 1 ); // bvec4 result
		pSSBOResult->MapToStd140();
		pSSBOResult->SetBindingPoint( 1 );
		
		pResizeInputData( 256 );
		
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



void deoglOcclusionTest::UpdateSSBO(){
	pSSBOInput->SetElementCount( pInputDataCount );
	
	if( pInputDataCount > 0 ){
		deoglSPBlockSSBO &vbo = pSSBOInput;
		const deoglSPBMapBuffer mapped( vbo );
		int i;
		
		for( i=0; i<pInputDataCount; i++ ){
			const sInputData &inputData = pInputData[ i ];
			vbo.SetParameterDataVec3( 0, i, inputData.minExtend );
			vbo.SetParameterDataVec3( 1, i, inputData.maxExtend );
		}
	}
	
	pSSBOResult->SetElementCount( ( pInputDataCount - 1 ) / 4 + 1 );
	pSSBOResult->EnsureBuffer();
}

void deoglOcclusionTest::UpdateResults(){
	if( pInputDataCount == 0 || ! pSSBOResult ){
		return;
	}
	
	const uint32_t * const result = ( const uint32_t * ) pSSBOResult->ReadBuffer();
	int i;
	
	for( i=0; i<pInputDataCount; i++ ){
		if( ! result[ i ] && pInputListeners[ i ] ){
			pInputListeners[ i ]->OcclusionTestInvisible();
		}
	}
}



// Private Functions
//////////////////////

void deoglOcclusionTest::pCleanUp(){
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
