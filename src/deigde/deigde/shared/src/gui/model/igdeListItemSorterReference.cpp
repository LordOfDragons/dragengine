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

#include "igdeListItemSorter.h"
#include "igdeListItemSorterReference.h"



// Class igdeListItemSorterReference
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeListItemSorterReference::igdeListItemSorterReference(){
}

igdeListItemSorterReference::igdeListItemSorterReference( igdeListItemSorter *sorter ) :
deObjectReference( sorter ){
}

igdeListItemSorterReference::igdeListItemSorterReference( const igdeListItemSorterReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeListItemSorterReference::TakeOver( igdeListItemSorter *sorter ){
	deObjectReference::TakeOver( sorter );
}

bool igdeListItemSorterReference::operator!() const{
	return deObjectReference::operator!();
}

igdeListItemSorterReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeListItemSorterReference::operator igdeListItemSorter *() const{
	return ( igdeListItemSorter* )operator deObject *();
}

igdeListItemSorterReference::operator igdeListItemSorter &() const{
	return ( igdeListItemSorter& )operator deObject &();
}

igdeListItemSorter *igdeListItemSorterReference::operator->() const{
	return ( igdeListItemSorter* )deObjectReference::operator->();
}

igdeListItemSorterReference &igdeListItemSorterReference::operator=( igdeListItemSorter *sorter ){
	deObjectReference::operator=( sorter );
	return *this;
}

igdeListItemSorterReference &igdeListItemSorterReference::operator=( const igdeListItemSorterReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeListItemSorterReference::operator==( igdeListItemSorter *sorter ) const{
	return deObjectReference::operator==( sorter );
}

bool igdeListItemSorterReference::operator!=( igdeListItemSorter *sorter ) const{
	return deObjectReference::operator!=( sorter );
}

bool igdeListItemSorterReference::operator==( const igdeListItemSorterReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeListItemSorterReference::operator!=( const igdeListItemSorterReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
