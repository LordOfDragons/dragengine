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

#include "ceActorPose.h"
#include "ceActorPoseList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceActorPose
//////////////////////

// Constructor, destructor
////////////////////////////

ceActorPoseList::ceActorPoseList(){
}

ceActorPoseList::~ceActorPoseList(){
}



// Management
///////////////

int ceActorPoseList::GetCount() const{
	return pGestures.GetCount();
}

ceActorPose *ceActorPoseList::GetAt( int index ) const{
	return ( ceActorPose* )pGestures.GetAt( index );
}

ceActorPose *ceActorPoseList::GetNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	ceActorPose *pose;
	int i;
	
	for( i=0; i<count; i++ ){
		pose = ( ceActorPose* )pGestures.GetAt( i );
		
		if( pose->GetName().Equals( name ) ){
			return pose;
		}
	}
	
	return NULL;
}

int ceActorPoseList::IndexOf( ceActorPose *pose ) const{
	return pGestures.IndexOf( pose );
}

int ceActorPoseList::IndexOfNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceActorPose* )pGestures.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceActorPoseList::Has( ceActorPose *pose ) const{
	return pGestures.Has( pose );
}

bool ceActorPoseList::HasNamed( const char *name ) const{
	const int count = pGestures.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceActorPose* )pGestures.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceActorPoseList::Add( ceActorPose *pose ){
	if( ! pose || HasNamed( pose->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pGestures.Add( pose );
}

void ceActorPoseList::Remove( ceActorPose *pose ){
	pGestures.Remove( pose );
}

void ceActorPoseList::RemoveAll(){
	pGestures.RemoveAll();
}



ceActorPoseList &ceActorPoseList::operator=( const ceActorPoseList &list ){
	pGestures = list.pGestures;
	return *this;
}
