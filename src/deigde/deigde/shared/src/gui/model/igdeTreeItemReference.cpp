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

#include "igdeTreeItem.h"
#include "igdeTreeItemReference.h"



// Class igdeTreeItemReference
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTreeItemReference::igdeTreeItemReference(){
}

igdeTreeItemReference::igdeTreeItemReference( igdeTreeItem *treeItem ) :
deObjectReference( treeItem ){
}

igdeTreeItemReference::igdeTreeItemReference( const igdeTreeItemReference &reference ) :
deObjectReference( reference ){
}



// Management
///////////////

void igdeTreeItemReference::TakeOver( igdeTreeItem *treeItem ){
	deObjectReference::TakeOver( treeItem );
}

bool igdeTreeItemReference::operator!() const{
	return deObjectReference::operator!();
}

igdeTreeItemReference::operator bool() const{
	return deObjectReference::operator bool();
}

igdeTreeItemReference::operator igdeTreeItem *() const{
	return ( igdeTreeItem* )operator deObject *();
}

igdeTreeItemReference::operator igdeTreeItem &() const{
	return ( igdeTreeItem& )operator deObject &();
}

igdeTreeItem *igdeTreeItemReference::operator->() const{
	return ( igdeTreeItem* )deObjectReference::operator->();
}

igdeTreeItemReference &igdeTreeItemReference::operator=( igdeTreeItem *treeItem ){
	deObjectReference::operator=( treeItem );
	return *this;
}

igdeTreeItemReference &igdeTreeItemReference::operator=( const igdeTreeItemReference &reference ){
	deObjectReference::operator=( reference );
	return *this;
}

bool igdeTreeItemReference::operator==( igdeTreeItem *treeItem ) const{
	return deObjectReference::operator==( treeItem );
}

bool igdeTreeItemReference::operator!=( igdeTreeItem *treeItem ) const{
	return deObjectReference::operator!=( treeItem );
}

bool igdeTreeItemReference::operator==( const igdeTreeItemReference &reference ) const{
	return deObjectReference::operator==( reference );
}

bool igdeTreeItemReference::operator!=( const igdeTreeItemReference &reference ) const{
	return deObjectReference::operator!=( reference );
}
