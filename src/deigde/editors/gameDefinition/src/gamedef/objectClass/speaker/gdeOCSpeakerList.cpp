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

#include "gdeOCSpeaker.h"
#include "gdeOCSpeakerList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCSpeaker
/////////////////////

// Constructor, destructor
////////////////////////////

gdeOCSpeakerList::gdeOCSpeakerList(){
}

gdeOCSpeakerList::gdeOCSpeakerList( const gdeOCSpeakerList &list ) :
pSpeakers( list.pSpeakers ){
}

gdeOCSpeakerList::~gdeOCSpeakerList(){
}



// Management
///////////////

int gdeOCSpeakerList::GetCount() const{
	return pSpeakers.GetCount();
}

gdeOCSpeaker *gdeOCSpeakerList::GetAt( int index ) const{
	return ( gdeOCSpeaker* )pSpeakers.GetAt( index );
}

int gdeOCSpeakerList::IndexOf( gdeOCSpeaker *speaker ) const{
	return pSpeakers.IndexOf( speaker );
}

bool gdeOCSpeakerList::Has( gdeOCSpeaker *speaker ) const{
	return pSpeakers.Has( speaker );
}

void gdeOCSpeakerList::Add( gdeOCSpeaker *speaker ){
	if( ! speaker || Has( speaker ) ){
		DETHROW( deeInvalidParam );
	}
	pSpeakers.Add( speaker );
}

void gdeOCSpeakerList::Remove( gdeOCSpeaker *speaker ){
	pSpeakers.Remove( speaker );
}

void gdeOCSpeakerList::RemoveAll(){
	pSpeakers.RemoveAll();
}



gdeOCSpeakerList &gdeOCSpeakerList::operator=( const gdeOCSpeakerList &list ){
	pSpeakers = list.pSpeakers;
	return *this;
}
