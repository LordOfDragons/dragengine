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

#include "gdeOCSnapPoint.h"
#include "gdeOCSnapPointList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCSnapPoint
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCSnapPointList::gdeOCSnapPointList(){
}

gdeOCSnapPointList::gdeOCSnapPointList( const gdeOCSnapPointList &list ) :
pSnapPoints( list.pSnapPoints ){
}

gdeOCSnapPointList::~gdeOCSnapPointList(){
}



// Management
///////////////

int gdeOCSnapPointList::GetCount() const{
	return pSnapPoints.GetCount();
}

gdeOCSnapPoint *gdeOCSnapPointList::GetAt( int index ) const{
	return ( gdeOCSnapPoint* )pSnapPoints.GetAt( index );
}

int gdeOCSnapPointList::IndexOf( gdeOCSnapPoint *snapPoint ) const{
	return pSnapPoints.IndexOf( snapPoint );
}

bool gdeOCSnapPointList::Has( gdeOCSnapPoint *snapPoint ) const{
	return pSnapPoints.Has( snapPoint );
}

void gdeOCSnapPointList::Add( gdeOCSnapPoint *snapPoint ){
	if( ! snapPoint || Has( snapPoint ) ){
		DETHROW( deeInvalidParam );
	}
	pSnapPoints.Add( snapPoint );
}

void gdeOCSnapPointList::Remove( gdeOCSnapPoint *snapPoint ){
	pSnapPoints.Remove( snapPoint );
}

void gdeOCSnapPointList::RemoveAll(){
	pSnapPoints.RemoveAll();
}



gdeOCSnapPointList &gdeOCSnapPointList::operator=( const gdeOCSnapPointList &list ){
	pSnapPoints = list.pSnapPoints;
	return *this;
}
