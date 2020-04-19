/* 
 * Drag[en]gine Bullet Physics Module
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
#include "debpDMBConvexVolumeFace.h"
#include "debpDMBConvexVolumeList.h"
#include "dragengine/common/math/decConvexVolume.h"
#include "dragengine/common/math/decConvexVolumeFace.h"
#include "dragengine/common/math/decConvexVolumeList.h"
#include "dragengine/common/exceptions.h"



// Class debpDMBConvexVolumeList
///////////////////////////////////

// Constructor, destructor
////////////////////////////

debpDMBConvexVolumeList::debpDMBConvexVolumeList(){
}

debpDMBConvexVolumeList::~debpDMBConvexVolumeList(){
}



// Subclassing
////////////////

decConvexVolumeFace *debpDMBConvexVolumeList::CreateVolumeFace( decConvexVolumeFace *face ){
	debpDMBConvexVolumeFace *newFace = NULL;
	
	newFace = new debpDMBConvexVolumeFace;
	if( ! newFace ) DETHROW( deeOutOfMemory );
	
	if( face ){
		debpDMBConvexVolumeFace *dmbFace = ( debpDMBConvexVolumeFace* )face;
		
		newFace->SetDecalFace( dmbFace->GetDecalFace() );
	}
	
	return newFace;
}
