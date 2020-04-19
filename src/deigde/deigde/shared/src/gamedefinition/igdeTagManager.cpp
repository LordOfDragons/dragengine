/* 
 * Drag[en]gine IGDE
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

#include "igdeTagManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringSet.h>



// Class igdeTagManager
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeTagManager::igdeTagManager(){
}

igdeTagManager::igdeTagManager( const igdeTagManager &tagManager ){
	pTags = tagManager.pTags;
}

igdeTagManager::~igdeTagManager(){
}



// Management
///////////////

int igdeTagManager::GetTagCount() const{
	return pTags.GetCount();
}

const decString &igdeTagManager::GetTagAt( int index ) const{
	return pTags.GetAt( index );
}

bool igdeTagManager::HasTag( const char *tag ) const{
	return pTags.Has( tag );
}

int igdeTagManager::IndexOfTag( const char *tag ) const{
	if( ! tag ){
		DETHROW( deeInvalidParam );
	}
	
	return pTags.IndexOf( tag );
}

void igdeTagManager::AddTag( const char *tag ){
	if( ! tag || ! tag[ 0 ] ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pTags.Has( tag ) ){
		pTags.Add( tag );
	}
}

void igdeTagManager::RemoveTag( const char *tag ){
	const int index = pTags.IndexOf( tag );
	
	if( index != -1 ){
		pTags.RemoveFrom( index );
	}
}

void igdeTagManager::RemoveAllTags(){
	pTags.RemoveAll();
}



bool igdeTagManager::HasAllOf( const igdeTagManager &tags ) const{
	const int tagCount = tags.GetTagCount();
	int i;
	
	if( tagCount == 0 ){
		return false;
	}
	
	for( i=0; i<tagCount; i++ ){
		if( ! pTags.Has( tags.GetTagAt( i ) ) ){
			return false;
		}
	}
	
	return true;
}

bool igdeTagManager::HasAnyOf( const igdeTagManager &tags ) const{
	const int tagCount = tags.GetTagCount();
	int i;
	
	for( i=0; i<tagCount; i++ ){
		if( pTags.Has( tags.GetTagAt( i ) ) ){
			return true;
		}
	}
	
	return false;
}

bool igdeTagManager::HasNoneOf( const igdeTagManager &tags ) const{
	return ! HasAnyOf( tags );
}



void igdeTagManager::AddTagsFrom( const igdeTagManager &tags ){
	const int tagCount = tags.GetTagCount();
	int i;
	
	for( i=0; i<tagCount; i++ ){
		AddTag( tags.GetTagAt( i ) );
	}
}

decStringSet igdeTagManager::ToSet() const{
	decStringSet set;
	
	const int count = pTags.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		set.Add( pTags.GetAt( i ) );
	}
	
	return set;
}



// Operators
//////////////

igdeTagManager &igdeTagManager::operator=( const igdeTagManager &tags ){
	pTags = tags.pTags;
	return *this;
}

igdeTagManager &igdeTagManager::operator=( const decStringSet &tags ){
	const int count = tags.GetCount();
	int i;
	
	pTags.RemoveAll();
	for( i=0; i<count; i++ ){
		pTags.Add( tags.GetAt( i ) );
	}
	
	return *this;
}

igdeTagManager::operator decStringSet() const{
	return ToSet();
}
