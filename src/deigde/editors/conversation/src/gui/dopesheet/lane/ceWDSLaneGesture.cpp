/* 
 * Drag[en]gine IGDE Conversation Editor
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
#include <deigde/undo/igdeUndoReference.h>
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
