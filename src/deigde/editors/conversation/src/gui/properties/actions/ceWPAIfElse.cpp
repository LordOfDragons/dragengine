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
#include <deigde/undo/igdeUndo::Ref.h>
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
