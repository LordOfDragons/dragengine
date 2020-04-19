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

#include "ceWPAIfElse.h"
#include "../ceWPTopic.h"
#include "../model/ceWPTTreeItemModel.h"
#include "../model/action/ifelse/ceWPTTIMAIfElse.h"
#include "../model/action/ifelse/ceWPTTIMAIfElseIfCase.h"
#include "../model/action/ifelse/ceWPTTIMAIfElseIfCaseActions.h"
#include "../model/action/ifelse/ceWPTTIMAIfElseIfCaseCondition.h"
#include "../model/action/ifelse/ceWPTTIMAIfElseElse.h"
#include "../../ceWindowMain.h"
#include "../../../conversation/ceConversation.h"
#include "../../../conversation/action/ceCAIfElse.h"
#include "../../../conversation/action/ceCAIfElseCase.h"
#include "../../../conversation/file/ceConversationFile.h"
#include "../../../conversation/topic/ceConversationTopic.h"
#include "../../../configuration/ceConfiguration.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/undo/igdeUndoReference.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class ceWPAIfElse
/////////////////////

// Constructor, destructor
////////////////////////////

ceWPAIfElse::ceWPAIfElse( ceWPTopic &parentPanel ) : ceWPAction( parentPanel ){
	CreateGUICommon( *this );
}

ceWPAIfElse::~ceWPAIfElse(){
}



// Management
///////////////

ceCAIfElse *ceWPAIfElse::GetAction() const{
	ceWPTTreeItemModel * const selected = GetParentPanel().GetActionTreeItem();
	if( ! selected ){
		return NULL;
	}
	
	ceWPTTIMAIfElse *modelIfElse = NULL;
	
	switch( selected->GetType() ){
	case ceWPTTreeItemModel::etActionIfElse:
		modelIfElse = ( ceWPTTIMAIfElse* )selected;
		break;
		
	case ceWPTTreeItemModel::etActionIfElseCase:
		modelIfElse = ( ( ceWPTTIMAIfElseIfCase* )selected )->GetModelIfElse();
		break;
		
	case ceWPTTreeItemModel::etActionIfElseCaseCondition:
		modelIfElse = ( ( ceWPTTIMAIfElseIfCaseCondition* )selected )->GetModelIfElse();
		break;
		
	case ceWPTTreeItemModel::etActionIfElseCaseActions:
		modelIfElse = ( ( ceWPTTIMAIfElseIfCaseActions* )selected )->GetModelIfElse();
		break;
		
	case ceWPTTreeItemModel::etActionIfElseElse:
		modelIfElse = ( ( ceWPTTIMAIfElseElse* )selected )->GetModelIfElse();
		break;
		
	default:
		break;
	}
	
	if( modelIfElse ){
		return modelIfElse->GetActionIfElse();
		
	}else{
		return NULL;
	}
}

ceCAIfElseCase *ceWPAIfElse::GetActiveCase() const{
	ceWPTTreeItemModel * const selected = GetParentPanel().GetActionTreeItem();
	if( ! selected ){
		return NULL;
	}
	
	ceWPTTIMAIfElseIfCase *modelIfCase = NULL;
	
	switch( selected->GetType() ){
	case ceWPTTreeItemModel::etActionIfElseCase:
		modelIfCase = ( ceWPTTIMAIfElseIfCase* )selected;
		break;
		
	case ceWPTTreeItemModel::etActionIfElseCaseCondition:
		modelIfCase = ( ( ceWPTTIMAIfElseIfCaseCondition* )selected )->GetModelIfCase();
		break;
		
	case ceWPTTreeItemModel::etActionIfElseCaseActions:
		modelIfCase = ( ( ceWPTTIMAIfElseIfCaseActions* )selected )->GetModelIfCase();
		break;
		
	default:
		break;
	}
	
	if( modelIfCase ){
		return modelIfCase->GetIfCase();
		
	}else{
		return NULL;
	}
}

void ceWPAIfElse::UpdateAction(){
	UpdateCommonParams();
}
