/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "seUMappedSetName.h"

#include <dragengine/common/exceptions.h>



// Class seUMappedSetName
///////////////////////////

// Constructor, destructor
////////////////////////////

seUMappedSetName::seUMappedSetName( seMapped *mapped, const char *newName ) :
pMapped( mapped ),
pOldName( mapped ? mapped->GetName().GetString() : "" ),
pNewName( newName )
{
	DEASSERT_NOTNULL( pMapped )
	
	SetShortInfo( "Set Mapped Name" );
}

seUMappedSetName::~seUMappedSetName(){
}



// Management
///////////////

void seUMappedSetName::Undo(){
	pMapped->SetName( pOldName );
}

void seUMappedSetName::Redo(){
	pMapped->SetName( pNewName );
}
