/* 
 * Drag[en]gine IGDE World Editor
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

#include "meObjectTexture.h"
#include "meObjectTextureList.h"

#include <dragengine/common/exceptions.h>



// Class meObjectTextureList
//////////////////////////////

// Constructor, destructor
////////////////////////////

meObjectTextureList::meObjectTextureList(){
}

meObjectTextureList::~meObjectTextureList(){
}



// Management
///////////////

int meObjectTextureList::GetTextureCount() const{
	return pTextures.GetCount();
}

meObjectTexture *meObjectTextureList::GetTextureAt( int position ) const{
	return ( meObjectTexture* )pTextures.GetAt( position );
}

int meObjectTextureList::IndexOfTexture( meObjectTexture *texture ) const{
	return pTextures.IndexOf( texture );
}

bool meObjectTextureList::HasTexture( meObjectTexture *texture ){
	return pTextures.Has( texture );
}

void meObjectTextureList::AddTexture( meObjectTexture *texture ){
	if( ! texture ) DETHROW( deeInvalidParam );
	
	pTextures.Add( texture );
}

void meObjectTextureList::AddTextureIfAbsent( meObjectTexture *texture ){
	if( ! texture ) DETHROW( deeInvalidParam );
	
	pTextures.AddIfAbsent( texture );
}

void meObjectTextureList::RemoveTexture( meObjectTexture *texture ){
	if( ! texture ) DETHROW( deeInvalidParam );
	
	pTextures.Remove( texture );
}

void meObjectTextureList::RemoveTextureIfPresent( meObjectTexture *texture ){
	if( ! texture ) DETHROW( deeInvalidParam );
	
	pTextures.RemoveIfPresent( texture );
}

void meObjectTextureList::RemoveAllTextures(){
	pTextures.RemoveAll();
}
