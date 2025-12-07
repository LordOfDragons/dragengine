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

#include "igdeTriggerExpression.h"
#include "igdeTriggerExpressionParser.h"
#include "igdeTriggerExpressionParserState.h"
#include "igdeTriggerExpressionComponent.h"

#include <dragengine/common/exceptions.h>



// Class igdeTriggerExpressionParser
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTriggerExpressionParser::igdeTriggerExpressionParser(){
	pSymbolNegate = '!';
	pSymbolCurState = '@';
	pSymbolAnd = '&';
	pSymbolOr = '|';
	pSymbolGroupStart = '(';
	pSymbolGroupEnd = ')';
	pSymbolQuote = '"';
	
	pExceptionOnErrors = false;
	
	pRequiresQuoteCharacters.AppendCharacter(' ');
	pRequiresQuoteCharacters.AppendCharacter('\t');
	pRequiresQuoteCharacters.AppendCharacter(pSymbolNegate);
	pRequiresQuoteCharacters.AppendCharacter(pSymbolCurState);
	pRequiresQuoteCharacters.AppendCharacter(pSymbolAnd);
	pRequiresQuoteCharacters.AppendCharacter(pSymbolOr);
	pRequiresQuoteCharacters.AppendCharacter(pSymbolGroupStart);
	pRequiresQuoteCharacters.AppendCharacter(pSymbolGroupEnd);
	pRequiresQuoteCharacters.AppendCharacter(pSymbolQuote);
}

igdeTriggerExpressionParser::~igdeTriggerExpressionParser(){
}



// Management
///////////////

void igdeTriggerExpressionParser::SetExceptionOnErrors(bool exceptionsOnErrors){
	pExceptionOnErrors = exceptionsOnErrors;
}



igdeTriggerExpression::Ref igdeTriggerExpressionParser::StringToExpression(const char *string) const{
	const igdeTriggerExpression::Ref expression(igdeTriggerExpression::Ref::NewWith());
	
	igdeTriggerExpressionParserState state(string);
	expression->SetRootComponent(ParseExpressionComponent(state, false, false, false));
	
	return expression;
}

igdeTriggerExpressionComponent::Ref igdeTriggerExpressionParser::ParseExpressionComponent(
igdeTriggerExpressionParserState &state, bool requireEnd, bool initCurState, bool initNegate) const{
	// modes:
	// 0 => parse trigger/child(0) (negate,curState allowed)
	// 1 => parse logical operator
	// 2 => parse child(1..N) (negate,curState allowed)
	// 3 => parse identical logical operator or end of group or end of expression
	// 4 => exit parser
	// 5 => exit parser due to exception
	bool curState = initCurState;
	bool negate = initNegate;
	decString name;
	int mode = 0;
	
	igdeTriggerExpressionComponent::Ref child, component(
		igdeTriggerExpressionComponent::Ref::NewWith());
	
	while(mode != 4 && mode != 5 && state.HasMoreCharacters()){
		const int character = state.GetNextCharacter();
		
		if(mode == 2 && ! component->GetTargetName().IsEmpty()){
			child.TakeOverWith();
			child->SetTargetName(component->GetTargetName());
			child->SetNegate(component->GetNegate());
			child->SetCurState(component->GetCurState());
			component->AddChild(child);
			component->SetTargetName("");
			component->SetNegate(false);
			component->SetCurState(false);
		}
		
		if(character == ' ' || character == '\t'){
			continue;
			
		}else if(character == pSymbolNegate){
			switch(mode){
			case 0:
			case 2:
				negate = true;
				break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			
		}else if(character == pSymbolCurState){
			switch(mode){
			case 0:
			case 2:
				curState = true;
				break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			
		}else if(character == pSymbolGroupStart){
			if(curState){
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
				break;
			}
			
			switch(mode){
			case 0:{
				child = ParseExpressionComponent(state, true, false, false);
				child->SetNegate(negate);
				component->AddChild(child);
				
				negate = false;
				curState = false;
				mode = 1;
				}break;
				
			case 2:
				child = ParseExpressionComponent(state, true, false, false);
				child->SetNegate(negate);
				component->AddChild(child);
				
				negate = false;
				curState = false;
				mode = 3;
				break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			
		}else if(character == pSymbolGroupEnd){
			switch(mode){
			case 3:
				if(! requireEnd){
					state.AdjustPosition(-1);
				}
				mode = 4;
				break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			break;
			
		}else if(character == pSymbolAnd){
			switch(mode){
			case 1:
				component->SetType(igdeTriggerExpressionComponent::ectAnd);
				mode = 2;
				break;
				
			case 3:
				if(component->GetType() != igdeTriggerExpressionComponent::ectAnd){
					if(pExceptionOnErrors){
						DETHROW(deeInvalidParam); // and/or changed inside list
					}
					mode = 5;
					break;
				}
				mode = 2;
				break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			
		}else if(character == pSymbolOr){
			switch(mode){
			case 1:
				component->SetType(igdeTriggerExpressionComponent::ectOr);
				mode = 2;
				break;
				
			case 3:
				if(component->GetType() != igdeTriggerExpressionComponent::ectOr){
					if(pExceptionOnErrors){
						DETHROW(deeInvalidParam); // and/or changed inside list
					}
					mode = 5;
					break;
				}
				mode = 2;
				break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			
		}else if(character == pSymbolQuote){
			switch(mode){
			case 0:
				ParseTargetName(name, state, true);
				component->SetTargetName(name);
				component->SetNegate(negate);
				component->SetCurState(curState);
				
				negate = false;
				curState = false;
				
				mode = 1;
				break;
				
			case 2:{
				ParseTargetName(name, state, true);
				child.TakeOverWith();
				child->SetTargetName(name);
				child->SetNegate(negate);
				child->SetCurState(curState);
				component->AddChild(child);
				
				negate = false;
				curState = false;
				mode = 3;
				}break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			
		// ( character >= 'a' && character <= 'z' ) || ( character >= 'A' && character <= 'Z' ) || character == '_'
		}else{
			state.AdjustPosition(-1);
			
			switch(mode){
			case 0:
				ParseTargetName(name, state, false);
				component->SetTargetName(name);
				component->SetNegate(negate);
				component->SetCurState(curState);
				
				negate = false;
				curState = false;
				mode = 1;
				break;
				
			case 2:{
				ParseTargetName(name, state, false);
				child.TakeOverWith();
				child->SetTargetName(name);
				child->SetNegate(negate);
				child->SetCurState(curState);
				component->AddChild(child);
				
				negate = false;
				curState = false;
				mode = 3;
				}break;
				
			default:
				if(pExceptionOnErrors){
					DETHROW(deeInvalidParam);
				}
				mode = 5;
			}
			
		//}else{
		//	DETHROW( deeInvalidParam );
		}
	}
	
	if(requireEnd && mode != 4){
		if(pExceptionOnErrors){
			DETHROW(deeInvalidParam);
		}
		mode = 5;
	}
	
	if(mode == 0){
		return nullptr;
	}
	
	if(mode != 5){
		if(mode == 2){
			// missing target after and/or. add an empty one to allow editors to work properly
			child.TakeOverWith();
			child->SetNegate(negate);
			child->SetCurState(curState);
			component->AddChild(child);
		}
		
		if(component->GetChildCount() == 1){
			child = component->GetChildAt(0);
			if(child->GetTargetName().IsEmpty()){
				component = child;
				
			}else{
				component->SetTargetName(child->GetTargetName());
				component->SetNegate(child->GetNegate());
				component->SetCurState(child->GetCurState());
				component->RemoveAllChildren();
			}
		}
	}
	
	return component;
}

void igdeTriggerExpressionParser::ParseTargetName(decString &name, igdeTriggerExpressionParserState &state, bool quoted) const{
	// modes
	// 0 => parse first character
	// 1 => parse other characters
	char character;
	int mode = 0;
	
	name = "";
	
	while(state.HasMoreCharacters()){
		character = state.GetNextCharacter();
		
		if(quoted){
			if(character == '\\'){
				if(! state.HasMoreCharacters()){
					DETHROW(deeInvalidParam);
				}
				name.AppendCharacter(state.GetNextCharacter());
				mode = 1;
				
			}else if(character == pSymbolQuote){
				if(mode == 0){
					DETHROW(deeInvalidParam);
				}
				break;
				
			}else{
				name.AppendCharacter(character);
				mode = 1;
			}
			
		}else{
			if(pRequiresQuoteCharacters.Find(character) == -1){
			/*if( ( character >= 'a' && character <= 'z' ) || ( character >= 'A' && character <= 'Z' )
			|| character == '_' || character == '.' || (character >= '0' && character <= '9')){*/
				name.AppendCharacter(character);
				mode = 1;
				
			}else{
				if(mode == 0){
					DETHROW(deeInvalidParam);
				}
				
				state.AdjustPosition(-1);
				break;
			}
		}
	}
}

void igdeTriggerExpressionParser::ExpressionToString(decString &string, const igdeTriggerExpression &expression) const{
	const igdeTriggerExpressionComponent * const component = expression.GetRootComponent();
	
	if(component){
		ExpressionComponentToString(string, *component, false);
		
	}else{
		string = "";
	}
}

void igdeTriggerExpressionParser::ExpressionComponentToString(decString &string,
const igdeTriggerExpressionComponent &component, bool grouping) const{
	const decString &targetName = component.GetTargetName();
	const int type = component.GetType();
	decString subString;
	
	string.Empty();
	
	if(component.GetNegate()){
		string.AppendCharacter(pSymbolNegate);
		grouping = true;
	}
	if(component.GetCurState() && type == igdeTriggerExpressionComponent::ectTarget){
		string.AppendCharacter(pSymbolCurState);
		grouping = true;
	}
	
	switch(type){
	case igdeTriggerExpressionComponent::ectTarget:
		QuoteTriggerName(subString, targetName);
		string += subString;
		break;
		
	case igdeTriggerExpressionComponent::ectAnd:{
		const int count = component.GetChildCount();
		int i;
		
		if(grouping){
			string.AppendCharacter(pSymbolGroupStart);
		}
		for(i=0; i<count; i++){
			if(i > 0){
				string.AppendCharacter(pSymbolAnd);
			}
			ExpressionComponentToString(subString, *component.GetChildAt(i), true);
			string += subString;
		}
		if(grouping){
			string.AppendCharacter(pSymbolGroupEnd);
		}
		
		}break;
		
	case igdeTriggerExpressionComponent::ectOr:{
		const int count = component.GetChildCount();
		int i;
		
		if(grouping){
			string.AppendCharacter(pSymbolGroupStart);
		}
		for(i=0; i<count; i++){
			if(i > 0){
				string.AppendCharacter(pSymbolOr);
			}
			ExpressionComponentToString(subString, *component.GetChildAt(i), true);
			string += subString;
		}
		if(grouping){
			string.AppendCharacter(pSymbolGroupEnd);
		}
		
		}break;
		
	default:
		break;
	}
}

void igdeTriggerExpressionParser::QuoteTriggerName(decString &string, const decString &name) const{
	if(name.Find(pRequiresQuoteCharacters) == -1){
		string = name;
		
	}else{
		string.Format("%c%s%c", pSymbolQuote, name.GetReplacedString("\"", "\\\"").GetString(), pSymbolQuote);
	}
}
