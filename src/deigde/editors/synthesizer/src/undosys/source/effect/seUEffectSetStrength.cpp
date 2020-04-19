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

#include "seUEffectSetStrength.h"
#include "../../../synthesizer/effect/seEffect.h"

#include <dragengine/common/exceptions.h>



// Class seUEffectSetStrength
// ////////////////////////////

// Constructor, destructor
////////////////////////////

seUEffectSetStrength::seUEffectSetStrength( seEffect *effect, float newStrength ) :
pEffect( NULL )
{
	if( ! effect ){
		DETHROW( deeInvalidParam );
	}
	
	pOldStrength = effect->GetStrength();
	pNewStrength = newStrength;
	
	try{
		pEffect = effect;
		pEffect->AddReference();
		
		SetShortInfo( "Effect set strength" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

seUEffectSetStrength::~seUEffectSetStrength(){
	pCleanUp();
}



// Management
///////////////

void seUEffectSetStrength::Undo(){
	pEffect->SetStrength( pOldStrength );
}

void seUEffectSetStrength::Redo(){
	pEffect->SetStrength( pNewStrength );
}



// Private Functions
//////////////////////

void seUEffectSetStrength::pCleanUp(){
	if( pEffect ){
		pEffect->FreeReference();
	}
}
