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

#include "seUMappedSetOutputUpper.h"

#include <dragengine/common/exceptions.h>



// Class seUMappedSetOutputUpper
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seUMappedSetOutputUpper::seUMappedSetOutputUpper( seMapped *mapped, float newOutputUpper ) :
pMapped( mapped ),
pOldOutputUpper( mapped ? mapped->GetOutputUpper() : 1.0f ),
pNewOutputUpper( newOutputUpper )
{
	DEASSERT_NOTNULL( pMapped )
	
	SetShortInfo( "Set Mapped Output Upper" );
}

seUMappedSetOutputUpper::~seUMappedSetOutputUpper(){
}



// Management
///////////////

void seUMappedSetOutputUpper::Undo(){
	pMapped->SetOutputUpper( pOldOutputUpper );
}

void seUMappedSetOutputUpper::Redo(){
	pMapped->SetOutputUpper( pNewOutputUpper );
}
