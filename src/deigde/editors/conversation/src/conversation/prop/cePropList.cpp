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

#include "ceProp.h"
#include "cePropList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceProp
/////////////////

// Constructor, destructor
////////////////////////////

cePropList::cePropList(){
}

cePropList::~cePropList(){
}



// Management
///////////////

int cePropList::GetCount() const{
	return pProps.GetCount();
}

ceProp *cePropList::GetAt( int index ) const{
	return ( ceProp* )pProps.GetAt( index );
}

int cePropList::IndexOf( ceProp *prop ) const{
	return pProps.IndexOf( prop );
}

bool cePropList::Has( ceProp *prop ) const{
	return pProps.Has( prop );
}

void cePropList::Add( ceProp *prop ){
	if( ! prop ){
		DETHROW( deeInvalidParam );
	}
	
	pProps.Add( prop );
}

void cePropList::Remove( ceProp *prop ){
	pProps.Remove( prop );
}

void cePropList::RemoveAll(){
	pProps.RemoveAll();
}



cePropList &cePropList::operator=( const cePropList &list ){
	pProps = list.pProps;
	return *this;
}
