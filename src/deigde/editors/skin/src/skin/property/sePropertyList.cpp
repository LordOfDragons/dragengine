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

#include "seProperty.h"
#include "sePropertyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seProperty
/////////////////////

// Constructor, destructor
////////////////////////////

sePropertyList::sePropertyList(){
}

sePropertyList::~sePropertyList(){
}



// Management
///////////////

int sePropertyList::GetCount() const{
	return pProperties.GetCount();
}

seProperty *sePropertyList::GetAt( int index ) const{
	return ( seProperty* )pProperties.GetAt( index );
}

seProperty *sePropertyList::GetNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	seProperty *property;
	int p;
	
	for( p=0; p<count; p++ ){
		property = ( seProperty* )pProperties.GetAt( p );
		
		if( property->GetName().Equals( name ) ){
			return property;
		}
	}
	
	return NULL;
}

int sePropertyList::IndexOf( seProperty *property ) const{
	return pProperties.IndexOf( property );
}

int sePropertyList::IndexOfNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seProperty* )pProperties.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool sePropertyList::Has( seProperty *property ) const{
	return pProperties.Has( property );
}

bool sePropertyList::HasNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( seProperty* )pProperties.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void sePropertyList::Add( seProperty *property ){
	if( ! property || HasNamed( property->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pProperties.Add( property );
}

void sePropertyList::InsertAt( seProperty *property, int index ){
	if( ! property || HasNamed( property->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pProperties.Insert( property, index );
}

void sePropertyList::MoveTo( seProperty *property, int index ){
	pProperties.Move( property, index );
}

void sePropertyList::Remove( seProperty *property ){
	pProperties.Remove( property );
}

void sePropertyList::RemoveAll(){
	pProperties.RemoveAll();
}



sePropertyList &sePropertyList::operator=( const sePropertyList &list ){
	pProperties = list.pProperties;
	return *this;
}
