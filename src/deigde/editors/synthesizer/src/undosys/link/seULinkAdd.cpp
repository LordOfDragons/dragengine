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

#include "seULinkAdd.h"

#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/link/seLink.h"

#include <dragengine/common/exceptions.h>



// Class seULinkAdd
/////////////////////

// Constructor, destructor
////////////////////////////

seULinkAdd::seULinkAdd( seSynthesizer *synthesizer, seLink *link ) :
pSynthesizer( NULL ),
pLink( NULL )
{
	if( ! link || ! synthesizer ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add Link" );
	
	pSynthesizer = synthesizer;
	pSynthesizer->AddReference();
	
	pLink = link;
	pLink->AddReference();
}

seULinkAdd::~seULinkAdd(){
	if( pLink ){
		pLink->FreeReference();
	}
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
}



// Management
///////////////

void seULinkAdd::Undo(){
	pSynthesizer->RemoveLink( pLink );
}

void seULinkAdd::Redo(){
	pSynthesizer->AddLink( pLink );
	pSynthesizer->SetActiveLink( pLink );
}
