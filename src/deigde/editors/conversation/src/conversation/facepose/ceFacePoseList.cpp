/* 
 * Drag[en]gine IGDE Conversation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
