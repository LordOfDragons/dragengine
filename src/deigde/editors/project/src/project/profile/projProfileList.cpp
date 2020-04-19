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

#include "projProfile.h"
#include "projProfileList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class projProfile
///////////////////////

// Constructor, destructor
////////////////////////////

projProfileList::projProfileList(){
}

projProfileList::projProfileList( const projProfileList &copy ) :
pProfiles( copy.pProfiles ){
}

projProfileList::~projProfileList(){
}



// Management
///////////////

int projProfileList::GetCount() const{
	return pProfiles.GetCount();
}

projProfile *projProfileList::GetAt( int index ) const{
	return ( projProfile* )pProfiles.GetAt( index );
}

projProfile *projProfileList::GetNamed( const char *name ) const{
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		projProfile * const profile = ( projProfile* )pProfiles.GetAt( i );
		
		if( profile->GetName() == name ){
			return profile;
		}
	}
	
	return NULL;
}

int projProfileList::IndexOf( projProfile *profile ) const{
	return pProfiles.IndexOf( profile );
}

int projProfileList::IndexOfNamed( const char *name ) const{
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( projProfile* )pProfiles.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool projProfileList::Has( projProfile *profile ) const{
	return pProfiles.Has( profile );
}

bool projProfileList::HasNamed( const char *name ) const{
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( projProfile* )pProfiles.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void projProfileList::Add( projProfile *profile ){
	if( ! profile || HasNamed( profile->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pProfiles.Add( profile );
}

void projProfileList::Remove( projProfile *profile ){
	pProfiles.Remove( profile );
}

void projProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}



projProfileList &projProfileList::operator=( const projProfileList &list ){
	pProfiles = list.pProfiles;
	return *this;
}
