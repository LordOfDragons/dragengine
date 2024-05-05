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

#include "seMapped.h"
#include "seMappedList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seMapped
////////////////////

// Constructor, destructor
////////////////////////////

seMappedList::seMappedList(){
}

seMappedList::~seMappedList(){
}



// Management
///////////////

int seMappedList::GetCount() const{
	return pMapped.GetCount();
}

seMapped *seMappedList::GetAt( int index ) const{
	return ( seMapped* )pMapped.GetAt( index );
}

seMapped *seMappedList::GetNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		seMapped * const mapped = ( seMapped* )pMapped.GetAt( i );
		if( mapped->GetName().Equals( name ) ){
			return mapped;
		}
	}
	
	return nullptr;
}

int seMappedList::IndexOf( seMapped *mapped ) const{
	return pMapped.IndexOf( mapped );
}

int seMappedList::IndexOfNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( seMapped* )pMapped.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool seMappedList::Has( seMapped *mapped ) const{
	return pMapped.Has( mapped );
}

bool seMappedList::HasNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( seMapped* )pMapped.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void seMappedList::Add( seMapped *mapped ){
	DEASSERT_NOTNULL( mapped )
	DEASSERT_FALSE( HasNamed( mapped->GetName() ) )
	
	pMapped.Add( mapped );
}

void seMappedList::Remove( seMapped *mapped ){
	pMapped.Remove( mapped );
}

void seMappedList::RemoveAll(){
	pMapped.RemoveAll();
}



seMappedList &seMappedList::operator=( const seMappedList &list ){
	pMapped = list.pMapped;
	return *this;
}
