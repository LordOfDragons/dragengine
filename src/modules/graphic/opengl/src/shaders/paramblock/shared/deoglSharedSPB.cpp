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

#include "deoglSharedSPB.h"
#include "deoglSharedSPBElement.h"
#include "../deoglShaderParameterBlock.h"
#include "../../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedSPB
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedSPB::deoglSharedSPB( const deoglShaderParameterBlock::Ref &parameterBlock ) :
pParameterBlock( parameterBlock ),
pElements( NULL ),
pSize( parameterBlock->GetElementCount() ),
pCount( 0 )
{
	DEASSERT_TRUE( parameterBlock->GetElementCount() >= 1 )
	
	pElements = new deoglSharedSPBElement*[ pSize ];
	memset( pElements, '\0', sizeof( deoglSharedSPB* ) * pSize );
}

deoglSharedSPB::~deoglSharedSPB(){
	if( pElements ){
		delete [] pElements;
	}
}



// Management
///////////////

deoglSharedSPBElement *deoglSharedSPB::GetElementAt( int index ) const{
	return pElements[ index ];
}

deoglSharedSPBElement *deoglSharedSPB::AddElement(){
	if( pCount == pSize ){
		return NULL;
	}
	
	int i;
	for( i=0; i<pSize; i++ ){
		if( pElements[ i ] ){
			continue;
		}
		
		pElements[ i ] = new deoglSharedSPBElement( *this, i );
		pCount++;
		return pElements[ i ];
	}
	
	DETHROW( deeInvalidParam );
}

void deoglSharedSPB::RemoveElement( int index ){
	if( index < 0 || index >= pSize || ! pElements[ index ] ){
		DETHROW( deeInvalidParam );
	}
	
	pElements[ index ] = NULL;
	pCount--;
}
