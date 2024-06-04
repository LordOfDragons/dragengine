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
