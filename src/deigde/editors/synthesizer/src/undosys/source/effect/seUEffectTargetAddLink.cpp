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

#include "seUEffectTargetAddLink.h"
#include "../../../synthesizer/effect/seEffect.h"
#include "../../../synthesizer/link/seLink.h"
#include "../../../synthesizer/controller/seControllerTarget.h"

#include <dragengine/common/exceptions.h>



// Class seUEffectTargetAddLink
/////////////////////////////////

// Constructor, destructor
////////////////////////////

seUEffectTargetAddLink::seUEffectTargetAddLink( seEffect *effect, seControllerTarget *target, seLink *link ) :
pEffect( NULL ),
pTarget( NULL ),
pLink( NULL )
{
	if( ! effect || ! target || ! link ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Effect target add link" );
	
	pEffect = effect;
	pEffect->AddReference();
	
	pTarget = target;
	
	pLink = link;
	pLink->AddReference();
}

seUEffectTargetAddLink::~seUEffectTargetAddLink(){
	if( pLink ){
		pLink->FreeReference();
	}
	if( pEffect ){
		pEffect->FreeReference();
	}
}



// Management
///////////////

void seUEffectTargetAddLink::Undo(){
	pTarget->RemoveLink( pLink );
	pEffect->NotifyEffectChanged();
}

void seUEffectTargetAddLink::Redo(){
	pTarget->AddLink( pLink );
	pEffect->NotifyEffectChanged();
}
