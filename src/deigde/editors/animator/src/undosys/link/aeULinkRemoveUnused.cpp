/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeULinkRemoveUnused.h"

#include "../../animator/aeAnimator.h"
#include "../../animator/link/aeLink.h"
#include "../../animator/rule/aeRule.h"

#include <dragengine/common/exceptions.h>



// Class aeULinkRemoveUnused
//////////////////////////////

// Constructor, destructor
////////////////////////////

aeULinkRemoveUnused::aeULinkRemoveUnused( aeAnimator *animator ) :
pAnimator( NULL )
{
	if( ! animator ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove unused links" );
	
	const int count = animator->GetLinks().GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		aeLink * const link = animator->GetLinks().GetAt( i );
		if( animator->CountLinkUsage( link ) == 0 ){
			pLinks.Add( link );
		}
	}
	
	pAnimator = animator;
	pAnimator->AddReference();
}

aeULinkRemoveUnused::~aeULinkRemoveUnused(){
	if( pAnimator ){
		pAnimator->FreeReference();
	}
}



// Management
///////////////

void aeULinkRemoveUnused::Undo(){
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pAnimator->AddLink( pLinks.GetAt( i ) );
	}
}

void aeULinkRemoveUnused::Redo(){
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pAnimator->RemoveLink( pLinks.GetAt( i ) );
	}
}
