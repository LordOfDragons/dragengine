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
#include <string.h>
#include <stdlib.h>

#include "seUPropertyNodeRemoveMask.h"
#include "../../../skin/property/seProperty.h"
#include "../../../skin/property/node/sePropertyNode.h"
#include "../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodeRemoveMask
////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeRemoveMask::seUPropertyNodeRemoveMask( sePropertyNode *node ) :
pNode( NULL ),
pMask( NULL )
{
	if( ! node || ! node->GetProperty() || ! node->GetMask() ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Node remove mask" );
	
	pMask = node->GetMask();
	pMask->AddReference();
	
	pOldPosition = pMask->GetPosition();
	pOldSize = pMask->GetSize();
	pOldRotation = pMask->GetRotation();
	pOldShearing = pMask->GetShearing();
	
	pNode = node;
	node->AddReference();
}

seUPropertyNodeRemoveMask::~seUPropertyNodeRemoveMask(){
	if( pNode ){
		pNode->FreeReference();
	}
	if( pMask ){
		pMask->FreeReference();
	}
}



// Management
///////////////

void seUPropertyNodeRemoveMask::Undo(){
	pNode->GetParent()->RemoveNode( pMask );
	
	pMask->SetPosition( pOldPosition );
	pMask->SetSize( pOldSize );
	pMask->SetRotation( pOldRotation );
	pMask->SetShearing( pOldShearing );
	
	pNode->SetMask( pMask );
}

void seUPropertyNodeRemoveMask::Redo(){
	const decTexMatrix2 matrix( pMask->CreateParentTransformMatrix() * pNode->CreateParentTransformMatrix() );
	pNode->SetMask( NULL );
	pMask->SetFromMatrix( matrix, pOldSize, pOldRotation );
	pNode->GetParent()->AddNode( pMask );
}
