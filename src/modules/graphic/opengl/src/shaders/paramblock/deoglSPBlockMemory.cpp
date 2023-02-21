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

#include "deoglSPBParameter.h"
#include "deoglSPBlockMemory.h"
#include "../deoglShaderCompiled.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglSPBlockMemory
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSPBlockMemory::deoglSPBlockMemory( deoglRenderThread &renderThread ) :
deoglShaderParameterBlock( renderThread ),
pBuffer( NULL ),
pBufferCapacity( 0 ){
}

deoglSPBlockMemory::deoglSPBlockMemory( const deoglSPBlockMemory &paramBlock ) :
deoglShaderParameterBlock( paramBlock ),
pBuffer( NULL ),
pBufferCapacity( 0 ){
}

deoglSPBlockMemory::~deoglSPBlockMemory(){
	if( IsBufferMapped() ){
		pClearMapped();
	}
	if( pBuffer ){
		delete [] pBuffer;
	}
}



// Management
///////////////

void deoglSPBlockMemory::Activate() const{
	DETHROW( deeInvalidParam );
}

void deoglSPBlockMemory::Activate( int ) const{
	DETHROW( deeInvalidParam );
}

void deoglSPBlockMemory::Deactivate() const{
	DETHROW( deeInvalidParam );
}

void deoglSPBlockMemory::Deactivate( int ) const{
	DETHROW( deeInvalidParam );
}

void deoglSPBlockMemory::MapBuffer(){
	if( IsBufferMapped() || GetBufferSize() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pGrowBuffer();
	pSetMapped( pBuffer );
}

void deoglSPBlockMemory::MapBuffer( int element ){
	MapBuffer( element, 1 );
}

void deoglSPBlockMemory::MapBuffer ( int element, int count ){
	if( IsBufferMapped() || GetBufferSize() == 0 || element < 0 || count < 1
	|| element + count > GetElementCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pGrowBuffer();
	pSetMapped( pBuffer, element, count );
}

void deoglSPBlockMemory::UnmapBuffer(){
	if( ! IsBufferMapped() ){
		DETHROW( deeInvalidParam );
	}
	
	pClearMapped();
}

deoglShaderParameterBlock *deoglSPBlockMemory::Copy() const{
	return new deoglSPBlockMemory( *this );
}



// Private Functions
//////////////////////

void deoglSPBlockMemory::pGrowBuffer(){
	const int bufferSize = GetBufferSize();
	
	if( pBuffer && bufferSize > pBufferCapacity ){
		char * const newBuffer = new char[ bufferSize ];
		if( pBufferCapacity > 0 ){
			memcpy( newBuffer, pBuffer, pBufferCapacity );
		}
		delete [] pBuffer;
		pBuffer = newBuffer;
		pBufferCapacity = bufferSize;
	}
	
	if( ! pBuffer ){
		pBuffer = new char[ bufferSize ];
		pBufferCapacity = bufferSize;
	}
}
