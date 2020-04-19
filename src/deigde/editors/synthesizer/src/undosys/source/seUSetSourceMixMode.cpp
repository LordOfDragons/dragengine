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

#include "seUSetSourceMixMode.h"
#include "../../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>



// Class seUSetSourceMixMode
//////////////////////////////

// Constructor, destructor
////////////////////////////

seUSetSourceMixMode::seUSetSourceMixMode( seSource *source, deSynthesizerSource::eMixModes newMode ) :
pSource( NULL )
{
	if( ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pOldMode = source->GetMixMode();
	pNewMode = newMode;
	
	try{
		pSource = source;
		pSource->AddReference();
		
		SetShortInfo( "Source set blend mode" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

seUSetSourceMixMode::~seUSetSourceMixMode(){
	pCleanUp();
}



// Management
///////////////

void seUSetSourceMixMode::Undo(){
	pSource->SetMixMode( pOldMode );
}

void seUSetSourceMixMode::Redo(){
	pSource->SetMixMode( pNewMode );
}



// Private Functions
//////////////////////

void seUSetSourceMixMode::pCleanUp(){
	if( pSource ){
		pSource->FreeReference();
	}
}
