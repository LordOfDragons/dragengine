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

#include "ceWDSLaneFacePose.h"
#include "../ceWindowDopeSheet.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/facepose/ceFacePose.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFaceAdd.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFaceMove.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFaceRemove.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFaceSetDuration.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFaceSet.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFaceSetPause.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFacesScale.h"
#include "../../../undosys/action/actorSpeak/facePose/ceUCAASpeakFaceClear.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class ceWDSLaneFacePose
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneFacePose::ceWDSLaneFacePose(ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description) :
ceWDSLane(dopeSheet, index, label, description){
}

ceWDSLaneFacePose::~ceWDSLaneFacePose(){
}



// Management
///////////////

const ceStripList &ceWDSLaneFacePose::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetFacePoseList() : GetEmptyList();
}

void ceWDSLaneFacePose::FillIDList(decStringList &list){
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	if(! action){
		return;
	}
	
	const ceFacePoseList facePoseList(GetWindow().GetConversation()->AllFacePoses());
	const int facePoseCount = facePoseList.GetCount();
	int i;
	for(i=0; i<facePoseCount; i++){
		list.Add(facePoseList.GetAt(i)->GetName());
	}
}

igdeUndo *ceWDSLaneFacePose::UndoStripAdd(ceStrip *strip, int index){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceAdd(GetWindow().GetTopic(), action, strip, index) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripRemove(ceStrip *strip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceRemove(GetWindow().GetTopic(), action, strip) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceClear(GetWindow().GetTopic(), action) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripReplace(ceStrip *strip, ceStrip *withStrip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceSet(GetWindow().GetTopic(), action, strip, withStrip) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripMove(ceStrip *strip, int toIndex){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceMove(GetWindow().GetTopic(), action, strip, toIndex) : NULL;
}

ceUCAASpeakStripSetPause *ceWDSLaneFacePose::UndoStripSetPause(ceStrip *strip, float pause){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceSetPause(GetWindow().GetTopic(), action, strip, pause) : NULL;
}

ceUCAASpeakStripSetDuration *ceWDSLaneFacePose::UndoStripSetDuration(ceStrip *strip, float duration){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceSetDuration(GetWindow().GetTopic(), action, strip, duration) : NULL;
}

ceUCAASpeakStripsScale *ceWDSLaneFacePose::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFacesScale(GetWindow().GetTopic(), action) : NULL;
}
