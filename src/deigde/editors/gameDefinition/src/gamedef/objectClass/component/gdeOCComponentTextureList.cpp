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

#include "gdeOCComponentTexture.h"
#include "gdeOCComponentTextureList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCComponentTexture
////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCComponentTextureList::gdeOCComponentTextureList(){
}

gdeOCComponentTextureList::gdeOCComponentTextureList( const gdeOCComponentTextureList &list ) :
pTextures( list.pTextures ){
}

gdeOCComponentTextureList::~gdeOCComponentTextureList(){
}



// Management
///////////////

int gdeOCComponentTextureList::GetCount() const{
	return pTextures.GetCount();
}

gdeOCComponentTexture *gdeOCComponentTextureList::GetAt( int index ) const{
	return ( gdeOCComponentTexture* )pTextures.GetAt( index );
}

gdeOCComponentTexture *gdeOCComponentTextureList::GetNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeOCComponentTexture * const texture = ( gdeOCComponentTexture* )pTextures.GetAt( i );
		
		if( texture->GetName() == name ){
			return texture;
		}
	}
	
	return NULL;
}

int gdeOCComponentTextureList::IndexOf( gdeOCComponentTexture *texture ) const{
	return pTextures.IndexOf( texture );
}

int gdeOCComponentTextureList::IndexOfNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeOCComponentTexture* )pTextures.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool gdeOCComponentTextureList::Has( gdeOCComponentTexture *texture ) const{
	return pTextures.Has( texture );
}

bool gdeOCComponentTextureList::HasNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeOCComponentTexture* )pTextures.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void gdeOCComponentTextureList::Add( gdeOCComponentTexture *texture ){
	if( ! texture || HasNamed( texture->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTextures.Add( texture );
}

void gdeOCComponentTextureList::Remove( gdeOCComponentTexture *texture ){
	pTextures.Remove( texture );
}

void gdeOCComponentTextureList::RemoveAll(){
	pTextures.RemoveAll();
}



gdeOCComponentTextureList &gdeOCComponentTextureList::operator=( const gdeOCComponentTextureList &list ){
	pTextures = list.pTextures;
	return *this;
}
