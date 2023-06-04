/* 
 * Drag[en]gine IGDE Speech Animation Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "saeUSAnimSetNeutralVertexPositionSets.h"
#include "../../sanimation/saeSAnimation.h"

#include <dragengine/common/exceptions.h>



// Class saeUSAnimSetNeutralVertexPositionSets
////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

saeUSAnimSetNeutralVertexPositionSets::saeUSAnimSetNeutralVertexPositionSets(
	saeSAnimation *sanimation, const decStringSet &sets ) :
pSAnimation( nullptr ),
pNewSets( sets )
{
	DEASSERT_NOTNULL( sanimation )
	
	SetShortInfo( "Set neutral vertex position sets" );
	
	pOldSets = sanimation->GetNeutralVertexPositionSets();
	
	pSAnimation = sanimation;
	sanimation->AddReference();
}

saeUSAnimSetNeutralVertexPositionSets::~saeUSAnimSetNeutralVertexPositionSets(){
	if( pSAnimation ){
		pSAnimation->FreeReference();
	}
}



// Management
///////////////

void saeUSAnimSetNeutralVertexPositionSets::Undo(){
	pSAnimation->SetNeutralVertexPositionSets( pOldSets );
}

void saeUSAnimSetNeutralVertexPositionSets::Redo(){
	pSAnimation->SetNeutralVertexPositionSets( pNewSets );
}
