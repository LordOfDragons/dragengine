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

#include "gdeOCComponent.h"
#include "gdeOCComponentList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCComponent
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCComponentList::gdeOCComponentList(){
}

gdeOCComponentList::gdeOCComponentList( const gdeOCComponentList &list ) :
pComponents( list.pComponents ){
}

gdeOCComponentList::~gdeOCComponentList(){
}



// Management
///////////////

int gdeOCComponentList::GetCount() const{
	return pComponents.GetCount();
}

gdeOCComponent *gdeOCComponentList::GetAt( int index ) const{
	return ( gdeOCComponent* )pComponents.GetAt( index );
}

int gdeOCComponentList::IndexOf( gdeOCComponent *component ) const{
	return pComponents.IndexOf( component );
}

bool gdeOCComponentList::Has( gdeOCComponent *component ) const{
	return pComponents.Has( component );
}

void gdeOCComponentList::Add( gdeOCComponent *component ){
	if( ! component || Has( component ) ){
		DETHROW( deeInvalidParam );
	}
	
	pComponents.Add( component );
}

void gdeOCComponentList::Remove( gdeOCComponent *component ){
	pComponents.Remove( component );
}

void gdeOCComponentList::RemoveAll(){
	pComponents.RemoveAll();
}



gdeOCComponentList &gdeOCComponentList::operator=( const gdeOCComponentList &list ){
	pComponents = list.pComponents;
	return *this;
}
