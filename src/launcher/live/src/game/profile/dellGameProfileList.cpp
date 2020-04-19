/* 
 * Drag[en]gine Live Launcher
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

#include "dellGameProfile.h"
#include "dellGameProfileList.h"

#include <dragengine/common/exceptions.h>



// Class dellGameProfileList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellGameProfileList::dellGameProfileList(){
}

dellGameProfileList::~dellGameProfileList(){
	RemoveAll();
}



// Management
///////////////

int dellGameProfileList::GetCount() const{
	return pProfiles.GetCount();
}

dellGameProfile *dellGameProfileList::GetAt( int index ) const{
	return ( dellGameProfile* )pProfiles.GetAt( index );
}

dellGameProfile *dellGameProfileList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pProfiles.GetCount();
	dellGameProfile *profile;
	int i;
	
	for( i=0; i<count; i++ ){
		profile = ( dellGameProfile* )pProfiles.GetAt( i );
		if( profile->GetName().Equals( name ) ){
			return profile;
		}
	}
	
	return NULL;
}

bool dellGameProfileList::Has( dellGameProfile *profile ) const{
	return pProfiles.Has( profile );
}

bool dellGameProfileList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

int dellGameProfileList::IndexOf( dellGameProfile *profile ) const{
	return pProfiles.IndexOf( profile );
}

int dellGameProfileList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( dellGameProfile* )pProfiles.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dellGameProfileList::Add( dellGameProfile *profile ){
	if( ! profile || HasNamed( profile->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pProfiles.Add( profile );
}

void dellGameProfileList::Remove( dellGameProfile *profile ){
	const int index = IndexOf( profile );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pProfiles.RemoveFrom( index );
}

void dellGameProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}

void dellGameProfileList::ValidateProfiles( dellLauncher &launcher ){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( dellGameProfile* )pProfiles.GetAt( i ) )->Verify( launcher );
	}
}
