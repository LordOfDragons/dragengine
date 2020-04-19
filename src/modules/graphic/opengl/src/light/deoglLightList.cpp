/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglRLight.h"
#include "deoglLightList.h"

#include <dragengine/common/exceptions.h>



// Class deoglLightList
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightList::deoglLightList(){
}

deoglLightList::deoglLightList( const deoglLightList &copy ) :
pLights( copy.pLights ){
}

deoglLightList::~deoglLightList(){
}



// Management
///////////////

int deoglLightList::GetCount() const{
	return pLights.GetCount();
}

deoglRLight *deoglLightList::GetAt( int index ) const{
	return ( deoglRLight* )pLights.GetAt( index );
}

bool deoglLightList::Has( deoglRLight *light ) const{
	return pLights.Has( light );
}

void deoglLightList::Add( deoglRLight *light ){
	pLights.Add( light );
}

bool deoglLightList::AddIfMissing( deoglRLight *light ){
	const int count = pLights.GetCount();
	pLights.AddIfAbsent( light );
	return pLights.GetCount() > count;
}

void deoglLightList::Remove( deoglRLight *light ){
	pLights.Remove( light );
}

bool deoglLightList::RemoveIfExisting( deoglRLight *light ){
	const int count = pLights.GetCount();
	pLights.RemoveIfPresent( light );
	return pLights.GetCount() < count;
}

void deoglLightList::RemoveAll(){
	pLights.RemoveAll();
}
