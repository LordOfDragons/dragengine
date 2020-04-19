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

#include "igdeTreeItemSorter.h"
#include "igdeTreeItemSorterReference.h"



// Class igdeTreeItemSorterReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeItemSorterReference::igdeTreeItemSorterReference(){
}

igdeTreeItemSorterReference::igdeTreeItemSorterReference( igdeTreeItemSorter *sorter ) :
deObjectReference( sorter ){
}

igdeTreeItemSorterReference::igdeTreeItemSorterReference( const igdeTreeItemSorterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeItemSorterReference::TakeOver( igdeTreeItemSorter *sorter ){
	deObjectReference::TakeOver( sorter );
}

bool igdeTreeItemSorterReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeItemSorterReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeItemSorterReference::operator igdeTreeItemSorter *() const{
	return ( igdeTreeItemSorter* )operator deObject *();
}

igdeTreeItemSorterReference::operator igdeTreeItemSorter &() const{
	return ( igdeTreeItemSorter& )operator deObject &();
}

igdeTreeItemSorter *igdeTreeItemSorterReference::operator->() const{
	return ( igdeTreeItemSorter* )deObjectReference::operator->();
}

igdeTreeItemSorterReference &igdeTreeItemSorterReference::operator=( igdeTreeItemSorter *sorter ){
	deObjectReference::operator=( sorter );
	return *this;
}

igdeTreeItemSorterReference &igdeTreeItemSorterReference::operator=( const igdeTreeItemSorterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeItemSorterReference::operator==( igdeTreeItemSorter *sorter ) const{
	return deObjectReference::operator==( sorter );
}

bool igdeTreeItemSorterReference::operator!=( igdeTreeItemSorter *sorter ) const{
	return deObjectReference::operator!=( sorter );
}

bool igdeTreeItemSorterReference::operator==( const igdeTreeItemSorterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeItemSorterReference::operator!=( const igdeTreeItemSorterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
