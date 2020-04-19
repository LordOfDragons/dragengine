/* 
 * Drag[en]gine IGDE World Editor
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

#include "meObject.h"
#include "meObjectList.h"

#include <dragengine/common/exceptions.h>



// Class meObjectList
/////////////////////////

// Constructor, destructor
////////////////////////////

meObjectList::meObjectList(){
}

meObjectList::~meObjectList(){
}



// Management
///////////////

int meObjectList::GetCount() const{
	return pObjects.GetCount();
}

meObject *meObjectList::GetAt( int position ) const{
	return ( meObject* )pObjects.GetAt( position );
}

int meObjectList::IndexOf( meObject *object ) const{
	return pObjects.IndexOf( object );
}

bool meObjectList::Has( meObject *object ) const{
	return pObjects.Has( object );
}

void meObjectList::Add( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.Add( object );
}

void meObjectList::AddIfAbsent( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.AddIfAbsent( object );
}

void meObjectList::Remove( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.Remove( object );
}

void meObjectList::RemoveIfPresent( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.RemoveIfPresent( object );
}

void meObjectList::RemoveAll(){
	pObjects.RemoveAll();
}



meObjectList &meObjectList::operator=( const meObjectList &list ){
	pObjects = list.pObjects;
	return *this;
}
