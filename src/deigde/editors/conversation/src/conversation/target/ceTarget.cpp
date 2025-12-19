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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceTarget.h"
#include "../ceConversation.h"
#include "../playback/cePlayback.h"
#include "../playback/cePlaybackCamera.h"
#include "../actor/ceConversationActor.h"
#include "../coordsystem/ceCoordSystem.h"

#include <deigde/gui/igdeCamera.h>

#include <dragengine/common/exceptions.h>



// Class ceTarget
///////////////////

// Constructor, destructor
////////////////////////////

ceTarget::ceTarget(const char *name) :
pConversation(nullptr),
pName(name){
}

ceTarget::ceTarget(const ceTarget &target){
	pConversation = nullptr;
	pName = target.GetName();
	   pActor = target.GetActor();
	   pCoordSystem = target.GetCoordSystem();
	pBone = target.GetBone();
	pPosition = target.GetPosition();
	pOrientation = target.GetOrientation();
}

ceTarget::~ceTarget(){
}



// Management
///////////////

void ceTarget::SetConversation(ceConversation *conversation){
	pConversation = conversation;
}

void ceTarget::SetName(const char *name){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	if(!pName.Equals(name)){
		if(pConversation){
			DEASSERT_FALSE(pConversation->GetTargets().HasMatching( [&](const ceTarget &t){
				return t.GetName() == name && &t != this; }))
		}
		
		pName = name;
		
		if(pConversation){
			pConversation->NotifyTargetChanged(this);
		}
	}
}

void ceTarget::SetActor(const char *id){
	if(!id){
		DETHROW(deeInvalidParam);
	}
	
	if(!pActor.Equals(id)){
		      pActor = id;
		
		if(pConversation){
			pConversation->NotifyTargetChanged(this);
		}
	}
}

void ceTarget::SetCoordSystem(const char *id){
	if(!id){
		DETHROW(deeInvalidParam);
	}
	
	if(!pCoordSystem.Equals(id)){
		      pCoordSystem = id;
		
		if(pConversation){
			pConversation->NotifyTargetChanged(this);
		}
	}
}

void ceTarget::SetBone(const char *bone){
	if(!bone){
		DETHROW(deeInvalidParam);
	}
	
	if(!pBone.Equals(bone)){
		pBone = bone;
		
		if(pConversation){
			pConversation->NotifyTargetChanged(this);
		}
	}
}

void ceTarget::SetPosition(const decVector &position){
	if(!position.IsEqualTo(pPosition)){
		pPosition = position;
		
		if(pConversation){
			pConversation->NotifyTargetChanged(this);
		}
	}
}

void ceTarget::SetOrientation(const decVector &orientation){
	if(!orientation.IsEqualTo(pOrientation)){
		pOrientation = orientation;
		
		if(pConversation){
			pConversation->NotifyTargetChanged(this);
		}
	}
}



void ceTarget::GetCoordinateSystem(cePlayback &playback, decMatrix &coordinateSystem){
	coordinateSystem.SetRT(pOrientation * DEG2RAD, pPosition);
	//coordinateSystem = decMatrix::CreateTranslation( pPosition )
	//	* decMatrix::CreateRotation( pOrientation * DEG2RAD );
	
	const ceConversationActorList &actorList = playback.GetConversation().GetActorList();
	
	// coordinate system
	if(!pCoordSystem.IsEmpty()){
		const ceCoordSystem * const coordSystem = playback.GetConversation().GetCoordSystemList().GetWithIDOrAliasID(pCoordSystem);
		if(coordSystem){
			coordinateSystem *= decMatrix::CreateRT(coordSystem->GetOrientation() * DEG2RAD, coordSystem->GetPosition());
		}
		
		// actor
	}else if(!pActor.IsEmpty()){
		const ceConversationActor * const actor = actorList.GetWithIDOrAliasID(pActor);
		if(actor){
			if(!pBone.IsEmpty()){
				coordinateSystem *= actor->GetBoneMatrix(pBone);
			}
			coordinateSystem *= decMatrix::CreateRT(actor->GetOrientation() * DEG2RAD, actor->GetPosition());
		}
		
	// otherwise use current camera location
	}else{
		coordinateSystem *= playback.GetConversation().GetCamera()->GetViewMatrix();
	}
}

