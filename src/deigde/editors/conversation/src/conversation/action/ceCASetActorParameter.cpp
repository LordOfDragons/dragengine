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

#include "ceCASetActorParameter.h"

#include <dragengine/common/exceptions.h>



// Class ceCASetActorParameter
////////////////////////////////

// Constructor, destructor
////////////////////////////

ceCASetActorParameter::ceCASetActorParameter() :
ceConversationAction(eatSetActorParameter),
pName("Parameter"),
pOperator(eopSet),
pValue(0){
}

ceCASetActorParameter::ceCASetActorParameter(const ceCASetActorParameter &action) :
ceConversationAction(action),
pActor(action.pActor),
pName(action.pName),
pOperator(action.pOperator),
pValue(action.pValue),
pValueVariable(action.pValueVariable){
}

ceCASetActorParameter::~ceCASetActorParameter(){
}



// Management
///////////////

void ceCASetActorParameter::SetActor(const char *id){
	pActor = id;
}

void ceCASetActorParameter::SetName(const char *name){
	pName = name;
}

void ceCASetActorParameter::SetOperator(eOperators aOperator){
	pOperator = aOperator;
}

void ceCASetActorParameter::SetValue(int value){
	pValue = value;
}

void ceCASetActorParameter::SetValueVariable(const char *variable){
	pValueVariable = variable;
}



ceConversationAction *ceCASetActorParameter::CreateCopy() const{
	return new ceCASetActorParameter(*this);
}
