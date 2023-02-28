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
		data[ i ].spbInstance = ( uint32_t )subInstance.instance;
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
