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

#include "gdeOCNavigationSpace.h"
#include "gdeOCNavigationSpaceList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCNavigationSpace
///////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCNavigationSpaceList::gdeOCNavigationSpaceList(){
}

gdeOCNavigationSpaceList::gdeOCNavigationSpaceList( const gdeOCNavigationSpaceList &list ) :
pNavigationSpaces( list.pNavigationSpaces ){
}

gdeOCNavigationSpaceList::~gdeOCNavigationSpaceList(){
}



// Management
///////////////

int gdeOCNavigationSpaceList::GetCount() const{
	return pNavigationSpaces.GetCount();
}

gdeOCNavigationSpace *gdeOCNavigationSpaceList::GetAt( int index ) const{
	return ( gdeOCNavigationSpace* )pNavigationSpaces.GetAt( index );
}

int gdeOCNavigationSpaceList::IndexOf( gdeOCNavigationSpace *space ) const{
	return pNavigationSpaces.IndexOf( space );
}

bool gdeOCNavigationSpaceList::Has( gdeOCNavigationSpace *space ) const{
	return pNavigationSpaces.Has( space );
}

void gdeOCNavigationSpaceList::Add( gdeOCNavigationSpace *space ){
	if( ! space || Has( space ) ){
		DETHROW( deeInvalidParam );
	}
	pNavigationSpaces.Add( space );
}

void gdeOCNavigationSpaceList::Remove( gdeOCNavigationSpace *space ){
	pNavigationSpaces.Remove( space );
}

void gdeOCNavigationSpaceList::RemoveAll(){
	pNavigationSpaces.RemoveAll();
}



gdeOCNavigationSpaceList &gdeOCNavigationSpaceList::operator=( const gdeOCNavigationSpaceList &list ){
	pNavigationSpaces = list.pNavigationSpaces;
	return *this;
}
