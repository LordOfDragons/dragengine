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

#include "seUSetSourceMinVolume.h"
#include "../../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>



// Class seUSetSourceMinVolume
// /////////////////////////////

// Constructor, destructor
////////////////////////////

seUSetSourceMinVolume::seUSetSourceMinVolume( seSource *source, float newValue ) :
pSource( NULL )
{
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pOldValue = source->GetMinVolume();
	pNewValue = newValue;
	
	try{
		pSource = source;
		pSource->AddReference();
		
		SetShortInfo( "Source set minimum volume" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

seUSetSourceMinVolume::~seUSetSourceMinVolume(){
	pCleanUp();
}



// Management
///////////////

void seUSetSourceMinVolume::Undo(){
	pSource->SetMinVolume( pOldValue );
}

void seUSetSourceMinVolume::Redo(){
	pSource->SetMinVolume( pNewValue );
}



// Private Functions
//////////////////////

void seUSetSourceMinVolume::pCleanUp(){
	if( pSource ){
		pSource->FreeReference();
	}
}
