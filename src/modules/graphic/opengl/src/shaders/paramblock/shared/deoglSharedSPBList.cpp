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

#include "deoglSharedSPB.h"
#include "deoglSharedSPBList.h"
#include "deoglSharedSPBElement.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>



// Class deoglSharedSPBList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedSPBList::deoglSharedSPBList( deoglRenderThread &renderThread, deoglShaderParameterBlock *layout ) :
pRenderThread( renderThread ),
pLayout( layout )
{
	DEASSERT_NOTNULL( layout )
	DEASSERT_TRUE( layout->GetElementCount() >= 1 )
	
	pSize = layout->GetElementCount();
}

deoglSharedSPBList::~deoglSharedSPBList(){
	pSPBs.RemoveAll();
}



// Management
///////////////

int deoglSharedSPBList::GetSize() const{
	return pLayout->GetElementCount();
}

int deoglSharedSPBList::GetCount() const{
	return pSPBs.GetCount();
}

deoglSharedSPB *deoglSharedSPBList::GetAt( int index ) const{
	return ( deoglSharedSPB* )pSPBs.GetAt( index );
}

deoglSharedSPBElement *deoglSharedSPBList::AddElement(){
	const int count = pSPBs.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglSharedSPBElement * const element = ( ( deoglSharedSPB* )pSPBs.GetAt( i ) )->AddElement();
		if( element ){
			return element;
		}
	}
	
	const deoglSharedSPB::Ref spb( deoglSharedSPB::Ref::New( new deoglSharedSPB( pCreateBlock() ) ) );
	pSPBs.Add( spb );
	
	return spb->AddElement();
}
