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

#include "seUPropertyNodeData.h"
#include "../../../../skin/property/node/sePropertyNode.h"
#include "../../../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodeData
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeData::seUPropertyNodeData( sePropertyNode *node ) :
pNode( NULL )
{
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	
	pPosition = node->GetPosition();
	pSize = node->GetSize();
	pRotation = node->GetRotation();
	pShearing = node->GetShearing();
	pMatrix = node->CreateScreenTransformMatrix();
	
	pNode = node;
	node->AddReference();
}

seUPropertyNodeData::~seUPropertyNodeData(){
	if( pNode ){
		pNode->FreeReference();
	}
}



// Management
///////////////

decTexMatrix2 seUPropertyNodeData::CreateInverseParentMatrix() const{
	if( pNode->GetMaskParent() ){
		return pNode->GetMaskParent()->CreateScreenTransformMatrix().Invert().ToTexMatrix2();
		
	}else if( pNode->GetParent() ){
		return pNode->GetParent()->CreateScreenTransformMatrix().Invert().ToTexMatrix2();
		
	}else{
		return decTexMatrix2();
	}
}
