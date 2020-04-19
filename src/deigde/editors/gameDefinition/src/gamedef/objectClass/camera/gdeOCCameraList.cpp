/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeOCCamera.h"
#include "gdeOCCameraList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class gdeOCCamera
//////////////////////

// Constructor, destructor
////////////////////////////

gdeOCCameraList::gdeOCCameraList(){
}

gdeOCCameraList::gdeOCCameraList( const gdeOCCameraList &list ) :
pCameras( list.pCameras ){
}

gdeOCCameraList::~gdeOCCameraList(){
}



// Management
///////////////

int gdeOCCameraList::GetCount() const{
	return pCameras.GetCount();
}

gdeOCCamera *gdeOCCameraList::GetAt( int index ) const{
	return ( gdeOCCamera* )pCameras.GetAt( index );
}

int gdeOCCameraList::IndexOf( gdeOCCamera *camera ) const{
	return pCameras.IndexOf( camera );
}

bool gdeOCCameraList::Has( gdeOCCamera *camera ) const{
	return pCameras.Has( camera );
}

void gdeOCCameraList::Add( gdeOCCamera *camera ){
	if( ! camera || Has( camera ) ){
		DETHROW( deeInvalidParam );
	}
	pCameras.Add( camera );
}

void gdeOCCameraList::Remove( gdeOCCamera *camera ){
	pCameras.Remove( camera );
}

void gdeOCCameraList::RemoveAll(){
	pCameras.RemoveAll();
}



gdeOCCameraList &gdeOCCameraList::operator=( const gdeOCCameraList &list ){
	pCameras = list.pCameras;
	return *this;
}
