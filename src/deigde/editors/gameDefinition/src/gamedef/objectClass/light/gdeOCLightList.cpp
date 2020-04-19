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

#include "gdeOCLight.h"
#include "gdeOCLightList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCLight
/////////////////////

// Constructor, destructor
////////////////////////////

gdeOCLightList::gdeOCLightList(){
}

gdeOCLightList::gdeOCLightList( const gdeOCLightList &list ) :
pLights( list.pLights ){
}

gdeOCLightList::~gdeOCLightList(){
}



// Management
///////////////

int gdeOCLightList::GetCount() const{
	return pLights.GetCount();
}

gdeOCLight *gdeOCLightList::GetAt( int index ) const{
	return ( gdeOCLight* )pLights.GetAt( index );
}

int gdeOCLightList::IndexOf( gdeOCLight *light ) const{
	return pLights.IndexOf( light );
}

bool gdeOCLightList::Has( gdeOCLight *light ) const{
	return pLights.Has( light );
}

void gdeOCLightList::Add( gdeOCLight *light ){
	if( ! light || Has( light ) ){
		DETHROW( deeInvalidParam );
	}
	pLights.Add( light );
}

void gdeOCLightList::Remove( gdeOCLight *light ){
	pLights.Remove( light );
}

void gdeOCLightList::RemoveAll(){
	pLights.RemoveAll();
}



gdeOCLightList &gdeOCLightList::operator=( const gdeOCLightList &list ){
	pLights = list.pLights;
	return *this;
}
