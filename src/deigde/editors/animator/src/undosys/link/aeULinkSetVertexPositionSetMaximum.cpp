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

#include "aeULinkSetVertexPositionSetMaximum.h"

#include "../../animator/link/aeLink.h"

#include <dragengine/common/exceptions.h>



// Class aeULinkSetVertexPositionSetMaximum
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeULinkSetVertexPositionSetMaximum::aeULinkSetVertexPositionSetMaximum( aeLink *link, float newValue ){
	DEASSERT_NOTNULL( link )
	
	pLink = nullptr;
	
	SetShortInfo( "Link set vertex position set maximum" );
	
	pLink = link;
	pLink->AddReference();
	
	pOldValue = link->GetVertexPositionSetMaximum();
	pNewValue = newValue;
}

aeULinkSetVertexPositionSetMaximum::~aeULinkSetVertexPositionSetMaximum(){
	if( pLink ){
		pLink->FreeReference();
	}
}



// Management
///////////////

void aeULinkSetVertexPositionSetMaximum::Undo(){
	pLink->SetVertexPositionSetMaximum( pOldValue );
}

void aeULinkSetVertexPositionSetMaximum::Redo(){
	pLink->SetVertexPositionSetMaximum( pNewValue );
}
