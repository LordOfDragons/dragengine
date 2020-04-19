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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDCSnapPoint.h"
#include "igdeGDCSnapPointList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class igdeGDCSnapPoint
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCSnapPointList::igdeGDCSnapPointList(){
}

igdeGDCSnapPointList::igdeGDCSnapPointList( const igdeGDCSnapPointList &list ) :
pSnapPoints( list.pSnapPoints ){
}

igdeGDCSnapPointList::~igdeGDCSnapPointList(){
}



// Management
///////////////

int igdeGDCSnapPointList::GetCount() const{
	return pSnapPoints.GetCount();
}

igdeGDCSnapPoint *igdeGDCSnapPointList::GetAt( int index ) const{
	return ( igdeGDCSnapPoint* )pSnapPoints.GetAt( index );
}

int igdeGDCSnapPointList::IndexOf( igdeGDCSnapPoint *snapPoint ) const{
	return pSnapPoints.IndexOf( snapPoint );
}

bool igdeGDCSnapPointList::Has( igdeGDCSnapPoint *snapPoint ) const{
	return pSnapPoints.Has( snapPoint );
}

void igdeGDCSnapPointList::Add( igdeGDCSnapPoint *snapPoint ){
	if( ! snapPoint || Has( snapPoint ) ){
		DETHROW( deeInvalidParam );
	}
	pSnapPoints.Add( snapPoint );
}

void igdeGDCSnapPointList::Remove( igdeGDCSnapPoint *snapPoint ){
	pSnapPoints.Remove( snapPoint );
}

void igdeGDCSnapPointList::RemoveAll(){
	pSnapPoints.RemoveAll();
}



igdeGDCSnapPointList &igdeGDCSnapPointList::operator=( const igdeGDCSnapPointList &list ){
	pSnapPoints = list.pSnapPoints;
	return *this;
}
