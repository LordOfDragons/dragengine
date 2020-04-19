/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#include "peeType.h"
#include "peeTypeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class peeType
//////////////////

// Constructor, destructor
////////////////////////////

peeTypeList::peeTypeList(){
}

peeTypeList::~peeTypeList(){
}



// Management
///////////////

int peeTypeList::GetCount() const{
	return pTypes.GetCount();
}

peeType *peeTypeList::GetAt( int index ) const{
	return ( peeType* )pTypes.GetAt( index );
}

peeType *peeTypeList::GetNamed( const char *name ) const{
	const int count = pTypes.GetCount();
	peeType *type;
	int p;
	
	for( p=0; p<count; p++ ){
		type = ( peeType* )pTypes.GetAt( p );
		
		if( type->GetName().Equals( name ) ){
			return type;
		}
	}
	
	return NULL;
}

int peeTypeList::IndexOf( peeType *type ) const{
	return pTypes.IndexOf( type );
}

int peeTypeList::IndexOfNamed( const char *name ) const{
	const int count = pTypes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( peeType* )pTypes.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool peeTypeList::Has( peeType *type ) const{
	return pTypes.Has( type );
}

bool peeTypeList::HasNamed( const char *name ) const{
	const int count = pTypes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( peeType* )pTypes.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void peeTypeList::Add( peeType *type ){
	if( ! type || HasNamed( type->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTypes.Add( type );
}

void peeTypeList::InsertAt( peeType *type, int index ){
	if( ! type || HasNamed( type->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTypes.Insert( type, index );
}

void peeTypeList::MoveTo( peeType *type, int index ){
	pTypes.Move( type, index );
}

void peeTypeList::Remove( peeType *type ){
	pTypes.Remove( type );
}

void peeTypeList::RemoveAll(){
	pTypes.RemoveAll();
}



peeTypeList &peeTypeList::operator=( const peeTypeList &list ){
	pTypes = list.pTypes;
	return *this;
}
