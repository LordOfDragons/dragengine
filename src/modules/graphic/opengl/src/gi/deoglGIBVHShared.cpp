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

#include "deoglGIBVHShared.h"
#include "../capabilities/deoglCapabilities.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOUInt16.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOShared.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIBVHShared
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIBVHShared::deoglGIBVHShared( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pTBONodeBox( NULL ),
pTBOIndex( NULL ),
pTBOFace( NULL ),
pTBOVertex( NULL ),
pTBOTexCoord( NULL ),
pTBOMaterial( NULL ),
pTBOMaterial2( NULL )
{
	try{
		pTBONodeBox = new deoglDynamicTBOFloat32( renderThread, 4 );
		pTBOIndex = new deoglDynamicTBOUInt16( renderThread, 2 );
		pTBOFace = new deoglDynamicTBOUInt16( renderThread, 4 );
		pTBOVertex = new deoglDynamicTBOFloat32( renderThread, 4 );
		pTBOTexCoord = new deoglDynamicTBOFloat16( renderThread, 2 );
		pTBOMaterial = new deoglDynamicTBOUInt32( renderThread, 4 );
		pTBOMaterial2 = new deoglDynamicTBOFloat16( renderThread, 4 );
		
		pSharedTBONode = new deoglDynamicTBOShared( pTBOIndex, 1, pTBONodeBox, 2 );
		pSharedTBOFace = new deoglDynamicTBOShared( pTBOFace, 1, pTBOTexCoord, 3 );
		pSharedTBOVertex = new deoglDynamicTBOShared( pTBOVertex, 1 );
		pSharedTBOMaterial = new deoglDynamicTBOShared( pTBOMaterial, 1, pTBOMaterial2, 3 );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIBVHShared::~deoglGIBVHShared(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglGIBVHShared::pCleanUp(){
	if( pSharedTBONode ){
		pSharedTBONode->FreeReference();
	}
	if( pSharedTBOFace ){
		pSharedTBOFace->FreeReference();
	}
	if( pSharedTBOVertex ){
		pSharedTBOVertex->FreeReference();
	}
	if( pSharedTBOMaterial ){
		pSharedTBOMaterial->FreeReference();
	}
	
	if( pTBONodeBox ){
		pTBONodeBox->FreeReference();
	}
	if( pTBOIndex ){
		pTBOIndex->FreeReference();
	}
	if( pTBOFace ){
		pTBOFace->FreeReference();
	}
	if( pTBOVertex ){
		pTBOVertex->FreeReference();
	}
	if( pTBOTexCoord ){
		pTBOTexCoord->FreeReference();
	}
	if( pTBOMaterial ){
		pTBOMaterial->FreeReference();
	}
	if( pTBOMaterial2 ){
		pTBOMaterial2->FreeReference();
	}
}
