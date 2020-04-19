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
#include <stdlib.h>
#include <string.h>

#include "seUSourceTargetAddLink.h"
#include "../../synthesizer/source/seSource.h"
#include "../../synthesizer/link/seLink.h"
#include "../../synthesizer/controller/seControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceTargetAddLink
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceTargetAddLink::seUSourceTargetAddLink( seSource *source, seControllerTarget *target, seLink *link ) :
pSource( NULL ),
pTarget( NULL ),
pLink( NULL )
{
	if( ! source || ! target || ! link ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Source Target Add Link" );
	
	pSource = source;
	pSource->AddReference();
	
	pTarget = target;
	
	pLink = link;
	pLink->AddReference();
}

seUSourceTargetAddLink::~seUSourceTargetAddLink(){
	if( pLink ){
		pLink->FreeReference();
	}
	if( pSource ){
		pSource->FreeReference();
	}
}



// Management
///////////////

void seUSourceTargetAddLink::Undo(){
	pTarget->RemoveLink( pLink );
	pSource->NotifySourceChanged();
}

void seUSourceTargetAddLink::Redo(){
	pTarget->AddLink( pLink );
	pSource->NotifySourceChanged();
}
