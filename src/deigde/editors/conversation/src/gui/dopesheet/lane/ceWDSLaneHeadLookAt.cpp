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

#include "ceWDSLaneHeadLookAt.h"
#include "../ceWindowDopeSheet.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAActorSpeak.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLAAdd.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLAMove.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLARemove.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLASetDuration.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLASet.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLASetPause.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLAScale.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLAClear.h"
#include "../../../undosys/action/actorSpeak/headLookAt/ceUCAASpeakHeadLAFromEyesLA.h"

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

class cActionFromEyesLookAt : public igdeAction{
protected:
	ceWDSLaneHeadLookAt &pLane;
	
public:
	cActionFromEyesLookAt( ceWDSLaneHeadLookAt &lane ) : igdeAction( "Set from Eyes Look-At",
		NULL, "Set from Eyes Look-At" ),
	pLane( lane ){ }
	
	virtual void OnAction(){
		ceCAActorSpeak * const action = pLane.GetWindow().GetActionASpeak();
		if( ! action ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new ceUCAASpeakHeadLAFromEyesLA( pLane.GetWindow().GetTopic(), action ) );
		pLane.GetWindow().GetConversation()->GetUndoSystem()->Add( undo );
	}
	
	virtual void Update(){
		SetEnabled( pLane.GetWindow().GetActionASpeak() );
	}
};

}



// Class ceWDSLaneHeadLookAt
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWDSLaneHeadLookAt::ceWDSLaneHeadLookAt( ceWindowDopeSheet &dopeSheet, int index,
	const char *label, const char *description ) :
ceWDSLane( dopeSheet, index, label, description ){
}

ceWDSLaneHeadLookAt::~ceWDSLaneHeadLookAt(){
}



// Management
///////////////

void ceWDSLaneHeadLookAt::OnContextMenu( igdeMenuCascade &menu, const decPoint &position ){
	ceWDSLane::OnContextMenu( menu, position );
	
	igdeUIHelper &helper = menu.GetEnvironment().GetUIHelper();
	helper.MenuCommand( menu, new cActionFromEyesLookAt( *this ), true );
}



const ceStripList &ceWDSLaneHeadLookAt::GetStripList() const{
	const ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? action->GetHeadLookAtList() : GetEmptyList();
}

void ceWDSLaneHeadLookAt::FillIDList( decStringList &list ){
	FillIDListLookAt( list );
}

igdeUndo *ceWDSLaneHeadLookAt::UndoStripAdd( ceStrip *strip, int index ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLAAdd( GetWindow().GetTopic(), action, strip, index ) : NULL;
}

igdeUndo *ceWDSLaneHeadLookAt::UndoStripRemove( ceStrip *strip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLARemove( GetWindow().GetTopic(), action, strip ) : NULL;
}

igdeUndo *ceWDSLaneHeadLookAt::UndoStripRemoveAll(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLAClear( GetWindow().GetTopic(), action ) : NULL;
}

igdeUndo *ceWDSLaneHeadLookAt::UndoStripReplace( ceStrip *strip, ceStrip *withStrip ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLASet( GetWindow().GetTopic(), action, strip, withStrip ) : NULL;
}

igdeUndo *ceWDSLaneHeadLookAt::UndoStripMove( ceStrip *strip, int toIndex ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLAMove( GetWindow().GetTopic(), action, strip, toIndex ) : NULL;
}

ceUCAASpeakStripSetPause *ceWDSLaneHeadLookAt::UndoStripSetPause( ceStrip *strip, float pause ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLASetPause( GetWindow().GetTopic(), action, strip, pause ) : NULL;
}

ceUCAASpeakStripSetDuration *ceWDSLaneHeadLookAt::UndoStripSetDuration( ceStrip *strip, float duration ){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLASetDuration( GetWindow().GetTopic(), action, strip, duration ) : NULL;
}

ceUCAASpeakStripsScale *ceWDSLaneHeadLookAt::UndoScaleStrips(){
	ceCAActorSpeak * const action = GetWindow().GetActionASpeak();
	return action ? new ceUCAASpeakHeadLAScale( GetWindow().GetTopic(), action ) : NULL;
}
