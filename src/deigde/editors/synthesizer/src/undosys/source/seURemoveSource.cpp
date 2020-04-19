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

#include "seURemoveSource.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/source/seSource.h"

#include <dragengine/common/exceptions.h>



// Class seURemoveSource
////////////////////////

// Constructor, destructor
////////////////////////////

seURemoveSource::seURemoveSource( seSynthesizer *synthesizer, seSource *source ) :
pSynthesizer( NULL ),
pSource( NULL )
{
	if( ! synthesizer || ! source ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = synthesizer->GetSources().IndexOf( source );
	if( pIndex == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pSynthesizer = synthesizer;
	synthesizer->AddReference();
	
	pSource = source;
	source->AddReference();
}

seURemoveSource::~seURemoveSource(){
	pCleanUp();
}



// Management
///////////////

void seURemoveSource::Undo(){
	pSynthesizer->InsertSourceAt( pSource, pIndex );
}

void seURemoveSource::Redo(){
	pSynthesizer->RemoveSource( pSource );
}



// Private Functions
//////////////////////

void seURemoveSource::pCleanUp(){
	if( pSource ){
		pSource->FreeReference();
	}
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
}
