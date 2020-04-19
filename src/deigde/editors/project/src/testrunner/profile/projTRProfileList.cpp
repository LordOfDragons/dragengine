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
#include <stdlib.h>
#include <string.h>

#include "projTRProfile.h"
#include "projTRProfileList.h"

#include <dragengine/common/exceptions.h>



// Class projTRProfileList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

projTRProfileList::projTRProfileList(){
}

projTRProfileList::~projTRProfileList(){
	RemoveAll();
}



// Management
///////////////

int projTRProfileList::GetCount() const{
	return pProfiles.GetCount();
}

projTRProfile *projTRProfileList::GetAt( int index ) const{
	return ( projTRProfile* )pProfiles.GetAt( index );
}

projTRProfile *projTRProfileList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pProfiles.GetCount();
	projTRProfile *profile;
	int i;
	
	for( i=0; i<count; i++ ){
		profile = ( projTRProfile* )pProfiles.GetAt( i );
		if( profile->GetName().Equals( name ) ){
			return profile;
		}
	}
	
	return NULL;
}

bool projTRProfileList::Has( projTRProfile *profile ) const{
	return pProfiles.Has( profile );
}

bool projTRProfileList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

int projTRProfileList::IndexOf( projTRProfile *profile ) const{
	return pProfiles.IndexOf( profile );
}

int projTRProfileList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( projTRProfile* )pProfiles.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void projTRProfileList::Add( projTRProfile *profile ){
	if( ! profile || HasNamed( profile->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pProfiles.Add( profile );
}

void projTRProfileList::Remove( projTRProfile *profile ){
	const int index = IndexOf( profile );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pProfiles.RemoveFrom( index );
}

void projTRProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}

void projTRProfileList::ValidateProfiles( deEngine &engine ){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( projTRProfile* )pProfiles.GetAt( i ) )->Verify( engine );
	}
}
