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

#include "gdeOCBillboard.h"
#include "gdeOCBillboardList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCBillboard
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCBillboardList::gdeOCBillboardList(){
}

gdeOCBillboardList::gdeOCBillboardList( const gdeOCBillboardList &list ) :
pBillboards( list.pBillboards ){
}

gdeOCBillboardList::~gdeOCBillboardList(){
}



// Management
///////////////

int gdeOCBillboardList::GetCount() const{
	return pBillboards.GetCount();
}

gdeOCBillboard *gdeOCBillboardList::GetAt( int index ) const{
	return ( gdeOCBillboard* )pBillboards.GetAt( index );
}

int gdeOCBillboardList::IndexOf( gdeOCBillboard *billboard ) const{
	return pBillboards.IndexOf( billboard );
}

bool gdeOCBillboardList::Has( gdeOCBillboard *billboard ) const{
	return pBillboards.Has( billboard );
}

void gdeOCBillboardList::Add( gdeOCBillboard *billboard ){
	if( ! billboard || Has( billboard ) ){
		DETHROW( deeInvalidParam );
	}
	pBillboards.Add( billboard );
}

void gdeOCBillboardList::Remove( gdeOCBillboard *billboard ){
	pBillboards.Remove( billboard );
}

void gdeOCBillboardList::RemoveAll(){
	pBillboards.RemoveAll();
}



gdeOCBillboardList &gdeOCBillboardList::operator=( const gdeOCBillboardList &list ){
	pBillboards = list.pBillboards;
	return *this;
}
