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

#include "ceSAWord.h"
#include "ceSAWordList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceSAWord
///////////////////

// Constructor, destructor
////////////////////////////

ceSAWordList::ceSAWordList(){
}

ceSAWordList::~ceSAWordList(){
}



// Management
///////////////

int ceSAWordList::GetCount() const{
	return pWords.GetCount();
}

ceSAWord *ceSAWordList::GetAt( int index ) const{
	return ( ceSAWord* )pWords.GetAt( index );
}

ceSAWord *ceSAWordList::GetNamed( const char *name ) const{
	const int count = pWords.GetCount();
	ceSAWord *word;
	int p;
	
	for( p=0; p<count; p++ ){
		word = ( ceSAWord* )pWords.GetAt( p );
		
		if( word->GetName().Equals( name ) ){
			return word;
		}
	}
	
	return NULL;
}

int ceSAWordList::IndexOf( ceSAWord *word ) const{
	return pWords.IndexOf( word );
}

int ceSAWordList::IndexOfNamed( const char *name ) const{
	const int count = pWords.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( ceSAWord* )pWords.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool ceSAWordList::Has( ceSAWord *word ) const{
	return pWords.Has( word );
}

bool ceSAWordList::HasNamed( const char *name ) const{
	const int count = pWords.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( ceSAWord* )pWords.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceSAWordList::Add( ceSAWord *word ){
	if( ! word || HasNamed( word->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pWords.Add( word );
}

void ceSAWordList::Remove( ceSAWord *word ){
	pWords.Remove( word );
}

void ceSAWordList::RemoveAll(){
	pWords.RemoveAll();
}



ceSAWordList &ceSAWordList::operator=( const ceSAWordList &list ){
	pWords = list.pWords;
	return *this;
}
