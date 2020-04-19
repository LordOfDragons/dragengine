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

#include "seUSynthesizerSetSampleCount.h"
#include "../../synthesizer/seSynthesizer.h"

#include <dragengine/common/exceptions.h>



// Class seUSynthesizerSetSampleCount
// ////////////////////////////////////

// Constructor, destructor
////////////////////////////

seUSynthesizerSetSampleCount::seUSynthesizerSetSampleCount( seSynthesizer *synthesizer, int newSampleCount ) :
pSynthesizer( NULL )
{
	if( ! synthesizer ){
		DETHROW( deeInvalidParam );
	}
	
	pOldSampleCount = synthesizer->GetSampleCount();
	pNewSampleCount = newSampleCount;
	
	try{
		pSynthesizer = synthesizer;
		pSynthesizer->AddReference();
		
		SetShortInfo( "Synthesizer set sample count" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

seUSynthesizerSetSampleCount::~seUSynthesizerSetSampleCount(){
	pCleanUp();
}



// Management
///////////////

void seUSynthesizerSetSampleCount::Undo(){
	pSynthesizer->SetSampleCount( pOldSampleCount );
}

void seUSynthesizerSetSampleCount::Redo(){
	pSynthesizer->SetSampleCount( pNewSampleCount );
}



// Private Functions
//////////////////////

void seUSynthesizerSetSampleCount::pCleanUp(){
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
}
