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

#include "seUSourceChainPathSoundMoveUp.h"
#include "../../../synthesizer/source/seSourceChain.h"

#include <dragengine/common/exceptions.h>



// Class seUSourceChainPathSoundMoveUp
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSourceChainPathSoundMoveUp::seUSourceChainPathSoundMoveUp( seSourceChain *source, int index ) :
pSource( NULL )
{
	if( ! source || index < 1 || index >= source->GetPathSounds().GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = index;
	
	SetShortInfo( "Chain source move path sound up" );
	
	pSource = source;
	pSource->AddReference();
}

seUSourceChainPathSoundMoveUp::~seUSourceChainPathSoundMoveUp(){
	if( pSource ){
		pSource->FreeReference();
	}
}



// Management
///////////////

void seUSourceChainPathSoundMoveUp::Undo(){
	pSource->MovePathSound( pIndex - 1, pIndex );
}

void seUSourceChainPathSoundMoveUp::Redo(){
	pSource->MovePathSound( pIndex, pIndex - 1 );
}
