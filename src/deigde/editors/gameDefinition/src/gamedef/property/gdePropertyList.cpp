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

#include "gdeProperty.h"
#include "gdePropertyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeProperty
//////////////////////

// Constructor, destructor
////////////////////////////

gdePropertyList::gdePropertyList(){
}

gdePropertyList::gdePropertyList( const gdePropertyList &list ) :
pProperties( list.pProperties ){
}

gdePropertyList::~gdePropertyList(){
}



// Management
///////////////

int gdePropertyList::GetCount() const{
	return pProperties.GetCount();
}

gdeProperty *gdePropertyList::GetAt( int index ) const{
	return ( gdeProperty* )pProperties.GetAt( index );
}

gdeProperty *gdePropertyList::GetNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		gdeProperty * const property = ( gdeProperty* )pProperties.GetAt( i );
		
		if( property->GetName() == name ){
			return property;
		}
	}
	
	return NULL;
}

int gdePropertyList::IndexOf( gdeProperty *property ) const{
	return pProperties.IndexOf( property );
}

int gdePropertyList::IndexOfNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool gdePropertyList::Has( gdeProperty *property ) const{
	return pProperties.Has( property );
}

bool gdePropertyList::HasNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( gdeProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void gdePropertyList::Add( gdeProperty *property ){
	if( ! property || HasNamed( property->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pProperties.Add( property );
}

void gdePropertyList::Remove( gdeProperty *property ){
	pProperties.Remove( property );
}

void gdePropertyList::RemoveAll(){
	pProperties.RemoveAll();
}



gdePropertyList &gdePropertyList::operator=( const gdePropertyList &list ){
	pProperties = list.pProperties;
	return *this;
}
