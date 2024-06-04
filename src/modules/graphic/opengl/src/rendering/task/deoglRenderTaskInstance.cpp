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

#include "deoglRenderTaskInstance.h"
#include "deoglComputeRenderTask.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../../shaders/paramblock/deoglSPBlockMemory.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskInstance
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskInstance::deoglRenderTaskInstance() :
pInstance( NULL ),

pSubInstances( NULL ),
pSubInstanceCount( 0 ),
pSubInstanceSize( 0 ),

pSIIndexInstanceSPB( NULL ),
pSIIndexInstanceFirst( 0 ),
pDrawIndirectIndex( 0 ),
pDrawIndirectCount( 0 ){
}

deoglRenderTaskInstance::~deoglRenderTaskInstance(){
	if( pSubInstances ){
		delete [] pSubInstances;
	}
}



// Management
///////////////

void deoglRenderTaskInstance::SetInstance( const deoglRenderTaskSharedInstance *instance ){
	pInstance = instance;
}



const deoglRenderTaskInstance::sSubInstance & deoglRenderTaskInstance::GetSubInstanceAt( int index ) const{
	if( index < 0 || index >= pSubInstanceCount ){
		DETHROW( deeInvalidParam );
	}
	return pSubInstances[ index ];
}

void deoglRenderTaskInstance::AddSubInstance( int indexInstance, int flags ){
	if( pSubInstanceCount == pSubInstanceSize ){
		const int newSize = pSubInstanceCount * 3 / 2 + 1;
		sSubInstance * const newArray = new sSubInstance[ newSize ];
		if( pSubInstances ){
			memcpy( newArray, pSubInstances, sizeof( sSubInstance ) * pSubInstanceCount );
			delete [] pSubInstances;
		}
		pSubInstances = newArray;
		pSubInstanceSize = newSize;
	}
	
	sSubInstance &subInstance = pSubInstances[ pSubInstanceCount++ ];
	subInstance.instance = indexInstance;
	subInstance.flags = flags;
}

void deoglRenderTaskInstance::SetSIIndexInstanceParam( deoglShaderParameterBlock *paramBlock,
int firstIndex ){
	pSIIndexInstanceSPB = paramBlock;
	pSIIndexInstanceFirst = firstIndex;
}

void deoglRenderTaskInstance::WriteSIIndexInstanceInt( bool useFlags ){
	DEASSERT_NOTNULL( pSIIndexInstanceSPB )
	int i;
	
	if( useFlags ){
		struct sIndexFlags{
			GLuint index;
			GLuint flags;
		};
		sIndexFlags * const data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		for( i=0; i<pSubInstanceCount; i++ ){
			const sSubInstance &subInstance = pSubInstances[ i ];
			data[ i ].index = ( GLuint )subInstance.instance;
			data[ i ].flags = ( GLuint )subInstance.flags;
		}
		
	}else{
		struct sIndex{
			GLuint index;
		};
		sIndex * const data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
		for( i=0; i<pSubInstanceCount; i++ ){
			data[ i ].index = ( GLuint )pSubInstances[ i ].instance;
		}
	}
}

void deoglRenderTaskInstance::WriteSIIndexInstanceCompute(){
	DEASSERT_NOTNULL( pSIIndexInstanceSPB )
	
	deoglComputeRenderTask::sStep * const data = ( deoglComputeRenderTask::sStep* )
		pSIIndexInstanceSPB->GetMappedBuffer() + pSIIndexInstanceFirst;
	
	int i;
	for( i=0; i<pSubInstanceCount; i++ ){
		const sSubInstance &subInstance = pSubInstances[ i ];
		data[ i ].spbInstance = ( uint32_t )( subInstance.instance + 1 );
		data[ i ].specialFlags = ( uint32_t )subInstance.flags;
	}
}

void deoglRenderTaskInstance::SetDrawIndirectIndex( int index ){
	pDrawIndirectIndex = index;
}

void deoglRenderTaskInstance::SetDrawIndirectCount( int count ){
	pDrawIndirectCount = count;
}



void deoglRenderTaskInstance::Reset(){
	pInstance = NULL;
	pSubInstanceCount = 0;
	pSIIndexInstanceSPB = NULL;
	pSIIndexInstanceFirst = 0;
	pDrawIndirectIndex = 0;
	pDrawIndirectCount = 0;
}
