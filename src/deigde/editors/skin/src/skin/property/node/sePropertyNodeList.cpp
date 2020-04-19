/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "sePropertyNode.h"
#include "sePropertyNodeList.h"

#include <dragengine/common/exceptions.h>



// Class sePropertyNodeList
/////////////////////////////

// Constructor, destructor
////////////////////////////

sePropertyNodeList::sePropertyNodeList(){
}

sePropertyNodeList::sePropertyNodeList( const sePropertyNodeList &list ) : pNodes( list.pNodes ){
}

sePropertyNodeList::~sePropertyNodeList(){
}



// Management
///////////////

int sePropertyNodeList::GetCount() const{
	return pNodes.GetCount();
}

sePropertyNode *sePropertyNodeList::GetAt( int index ) const{
	return ( sePropertyNode* )pNodes.GetAt( index );
}

bool sePropertyNodeList::Has( sePropertyNode *node ) const{
	return pNodes.Has( node );
}

int sePropertyNodeList::IndexOf( sePropertyNode *node ) const{
	return pNodes.IndexOf( node );
}

void sePropertyNodeList::Add( sePropertyNode *node ){
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	pNodes.Add( node );
}

void sePropertyNodeList::AddIfAbsent( sePropertyNode *node ){
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	pNodes.AddIfAbsent( node );
}

void sePropertyNodeList::Insert( int index, sePropertyNode *node ){
	if( ! node ){
		DETHROW( deeInvalidParam );
	}
	pNodes.Insert( node, index );
}

void sePropertyNodeList::Remove( sePropertyNode *node ){
	pNodes.Remove( node );
}

void sePropertyNodeList::RemoveIfPresent( sePropertyNode *node ){
	pNodes.RemoveIfPresent( node );
}

void sePropertyNodeList::RemoveAll(){
	pNodes.RemoveAll();
}

void sePropertyNodeList::Move( sePropertyNode *node, int index ){
	pNodes.Move( node, index );
}



// Operators
//////////////

sePropertyNodeList &sePropertyNodeList::operator=( const sePropertyNodeList &list ){
	pNodes = list.pNodes;
	return *this;
}
