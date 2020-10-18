/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceLookAt.h"
#include "ceLookAtList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceLookAt
///////////////////

// Constructor, destructor
////////////////////////////

ceLookAtList::ceLookAtList(){
}

ceLookAtList::~ceLookAtList(){
}



// Management
///////////////

int ceLookAtList::GetCount() const{
	return pLookAts.GetCount();
}

ceLookAt *ceLookAtList::GetAt( int index ) const{
	return ( ceLookAt* )pLookAts.GetAt( index );
}

ceLookAt *ceLookAtList::GetNamed( const char *name ) const{
	const int count = pLookAts.GetCount();
	ceLookAt *lookat;
	int i;
	
	for( i=0; i<count; i++ ){
		lookat = ( ceLookAt* )pLookAts.GetAt( i );
		
		if( lookat->GetName().Equals( name ) ){
			return lookat;
		}
	}
	
	return NULL;
}

int ceLookAtList::IndexOf( ceLookAt *lookat ) const{
	return pLookAts.IndexOf( lookat );
}

int ceLookAtList::IndexOfNamed( const char *name ) const{
	const int count = pLookAts.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceLookAt* )pLookAts.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceLookAtList::Has( ceLookAt *lookat ) const{
	return pLookAts.Has( lookat );
}

bool ceLookAtList::HasNamed( const char *name ) const{
	const int count = pLookAts.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceLookAt* )pLookAts.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceLookAtList::Add( ceLookAt *lookat ){
	if( ! lookat || HasNamed( lookat->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pLookAts.Add( lookat );
}

void ceLookAtList::Remove( ceLookAt *lookat ){
	pLookAts.Remove( lookat );
}

void ceLookAtList::RemoveAll(){
	pLookAts.RemoveAll();
}



ceLookAtList &ceLookAtList::operator=( const ceLookAtList &list ){
	pLookAts = list.pLookAts;
	return *this;
}

ceLookAtList &ceLookAtList::operator+=( const ceLookAtList &list ){
	pLookAts += list.pLookAts;
	return *this;
}
