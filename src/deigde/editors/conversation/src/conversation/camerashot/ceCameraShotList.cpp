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

#include "ceCameraShot.h"
#include "ceCameraShotList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceCameraShot
///////////////////////

// Constructor, destructor
////////////////////////////

ceCameraShotList::ceCameraShotList(){
}

ceCameraShotList::~ceCameraShotList(){
}



// Management
///////////////

int ceCameraShotList::GetCount() const{
	return pShots.GetCount();
}

ceCameraShot *ceCameraShotList::GetAt( int index ) const{
	return ( ceCameraShot* )pShots.GetAt( index );
}

ceCameraShot *ceCameraShotList::GetNamed( const char *name ) const{
	const int count = pShots.GetCount();
	ceCameraShot *cameraShot;
	int i;
	
	for( i=0; i<count; i++ ){
		cameraShot = ( ceCameraShot* )pShots.GetAt( i );
		
		if( cameraShot->GetName().Equals( name ) ){
			return cameraShot;
		}
	}
	
	return NULL;
}

int ceCameraShotList::IndexOf( ceCameraShot *cameraShot ) const{
	return pShots.IndexOf( cameraShot );
}

int ceCameraShotList::IndexOfNamed( const char *name ) const{
	const int count = pShots.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceCameraShot* )pShots.GetAt( i ) )->GetName().Equals( name ) ){
			return i;
		}
	}
	
	return -1;
}

bool ceCameraShotList::Has( ceCameraShot *cameraShot ) const{
	return pShots.Has( cameraShot );
}

bool ceCameraShotList::HasNamed( const char *name ) const{
	const int count = pShots.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( ceCameraShot* )pShots.GetAt( i ) )->GetName().Equals( name ) ){
			return true;
		}
	}
	
	return false;
}

void ceCameraShotList::Add( ceCameraShot *cameraShot ){
	if( ! cameraShot || HasNamed( cameraShot->GetName().GetString() ) ) DETHROW( deeInvalidParam );
	
	pShots.Add( cameraShot );
}

void ceCameraShotList::Remove( ceCameraShot *cameraShot ){
	pShots.Remove( cameraShot );
}

void ceCameraShotList::RemoveAll(){
	pShots.RemoveAll();
}



ceCameraShotList &ceCameraShotList::operator=( const ceCameraShotList &list ){
	pShots = list.pShots;
	return *this;
}
