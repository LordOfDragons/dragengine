/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglSPBMapBufferRead.h"
#include "deoglSPBlockSSBO.h"


// Class deoglSPBMapBufferRead
////////////////////////////////

// Constructor, destructor
////////////////////////////


deoglSPBMapBufferRead::deoglSPBMapBufferRead( deoglSPBlockSSBO &block ) :
pBlock( block ),
pElement( 0 ),
pCount( block.GetElementCount() ),
pMapped( false )
{
	Map();
}

deoglSPBMapBufferRead::deoglSPBMapBufferRead( deoglSPBlockSSBO &block, int element ) :
pBlock( block ),
pElement( element ),
pCount( 1 ),
pMapped( false )
{
	Map();
}

deoglSPBMapBufferRead::deoglSPBMapBufferRead( deoglSPBlockSSBO &block, int element, int count ) :
pBlock( block ),
pElement( element ),
pCount( count ),
pMapped( false )
{
	Map();
}

deoglSPBMapBufferRead::~deoglSPBMapBufferRead(){
	Unmap();
}



// Management
///////////////

void deoglSPBMapBufferRead::Map(){
	if( pMapped ){
		return;
	}
	
	pBlock.MapBufferRead( pElement, pCount );
	pMapped = true;
}

void deoglSPBMapBufferRead::Unmap(){
	if( ! pMapped ){
		return;
	}
	
	pBlock.UnmapBufferRead();
	pMapped = false;
}
