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

pSubInstanceCount( 0 ),
pSIIndexInstanceSPB( NULL ),
pSIIndexInstanceFirst( 0 ),

pNextInstance( NULL ),

pLLNext( NULL ){
}

deoglRenderTaskInstance::~deoglRenderTaskInstance(){
}



// Management
///////////////

void deoglRenderTaskInstance::SetInstance( const deoglRenderTaskSharedInstance *instance ){
	pInstance = instance;
}



void deoglRenderTaskInstance::SetSubInstanceCount( int subInstanceCount ){
	pSubInstanceCount = subInstanceCount;
}

void deoglRenderTaskInstance::AddSubInstance( int indexInstance, int flags ){
	pSIIndexInstance.Add( indexInstance );
	pSIFlags.Add( flags );
	pSubInstanceCount++;
}

void deoglRenderTaskInstance::SetSIIndexInstanceParam( deoglShaderParameterBlock *paramBlock,
int firstIndex ){
	pSIIndexInstanceSPB = paramBlock;
	pSIIndexInstanceFirst = firstIndex;
}

void deoglRenderTaskInstance::WriteSIIndexInstanceInt( bool useFlags ){
	if( ! pSIIndexInstanceSPB ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pSIIndexInstance.GetCount();
	int i;
	
	if( useFlags ){
		struct sIndexFlags{
			GLuint index;
			GLuint flags;
		};
		sIndexFlags * const data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLuint )pSIIndexInstance.GetAt( i );
			data[ i ].flags = ( GLuint )pSIFlags.GetAt( i );
		}
		
	}else{
		struct sIndex{
			GLuint index;
		};
		sIndex * const data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLuint )pSIIndexInstance.GetAt( i );
		}
	}
}

void deoglRenderTaskInstance::WriteSIIndexInstanceShort( bool useFlags ){
	const int count = pSIIndexInstance.GetCount();
	int i;
	
	if( useFlags ){
		struct sIndexFlags{
			GLushort index;
			GLushort flags;
		};
		sIndexFlags * const data = ( sIndexFlags* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLushort )pSIIndexInstance.GetAt( i );
			data[ i ].flags = ( GLushort )pSIFlags.GetAt( i );
		}
		
	}else{
		struct sIndex{
			GLuint index;
		};
		sIndex * const data = ( sIndex* )pSIIndexInstanceSPB->GetMappedBuffer()
			+ pSIIndexInstanceFirst;
		for( i=0; i<count; i++ ){
			data[ i ].index = ( GLushort )pSIIndexInstance.GetAt( i );
		}
	}
}



void deoglRenderTaskInstance::Clear(){
	pInstance = NULL;
	pSubInstanceCount = 0;
	pSIIndexInstance.RemoveAll();
	pSIFlags.RemoveAll();
	pSIIndexInstanceSPB = NULL;
	pSIIndexInstanceFirst = 0;
}

void deoglRenderTaskInstance::SetNextInstance( deoglRenderTaskInstance *instance ){
	pNextInstance = instance;
}



// Linked List
////////////////

void deoglRenderTaskInstance::SetLLNext( deoglRenderTaskInstance *instance ){
	pLLNext = instance;
}
