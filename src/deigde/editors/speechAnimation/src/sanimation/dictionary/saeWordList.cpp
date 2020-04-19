/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#include "saeWord.h"
#include "saeWordList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class saeWord
//////////////////

// Constructor, destructor
////////////////////////////

saeWordList::saeWordList(){
}

saeWordList::saeWordList( const saeWordList &list ) :
pWords( list.pWords ){
}

saeWordList::~saeWordList(){
}



// Management
///////////////

int saeWordList::GetCount() const{
	return pWords.GetCount();
}

saeWord *saeWordList::GetAt( int index ) const{
	return ( saeWord* )pWords.GetAt( index );
}

saeWord *saeWordList::GetNamed( const char *name ) const{
	const int count = pWords.GetCount();
	saeWord *word;
	int p;
	
	for( p=0; p<count; p++ ){
		word = ( saeWord* )pWords.GetAt( p );
		
		if( word->GetName().Equals( name ) ){
			return word;
		}
	}
	
	return NULL;
}

int saeWordList::IndexOf( saeWord *word ) const{
	return pWords.IndexOf( word );
}

int saeWordList::IndexOfNamed( const char *name ) const{
	const int count = pWords.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( saeWord* )pWords.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool saeWordList::Has( saeWord *word ) const{
	return pWords.Has( word );
}

bool saeWordList::HasNamed( const char *name ) const{
	const int count = pWords.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( saeWord* )pWords.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void saeWordList::Add( saeWord *word ){
	if( ! word || HasNamed( word->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pWords.Add( word );
}

void saeWordList::Remove( saeWord *word ){
	pWords.Remove( word );
}

void saeWordList::RemoveAll(){
	pWords.RemoveAll();
}



saeWordList &saeWordList::operator=( const saeWordList &list ){
	pWords = list.pWords;
	return *this;
}
