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

#include "seUPropertyNodeTextSetColor.h"
#include "../../../../skin/property/node/sePropertyNodeText.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertyNodeTextSetColor
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertyNodeTextSetColor::seUPropertyNodeTextSetColor(
sePropertyNodeText *node, const decColor &newValue ) :
pNode( NULL ),
pNewValue( newValue )
{
	if( ! node || ! node->GetProperty() ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Node text set color" );
	
	pOldValue = node->GetColor();
	
	pNode = node;
	node->AddReference();
}

seUPropertyNodeTextSetColor::~seUPropertyNodeTextSetColor(){
	if( pNode ){
		pNode->FreeReference();
	}
}



// Management
///////////////

void seUPropertyNodeTextSetColor::Undo(){
	pNode->SetColor( pOldValue );
}

void seUPropertyNodeTextSetColor::Redo(){
	pNode->SetColor( pNewValue );
}
