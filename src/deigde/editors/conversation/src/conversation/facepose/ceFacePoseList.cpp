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

#include "ceFacePose.h"
#include "ceFacePoseList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceFacePose
/////////////////////

// Constructor, destructor
////////////////////////////

ceFacePoseList::ceFacePoseList(){
}

ceFacePoseList::~ceFacePoseList(){
}



// Management
///////////////

int ceFacePoseList::GetCount() const{
	return pFacePoses.GetCount();
}

ceFacePose *ceFacePoseList::GetAt( int index ) const{
	return ( ceFacePose* )pFacePoses.GetAt( index );
}

ceFacePose *ceFacePoseList::GetNamed( const char *name ) const{
	const int count = pFacePoses.GetCount();
	ceFacePose *facePose;
	int i;
	
	for( i=0; i<count; i++ ){
		facePose = ( ceFacePose* )pFacePoses.GetAt( i );
		
		if( facePose->GetName().Equals( name ) ){
			return facePose;
		}
	}
	
	return NULL;
}

int ceFacePoseList::IndexOf( ceFacePose *facePose ) const{
	return pFacePoses.IndexOf( facePose );
}

int ceFacePoseList::IndexOfNamed( const char *name ) const{
	const int count = pFacePoses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceFacePose* )pFacePoses.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceFacePoseList::Has( ceFacePose *facePose ) const{
	return pFacePoses.Has( facePose );
}

bool ceFacePoseList::HasNamed( const char *name ) const{
	const int count = pFacePoses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceFacePose* )pFacePoses.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceFacePoseList::Add( ceFacePose *facePose ){
	if( ! facePose || HasNamed( facePose->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pFacePoses.Add( facePose );
}

void ceFacePoseList::Remove( ceFacePose *facePose ){
	pFacePoses.Remove( facePose );
}

void ceFacePoseList::RemoveAll(){
	pFacePoses.RemoveAll();
}



ceFacePoseList &ceFacePoseList::operator=( const ceFacePoseList &list ){
	pFacePoses = list.pFacePoses;
	return *this;
}

ceFacePoseList &ceFacePoseList::operator+=( const ceFacePoseList &list ){
	pFacePoses += list.pFacePoses;
	return *this;
}
