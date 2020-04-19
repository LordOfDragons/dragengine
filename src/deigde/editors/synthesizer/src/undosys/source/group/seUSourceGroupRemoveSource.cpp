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

#include "seUSourceGroupRemoveSource.h"
#include "../../../synthesizer/source/seSourceGroup.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceGroupRemoveSource
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceGroupRemoveSource::seUSourceGroupRemoveSource( seSourceGroup *group, seSource *source ) :
pGroup( NULL ),
pSource( NULL )
{
	if( ! group || ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = group->GetSources().IndexOf( source );
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pGroup = group;
	group->AddReference();
	
	pSource = source;
	source->AddReference();
}

seUSourceGroupRemoveSource::~seUSourceGroupRemoveSource(){
	if( pSource ){
		pSource->FreeReference();
	}
	if( pGroup ){
		pGroup->FreeReference();
	}
}



// Management
///////////////

void seUSourceGroupRemoveSource::Undo(){
	pGroup->InsertSourceAt( pSource, pIndex );
}

void seUSourceGroupRemoveSource::Redo(){
	pGroup->RemoveSource( pSource );
}
