/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reUToggleBoneIKLocked.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>



// Class reUToggleBoneIKLocked
////////////////////////////////

// Constructor, destructor
////////////////////////////

reUToggleBoneIKLocked::reUToggleBoneIKLocked( reRigBone *bone, int axis ) :
pAxis( axis )
{
	if( ! bone ) DETHROW( deeInvalidParam );
	
	pBone = bone;
	pBone->AddReference();
	
	try{
		decString text;
		text.Format( "Toggle Bone IK Locked %c", 'X' + axis );
		SetShortInfo( text );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

reUToggleBoneIKLocked::~reUToggleBoneIKLocked(){
	pCleanUp();
}



// Management
///////////////

void reUToggleBoneIKLocked::Undo(){
	pBone->SetIKLocked( pAxis, ! pBone->GetIKLocked( pAxis ) );
}

void reUToggleBoneIKLocked::Redo(){
	pBone->SetIKLocked( pAxis, ! pBone->GetIKLocked( pAxis ) );
}



// Private Functions
//////////////////////

void reUToggleBoneIKLocked::pCleanUp(){
	if( pBone ) pBone->FreeReference();
}
