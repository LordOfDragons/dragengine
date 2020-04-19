/* 
 * Drag[en]gine Android Launcher
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

#include "dealGameProfile.h"
#include "dealGameProfileList.h"

#include "../../common/exceptions.h"



// Class dealGameProfileList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGameProfileList::dealGameProfileList(){
}

dealGameProfileList::~dealGameProfileList(){
	RemoveAll();
}



// Management
///////////////

int dealGameProfileList::GetCount() const{
	return pProfiles.GetCount();
}

dealGameProfile *dealGameProfileList::GetAt( int index ) const{
	return ( dealGameProfile* )pProfiles.GetAt( index );
}

dealGameProfile *dealGameProfileList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pProfiles.GetCount();
	dealGameProfile *profile;
	
	for( i=0; i<count; i++ ){
		profile = ( dealGameProfile* )pProfiles.GetAt( i );
		if( profile->GetName().Equals( name ) ){
			return profile;
		}
	}
	
	return NULL;
}

bool dealGameProfileList::Has( dealGameProfile *profile ) const{
	return pProfiles.Has( profile );
}

bool dealGameProfileList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

int dealGameProfileList::IndexOf( dealGameProfile *profile ) const{
	return pProfiles.IndexOf( profile );
}

int dealGameProfileList::IndexOfNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i, count = pProfiles.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( dealGameProfile* )pProfiles.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void dealGameProfileList::Add( dealGameProfile *profile ){
	if( ! profile || HasNamed( profile->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pProfiles.Add( profile );
}

void dealGameProfileList::Remove( dealGameProfile *profile ){
	int index = IndexOf( profile );
	
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pProfiles.RemoveFrom( index );
}

void dealGameProfileList::RemoveAll(){
	pProfiles.RemoveAll();
}

void dealGameProfileList::ValidateProfiles( dealLauncher &launcher ){
	int i, count = pProfiles.GetCount();
	
	for( i=0; i<count; i++ ){
		( ( dealGameProfile* )pProfiles.GetAt( i ) )->Verify( launcher );
	}
}
