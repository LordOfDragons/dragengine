/* 
 * Drag[en]gine IGDE Project Editor
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

#include "projUProfileSetIdentifier.h"
#include "../../project/profile/projProfile.h"

#include <dragengine/common/exceptions.h>



// Class projUProfileSetIdentifier
////////////////////////////////////

// Constructor, destructor
////////////////////////////

projUProfileSetIdentifier::projUProfileSetIdentifier( projProfile *profile, const decUuid &newValue ) :
pProfile( NULL ),
pNewValue( newValue )
{
	if( ! profile ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Profile set identifier" );
	
	pOldValue = profile->GetIdentifier();
	
	pProfile = profile;
	profile->AddReference();
}

projUProfileSetIdentifier::~projUProfileSetIdentifier(){
	if( pProfile ){
		pProfile->FreeReference();
	}
}



// Management
///////////////

void projUProfileSetIdentifier::Undo(){
	pProfile->SetIdentifier( pOldValue );
}

void projUProfileSetIdentifier::Redo(){
	pProfile->SetIdentifier( pNewValue );
}
