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

#include "seUSourceSynthConTargetLess.h"
#include "../../../synthesizer/controller/seController.h"
#include "../../../synthesizer/source/seSourceSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceSynthConTargetLess
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceSynthConTargetLess::seUSourceSynthConTargetLess( seSourceSynthesizer *source ) :
pSource( NULL ),
pOldController( NULL )
{
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Synthesizer source less connection targets" );
	
	pOldController = source->GetControllerAt( source->GetConnectionCount() - 1 );
	if( pOldController ){
		pOldController->AddReference();
	}
	
	pSource = source;
	pSource->AddReference();
}

seUSourceSynthConTargetLess::~seUSourceSynthConTargetLess(){
	if( pOldController ){
		pOldController->FreeReference();
	}
	if( pSource ){
		pSource->FreeReference();
	}
}



// Management
///////////////

void seUSourceSynthConTargetLess::Undo(){
	const int index = pSource->GetConnectionCount();
	
	pSource->SetConnectionCount( index + 1 );
	pSource->SetControllerAt( index, pOldController );
}

void seUSourceSynthConTargetLess::Redo(){
	pSource->SetConnectionCount( pSource->GetConnectionCount() - 1 );
}
