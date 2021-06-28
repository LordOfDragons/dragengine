/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglComponentList.h"

#include <dragengine/common/exceptions.h>



// Class deoglComponentList
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponentList::deoglComponentList(){
}

deoglComponentList::~deoglComponentList(){
}



// Management
///////////////

int deoglComponentList::GetCount() const{
	return pComponents.GetCount();
}

deoglRComponent *deoglComponentList::GetAt( int index ) const{
	return ( deoglRComponent* )pComponents.GetAt( index );
}

void deoglComponentList::Add( deoglRComponent *component ){
	pComponents.Add( component );
}

void deoglComponentList::RemoveFrom( int index ){
	pComponents.RemoveFrom( index );
}

void deoglComponentList::RemoveAll(){
	pComponents.RemoveAll();
}
