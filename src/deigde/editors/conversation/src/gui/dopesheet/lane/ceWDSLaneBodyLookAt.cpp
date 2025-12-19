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

#include "ceWDSLaneBodyLookAt.h"
#include "../ceWindowDopeSheet.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLAAdd.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLAMove.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLARemove.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLASetDuration.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLASet.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLASetPause.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLAScale.h"
#include "../../../undosys/action/actorSpeak/bodyLookAt/ceUCAASpeakBodyLAClear.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class ceWDSLaneBodyLookAt
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneBodyLookAt::ceWDSLaneBodyLookAt(ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description) :
ceWDSLane(dopeSheet, index, label, description){
}

ceWDSLaneBodyLookAt::~ceWDSLaneBodyLookAt(){
}



// Management
///////////////

const ceStrip::List &ceWDSLaneBodyLookAt::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetBodyLookAts() : GetEmptyList();
}

void ceWDSLaneBodyLookAt::FillIDList(decStringList &list){
	FillIDListLookAt(list);
}

igdeUndo::Ref ceWDSLaneBodyLookAt::UndoStripAdd(ceStrip *strip, int index){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLAAdd::Ref::New(GetWindow().GetTopic(), action, strip, index) : ceUCAASpeakBodyLAAdd::Ref();
}

igdeUndo::Ref ceWDSLaneBodyLookAt::UndoStripRemove(ceStrip *strip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLARemove::Ref::New(GetWindow().GetTopic(), action, strip) : ceUCAASpeakBodyLARemove::Ref();
}

igdeUndo::Ref ceWDSLaneBodyLookAt::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLAClear::Ref::New(GetWindow().GetTopic(), action) : ceUCAASpeakBodyLAClear::Ref();
}

igdeUndo::Ref ceWDSLaneBodyLookAt::UndoStripReplace(ceStrip *strip, ceStrip *withStrip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLASet::Ref::New(GetWindow().GetTopic(), action, strip, withStrip) : ceUCAASpeakBodyLASet::Ref();
}

igdeUndo::Ref ceWDSLaneBodyLookAt::UndoStripMove(ceStrip *strip, int toIndex){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLAMove::Ref::New(GetWindow().GetTopic(), action, strip, toIndex) : ceUCAASpeakBodyLAMove::Ref();
}

ceUCAASpeakStripSetPause::Ref ceWDSLaneBodyLookAt::UndoStripSetPause(ceStrip *strip, float pause){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLASetPause::Ref::New(GetWindow().GetTopic(), action, strip, pause) : ceUCAASpeakBodyLASetPause::Ref();
}

ceUCAASpeakStripSetDuration::Ref ceWDSLaneBodyLookAt::UndoStripSetDuration(ceStrip *strip, float duration){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLASetDuration::Ref::New(GetWindow().GetTopic(), action, strip, duration) : ceUCAASpeakBodyLASetDuration::Ref();
}

ceUCAASpeakStripsScale::Ref ceWDSLaneBodyLookAt::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakBodyLAScale::Ref::New(GetWindow().GetTopic(), action) : ceUCAASpeakBodyLAScale::Ref();
}
