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

#include "ceWDSLaneEyesLookAt.h"
#include "../ceWindowDopeSheet.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLAAdd.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLAMove.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLARemove.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLASetDuration.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLASet.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLASetPause.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLAScale.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLAClear.h"
#include "../../../undosys/action/actorSpeak/eyesLookAt/ceUCAASpeakEyesLAFromHeadLA.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Actions
////////////

namespace {

class cActionFromHeadLookAt : public igdeAction{
public:
	typedef deTObjectReference<cActionFromHeadLookAt> Ref;
	
private:
protected:
	ceWDSLaneEyesLookAt &pLane;
	
public:
	cActionFromHeadLookAt(ceWDSLaneEyesLookAt &lane) : igdeAction("Set from Head Look-At",
		nullptr, "Set from Head Look-At"),
	pLane(lane){}
	
	void OnAction() override{
		ceCAActorSpeak * const action = pLane.GetWindow().GetActionASpeak();
		if(!action){
			return;
		}
		
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakEyesLAFromHeadLA::Ref::New(pLane.GetWindow().GetTopic(), action));
	}
	
	void Update() override{
		SetEnabled(pLane.GetWindow().GetActionASpeak());
	}
};

}



// Class ceWDSLaneEyesLookAt
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneEyesLookAt::ceWDSLaneEyesLookAt(ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description) :
ceWDSLane(dopeSheet, index, label, description){
}

ceWDSLaneEyesLookAt::~ceWDSLaneEyesLookAt(){
}



// Management
///////////////

void ceWDSLaneEyesLookAt::OnContextMenu(igdeMenuCascade &menu, const decPoint &position){
	ceWDSLane::OnContextMenu(menu, position);
	
	igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand(menu, cActionFromHeadLookAt::Ref::New(*this));
}



const ceStrip::List &ceWDSLaneEyesLookAt::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetEyesLookAtList() : GetEmptyList();
}

void ceWDSLaneEyesLookAt::FillIDList(decStringList &list){
	FillIDListLookAt(list);
}

igdeUndo::Ref ceWDSLaneEyesLookAt::UndoStripAdd(ceStrip *strip, int index){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLAAdd::Ref::New(GetWindow().GetTopic(), action, strip, index) : ceUCAASpeakEyesLAAdd::Ref();
}

igdeUndo::Ref ceWDSLaneEyesLookAt::UndoStripRemove(ceStrip *strip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLARemove::Ref::New(GetWindow().GetTopic(), action, strip) : ceUCAASpeakEyesLARemove::Ref();
}

igdeUndo::Ref ceWDSLaneEyesLookAt::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLAClear::Ref::New(GetWindow().GetTopic(), action) : ceUCAASpeakEyesLAClear::Ref();
}

igdeUndo::Ref ceWDSLaneEyesLookAt::UndoStripReplace(ceStrip *strip, ceStrip *withStrip){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLASet::Ref::New(GetWindow().GetTopic(), action, strip, withStrip) : ceUCAASpeakEyesLASet::Ref();
}

igdeUndo::Ref ceWDSLaneEyesLookAt::UndoStripMove(ceStrip *strip, int toIndex){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLAMove::Ref::New(GetWindow().GetTopic(), action, strip, toIndex) : ceUCAASpeakEyesLAMove::Ref();
}

ceUCAASpeakStripSetPause::Ref ceWDSLaneEyesLookAt::UndoStripSetPause(ceStrip *strip, float pause){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLASetPause::Ref::New(GetWindow().GetTopic(), action, strip, pause) : ceUCAASpeakEyesLASetPause::Ref();
}

ceUCAASpeakStripSetDuration::Ref ceWDSLaneEyesLookAt::UndoStripSetDuration(ceStrip *strip, float duration){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLASetDuration::Ref::New(GetWindow().GetTopic(), action, strip, duration) : ceUCAASpeakEyesLASetDuration::Ref();
}

ceUCAASpeakStripsScale::Ref ceWDSLaneEyesLookAt::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? ceUCAASpeakEyesLAScale::Ref::New(GetWindow().GetTopic(), action) : ceUCAASpeakEyesLAScale::Ref();
}
