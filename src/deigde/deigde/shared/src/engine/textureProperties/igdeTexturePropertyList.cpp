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

#include <stdlib.h>
#include <string.h>

#include "igdeTextureProperty.h"
#include "igdeTexturePropertyList.h"

#include <dragengine/common/exceptions.h>



// Class igdeTextureProperty
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTexturePropertyList::igdeTexturePropertyList(){
}

igdeTexturePropertyList::~igdeTexturePropertyList(){
	RemoveAll();
}



// Management
///////////////

int igdeTexturePropertyList::GetCount() const{
	return pProperties.GetCount();
}

const igdeTextureProperty *igdeTexturePropertyList::GetAt( int index ) const{
	return ( const igdeTextureProperty * )pProperties.GetAt( index );
}

const igdeTextureProperty *igdeTexturePropertyList::GetNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	const igdeTextureProperty *texture;
	int p;
	
	for( p=0; p<count; p++ ){
		texture = ( const igdeTextureProperty * )pProperties.GetAt( p );
		
		if( texture->GetName().Equals( name ) ){
			return texture;
		}
	}
	
	return NULL;
}

int igdeTexturePropertyList::IndexOf( const igdeTextureProperty * texture ) const{
	return pProperties.IndexOf( ( void* )texture );
}

int igdeTexturePropertyList::IndexOfNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( const igdeTextureProperty * )pProperties.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool igdeTexturePropertyList::Has( const igdeTextureProperty * texture ) const{
	return pProperties.Has( ( void* )texture );
}

bool igdeTexturePropertyList::HasNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( const igdeTextureProperty * )pProperties.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void igdeTexturePropertyList::Add( igdeTextureProperty *texture ){
	if( ! texture || HasNamed( texture->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pProperties.Add( texture );
}

void igdeTexturePropertyList::Remove( const igdeTextureProperty *texture ){
	pProperties.RemoveFrom( IndexOf( texture ) );
	delete texture;
}

void igdeTexturePropertyList::RemoveAll(){
	const int count = pProperties.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		delete ( igdeTextureProperty* )pProperties.GetAt( p );
	}
	
	pProperties.RemoveAll();
}
