/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglDynamicTBO.h"
#include "deoglDynamicTBOBlock.h"
#include "deoglDynamicTBOShared.h"

#include <dragengine/common/exceptions.h>



// Class deoglDynamicTBOBlock
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDynamicTBOBlock::deoglDynamicTBOBlock( deoglDynamicTBOShared *tbo ) :
pSharedTBO( tbo ),
pOffset( 0 ),
pSize( 0 ),
pEmpty( true )
{
	if( ! tbo ){
		DETHROW( deeInvalidParam );
	}
}

deoglDynamicTBOBlock::deoglDynamicTBOBlock( deoglDynamicTBOShared *tbo, int offset, int size ) :
pSharedTBO( tbo ),
pOffset( offset ),
pSize( size ),
pEmpty( true )
{
	if( ! tbo || offset < 0 || size < 0 ){
		DETHROW( deeInvalidParam );
	}
}

deoglDynamicTBOBlock::~deoglDynamicTBOBlock(){
}



// Management
///////////////

void deoglDynamicTBOBlock::DropSharedTBO(){
	pSharedTBO = NULL;
}

void deoglDynamicTBOBlock::SetOffset( int offset ){
	if( offset < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( offset == pOffset ){
		return;
	}
	
	pOffset = offset;
}

void deoglDynamicTBOBlock::SetSize( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( size == pSize ){
		return;
	}
	
	pSize = size;
	pData = NULL;
	pData2 = NULL;
}

void deoglDynamicTBOBlock::SetData( deoglDynamicTBO *tbo, deoglDynamicTBO *tbo2 ){
	pData = tbo;
	pData2 = tbo2;
}

void deoglDynamicTBOBlock::SetEmpty( bool empty ){
	pEmpty = empty;
	pData = NULL;
	pData2 = NULL;
}

void deoglDynamicTBOBlock::Drop(){
	if( pSharedTBO && ! pEmpty ){
		pSharedTBO->RemoveBlock( this );
	}
}

void deoglDynamicTBOBlock::WriteToTBO(){
	if( pEmpty || ! pSharedTBO ){
		return;
	}
	
	// write data to first TBO
	if( pData ){
		deoglDynamicTBO &tbo = *pSharedTBO->GetTBO();
		const int stride = tbo.GetComponentCount() * tbo.GetDataTypeSize() * pSharedTBO->GetStride();
		uint8_t * const tboData = tbo.GetData();
		
		//const int offset = pOffset * componentCount;
		//pTBO->SetTBO( offset, *pData );
		//pTBO->Update( offset, pSize );
		const uint8_t * const sourceData = ( ( deoglDynamicTBO* )( deObject* )pData )->GetData();
		if( sourceData ){
			memcpy( tboData + pOffset * stride, sourceData, pSize * stride );
		}
	}
	
	// write data to second TBO if present
	if( pData2 ){
		deoglDynamicTBO * const tbo2 = pSharedTBO->GetTBO2();
		if( tbo2 ){
			const int stride2 = tbo2->GetComponentCount() * tbo2->GetDataTypeSize() * pSharedTBO->GetStride2();
			uint8_t * const tboData2 = tbo2->GetData();
			
			const uint8_t * const sourceData = ( ( deoglDynamicTBO* )( deObject* )pData2 )->GetData();
			if( sourceData ){
				memcpy( tboData2 + pOffset * stride2, sourceData, pSize * stride2 );
			}
		}
	}
	
	// mark shared TBO dirty
	pSharedTBO->MarkDirty();
}
