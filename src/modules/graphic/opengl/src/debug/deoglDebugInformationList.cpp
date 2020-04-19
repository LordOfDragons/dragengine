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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "deoglDebugInformation.h"
#include "deoglDebugInformationList.h"

#include <dragengine/common/exceptions.h>



// Class deoglDebugInformationList
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugInformationList::deoglDebugInformationList(){
}

deoglDebugInformationList::~deoglDebugInformationList(){
}



// Management
///////////////

int deoglDebugInformationList::GetCount() const{
	return pChildren.GetCount();
}

deoglDebugInformation *deoglDebugInformationList::GetAt( int index ) const{
	return ( deoglDebugInformation* )pChildren.GetAt( index );
}

void deoglDebugInformationList::Add( deoglDebugInformation *child ){
	pChildren.Add( child );
}

void deoglDebugInformationList::Remove( deoglDebugInformation *child ){
	pChildren.Remove( child );
}

void deoglDebugInformationList::RemoveIfPresent( deoglDebugInformation *child ){
	pChildren.RemoveIfPresent( child );
}

void deoglDebugInformationList::RemoveAll(){
	pChildren.RemoveAll();
}
