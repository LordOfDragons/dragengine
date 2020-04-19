/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seUSourceGroupSetApplicationType.h"
#include "../../../synthesizer/source/seSourceGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceGroupSetApplicationType
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceGroupSetApplicationType::seUSourceGroupSetApplicationType(
seSourceGroup *source, deSynthesizerSourceGroup::eApplicationTypes newType ) :
pSource( NULL )
{
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pOldType = source->GetApplicationType();
	pNewType = newType;
	
	SetShortInfo( "Group source set application type" );
	
	pSource = source;
	pSource->AddReference();
}

seUSourceGroupSetApplicationType::~seUSourceGroupSetApplicationType(){
	if( pSource ){
		pSource->FreeReference();
	}
}



// Management
///////////////

void seUSourceGroupSetApplicationType::Undo(){
	pSource->SetApplicationType( pOldType );
}

void seUSourceGroupSetApplicationType::Redo(){
	pSource->SetApplicationType( pNewType );
}
