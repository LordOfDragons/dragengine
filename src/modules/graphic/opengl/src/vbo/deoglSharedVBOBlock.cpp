/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
