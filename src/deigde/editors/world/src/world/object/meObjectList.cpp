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

#include "meObject.h"
#include "meObjectList.h"

#include <dragengine/common/exceptions.h>



// Class meObjectList
/////////////////////////

// Constructor, destructor
////////////////////////////

meObjectList::meObjectList(){
}

meObjectList::~meObjectList(){
}



// Management
///////////////

int meObjectList::GetCount() const{
	return pObjects.GetCount();
}

meObject *meObjectList::GetAt( int position ) const{
	return ( meObject* )pObjects.GetAt( position );
}

int meObjectList::IndexOf( meObject *object ) const{
	return pObjects.IndexOf( object );
}

bool meObjectList::Has( meObject *object ) const{
	return pObjects.Has( object );
}

void meObjectList::Add( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.Add( object );
}

void meObjectList::AddIfAbsent( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.AddIfAbsent( object );
}

void meObjectList::Remove( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.Remove( object );
}

void meObjectList::RemoveIfPresent( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pObjects.RemoveIfPresent( object );
}

void meObjectList::RemoveAll(){
	pObjects.RemoveAll();
}



meObjectList &meObjectList::operator=( const meObjectList &list ){
	pObjects = list.pObjects;
	return *this;
}
