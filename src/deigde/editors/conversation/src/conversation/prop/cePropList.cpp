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

#include "ceProp.h"
#include "cePropList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceProp
/////////////////

// Constructor, destructor
////////////////////////////

cePropList::cePropList(){
}

cePropList::~cePropList(){
}



// Management
///////////////

int cePropList::GetCount() const{
	return pProps.GetCount();
}

ceProp *cePropList::GetAt( int index ) const{
	return ( ceProp* )pProps.GetAt( index );
}

int cePropList::IndexOf( ceProp *prop ) const{
	return pProps.IndexOf( prop );
}

bool cePropList::Has( ceProp *prop ) const{
	return pProps.Has( prop );
}

void cePropList::Add( ceProp *prop ){
	if( ! prop ){
		DETHROW( deeInvalidParam );
	}
	
	pProps.Add( prop );
}

void cePropList::Remove( ceProp *prop ){
	pProps.Remove( prop );
}

void cePropList::RemoveAll(){
	pProps.RemoveAll();
}



cePropList &cePropList::operator=( const cePropList &list ){
	pProps = list.pProps;
	return *this;
}
