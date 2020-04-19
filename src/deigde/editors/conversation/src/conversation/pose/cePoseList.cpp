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

#include "cePose.h"
#include "cePoseList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class cePose
////////////////////

// Constructor, destructor
////////////////////////////

cePoseList::cePoseList(){
}

cePoseList::~cePoseList(){
}



// Management
///////////////

int cePoseList::GetCount() const{
	return pPoses.GetCount();
}

cePose *cePoseList::GetAt( int index ) const{
	return ( cePose* )pPoses.GetAt( index );
}

cePose *cePoseList::GetNamed( const char *name ) const{
	const int count = pPoses.GetCount();
	cePose *pose;
	int i;
	
	for( i=0; i<count; i++ ){
		pose = ( cePose* )pPoses.GetAt( i );
		
		if( pose->GetName().Equals( name ) ){
			return pose;
		}
	}
	
	return NULL;
}

int cePoseList::IndexOf( cePose *pose ) const{
	return pPoses.IndexOf( pose );
}

int cePoseList::IndexOfNamed( const char *name ) const{
	const int count = pPoses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( cePose* )pPoses.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool cePoseList::Has( cePose *pose ) const{
	return pPoses.Has( pose );
}

bool cePoseList::HasNamed( const char *name ) const{
	const int count = pPoses.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( cePose* )pPoses.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void cePoseList::Add( cePose *pose ){
	if( ! pose || HasNamed( pose->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pPoses.Add( pose );
}

void cePoseList::Remove( cePose *pose ){
	pPoses.Remove( pose );
}

void cePoseList::RemoveAll(){
	pPoses.RemoveAll();
}



cePoseList &cePoseList::operator=( const cePoseList &list ){
	pPoses = list.pPoses;
	return *this;
}
