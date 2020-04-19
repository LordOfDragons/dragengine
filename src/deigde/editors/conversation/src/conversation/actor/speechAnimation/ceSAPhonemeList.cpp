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

#include "ceSAPhoneme.h"
#include "ceSAPhonemeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceSAPhoneme
//////////////////////

// Constructor, destructor
////////////////////////////

ceSAPhonemeList::ceSAPhonemeList(){
}

ceSAPhonemeList::~ceSAPhonemeList(){
}



// Management
///////////////

int ceSAPhonemeList::GetCount() const{
	return pPhonemes.GetCount();
}

ceSAPhoneme *ceSAPhonemeList::GetAt( int index ) const{
	return ( ceSAPhoneme* )pPhonemes.GetAt( index );
}

ceSAPhoneme *ceSAPhonemeList::GetIPA( int ipa ) const{
	const int count = pPhonemes.GetCount();
	ceSAPhoneme *phoneme;
	int p;
	
	for( p=0; p<count; p++ ){
		phoneme = ( ceSAPhoneme* )pPhonemes.GetAt( p );
		
		if( phoneme->GetIPA() == ipa ){
			return phoneme;
		}
	}
	
	return NULL;
}

int ceSAPhonemeList::IndexOf( ceSAPhoneme *phoneme ) const{
	return pPhonemes.IndexOf( phoneme );
}

int ceSAPhonemeList::IndexOfIPA( int ipa ) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( ceSAPhoneme* )pPhonemes.GetAt( p ) )->GetIPA() == ipa ){
			return p;
		}
	}
	
	return -1;
}

bool ceSAPhonemeList::Has( ceSAPhoneme *phoneme ) const{
	return pPhonemes.Has( phoneme );
}

bool ceSAPhonemeList::HasIPA( int ipa ) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( ceSAPhoneme* )pPhonemes.GetAt( p ) )->GetIPA() == ipa ){
			return true;
		}
	}
	
	return false;
}

void ceSAPhonemeList::Add( ceSAPhoneme *phoneme ){
	if( ! phoneme || HasIPA( phoneme->GetIPA() ) ) DETHROW( deeInvalidParam );
	
	pPhonemes.Add( phoneme );
}

void ceSAPhonemeList::Remove( ceSAPhoneme *phoneme ){
	pPhonemes.Remove( phoneme );
}

void ceSAPhonemeList::RemoveAll(){
	pPhonemes.RemoveAll();
}



ceSAPhonemeList &ceSAPhonemeList::operator=( const ceSAPhonemeList &list ){
	pPhonemes = list.pPhonemes;
	return *this;
}
