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

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "seClipboardDataEffect.h"
#include "../synthesizer/effect/seEffect.h"

#include <dragengine/common/exceptions.h>



// Class seClipboardDataEffect
////////////////////////////////

const char * const seClipboardDataEffect::TYPE_NAME = "effect";

// Constructor, destructor
////////////////////////////

seClipboardDataEffect::seClipboardDataEffect( seEffect *effect ) :
igdeClipboardData( TYPE_NAME )
{
	seEffect *copyEffect = NULL;
	
	try{
		copyEffect = effect->CreateCopy();
		pEffects.Add( copyEffect );
		copyEffect->FreeReference();
		
	}catch( const deException & ){
		if( copyEffect ){
			copyEffect->FreeReference();
		}
		throw;
	}
}

seClipboardDataEffect::seClipboardDataEffect( const seEffectList &effects ) :
igdeClipboardData( TYPE_NAME )
{
	const int count = effects.GetCount();
	seEffect *effect = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			effect = effects.GetAt( i )->CreateCopy();
			pEffects.Add( effect );
			effect->FreeReference();
			effect = NULL;
		}
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
}

seClipboardDataEffect::~seClipboardDataEffect(){
	pEffects.RemoveAll();
}
