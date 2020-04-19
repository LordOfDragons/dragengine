/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seSource.h"
#include "seSourceList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seSource
///////////////////////

// Constructor, destructor
////////////////////////////

seSourceList::seSourceList(){
}

seSourceList::seSourceList( const seSourceList &copy ) :
pSources( copy.pSources ){
}

seSourceList::~seSourceList(){
}



// Management
///////////////

int seSourceList::GetCount() const{
	return pSources.GetCount();
}

seSource *seSourceList::GetAt( int index ) const{
	return ( seSource* )pSources.GetAt( index );
}

int seSourceList::IndexOf( seSource *source ) const{
	return pSources.IndexOf( source );
}

bool seSourceList::Has( seSource *source ) const{
	return pSources.Has( source );
}

void seSourceList::Add( seSource *source ){
	if( ! source || Has( source ) ){
		DETHROW( deeInvalidParam );
	}
	pSources.Add( source );
}

void seSourceList::Insert( seSource *source, int index ){
	if( ! source || Has( source ) ){
		DETHROW( deeInvalidParam );
	}
	pSources.Insert( source, index );
}

void seSourceList::Move( seSource *source, int index ){
	pSources.Move( source, index );
}

void seSourceList::Remove( seSource *source ){
	pSources.Remove( source );
}

void seSourceList::RemoveAll(){
	pSources.RemoveAll();
}



// Operators
//////////////

seSourceList &seSourceList::operator=( const seSourceList &list ){
	pSources = list.pSources;
	return *this;
}

seSourceList &seSourceList::operator+=( const seSourceList &list ){
	pSources += list.pSources;
	return *this;
}
