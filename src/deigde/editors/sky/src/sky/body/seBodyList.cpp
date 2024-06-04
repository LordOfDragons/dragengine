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

#include "seBody.h"
#include "seBodyList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class seBody
////////////////////

// Constructor, destructor
////////////////////////////

seBodyList::seBodyList(){
}

seBodyList::~seBodyList(){
}



// Management
///////////////

int seBodyList::GetCount() const{
	return pBodies.GetCount();
}

seBody *seBodyList::GetAt( int index ) const{
	return ( seBody* )pBodies.GetAt( index );
}

int seBodyList::IndexOf( seBody *body ) const{
	return pBodies.IndexOf( body );
}

bool seBodyList::Has( seBody *body ) const{
	return pBodies.Has( body );
}

void seBodyList::Add( seBody *body ){
	if( ! body ){
		DETHROW( deeInvalidParam );
	}
	pBodies.Add( body );
}

void seBodyList::InsertAt( seBody *body, int index ){
	if( ! body ){
		DETHROW( deeInvalidParam );
	}
	pBodies.Insert( body, index );
}

void seBodyList::MoveTo( seBody *body, int index ){
	pBodies.Move( body, index );
}

void seBodyList::Remove( seBody *body ){
	pBodies.Remove( body );
}

void seBodyList::RemoveAll(){
	pBodies.RemoveAll();
}



seBodyList &seBodyList::operator=( const seBodyList &list ){
	pBodies = list.pBodies;
	return *this;
}
