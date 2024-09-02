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
