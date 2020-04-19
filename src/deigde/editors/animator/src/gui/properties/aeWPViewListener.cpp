/* 
 * Drag[en]gine IGDE Animator Editor
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
#include <string.h>

#include "aeWPView.h"
#include "aeWPViewListener.h"
#include "../../animator/aeAnimator.h"

#include "dragengine/common/exceptions.h"



// Class aeWPViewListener
///////////////////////////

// Constructor, destructor
////////////////////////////

aeWPViewListener::aeWPViewListener( aeWPView &panel ) :
pPanel( panel ){
}

aeWPViewListener::~aeWPViewListener(){
}



// Notifications
//////////////////

void aeWPViewListener::RigChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateBoneList();
}

void aeWPViewListener::AnimatorChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdatePath();
	pPanel.UpdateView();
}

void aeWPViewListener::ModelChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateBoneList();
	pPanel.UpdatePath();
	pPanel.UpdateView();
}

void aeWPViewListener::SkyChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateSky();
}

void aeWPViewListener::EnvObjectChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateEnvObject();
}

void aeWPViewListener::AnimationChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateBoneList();
	pPanel.UpdatePath();
	pPanel.UpdateView();
}

void aeWPViewListener::PlaybackChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdatePlayback();
}

void aeWPViewListener::ViewChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateView();
}



void aeWPViewListener::CameraChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateCamera();
}

void aeWPViewListener::CameraViewChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateCameraView();
}



void aeWPViewListener::ActiveAttachmentChanged( aeAnimator *animator, aeAttachment* ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.SelectActiveAttachment();
	pPanel.UpdateAttachment();
}

void aeWPViewListener::AttachmentChanged( aeAnimator*, aeAttachment *attachment ){
	if( pPanel.GetAttachment() != attachment ){
		return;
	}
	
	pPanel.UpdateAttachmentList();
}

void aeWPViewListener::AttachmentStructureChanged( aeAnimator *animator ){
	if( pPanel.GetAnimator() != animator ){
		return;
	}
	
	pPanel.UpdateAttachmentList();
}
