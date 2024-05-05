/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
