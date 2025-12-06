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
protected:
	ceWDSLaneEyesLookAt &pLane;
	
public:
	cActionFromHeadLookAt( ceWDSLaneEyesLookAt &lane ) : igdeAction( "Set from Head Look-At",
		NULL, "Set from Head Look-At" ),
	pLane( lane ){ }
	
	virtual void OnAction(){
		ceCAActorSpeak * const action = pLane.GetWindow().GetActionASpeak();
		if( ! action ){
			return;
		}
		
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add(
			ceUCAASpeakEyesLAFromHeadLA::Ref::NewWith(pLane.GetWindow().GetTopic(), action));
	}
	
	virtual void Update(){
		SetEnabled( pLane.GetWindow().GetActionASpeak() );
	}
};

}



// Class ceWDSLaneEyesLookAt
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneEyesLookAt::ceWDSLaneEyesLookAt( ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description ) :
ceWDSLane( dopeSheet, index, label, description ){
}

ceWDSLaneEyesLookAt::~ceWDSLaneEyesLookAt(){
}



// Management
///////////////

void ceWDSLaneEyesLookAt::OnContextMenu( igdeMenuCascade &menu, const decPoint &position ){
	ceWDSLane::OnContextMenu( menu, position );
	
	igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand( menu, new cActionFromHeadLookAt( *this ), true );
}



const ceStripList &ceWDSLaneEyesLookAt::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetEyesLookAtList() : GetEmptyList();
}

void ceWDSLaneEyesLookAt::FillIDList( decStringList &list ){
	FillIDListLookAt( list );
}

igdeUndo *ceWDSLaneEyesLookAt::UndoStripAdd( ceStrip *strip, int index ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLAAdd( GetWindow().GetTopic(), action, strip, index ) : NULL;
}

igdeUndo *ceWDSLaneEyesLookAt::UndoStripRemove( ceStrip *strip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLARemove( GetWindow().GetTopic(), action, strip ) : NULL;
}

igdeUndo *ceWDSLaneEyesLookAt::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLAClear( GetWindow().GetTopic(), action ) : NULL;
}

igdeUndo *ceWDSLaneEyesLookAt::UndoStripReplace( ceStrip *strip, ceStrip *withStrip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLASet( GetWindow().GetTopic(), action, strip, withStrip ) : NULL;
}

igdeUndo *ceWDSLaneEyesLookAt::UndoStripMove( ceStrip *strip, int toIndex ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLAMove( GetWindow().GetTopic(), action, strip, toIndex ) : NULL;
}

ceUCAASpeakStripSetPause *ceWDSLaneEyesLookAt::UndoStripSetPause( ceStrip *strip, float pause ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLASetPause( GetWindow().GetTopic(), action, strip, pause ) : NULL;
}

ceUCAASpeakStripSetDuration *ceWDSLaneEyesLookAt::UndoStripSetDuration( ceStrip *strip, float duration ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLASetDuration( GetWindow().GetTopic(), action, strip, duration ) : NULL;
}

ceUCAASpeakStripsScale *ceWDSLaneEyesLookAt::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakEyesLAScale( GetWindow().GetTopic(), action ) : NULL;
}
