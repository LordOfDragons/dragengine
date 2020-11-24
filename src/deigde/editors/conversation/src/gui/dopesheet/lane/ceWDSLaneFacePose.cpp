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
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class ceWDSLaneFacePose
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneFacePose::ceWDSLaneFacePose( ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description ) :
ceWDSLane( dopeSheet, index, label, description ){
}

ceWDSLaneFacePose::~ceWDSLaneFacePose(){
}



// Management
///////////////

const ceStripList &ceWDSLaneFacePose::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetFacePoseList() : GetEmptyList();
}

void ceWDSLaneFacePose::FillIDList( decStringList &list ){
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	if( ! action ){
		return;
	}
	
	const ceFacePoseList facePoseList( GetWindow().GetConversation()->AllFacePoses() );
	const int facePoseCount = facePoseList.GetCount();
	int i;
	for( i=0; i<facePoseCount; i++ ){
		list.Add( facePoseList.GetAt( i )->GetName() );
	}
}

igdeUndo *ceWDSLaneFacePose::UndoStripAdd( ceStrip *strip, int index ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceAdd( GetWindow().GetTopic(), action, strip, index ) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripRemove( ceStrip *strip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceRemove( GetWindow().GetTopic(), action, strip ) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceClear( GetWindow().GetTopic(), action ) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripReplace( ceStrip *strip, ceStrip *withStrip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceSet( GetWindow().GetTopic(), action, strip, withStrip ) : NULL;
}

igdeUndo *ceWDSLaneFacePose::UndoStripMove( ceStrip *strip, int toIndex ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceMove( GetWindow().GetTopic(), action, strip, toIndex ) : NULL;
}

ceUCAASpeakStripSetPause *ceWDSLaneFacePose::UndoStripSetPause( ceStrip *strip, float pause ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceSetPause( GetWindow().GetTopic(), action, strip, pause ) : NULL;
}

ceUCAASpeakStripSetDuration *ceWDSLaneFacePose::UndoStripSetDuration( ceStrip *strip, float duration ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFaceSetDuration( GetWindow().GetTopic(), action, strip, duration ) : NULL;
}

ceUCAASpeakStripsScale *ceWDSLaneFacePose::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakFacesScale( GetWindow().GetTopic(), action ) : NULL;
}
