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

#include <stdlib.h>

#include "reWPBone.h"
#include "reWPBoneListener.h"
#include "../../rig/reRig.h"
#include "../../rig/bone/reRigBone.h"
#include "../../rig/bone/reSelectionBones.h"

#include <dragengine/common/exceptions.h>



// Class reWPBoneListener
///////////////////////////

// Constructor, destructor
////////////////////////////

reWPBoneListener::reWPBoneListener( reWPBone &panel ) :
pPanel( panel ){
}

reWPBoneListener::~reWPBoneListener(){
}



// Notifications
//////////////////

void reWPBoneListener::BoneChanged( reRig *rig, reRigBone *bone ){
	if( bone != pPanel.GetBone() ){
		return;
	}
	
	pPanel.UpdateBone();
}

void reWPBoneListener::ActiveBoneChanged( reRig *rig ){
	if( rig != pPanel.GetRig() ){
		return;
	}
	
	pPanel.SetBone( rig->GetSelectionBones()->GetActiveBone() );
}
