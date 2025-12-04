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

#include "igdeGDProperty.h"
#include "igdeGDProperty::Ref.h"
#include "igdeGDPropertyList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDProperty
//////////////////////

// Constructor, destructor
////////////////////////////

igdeGDPropertyList::igdeGDPropertyList(){
}

igdeGDPropertyList::igdeGDPropertyList( const igdeGDPropertyList &list ) :
pProperties( list.pProperties ){
}

igdeGDPropertyList::~igdeGDPropertyList(){
}



// Management
///////////////

int igdeGDPropertyList::GetCount() const{
	return pProperties.GetCount();
}

igdeGDProperty *igdeGDPropertyList::GetAt( int index ) const{
	return ( igdeGDProperty* )pProperties.GetAt( index );
}

igdeGDProperty *igdeGDPropertyList::GetNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGDProperty * const property = ( igdeGDProperty* )pProperties.GetAt( i );
		if( property->GetName() == name ){
			return property;
		}
	}
	
	return NULL;
}

int igdeGDPropertyList::IndexOf( igdeGDProperty *property ) const{
	return pProperties.IndexOf( property );
}

int igdeGDPropertyList::IndexOfNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool igdeGDPropertyList::Has( igdeGDProperty *property ) const{
	return pProperties.Has( property );
}

bool igdeGDPropertyList::HasNamed( const char *name ) const{
	const int count = pProperties.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( igdeGDProperty* )pProperties.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void igdeGDPropertyList::Add( igdeGDProperty *property ){
	if( ! property || HasNamed( property->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	pProperties.Add( property );
}

void igdeGDPropertyList::Remove( igdeGDProperty *property ){
	pProperties.Remove( property );
}

void igdeGDPropertyList::RemoveAll(){
	pProperties.RemoveAll();
}

void igdeGDPropertyList::SetToDeepCopyFrom( const igdeGDPropertyList &list ){
	const int count = list.pProperties.GetCount();
	igdeGDProperty::Ref property;
	int i;
	
	for( i=0; i<count; i++ ){
		property.TakeOver( new igdeGDProperty( *list.GetAt( i ) ) );
		Add( property );
	}
}



igdeGDPropertyList &igdeGDPropertyList::operator=( const igdeGDPropertyList &list ){
	pProperties = list.pProperties;
	return *this;
}
