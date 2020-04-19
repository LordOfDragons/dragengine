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

#include "seUPropertySetConstructedSize.h"
#include "../../skin/property/seProperty.h"
#include "../../skin/property/node/sePropertyNodeGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUPropertySetConstructedSize
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUPropertySetConstructedSize::seUPropertySetConstructedSize(
seProperty *property, const decPoint3 &newValue ) :

pProperty( NULL ),
pNewValue( newValue )
{
	if( ! property ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Property constructed set size" );
	
	pOldValue = property->GetNodeGroup()->GetSize();
	
	pProperty = property;
	property->AddReference();
}

seUPropertySetConstructedSize::~seUPropertySetConstructedSize(){
	if( pProperty ){
		pProperty->FreeReference();
	}
}



// Management
///////////////

void seUPropertySetConstructedSize::Undo(){
	pProperty->GetNodeGroup()->SetSize( pOldValue );
}

void seUPropertySetConstructedSize::Redo(){
	pProperty->GetNodeGroup()->SetSize( pNewValue );
}
