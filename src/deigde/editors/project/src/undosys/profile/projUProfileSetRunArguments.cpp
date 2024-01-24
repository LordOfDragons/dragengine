/* 
 * Drag[en]gine IGDE Project Editor
 *
 * Copyright (C) 2024, Roland Plüss (roland@rptd.ch)
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

#include "projUProfileSetRunArguments.h"
#include "../../project/profile/projProfile.h"

#include <dragengine/common/exceptions.h>



// Class projUProfileSetRunArguments
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

projUProfileSetRunArguments::projUProfileSetRunArguments(
projProfile *profile, const char *newValue ) :
pProfile( NULL ),
pNewValue( newValue )
{
	DEASSERT_NOTNULL( profile )
	
	SetShortInfo( "Profile set run arguments" );
	
	pOldValue = profile->GetRunArguments();
	
	pProfile = profile;
	profile->AddReference();
}

projUProfileSetRunArguments::~projUProfileSetRunArguments(){
	if( pProfile ){
		pProfile->FreeReference();
	}
}



// Management
///////////////

void projUProfileSetRunArguments::Undo(){
	pProfile->SetRunArguments( pOldValue );
}

void projUProfileSetRunArguments::Redo(){
	pProfile->SetRunArguments( pNewValue );
}
