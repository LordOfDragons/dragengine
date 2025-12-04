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

#include "ceWDSLaneGesture.h"
#include "../ceWindowDopeSheet.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../conversation/gesture/ceGesture.h"
#include "../../../conversation/strip/ceStrip.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGestureAdd.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGestureMove.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGestureRemove.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGestureSetDuration.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGestureSet.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGestureSetPause.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGesturesScale.h"
#include "../../../undosys/action/actorSpeak/gesture/ceUCAASpeakGestureClear.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/undo/igdeUndo.h>
#include <deigde/undo/igdeUndo::Ref.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class ceWDSLaneGesture
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneGesture::ceWDSLaneGesture( ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description ) :
ceWDSLane( dopeSheet, index, label, description ){
}

ceWDSLaneGesture::~ceWDSLaneGesture(){
}



// Management
///////////////

const ceStripList &ceWDSLaneGesture::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetGestureList() : GetEmptyList();
}

void ceWDSLaneGesture::FillIDList( decStringList &list ){
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	if( ! action ){
		return;
	}
	
	const ceGestureList gestureList( GetWindow().GetConversation()->AllGestures() );
	const int gestureCount = gestureList.GetCount();
	int i;
	for( i=0; i<gestureCount; i++ ){
		list.Add( gestureList.GetAt( i )->GetName() );
	}
}

float ceWDSLaneGesture::DefaultDuration( const decString &id ){
	const ceGesture * const gesture = GetWindow().GetConversation()->GetGestureNamed( id );
	return gesture ? gesture->GetDuration() : ceWDSLane::DefaultDuration( id );
}

igdeUndo *ceWDSLaneGesture::UndoStripAdd( ceStrip *strip, int index ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGestureAdd( GetWindow().GetTopic(), action, strip, index ) : nullptr;
}

igdeUndo *ceWDSLaneGesture::UndoStripRemove( ceStrip *strip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGestureRemove( GetWindow().GetTopic(), action, strip ) : NULL;
}

igdeUndo *ceWDSLaneGesture::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGestureClear( GetWindow().GetTopic(), action ) : NULL;
}

igdeUndo *ceWDSLaneGesture::UndoStripReplace( ceStrip *strip, ceStrip *withStrip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGestureSet( GetWindow().GetTopic(), action, strip, withStrip ) : NULL;
}

igdeUndo *ceWDSLaneGesture::UndoStripMove( ceStrip *strip, int toIndex ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGestureMove( GetWindow().GetTopic(), action, strip, toIndex ) : NULL;
}

ceUCAASpeakStripSetPause *ceWDSLaneGesture::UndoStripSetPause( ceStrip *strip, float pause ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGestureSetPause( GetWindow().GetTopic(), action, strip, pause ) : NULL;
}

ceUCAASpeakStripSetDuration *ceWDSLaneGesture::UndoStripSetDuration( ceStrip *strip, float duration ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGestureSetDuration( GetWindow().GetTopic(), action, strip, duration ) : NULL;
}

ceUCAASpeakStripsScale *ceWDSLaneGesture::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakGesturesScale( GetWindow().GetTopic(), action ) : NULL;
}
