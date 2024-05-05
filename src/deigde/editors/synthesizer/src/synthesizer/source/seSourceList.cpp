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

#include "seSource.h"
#include "seSourceList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seSource
///////////////////////

// Constructor, destructor
////////////////////////////

seSourceList::seSourceList(){
}

seSourceList::seSourceList( const seSourceList &copy ) :
pSources( copy.pSources ){
}

seSourceList::~seSourceList(){
}



// Management
///////////////

int seSourceList::GetCount() const{
	return pSources.GetCount();
}

seSource *seSourceList::GetAt( int index ) const{
	return ( seSource* )pSources.GetAt( index );
}

int seSourceList::IndexOf( seSource *source ) const{
	return pSources.IndexOf( source );
}

bool seSourceList::Has( seSource *source ) const{
	return pSources.Has( source );
}

void seSourceList::Add( seSource *source ){
	if( ! source || Has( source ) ){
		DETHROW( deeInvalidParam );
	}
	pSources.Add( source );
}

void seSourceList::Insert( seSource *source, int index ){
	if( ! source || Has( source ) ){
		DETHROW( deeInvalidParam );
	}
	pSources.Insert( source, index );
}

void seSourceList::Move( seSource *source, int index ){
	pSources.Move( source, index );
}

void seSourceList::Remove( seSource *source ){
	pSources.Remove( source );
}

void seSourceList::RemoveAll(){
	pSources.RemoveAll();
}



// Operators
//////////////

seSourceList &seSourceList::operator=( const seSourceList &list ){
	pSources = list.pSources;
	return *this;
}

seSourceList &seSourceList::operator+=( const seSourceList &list ){
	pSources += list.pSources;
	return *this;
}
