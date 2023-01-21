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

#include "deoglSharedVBO.h"
#include "deoglSharedVBOList.h"
#include "deoglSharedVBOBlock.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedVBOBlock
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedVBOBlock::deoglSharedVBOBlock( deoglSharedVBO *vbo, int offset, int size,
int indexOffset, int indexCount ) :
pVBO( vbo ),
pOffset( offset ),
pSize( size ),
pData( NULL ),
pIndexOffset( indexOffset ),
pIndexCount( indexCount ),
pIndexData( NULL ),
pValid(  false ),
pEmpty( true )
{
	if( ! vbo || offset < 0 || size < 0 || indexOffset < 0 || indexCount < 0 ){
		DETHROW( deeInvalidParam );
	}
}

deoglSharedVBOBlock::~deoglSharedVBOBlock(){
	if( pIndexData ){
		delete [] pIndexData;
	}
	if( pData ){
		delete [] pData;
	}
}



// Management
///////////////

void deoglSharedVBOBlock::DropVBO(){
	pVBO = NULL;
}

void deoglSharedVBOBlock::DelayedRemove(){
	class cDelayedRemove : public deObject{
	private:
		deoglSharedVBOBlock * const pVBOBlock;
		
	public:
		cDelayedRemove( deoglSharedVBOBlock *vboBlock ) : pVBOBlock( vboBlock ){
			vboBlock->AddReference();
		}
		
	protected:
		~cDelayedRemove(){
			pVBOBlock->GetVBO()->RemoveBlock( pVBOBlock );
			pVBOBlock->FreeReference();
		}
	};
	
	pVBO->GetParentList()->GetRenderThread().GetDelayedOperations().AddReleaseObject(
		deObject::Ref::New( new cDelayedRemove( this ) ) );
}

void deoglSharedVBOBlock::SetSize( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size != pSize ){
		pSize = size;
		pReallocData();
	}
}

void deoglSharedVBOBlock::SetIndexCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pIndexCount ){
		pIndexCount = count;
		pReallocData();
	}
}

void deoglSharedVBOBlock::SetEmpty( bool empty ){
	if( empty != pEmpty ){
		pEmpty = empty;
		pReallocData();
	}
}

void deoglSharedVBOBlock::SetValid( bool valid ){
	pValid = valid;
	
	if( ! valid && pVBO ){
		pVBO->MarkDirty();
	}
}

void deoglSharedVBOBlock::Prepare(){
	if( ! pVBO ){
		DETHROW( deeInvalidParam );
	}
	
	pVBO->Prepare();
}



// Private Functions
//////////////////////

void deoglSharedVBOBlock::pReallocData(){
	if( ! pVBO ){
		DETHROW( deeInvalidParam );
	}
	
	if( pEmpty ){
		if( pIndexData ){
			delete [] pIndexData;
			pIndexData = NULL;
		}
		if( pData ){
			delete [] pData;
			pData = NULL;
		}
		
	}else{
		if( pSize > 0 && ! pData ){
			pData = new unsigned char[ pVBO->GetParentList()->GetLayout().GetStride() * pSize ];
		}
		if( pIndexCount > 0 && ! pIndexData ){
			const int indexSize = pVBO->GetParentList()->GetLayout().GetIndexSize();
			
			if( indexSize > 0 ){
				pIndexData = new unsigned char[ indexSize * pIndexCount ];
			}
		}
	}
	
	pValid = false;
}
