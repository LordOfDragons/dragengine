/* 
 * Drag[en]gine IGDE Skin Editor
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
#include <string.h>
#include <stdlib.h>

#include "seUPropertyNodeSetMask.h"
#include "../../../skin/property/seProperty.h"
#include "../../../skin/property/node/sePropertyNode.h"
#include "../../../skin/property/node/sePropertyNodeGroup.h"
#include "../../../skin/property/node/sePropertyNodeSelection.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodeSetMask
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeSetMask::seUPropertyNodeSetMask( sePropertyNode *node, sePropertyNode *mask ) :
pNode( NULL ),
pMask( NULL )
{
	if( ! node || ! node->GetProperty() || node->GetMask() || ! mask ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Node set mask" );
	
	pOldPosition = mask->GetPosition();
	pOldSize = mask->GetSize();
	pOldRotation = mask->GetRotation();
	pOldShearing = mask->GetShearing();
	
	pMask = mask;
	mask->AddReference();
	
	pNode = node;
	node->AddReference();
}

seUPropertyNodeSetMask::~seUPropertyNodeSetMask(){
	if( pNode ){
		pNode->FreeReference();
	}
	if( pMask ){
		pMask->FreeReference();
	}
}



// Management
///////////////

void seUPropertyNodeSetMask::Undo(){
	pNode->SetMask( NULL );
	
	pMask->SetPosition( pOldPosition );
	pMask->SetSize( pOldSize );
	pMask->SetRotation( pOldRotation );
	pMask->SetShearing( pOldShearing );
	
	pNode->GetParent()->AddNode( pMask );
}

void seUPropertyNodeSetMask::Redo(){
	const decTexMatrix2 matrix( ( pMask->CreateParentTransformMatrix().ToTexMatrix()
		* pNode->CreateParentTransformMatrix().Invert() ).ToTexMatrix2() );
	
	pNode->GetProperty()->GetNodeSelection().Remove( pMask );
	pNode->GetParent()->RemoveNode( pMask );
	pMask->SetFromMatrix( matrix, pOldSize, pOldRotation );
	pNode->SetMask( pMask );
}
