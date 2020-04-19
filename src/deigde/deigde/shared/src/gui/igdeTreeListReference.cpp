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

#include "igdeTreeList.h"
#include "igdeTreeListReference.h"



// Class igdeTreeListReference
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeListReference::igdeTreeListReference(){
}

igdeTreeListReference::igdeTreeListReference( igdeTreeList *treeList ) :
deObjectReference( treeList ){
}

igdeTreeListReference::igdeTreeListReference( const igdeTreeListReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeListReference::TakeOver( igdeTreeList *treeList ){
	deObjectReference::TakeOver( treeList );
}

bool igdeTreeListReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeListReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeListReference::operator igdeTreeList *() const{
	return ( igdeTreeList* )operator deObject *();
}

igdeTreeListReference::operator igdeTreeList &() const{
	return ( igdeTreeList& )operator deObject &();
}

igdeTreeList *igdeTreeListReference::operator->() const{
	return ( igdeTreeList* )deObjectReference::operator->();
}

igdeTreeListReference &igdeTreeListReference::operator=( igdeTreeList *treeList ){
	deObjectReference::operator=( treeList );
	return *this;
}

igdeTreeListReference &igdeTreeListReference::operator=( const igdeTreeListReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeListReference::operator==( igdeTreeList *treeList ) const{
	return deObjectReference::operator==( treeList );
}

bool igdeTreeListReference::operator!=( igdeTreeList *treeList ) const{
	return deObjectReference::operator!=( treeList );
}

bool igdeTreeListReference::operator==( const igdeTreeListReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeListReference::operator!=( const igdeTreeListReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
