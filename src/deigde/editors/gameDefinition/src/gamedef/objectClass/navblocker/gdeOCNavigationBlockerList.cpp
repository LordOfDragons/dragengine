/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeOCNavigationBlocker.h"
#include "gdeOCNavigationBlockerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCNavigationBlocker
/////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCNavigationBlockerList::gdeOCNavigationBlockerList(){
}

gdeOCNavigationBlockerList::gdeOCNavigationBlockerList( const gdeOCNavigationBlockerList &list ) :
pNavigationBlockers( list.pNavigationBlockers ){
}

gdeOCNavigationBlockerList::~gdeOCNavigationBlockerList(){
}



// Management
///////////////

int gdeOCNavigationBlockerList::GetCount() const{
	return pNavigationBlockers.GetCount();
}

gdeOCNavigationBlocker *gdeOCNavigationBlockerList::GetAt( int index ) const{
	return ( gdeOCNavigationBlocker* )pNavigationBlockers.GetAt( index );
}

int gdeOCNavigationBlockerList::IndexOf( gdeOCNavigationBlocker *blocker ) const{
	return pNavigationBlockers.IndexOf( blocker );
}

bool gdeOCNavigationBlockerList::Has( gdeOCNavigationBlocker *blocker ) const{
	return pNavigationBlockers.Has( blocker );
}

void gdeOCNavigationBlockerList::Add( gdeOCNavigationBlocker *blocker ){
	if( ! blocker || Has( blocker ) ){
		DETHROW( deeInvalidParam );
	}
	pNavigationBlockers.Add( blocker );
}

void gdeOCNavigationBlockerList::Remove( gdeOCNavigationBlocker *blocker ){
	pNavigationBlockers.Remove( blocker );
}

void gdeOCNavigationBlockerList::RemoveAll(){
	pNavigationBlockers.RemoveAll();
}



gdeOCNavigationBlockerList &gdeOCNavigationBlockerList::operator=( const gdeOCNavigationBlockerList &list ){
	pNavigationBlockers = list.pNavigationBlockers;
	return *this;
}
