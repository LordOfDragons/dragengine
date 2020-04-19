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

#include <stdio.h>
#include <stdlib.h>

#include "reWPView.h"
#include "reWPViewListener.h"
#include "../../rig/reRig.h"



// Class reWPViewListener
///////////////////////////

// Constructor, destructor
////////////////////////////

reWPViewListener::reWPViewListener( reWPView &panel ) :
pPanel( panel ){
}

reWPViewListener::~reWPViewListener(){
}



// Management
///////////////

void reWPViewListener::RootBoneChanged( reRig *rig ){
	pPanel.UpdateView();
}

void reWPViewListener::ResourceChanged( reRig *rig ){
	pPanel.UpdateMoveList();
	pPanel.UpdateResources();
}

void reWPViewListener::SkyChanged( reRig *rig ){
	pPanel.UpdateSky();
}

void reWPViewListener::EnvObjectChanged( reRig *rig ){
	pPanel.UpdateEnvObject();
}

void reWPViewListener::ViewChanged( reRig *rig ){
	pPanel.UpdateResources();
	pPanel.UpdateCamera();
	pPanel.UpdateView();
}



void reWPViewListener::CameraChanged( reRig *rig ){
	pPanel.UpdateCamera();
}

void reWPViewListener::CameraViewChanged( reRig *rig ){
	pPanel.UpdateCameraView();
}



void reWPViewListener::BoneCountChanged( reRig *rig ){
	pPanel.UpdateBoneList();
	pPanel.UpdateCamera();
}

void reWPViewListener::BoneChanged( reRig *rig, reRigBone *bone ){
	pPanel.UpdateBoneList();
	pPanel.UpdateCamera();
}
