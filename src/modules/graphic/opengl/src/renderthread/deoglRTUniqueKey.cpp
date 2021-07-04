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

#include "deoglRTUniqueKey.h"



// Class deoglRTUniqueKey
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTUniqueKey::deoglRTUniqueKey() :
pNextKey( 0 ){
}

deoglRTUniqueKey::~deoglRTUniqueKey(){
}



// Management
///////////////

unsigned int deoglRTUniqueKey::Get(){
	const int index = pFreeKeys.GetCount() - 1;
	unsigned int key;
	
	if( index >= 0 ){
		key = ( unsigned int )pFreeKeys.GetAt( index );
		pFreeKeys.RemoveFrom( index );
		
	}else{
		key = pNextKey++;
	}
	
	return key;
}

void deoglRTUniqueKey::Return( unsigned int key ){
	pFreeKeys.Add( ( int )key );
}
