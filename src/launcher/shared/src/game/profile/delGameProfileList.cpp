/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delGameProfile.h"
#include "delGameProfileList.h"

#include <dragengine/common/exceptions.h>



// Class delGameProfileList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGameProfileList::delGameProfileList(){
}

delGameProfileList::~delGameProfileList(){
	RemoveAll();
}



// Management
///////////////

int delGameProfileList::GetCount() const{
	return pProfiles.GetCount();
}

delGameProfile *delGameProfileList::GetAt( int index ) const{
	return ( delGameProfile* )pProfiles.GetAt( index );
}

delGameProfile *delGameProfileList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delGameProfile * const profile = ( delGameProfile* )pProfiles.GetAt( i );
		if( profile->GetName() == name ){
			return profile;
		}
	}
	
	return nullptr;
}

bool delGameProfileList::Has( delGameProfile *profile ) const{
	return pProfiles.Has( profile );
}

bool delGameProfileList::HasNamed( const char *name ) const{
	return GetNamed ( name );
}

int delGameProfileList::IndexOf( delGameProfile *profile ) const{
	return pProfiles.IndexOf( profile );
}

int delGameProfileList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW_INFO( deeNullPointer, "name" );
	}
	
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( delGameProfile* )pProfiles.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void delGameProfileList::Add( delGameProfile *profile ){
	if( ! profile ){
		DETHROW_INFO( deeNullPointer, "profile" );
	}
	if( HasNamed ( profile->GetName() ) ){
		DETHROW_INFO( deeInvalidParam, "named profile is present" );
	}
	
	pProfiles.Add( profile );
}

void delGameProfileList::Remove( delGameProfile *profile ){
	const int index = IndexOf ( profile );
	if( index == -1 ){
		DETHROW_INFO( deeInvalidParam, "profile is absent" );
	}
	
	pProfiles.RemoveFrom( index );
}

void delGameProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}

void delGameProfileList::ValidateAll( delLauncher &launcher ){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( delGameProfile* )pProfiles.GetAt( i ) )->Verify( launcher );
	}
}
