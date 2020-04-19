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
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>



// Class ceWDSLaneBodyLookAt
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneBodyLookAt::ceWDSLaneBodyLookAt( ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description ) :
ceWDSLane( dopeSheet, index, label, description ){
}

ceWDSLaneBodyLookAt::~ceWDSLaneBodyLookAt(){
}



// Management
///////////////

const ceStripList &ceWDSLaneBodyLookAt::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetBodyLookAtList() : GetEmptyList();
}

void ceWDSLaneBodyLookAt::FillIDList( decStringList &list ){
	FillIDListLookAt( list );
}

igdeUndo *ceWDSLaneBodyLookAt::UndoStripAdd( ceStrip *strip, int index ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLAAdd( GetWindow().GetTopic(), action, strip, index ) : NULL;
}

igdeUndo *ceWDSLaneBodyLookAt::UndoStripRemove( ceStrip *strip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLARemove( GetWindow().GetTopic(), action, strip ) : NULL;
}

igdeUndo *ceWDSLaneBodyLookAt::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLAClear( GetWindow().GetTopic(), action ) : NULL;
}

igdeUndo *ceWDSLaneBodyLookAt::UndoStripReplace( ceStrip *strip, ceStrip *withStrip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLASet( GetWindow().GetTopic(), action, strip, withStrip ) : NULL;
}

igdeUndo *ceWDSLaneBodyLookAt::UndoStripMove( ceStrip *strip, int toIndex ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLAMove( GetWindow().GetTopic(), action, strip, toIndex ) : NULL;
}

ceUCAASpeakStripSetPause *ceWDSLaneBodyLookAt::UndoStripSetPause( ceStrip *strip, float pause ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLASetPause( GetWindow().GetTopic(), action, strip, pause ) : NULL;
}

ceUCAASpeakStripSetDuration *ceWDSLaneBodyLookAt::UndoStripSetDuration( ceStrip *strip, float duration ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLASetDuration( GetWindow().GetTopic(), action, strip, duration ) : NULL;
}

ceUCAASpeakStripsScale *ceWDSLaneBodyLookAt::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakBodyLAScale( GetWindow().GetTopic(), action ) : NULL;
}
