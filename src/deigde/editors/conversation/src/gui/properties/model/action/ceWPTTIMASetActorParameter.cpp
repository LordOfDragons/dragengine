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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceWPTTIMASetActorParameter.h"
#include "../ceWPTTreeItem.h"
#include "../../../ceWindowMain.h"
#include "../../../../conversation/ceConversation.h"
#include "../../../../conversation/action/ceCASetActorParameter.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Constructor, destructor
////////////////////////////

ceWPTTIMASetActorParameter::ceWPTTIMASetActorParameter(ceWindowMain &windowMain,
ceConversation &conversation, ceCASetActorParameter *action) :
ceWPTTIMAction(windowMain, etActionSetActorParameter, conversation, action)
{
	SetIcon(windowMain.GetIconActionVariable());
	Update();
}

ceWPTTIMASetActorParameter::~ceWPTTIMASetActorParameter(){
}



// Management
///////////////

const char *ceWPTTIMASetActorParameter::GetOperatorText(
ceCASetActorParameter::eOperators anOperator){
	switch(anOperator){
	case ceCASetActorParameter::eopSet:
		return "=";
		
	case ceCASetActorParameter::eopIncrement:
		return "+=";
		
	case ceCASetActorParameter::eopDecrement:
		return "-=";
		
	case ceCASetActorParameter::eopRandom:
		return "random";
		
	default:
		DETHROW(deeInvalidParam);
	}
}

void ceWPTTIMASetActorParameter::Update(){
	const ceCASetActorParameter &action = *GetActionSetActorParameter();
	decString text;
	
	const char * const textOperator = GetOperatorText(action.GetOperator());
	
	text.Format("Set Actor Parameter (%s): '%s' %s", action.GetActor().GetString(),
		action.GetName().GetString(), textOperator);
	
	if(! action.GetValueVariable().IsEmpty()){
		text.AppendFormat("'%s'", action.GetValueVariable().GetString());
		
	}else{
		text.AppendFormat("%d", action.GetValue());
	}
	
	SetText(text);
}
