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
pValid( false ),
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
pValid( false ),
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
	SetValid( false );
}

void deoglDynamicTBOBlock::SetEmpty( bool empty ){
	pEmpty = empty;
	pData = NULL;
	pData2 = NULL;
}

void deoglDynamicTBOBlock::SetValid( bool valid ){
	pValid = valid;
	
	if( ! valid && pSharedTBO ){
		pSharedTBO->MarkDirty();
	}
}

void deoglDynamicTBOBlock::Drop(){
	if( pSharedTBO && ! pEmpty ){
		pSharedTBO->RemoveBlock( this );
	}
}
