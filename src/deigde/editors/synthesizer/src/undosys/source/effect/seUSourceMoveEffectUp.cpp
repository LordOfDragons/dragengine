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

#include "seUSourceMoveEffectUp.h"
#include "../../../synthesizer/effect/seEffect.h"
#include "../../../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceMoveEffectUp
////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceMoveEffectUp::seUSourceMoveEffectUp( seSource *source, seEffect *effect ) :
pSource( NULL ),
pEffect( NULL )
{
	if( ! effect || ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = source->GetEffects().IndexOf( effect );
	if( pIndex < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pSource = source;
	source->AddReference();
	
	pEffect = effect;
	effect->AddReference();
}

seUSourceMoveEffectUp::~seUSourceMoveEffectUp(){
	pCleanUp();
}



// Management
///////////////

void seUSourceMoveEffectUp::Undo(){
	pSource->MoveEffectTo( pEffect, pIndex );
}

void seUSourceMoveEffectUp::Redo(){
	pSource->MoveEffectTo( pEffect, pIndex - 1 );
}



// Private Functions
//////////////////////

void seUSourceMoveEffectUp::pCleanUp(){
	if( pEffect ){
		pEffect->FreeReference();
	}
	if( pSource ){
		pSource->FreeReference();
	}
}
