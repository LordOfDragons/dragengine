/* 
 * Drag[en]gine IGDE
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

#include "igdeGDCCTexture.h"
#include "igdeGDCCTextureList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCCTexture
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCCTextureList::igdeGDCCTextureList(){
}

igdeGDCCTextureList::igdeGDCCTextureList( const igdeGDCCTextureList &list ){
	pTextures = list.pTextures;
}

igdeGDCCTextureList::~igdeGDCCTextureList(){
}



// Management
///////////////

int igdeGDCCTextureList::GetCount() const{
	return pTextures.GetCount();
}

igdeGDCCTexture *igdeGDCCTextureList::GetAt( int index ) const{
	return ( igdeGDCCTexture* )pTextures.GetAt( index );
}

igdeGDCCTexture *igdeGDCCTextureList::GetNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	igdeGDCCTexture *texture;
	int i;
	
	for( i=0; i<count; i++ ){
		texture = ( igdeGDCCTexture* )pTextures.GetAt( i );
		
		if( texture->GetName().Equals( name ) ){
			return texture;
		}
	}
	
	return NULL;
}

int igdeGDCCTextureList::IndexOf( igdeGDCCTexture *texture ) const{
	return pTextures.IndexOf( texture );
}

int igdeGDCCTextureList::IndexOfNamed( const char *name ) const{
	const int count = pTextures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDCCTexture* )pTextures.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool igdeGDCCTextureList::Has( igdeGDCCTexture *texture ) const{
	return pTextures.Has( texture );
}

bool igdeGDCCTextureList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

void igdeGDCCTextureList::Add( igdeGDCCTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	pTextures.Add( texture );
}

void igdeGDCCTextureList::InsertAt( igdeGDCCTexture *texture, int index ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	pTextures.Insert( texture, index );
}

void igdeGDCCTextureList::MoveTo( igdeGDCCTexture *texture, int index ){
	pTextures.Move( texture, index );
}

void igdeGDCCTextureList::Remove( igdeGDCCTexture *texture ){
	pTextures.Remove( texture );
}

void igdeGDCCTextureList::RemoveAll(){
	pTextures.RemoveAll();
}



void igdeGDCCTextureList::SetToDeepCopyFrom( const igdeGDCCTextureList &list ){
	const int count = list.GetCount();
	igdeGDCCTexture *texture = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			texture = new igdeGDCCTexture( *list.GetAt( i ) );
			Add( texture );
			texture->FreeReference();
			texture = NULL;
		}
		
	}catch( const deException & ){
		if( texture ){
			texture->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCCTextureList &igdeGDCCTextureList::operator=( const igdeGDCCTextureList &list ){
	pTextures = list.pTextures;
	return *this;
}
