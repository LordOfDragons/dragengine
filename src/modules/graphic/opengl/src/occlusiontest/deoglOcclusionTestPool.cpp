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

#include "deoglOcclusionTest.h"
#include "deoglOcclusionTestPool.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTestPool
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTestPool::deoglOcclusionTestPool( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
}

deoglOcclusionTestPool::~deoglOcclusionTestPool(){
	int count = pOcclusionTests.GetCount();
	while( count > 0 ){
		delete ( deoglOcclusionTest* )pOcclusionTests.GetAt( --count );
	}
}



// Management
///////////////

deoglOcclusionTest *deoglOcclusionTestPool::Get(){
	const int index = pOcclusionTests.GetCount() - 1;
	if( index < 0 ){
		return new deoglOcclusionTest( pRenderThread );
	}
	
	deoglOcclusionTest * const occlusionTest = ( deoglOcclusionTest* )pOcclusionTests.GetAt( index );
	pOcclusionTests.RemoveFrom( index );
	return occlusionTest;
}

void deoglOcclusionTestPool::Return( deoglOcclusionTest *occlusionTest ){
	if( ! occlusionTest ){
		DETHROW( deeInvalidParam );
	}
	
	occlusionTest->RemoveAllInputData();
	
	pOcclusionTests.Add( occlusionTest );
}
