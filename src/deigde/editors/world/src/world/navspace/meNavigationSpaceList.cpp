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

#include "meNavigationSpace.h"
#include "meNavigationSpaceList.h"

#include <dragengine/common/exceptions.h>



// Class meNavigationSpaceList
////////////////////////////////

// Constructor, destructor
////////////////////////////

meNavigationSpaceList::meNavigationSpaceList(){
}

meNavigationSpaceList::~meNavigationSpaceList(){
}



// Management
///////////////

int meNavigationSpaceList::GetCount() const{
	return pNavSpaces.GetCount();
}

meNavigationSpace *meNavigationSpaceList::GetAt( int position ) const{
	return ( meNavigationSpace* )pNavSpaces.GetAt( position );
}

int meNavigationSpaceList::IndexOf( meNavigationSpace *navspaces ) const{
	return pNavSpaces.IndexOf( navspaces );
}

bool meNavigationSpaceList::Has( meNavigationSpace *navspaces ){
	return pNavSpaces.Has( navspaces );
}

void meNavigationSpaceList::Add( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.Add( navspaces );
}

void meNavigationSpaceList::AddIfAbsent( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.AddIfAbsent( navspaces );
}

void meNavigationSpaceList::Remove( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.Remove( navspaces );
}

void meNavigationSpaceList::RemoveIfPresent( meNavigationSpace *navspaces ){
	if( ! navspaces ) DETHROW( deeInvalidParam );
	
	pNavSpaces.RemoveIfPresent( navspaces );
}

void meNavigationSpaceList::RemoveAll(){
	pNavSpaces.RemoveAll();
}
