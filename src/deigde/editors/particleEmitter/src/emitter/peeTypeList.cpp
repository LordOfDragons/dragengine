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

#include "peeType.h"
#include "peeTypeList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class peeType
//////////////////

// Constructor, destructor
////////////////////////////

peeTypeList::peeTypeList(){
}

peeTypeList::~peeTypeList(){
}



// Management
///////////////

int peeTypeList::GetCount() const{
	return pTypes.GetCount();
}

peeType *peeTypeList::GetAt( int index ) const{
	return ( peeType* )pTypes.GetAt( index );
}

peeType *peeTypeList::GetNamed( const char *name ) const{
	const int count = pTypes.GetCount();
	peeType *type;
	int p;
	
	for( p=0; p<count; p++ ){
		type = ( peeType* )pTypes.GetAt( p );
		
		if( type->GetName().Equals( name ) ){
			return type;
		}
	}
	
	return NULL;
}

int peeTypeList::IndexOf( peeType *type ) const{
	return pTypes.IndexOf( type );
}

int peeTypeList::IndexOfNamed( const char *name ) const{
	const int count = pTypes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( peeType* )pTypes.GetAt( p ) )->GetName().Equals( name ) ){
			return p;
		}
	}
	
	return -1;
}

bool peeTypeList::Has( peeType *type ) const{
	return pTypes.Has( type );
}

bool peeTypeList::HasNamed( const char *name ) const{
	const int count = pTypes.GetCount();
	int p;
	
	for( p=0; p<count; p++ ){
		if( ( ( peeType* )pTypes.GetAt( p ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void peeTypeList::Add( peeType *type ){
	if( ! type || HasNamed( type->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTypes.Add( type );
}

void peeTypeList::InsertAt( peeType *type, int index ){
	if( ! type || HasNamed( type->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTypes.Insert( type, index );
}

void peeTypeList::MoveTo( peeType *type, int index ){
	pTypes.Move( type, index );
}

void peeTypeList::Remove( peeType *type ){
	pTypes.Remove( type );
}

void peeTypeList::RemoveAll(){
	pTypes.RemoveAll();
}



peeTypeList &peeTypeList::operator=( const peeTypeList &list ){
	pTypes = list.pTypes;
	return *this;
}
