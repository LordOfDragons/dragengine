/* 
 * Drag[en]gine IGDE Skin Editor
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

#include "seTexture.h"
#include "seTextureList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seTexture
////////////////////

// Constructor, destructor
////////////////////////////

seTextureList::seTextureList(){
}

seTextureList::~seTextureList(){
}



// Management
///////////////

int seTextureList::GetCount() const{
	return pTextures.GetCount();
}

seTexture *seTextureList::GetAt( int index ) const{
	return ( seTexture* )pTextures.GetAt( index );
}

seTexture *seTextureList::GetNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	seTexture *texture;
	int p;
	
	for( p=0; p<count; p++ ){
		texture = ( seTexture* )pTextures.GetAt( p );
		
		if( texture->GetName().Equals( name ) ){
			return texture;
		}
	}
	
	return NULL;
}

int seTextureList::IndexOf( seTexture *texture ) const{
	return pTextures.IndexOf( texture );
}

int seTextureList::IndexOfNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seTexture* )pTextures.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool seTextureList::Has( seTexture *texture ) const{
	return pTextures.Has( texture );
}

bool seTextureList::HasNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seTexture* )pTextures.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void seTextureList::Add( seTexture *texture ){
	if( ! texture || HasNamed( texture->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pTextures.Add( texture );
}

void seTextureList::InsertAt( seTexture *texture, int index ){
	if( ! texture || HasNamed( texture->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pTextures.Insert( texture, index );
}

void seTextureList::MoveTo( seTexture *texture, int index ){
	pTextures.Move( texture, index );
}

void seTextureList::Remove( seTexture *texture ){
	pTextures.Remove( texture );
}

void seTextureList::RemoveAll(){
	pTextures.RemoveAll();
}



seTextureList &seTextureList::operator=( const seTextureList &list ){
	pTextures = list.pTextures;
	return *this;
}
