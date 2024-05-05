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

#include "aeLink.h"
#include "aeLinkList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class aeLink
/////////////////

// Constructor, destructor
////////////////////////////

aeLinkList::aeLinkList(){
}

aeLinkList::aeLinkList( const aeLinkList &copy ) :
pLinks( copy.pLinks ){
}

aeLinkList::~aeLinkList(){
}



// Management
///////////////

int aeLinkList::GetCount() const{
	return pLinks.GetCount();
}

aeLink *aeLinkList::GetAt( int index ) const{
	return ( aeLink* )pLinks.GetAt( index );
}

aeLink *aeLinkList::GetNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		aeLink * const link = ( aeLink* )pLinks.GetAt( i );
		if( link->GetName() == name ){
			return link;
		}
	}
	
	return NULL;
}

int aeLinkList::IndexOf( aeLink *link ) const{
	return pLinks.IndexOf( link );
}

bool aeLinkList::Has( aeLink *link ) const{
	return pLinks.Has( link );
}

bool aeLinkList::HasNamed( const char *name ) const{
	return GetNamed( name ) != NULL;
}

void aeLinkList::Add( aeLink *link ){
	if( ! link || Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Add( link );
}

void aeLinkList::Insert( aeLink *link, int index ){
	if( ! link || Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	pLinks.Insert( link, index );
}

void aeLinkList::Move( aeLink *link, int index ){
	pLinks.Move( link, index );
}

void aeLinkList::Remove( aeLink *link ){
	pLinks.Remove( link );
}

void aeLinkList::RemoveAll(){
	pLinks.RemoveAll();
}



// Operators
//////////////

aeLinkList &aeLinkList::operator=( const aeLinkList &list ){
	pLinks = list.pLinks;
	return *this;
}

aeLinkList &aeLinkList::operator+=( const aeLinkList &list ){
	pLinks += list.pLinks;
	return *this;
}
