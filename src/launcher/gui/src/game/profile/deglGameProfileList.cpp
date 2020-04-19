/* 
 * Drag[en]gine GUI Launcher
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

#include "deglGameProfile.h"
#include "deglGameProfileList.h"

#include <dragengine/common/exceptions.h>



// Class deglGameProfileList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deglGameProfileList::deglGameProfileList(){
}

deglGameProfileList::~deglGameProfileList(){
	RemoveAllProfiles();
}



// Management
///////////////

int deglGameProfileList::GetProfileCount() const{
	return pProfiles.GetCount();
}

deglGameProfile *deglGameProfileList::GetProfileAt( int index ) const{
	return ( deglGameProfile* )pProfiles.GetAt( index );
}

deglGameProfile *deglGameProfileList::GetProfileNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pProfiles.GetCount();
	deglGameProfile *profile;
	
	for( i=0; i<count; i++ ){
		profile = ( deglGameProfile* )pProfiles.GetAt( i );
		if( profile->GetName().Equals( name ) ){
			return profile;
		}
	}
	
	return NULL;
}

bool deglGameProfileList::HasProfile( deglGameProfile *profile ) const{
	return pProfiles.Has( profile );
}

bool deglGameProfileList::HasProfileNamed( const char *name ) const{
	return GetProfileNamed( name ) != NULL;
}

int deglGameProfileList::IndexOfProfile( deglGameProfile *profile ) const{
	return pProfiles.IndexOf( profile );
}

int deglGameProfileList::IndexOfProfileNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	
	int i, count = pProfiles.GetCount();
	
	for( i=0; i<count; i++ ){
		if( ( ( deglGameProfile* )pProfiles.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

void deglGameProfileList::AddProfile( deglGameProfile *profile ){
	if( ! profile || HasProfileNamed( profile->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pProfiles.Add( profile );
}

void deglGameProfileList::RemoveProfile( deglGameProfile *profile ){
	int index = IndexOfProfile( profile );
	
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	pProfiles.RemoveFrom( index );
}

void deglGameProfileList::RemoveAllProfiles(){
	pProfiles.RemoveAll();
}

void deglGameProfileList::ValidateProfiles( deglLauncher &launcher ){
	int i, count = pProfiles.GetCount();
	
	for( i=0; i<count; i++ ){
		( ( deglGameProfile* )pProfiles.GetAt( i ) )->Verify( launcher );
	}
}
