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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPAComment.h"
#include "../ceWPTopic.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAComment.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"
#include "../../../undosys/action/comment/ceUCACommentSetComment.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Actions
////////////

namespace {

class cTextComment : public igdeTextAreaListener {
	ceWPAComment &pPanel;
	
public:
	cTextComment( ceWPAComment &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		ceConversationTopic * const topic = pPanel.GetParentPanel().GetTopic();
		ceCAComment * const action = pPanel.GetAction();
		if( ! topic || ! action || textArea->GetText() == action->GetComment() ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( new ceUCACommentSetComment( topic, action, textArea->GetText() ) );
		pPanel.GetParentPanel().GetConversation()->GetUndoSystem()->Add( undo );
	}
};

}



// Class ceWPAComment
///////////////////////

// Constructor, destructor
////////////////////////////

ceWPAComment::ceWPAComment( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelperProperties();
	
	helper.EditString( *this, "Comment:", "Comment", pEditComment, 8, new cTextComment( *this ) );
}

ceWPAComment::~ceWPAComment(){
}



// Management
///////////////

ceCAComment *ceWPAComment::GetAction() const{
	ceConversationAction * const action = GetParentPanel().GetAction();
	
	if( action && action->GetType() == ceConversationAction::eatComment ){
		return ( ceCAComment* )action;
		
	}else{
		return NULL;
	}
}

void ceWPAComment::UpdateAction(){
	const ceCAComment * const action = GetAction();
	
	if( action ){
		pEditComment->SetText( action->GetComment() );
		
	}else{
		pEditComment->ClearText();
	}
}
