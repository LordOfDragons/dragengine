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

#include "saePhoneme.h"
#include "saePhonemeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class saePhoneme
/////////////////////

// Constructor, destructor
////////////////////////////

saePhonemeList::saePhonemeList(){
}

saePhonemeList::~saePhonemeList(){
}



// Management
///////////////

int saePhonemeList::GetCount() const{
	return pPhonemes.GetCount();
}

saePhoneme *saePhonemeList::GetAt( int index ) const{
	return ( saePhoneme* )pPhonemes.GetAt( index );
}

saePhoneme *saePhonemeList::GetIPA( int ipa ) const{
	const int count = pPhonemes.GetCount();
	saePhoneme *phoneme;
	int p;
	
	for( p=0; p<count; p++ ){
		phoneme = ( saePhoneme* )pPhonemes.GetAt( p );
		
		if( phoneme->GetIPA() == ipa ){
			return phoneme;
		}
	}
	
	return NULL;
}

int saePhonemeList::IndexOf( saePhoneme *phoneme ) const{
	return pPhonemes.IndexOf( phoneme );
}

int saePhonemeList::IndexOfIPA( int ipa ) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( saePhoneme* )pPhonemes.GetAt( p ) )->GetIPA() == ipa ){
			return p;
		}
	}
	
	return -1;
}

bool saePhonemeList::Has( saePhoneme *phoneme ) const{
	return pPhonemes.Has( phoneme );
}

bool saePhonemeList::HasIPA( int ipa ) const{
	const int count = pPhonemes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( saePhoneme* )pPhonemes.GetAt( p ) )->GetIPA() == ipa ){
			return true;
		}
	}
	
	return false;
}

void saePhonemeList::Add( saePhoneme *phoneme ){
	if( ! phoneme || HasIPA( phoneme->GetIPA() ) ) DETHROW( deeInvalidParam );
	
	pPhonemes.Add( phoneme );
}

void saePhonemeList::Remove( saePhoneme *phoneme ){
	pPhonemes.Remove( phoneme );
}

void saePhonemeList::RemoveAll(){
	pPhonemes.RemoveAll();
}



saePhonemeList &saePhonemeList::operator=( const saePhonemeList &list ){
	pPhonemes = list.pPhonemes;
	return *this;
}
